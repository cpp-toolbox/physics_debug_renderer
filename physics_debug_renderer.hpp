#ifndef PHYSICS_DEBUG_RENDERER_HPP
#define PHYSICS_DEBUG_RENDERER_HPP

#ifndef JPH_DEBUG_RENDERER
#define JPH_DEBUG_RENDERER
#endif

#include <Jolt/Jolt.h>
#include <Jolt/Renderer/DebugRenderer.h>

#include <glm/gtc/matrix_transform.hpp>

#include "sbpt_generated_includes.hpp"
#include "../../state/camera/camera.hpp"
#include "Jolt/Physics/Character/CharacterVirtual.h"

class PhysicsDebugRenderer final : public JPH::DebugRenderer {

  public:
    explicit PhysicsDebugRenderer(ShaderCache &shader_cache, unsigned int &screen_width, unsigned int &screen_height,
                                  JPH::Ref<JPH::CharacterVirtual> &physics_character, Camera &camera, float &fov,
                                  float &render_distance);

    ;

    ShaderCache &shader_cache;
    unsigned int &screen_height;
    unsigned int &screen_width;
    JPH::Ref<JPH::CharacterVirtual> physics_character;
    Camera &camera;
    float &fov;
    float &render_distance;

    //  ~PhysicsDebugRenderer() override;

    /// Implementation of DebugRenderer interface
    void DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) override;

    void DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor,
                      ECastShadow inCastShadow = ECastShadow::Off) override;

    Batch CreateTriangleBatch(const Triangle *inTriangles, int inTriangleCount) override;

    Batch CreateTriangleBatch(const Vertex *inVertices, int inVertexCount, const JPH::uint32 *inIndices,
                              int inIndexCount) override;

    void DrawGeometry(JPH::RMat44Arg inModelMatrix, const JPH::AABox &inWorldSpaceBounds, float inLODScaleSq,
                      JPH::ColorArg inModelColor, const GeometryRef &inGeometry, ECullMode inCullMode,
                      ECastShadow inCastShadow, EDrawMode inDrawMode) override;

    void DrawText3D(JPH::RVec3Arg inPosition, const JPH::string_view &inString, JPH::ColorArg inColor,
                    float inHeight) override;

    // opengl related things
    unsigned int vbo, ibo, vao;
};

class ThatIHaveToMake : public JPH::RefTarget<ThatIHaveToMake> {};

class TriangleData : public JPH::RefTargetVirtual, public ThatIHaveToMake {
  public:
    int num_triangles;
    std::vector<float> triangle_vertices;

    TriangleData(const JPH::DebugRenderer::Triangle *triangles, int num_triangles) {

        this->num_triangles = num_triangles;
        this->uses_indices = false;

        for (int i = 0; i < num_triangles; i += 1) {
            JPH::DebugRenderer::Triangle triangle = triangles[i];

            JPH::DebugRenderer::Vertex v1 = triangle.mV[0];
            JPH::DebugRenderer::Vertex v2 = triangle.mV[1];
            JPH::DebugRenderer::Vertex v3 = triangle.mV[2];

            triangle_vertices.push_back(v1.mPosition.x);
            triangle_vertices.push_back(v1.mPosition.y);
            triangle_vertices.push_back(v1.mPosition.z);

            triangle_vertices.push_back(v2.mPosition.x);
            triangle_vertices.push_back(v2.mPosition.y);
            triangle_vertices.push_back(v2.mPosition.z);

            triangle_vertices.push_back(v3.mPosition.x);
            triangle_vertices.push_back(v3.mPosition.y);
            triangle_vertices.push_back(v3.mPosition.z);
        }
    }

    bool uses_indices;

    std::vector<float> vertices;
    std::vector<JPH::uint32> indices;
    //    std::vector<int> indices;

    TriangleData(const JPH::DebugRenderer::Vertex *vertices, int num_vertices, const JPH::uint32 *indices,
                 int num_indices) {
        this->uses_indices = true;

        for (int i = 0; i < num_vertices; i++) {
            JPH::DebugRenderer::Vertex vertex = vertices[i];
            this->vertices.push_back(vertex.mPosition.x);
            this->vertices.push_back(vertex.mPosition.y);
            this->vertices.push_back(vertex.mPosition.z);
        }

        for (int i = 0; i < num_indices; i++) {
            JPH::uint32 index = indices[i];
            //            int index = indices[i];
            this->indices.push_back(index);
        }
    }

    virtual void AddRef() override { ThatIHaveToMake::AddRef(); }
    virtual void Release() override {
        if (--mRefCount == 0)
            delete this;
    }
};

#endif // PHYSICS_DEBUG_RENDERER_HPP
