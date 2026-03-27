//
// Created by hugze on 02.11.2025.
//

#ifndef COLLIDERS_SAMPLE_H
#define COLLIDERS_SAMPLE_H

#include "contact_listener.h"
#include "engine/renderer.h"
#include "rendered_object.h"
#include "sample.h"
#include "world.h"

namespace colliders {
    class CollidersSample : public Sample, public listeners::ContactListener {
    public:
        explicit CollidersSample(core::Vec2F screen_size)
            : screen_size(screen_size) {
        }

        void Begin() override;

        void End() override;

        void FixedUpdate() override;

        void Update([[maybe_unused]] float dt) override;

        void OnGui() override;

        void UpdateObjectsCount();

        void AddCircle(float mass, core::Vec2F collider_offset, float radius, float restitution,
                       SDL_FColor color, core::Vec2F pos,
                       core::Vec2F vel, bool isTrigger);

        void AddAABB(float mass, core::Vec2F collider_offset, core::Vec2F size, float restitution,
                     SDL_FColor color, core::Vec2F pos, core::Vec2F vel,
                     bool isTrigger);

        void RemoveObject(utils::RenderedObject &r);

        void WallBounce() const;

        void SetColor();

        void OnTriggerEnter(physics::CollidersPair cols) override;

        void OnTriggerExit(physics::CollidersPair cols) override;

        void OnColliderEnter(physics::CollidersPair cols) override;

        void OnColliderExit(physics::CollidersPair cols) override;

        void Draw() override;

    private:
        core::Vec2F screen_size;
        std::vector<utils::RenderedObject> rendered_objects_;
        physics::ShapeType shape_to_add = physics::ShapeType::Circle;
        bool trigger_mode = false;
        bool show_quadtree = false;

        std::pair<core::Vec2F, core::Vec2F> initial_velocity_range_ = {{-100.0f, -100.0f}, {100.0f, 100.0f}};
        std::pair<float, float> initial_radius_range_ = {8.0f, 15.0f};
        std::pair<core::Vec2F, core::Vec2F> initial_aabb_size_range_ = {{15.0f, 15.0f}, {30.0f, 30.0f}};
        float initial_restitution_ = 1.0f;

        float max_initial_velocity = 200.0f;
        float max_initial_radius = 30.0f;
        float max_initial_aabb_size = 60.0f;
        float max_initial_restitution_ = 2.0f;

        int objects_capacity_ = 1500;
        int initial_objects_count_ = 50;
        int current_objects_count_ = 0;
    };
} // namespace colliders

#endif  // COLLIDERS_SAMPLE_H
