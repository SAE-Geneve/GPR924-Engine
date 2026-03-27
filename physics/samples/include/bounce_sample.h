//
// Created by hugze on 14.11.2025.
//

#ifndef BOUNCE_SAMPLE_H
#define BOUNCE_SAMPLE_H
#include "contact_listener.h"
#include "rendered_object.h"
#include "sample.h"

namespace colliders {
    class BounceSample : public Sample, public listeners::ContactListener {
    public:
        explicit BounceSample(core::Vec2F screen_size)
            : screen_size(screen_size) {
        }

        void Begin() override;
        void End() override;
        void FixedUpdate() override;
        void Update([[maybe_unused]] float dt) override;
        void OnGui() override;
        void Draw() override;

        static float constexpr G = 500.f;
        void ApplyGravitationForceTowardsGround() const;

        void OnTriggerEnter(physics::CollidersPair cols) override;
        void OnTriggerExit(physics::CollidersPair cols) override;

        void OnColliderEnter(physics::CollidersPair cols) override;
        void OnColliderExit(physics::CollidersPair cols) override;

    private:
        core::Vec2F screen_size;
        std::vector<utils::RenderedObject> rendered_objects_;
    };
}


#endif //BOUNCE_SAMPLE_H
