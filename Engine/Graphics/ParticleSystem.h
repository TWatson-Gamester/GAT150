#pragma once
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Texture.h"
#include "Renderer.h"
#include "Framework/System.h"
#include <vector>

namespace gn {

	class ParticleSystem : public GraphicsSystem{
	public:
		struct Particle {
			Vector2 position;
			Vector2 prevPosition;
			Vector2 velocity;
			std::shared_ptr<Texture> texture;
			float lifetime;
			bool isActive{ false };

			static bool IsNotActive(Particle p) { return !p.isActive; }

		};

	public:
		void Startup() override;
		void Shutdown() override;

		void Update(float dt) override;
		void Draw(Renderer* renderer) override;

		void Create(const Vector2& position, size_t count, float lifespan, std::shared_ptr<Texture> texture, float speed);
		void Create(const Vector2& position, size_t count, float lifespan, const std::vector<Color>& colors, float speed, float angle, float angleRange);

	private:
		std::vector<Particle> particles;
	};

}