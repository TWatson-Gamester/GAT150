#include "ParticleSystem.h"
#include "Math/Random.h"

namespace gn {

	void ParticleSystem::Startup() {
		particles.resize(10000);
	}

	void ParticleSystem::Shutdown() {
		particles.clear();
	}

	void ParticleSystem::Update(float dt) {
		for (Particle& p : particles) {
			if (p.isActive) {
				p.lifetime -= dt;
				p.isActive = p.lifetime > 0;
				p.prevPosition = p.position;
				p.position += (p.velocity * dt);
			}
		}
	}

	void ParticleSystem::Draw(Renderer* renderer) {
		for (const Particle& p : particles) {
			if (p.isActive) {
				renderer->Draw(p.texture, p.position, 0);
			}
		}
	}

	void ParticleSystem::Create(const Vector2& position, size_t count, float lifespan, std::shared_ptr<Texture> texture, float speed){
		for (size_t i = 0; i < count; i++) {
			auto p = std::find_if(particles.begin(), particles.end(), Particle::IsNotActive);
			if (p != particles.end()) {
				p->isActive = true;
				p->lifetime = lifespan;
				p->position = position;
				p->prevPosition = position;
				p->texture = texture;
				
				p->velocity = Vector2{ RandomRange(-1,1), RandomRange(-1,1) } * (speed * Random());
			}
		}
	}

	void ParticleSystem::Create(const Vector2& position, size_t count, float lifespan, const std::vector<Color>& colors, float speed, float angle, float angleRange){
		for (size_t i = 0; i < count; i++) {
			auto p = std::find_if(particles.begin(), particles.end(), Particle::IsNotActive);
			if (p != particles.end()) {
				p->isActive = true;
				p->lifetime = lifespan;
				p->position = position;
				p->prevPosition = position;

				p->velocity = Vector2::Rotate(gn::Vector2::right, angle) + gn::RandomRange(-angleRange, angleRange) *(speed * Random());
			}
		}
	}
}