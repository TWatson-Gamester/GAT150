#pragma once
#include "Component/Component.h"
#include "Math/Vector2.h"

namespace gn {

	class PhysicsComponent : public Component {
	public:
		void Update() override;
		void ApplyForce(const Vector2& force) { acceleration += force; };

		bool Write(const rapidjson::Value& value) const override;
		bool Read(const rapidjson::Value& value) override;

	public: 
		Vector2 velocity = {0,0};
		Vector2 acceleration = { 0,0 };
	};
}