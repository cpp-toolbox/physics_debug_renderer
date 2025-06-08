#ifndef PHYSICS_DEBUG_RENDERER_HPP
#define PHYSICS_DEBUG_RENDERER_HPP

#include <iostream>
#include <Jolt/Jolt.h>
#include <Jolt/Renderer/DebugRendererSimple.h>
#include <Jolt/Renderer/DebugRenderer.h>
#include <glm/glm.hpp>
#include <vector>

#include "sbpt_generated_includes.hpp"

class PhysicsDebugRenderer : public JPH::DebugRendererSimple {
  private:
    CW_V_TransformationWithColoredVertexShaderRenderer &renderer;

    glm::vec3 colorToVec3(JPH::ColorArg color) {
        return glm::vec3(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
    }

    glm::vec3 rvec3ToVec3(JPH::RVec3Arg vec) {
        return glm::vec3(static_cast<float>(vec.GetX()), static_cast<float>(vec.GetY()),
                         static_cast<float>(vec.GetZ()));
    }

    const float line_width = 0.05;

  public:
    explicit PhysicsDebugRenderer(CW_V_TransformationWithColoredVertexShaderRenderer &renderer) : renderer(renderer) {}

    virtual void DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) override {
        glm::vec3 from = rvec3ToVec3(inFrom);
        glm::vec3 to = rvec3ToVec3(inTo);
        glm::vec3 direction = glm::normalize(to - from);

        // Create two perpendicular vectors to the line direction
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        if (glm::abs(glm::dot(direction, up)) > 0.99f) {
            up = glm::vec3(1.0f, 0.0f, 0.0f);
        }

        glm::vec3 perpendicular1 = glm::normalize(glm::cross(direction, up));
        glm::vec3 perpendicular2 = glm::normalize(glm::cross(direction, perpendicular1));

        glm::vec3 offset1 = perpendicular1 * (line_width * 0.5f);
        glm::vec3 offset2 = perpendicular2 * (line_width * 0.5f);

        // First quad (using perpendicular1)
        std::vector<glm::vec3> positions = {
            // First quad vertices (0-3)
            from - offset1, // 0: bottom left
            from + offset1, // 1: bottom right
            to + offset1,   // 2: top right
            to - offset1,   // 3: top left

            // Second quad vertices (4-7) - perpendicular to first quad
            from - offset2, // 4: bottom left
            from + offset2, // 5: bottom right
            to + offset2,   // 6: top right
            to - offset2    // 7: top left
        };

        glm::vec3 color = colorToVec3(inColor);
        std::vector<glm::vec3> colors = {
            color, color, color, color, // First quad colors
            color, color, color, color  // Second quad colors
        };

        // Indices for both quads
        // First quad: two triangles (0,1,2) and (0,2,3)
        // Second quad: two triangles (4,5,6) and (4,6,7)
        std::vector<unsigned int> indices = {
            0, 1, 2, 0, 2, 3, // First quad
            4, 5, 6, 4, 6, 7  // Second quad
        };

        renderer.queue_draw(indices, positions, colors);
    }

    virtual void DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor,
                              ECastShadow inCastShadow) override {

        std::vector<glm::vec3> positions = {rvec3ToVec3(inV1), rvec3ToVec3(inV2), rvec3ToVec3(inV3)};

        glm::vec3 color = colorToVec3(inColor);
        std::vector<glm::vec3> colors = {color, color, color};

        std::vector<unsigned int> indices = {0, 1, 2};

        renderer.queue_draw(indices, positions, colors);
    }

    virtual void DrawText3D(JPH::RVec3Arg inPosition, const std::string_view &inString, JPH::ColorArg inColor,
                            float inHeight) override {

        // Not implemented as requested
    }
};

#endif // PHYSICS_DEBUG_RENDERER_HPP
