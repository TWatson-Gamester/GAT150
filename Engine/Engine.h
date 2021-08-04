#pragma once

//Core
#include "Core/FileSystem.h"

//Math
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Math/Random.h"
#include "Math/MathUtils.h"
#include "Math/Transform.h"

//Objects
#include "Object/Actor.h"
#include "Object/Scene.h"

//Graphics
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"

//Framework
#include "Framework/System.h"
#include "Framework/EventSystem.h"

//Resource
#include "Resource/ResourceSystem.h"

#include <vector>
#include <memory>
#include <algorithm>

namespace gn {
	class Engine {
	public:
		void Startup();
		void Shutdown();

		void Update(float dt);
		void Draw();

		template<typename T>
		T* Get();

	private:
		std::vector<std::unique_ptr<System>> systems;
	};

	template<typename T>
	inline T* Engine::Get()
	{
		for (auto& system : systems) {
			if ( dynamic_cast<T*>(system.get()) ) return dynamic_cast<T*>(system.get());
			//If it finds the system, then it will return a pointer to it, if not it returns null
		}

		return nullptr;
	}
}