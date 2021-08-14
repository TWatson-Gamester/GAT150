#include "Enemy.h"
#include "Projectile.h"
#include "Player.h"
#include "Engine.h"

void Enemy::Update(float dt) {

	Actor::Update(dt);

	if (scene->GetActor<Player>()) {
		gn::Vector2 direction = scene->GetActor<Player>()->transform.position - transform.position;
		gn::Vector2 forward = gn::Vector2::Rotate(gn::Vector2::right, transform.rotation);

		float turnAngle = gn::Vector2::SignedAngle(forward, direction.Normalized());
		transform.rotation = transform.rotation + turnAngle * (3 * dt);

		float angle = gn::Vector2::Angle(direction.Normalized(), forward);

		fireTimer -= dt;
		if (fireTimer <= 0 && angle <= gn::DegToRad(15)) {
			scene->engine->Get<gn::AudioSystem>()->PlayAudio("Enemy_Shot");
			fireTimer = fireRate;
			gn::Transform t = transform;
			t.scale = .5f;

			std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<gn::ResourceSystem>()->Get<gn::Texture>("EnemyShot.png", scene->engine->Get<gn::Renderer>()), 600.0f);
			projectile->tag = "Enemy";
			scene->AddActor(std::move(projectile));
		}
	}


	transform.position += gn::Vector2::Rotate(gn::Vector2::right, transform.rotation) * speed * dt;
	transform.position.x = gn::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = gn::Wrap(transform.position.y, 0.0f, 600.0f);
}

void Enemy::OnCollision(Actor* actor){
	if (dynamic_cast<Projectile*>(actor) && actor->tag == "Player") {
		this->destroy = true;
		actor->destroy = true;
		scene->engine->Get<gn::ParticleSystem>()->Create(transform.position, 10, 1.0f, scene->engine->Get<gn::ResourceSystem>()->Get<gn::Texture>("particle01.png", scene->engine->Get<gn::Renderer>()), 300.0f);
		scene->engine->Get<gn::AudioSystem>()->PlayAudio("explosion");

		gn::Event event;
		event.name = "AddPoints";
		event.data = 300;
		scene->engine->Get<gn::EventSystem>()->Notify(event);
	}
}
