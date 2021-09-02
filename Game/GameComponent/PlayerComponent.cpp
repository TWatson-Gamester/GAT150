#include "PlayerComponent.h"
#include "Engine.h"

using namespace gn;

PlayerComponent::~PlayerComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}

void PlayerComponent::Update(){
	if (owner->active) {
		Vector2 force = Vector2::zero;
		if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_A) == InputSystem::eKeyState::Hold) {
			force.x -= speed;
		}
		if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_D) == InputSystem::eKeyState::Hold) {
			force.x += speed;
		}
		if (contacts.size() > 0 && owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == InputSystem::eKeyState::Hold) {
			force.y -= jump;
		}

		PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
		assert(physicsComponent);

		physicsComponent->ApplyForce(force);

		SpriteAnimationComponent* spriteAnimationComponent = owner->GetComponent<SpriteAnimationComponent>();
		assert(spriteAnimationComponent);
		if (physicsComponent->velocity.x > 0) {
			spriteAnimationComponent->StartSequence("walk_right");
		}
		else if (physicsComponent->velocity.x < 0) {
			spriteAnimationComponent->StartSequence("walk_left");
		}
		else {
			spriteAnimationComponent->StartSequence("idle");
		}
	}
}



void PlayerComponent::Create(){
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&PlayerComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&PlayerComponent::OnCollisionExit, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("hurt", "audio/hurt.wav");
	owner->scene->engine->Get<AudioSystem>()->AddAudio("coin", "audio/coin.wav");
}

void PlayerComponent::OnCollisionEnter(const Event& event) {
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);
	
	if (iString_Compate(actor->tag, "ground")) {
		contacts.push_back(actor);
	}

	if (iString_Compate(actor->tag, "enemy")) {
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("hurt");

		Event event;
		event.name = "player_hit";
		event.data = true;
		owner->scene->engine->Get<EventSystem>()->Notify(event);
	}

	if (iString_Compate(actor->tag, "pickup")) {
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("coin");
		actor->destroy = true;

		Event event;
		event.name = "add_score";
		event.data = 10;
		owner->scene->engine->Get<EventSystem>()->Notify(event);
	}

	//std::cout << "Enter: " << actor->tag << std::endl;
}

void PlayerComponent::OnCollisionExit(const Event& event) {
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (iString_Compate(actor->tag, "ground")) {
		contacts.remove(actor);
	}

	//std::cout << "Exit: " << actor->tag << std::endl;

}

bool PlayerComponent::Write(const rapidjson::Value& value) const{
	return false;
}

bool PlayerComponent::Read(const rapidjson::Value& value){
	JSON_READ(value, speed);
	JSON_READ(value, jump);
	return true;
}
