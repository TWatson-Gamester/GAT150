#include "Engine.h"

namespace gn {
	void Engine::Startup(){
		systems.push_back(std::make_unique<EventSystem>());
		systems.push_back(std::make_unique<Renderer>());
		systems.push_back(std::make_unique<ResourceSystem>());

		std::for_each(systems.begin(), systems.end(), [](auto& system) {system->Startup(); });
	}

	void Engine::Shutdown(){
		std::for_each(systems.begin(), systems.end(), [](auto& system) {system->Shutdown(); });
	}

	void Engine::Update(float dt){
		std::for_each(systems.begin(), systems.end(), [dt](auto& system) {system->Update(dt); });
	}
	void Engine::Draw(){
		std::for_each(systems.begin(), systems.end(), [] (auto& system) mutable {
				if (dynamic_cast<GraphicsSystem*>(system.get())) {
					dynamic_cast<GraphicsSystem*>(system.get())->Draw();
				}
			});
	}
}