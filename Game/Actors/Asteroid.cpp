#include "Asteroid.h"
#include "Projectile.h"
#include "Player.h"
#include "Engine.h"
#include "Math/Random.h"

void Asteroid::Update(float dt) {

	Actor::Update(dt);

	transform.position += gn::Vector2::Rotate(gn::Vector2::right, transform.rotation) * speed * dt;
	transform.position.x = gn::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = gn::Wrap(transform.position.y, 0.0f, 600.0f);
}

void Asteroid::OnCollision(Actor* actor) {
	if (dynamic_cast<Projectile*>(actor) && actor->tag == "Player") {
		if (tag == "Large") {
			int smallerAsteroids = gn::RandomRangeInt(1, 4);
			for (int i = 0; i < smallerAsteroids; i++) {
				scene->AddActor(std::make_unique<Asteroid>(gn::Transform{ {transform.position.x,transform.position.y }, gn::RandomRange(0, 800), .1f }, scene->engine->Get<gn::ResourceSystem>()->Get<gn::Texture>("SmallAsteroid.png", scene->engine->Get<gn::Renderer>()), 10.0f, "Small"));
			}
		}

		this->destroy = true;
		actor->destroy = true;
		scene->engine->Get<gn::ParticleSystem>()->Create(transform.position, 10, 1.0f, scene->engine->Get<gn::ResourceSystem>()->Get<gn::Texture>("particle01.png", scene->engine->Get<gn::Renderer>()), 300.0f);
		scene->engine->Get<gn::AudioSystem>()->PlayAudio("explosion");

		gn::Event event;
		event.name = "AddPoints";
		if (tag == "Large") {
			event.data = 500;
		}
		else {
			event.data = 100;
		}
		scene->engine->Get<gn::EventSystem>()->Notify(event);
	}
}