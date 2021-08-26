#pragma once
#include "Object.h"
#include "Math/Transform.h"
#include "Component/Component.h"
#include "Core/Serializable.h"
#include <vector>
#include <memory>

namespace gn {
	//Forward declaration, Used for declaring pointers to a class / struct without having to truely declare it
	class Scene;
	class Texture;
	class Renderer;

	class Actor : public Object, public ISerializable {
	public:
		Actor() {};
		Actor(const Transform& transform) : transform{ transform } {};

		virtual void Initialize() {}

		virtual void Update(float dt);
		virtual void Draw(Renderer* renderer);

		virtual void OnCollision(Actor* actor) {}

		void AddChild(std::unique_ptr<Actor> actor);

		float GetRadius();

		void AddComponent(std::unique_ptr<Component> component);
		template<class T>
		T* AddComponent();
		template<class T>
		T* GetComponent();

		// Inherited via ISerializable
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		bool destroy = false;
		std::string name;
		std::string tag;

		Transform transform;
		Scene* scene { nullptr };

		Actor* parent{ nullptr };
		std::vector<std::unique_ptr<Actor>> children;

		std::vector<std::unique_ptr<Component>> components;

	};

	template<class T>
	inline T* Actor::AddComponent(){
		
		std::unique_ptr<T> component = std::make_unique<T>();
		component->owner = this;
		components.push_back(std::move(component));

		return dynamic_cast<T*>(components.back().get());
	}
	template<class T>
	inline T* Actor::GetComponent()
	{
		std::vector<T*> results;
		for (auto& component : components) {
			if (dynamic_cast<T*>(component.get())) {
				return dynamic_cast<T*>(component.get());
			}
		}
		return nullptr;
	}
}