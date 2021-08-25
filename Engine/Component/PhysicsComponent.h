#pragma once
#include "Component/Component.h"
#include "Math/Vector2.h"

namespace gn {

	class PhysicsComponent : public Component {
	public:
		void Update() override;
		virtual void ApplyForce(const Vector2& force) { acceleration += force; };

		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public: 
		Vector2 velocity = {0,0};
		Vector2 acceleration = { 0,0 };
		float drag = 1;
	};
}