#pragma once
#include "Object.h"
#include "Math/Transform.h"
#include <vector>
#include <memory>

namespace gn {
	//Forward declaration, Used for declaring pointers to a class / struct without having to truely declare it
	class Scene;

	class Actor : public Object {
	public:
		Actor() {};
		Actor(const Transform& transform) : transform{ transform } {};

		virtual void Initialize() {}

		virtual void Update(float dt);
		virtual void Draw();

		virtual void OnCollision(Actor* actor) {}

		void AddChild(std::unique_ptr<Actor> actor);

		float GetRadius();

	public:
		bool destroy = false;
		std::string tag;
		Transform transform;
		Scene* scene { nullptr };

		Actor* parent{ nullptr };
		std::vector<std::unique_ptr<Actor>> children;
	};
}