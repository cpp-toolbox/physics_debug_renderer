#include "physics_debug_renderer.hpp"

void draw_character_state(PhysicsDebugRenderer &physics_debug_renderer, const JPH::CharacterVirtual *inCharacter) {

    JPH::RMat44 inCharacterTransform = inCharacter->GetWorldTransform();
    JPH::Vec3Arg inCharacterVelocity = inCharacter->GetLinearVelocity();

    // Draw current location
    // Drawing prior to update since the physics system state is also that prior to the simulation step (so that all
    // detected collisions etc. make sense)
    physics_debug_renderer.DrawCoordinateSystem(inCharacterTransform, 0.1f);

    // Draw the state of the ground contact
    JPH::CharacterBase::EGroundState ground_state = inCharacter->GetGroundState();
    if (ground_state != JPH::CharacterBase::EGroundState::InAir) {
        JPH::RVec3 ground_position = inCharacter->GetGroundPosition();
        JPH::Vec3 ground_normal = inCharacter->GetGroundNormal();
        JPH::Vec3 ground_velocity = inCharacter->GetGroundVelocity();

        // Draw ground position
        physics_debug_renderer.DrawMarker(ground_position, JPH::Color::sRed, 0.1f);
        physics_debug_renderer.DrawArrow(ground_position, ground_position + 2.0f * ground_normal, JPH ::Color::sGreen,
                                         0.1f);

        // Draw ground velocity
        if (!ground_velocity.IsNearZero())
            physics_debug_renderer.DrawArrow(ground_position, ground_position + ground_velocity, JPH::Color::sBlue,
                                             0.1f);
    }

    // Draw provided character velocity
    if (!inCharacterVelocity.IsNearZero())
        physics_debug_renderer.DrawArrow(inCharacterTransform.GetTranslation(),
                                         inCharacterTransform.GetTranslation() + inCharacterVelocity,
                                         JPH::Color::sYellow, 0.1f);

    // Draw text info
    const JPH::PhysicsMaterial *ground_material = inCharacter->GetGroundMaterial();
    JPH::Vec3 horizontal_velocity = inCharacterVelocity;
    horizontal_velocity.SetY(0);

    std::ostringstream oss;
    oss << "State: " << JPH::CharacterBase::sToString(ground_state) << "\n"
        << "Mat: " << ground_material->GetDebugName() << "\n"
        << "Horizontal Vel: " << std::fixed << std::setprecision(1) << horizontal_velocity.Length() << " m/s\n"
        << "Vertical Vel: " << std::fixed << std::setprecision(1) << inCharacterVelocity.GetY() << " m/s";

    physics_debug_renderer.DrawText3D(inCharacterTransform.GetTranslation(), oss.str(), JPH::Color::sWhite, 0.25f);
}
