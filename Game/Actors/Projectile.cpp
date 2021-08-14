#include "Projectile.h"
#include "Engine.h"

void Projectile::Update(float dt) {

	Actor::Update(dt);

	lifetime -= dt;
	if (lifetime <= 0) {
		destroy = true;
	}

	transform.position += gn::Vector2::Rotate(gn::Vector2::right, transform.rotation) * speed * dt;
	transform.position.x = gn::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = gn::Wrap(transform.position.y, 0.0f, 600.0f);

	/*std::vector<gn::Color> colors = { gn::Color::white, gn::Color::red, gn::Color::green, gn::Color::blue, gn::Color::orange, gn::Color::purple, gn::Color::cyan, gn::Color::yellow };
	scene->engine->Get<gn::ParticleSystem>()->Create(transform.position, 10, .25f, colors[gn::RandomRangeInt(0, colors.size())], 150);*/

}