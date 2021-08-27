#include "PlayerComponent.h"
#include "Engine.h"

using namespace gn;

void PlayerComponent::Update(){
	Vector2 force = Vector2::zero;
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_A) == InputSystem::eKeyState::Hold) {
		force.x -= speed;
	}
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_D) == InputSystem::eKeyState::Hold) {
		force.x += speed;
	}
	if (contacts.size() > 0 && owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == InputSystem::eKeyState::Hold) {
		force.y -= 200;
	}

	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
	assert(physicsComponent);

	physicsComponent->ApplyForce(force);

	SpriteAnimationComponent* spriteAnimationComponent = owner->GetComponent<SpriteAnimationComponent>();
	assert(spriteAnimationComponent);
	if (physicsComponent->velocity.x > 0) {
		spriteAnimationComponent->StartSequence("walk_right");
	}
	else if(physicsComponent->velocity.x < 0){
		spriteAnimationComponent->StartSequence("walk_left");
	}
	else {
		spriteAnimationComponent->StartSequence("idle");
	}
}



void PlayerComponent::Create(){
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&PlayerComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&PlayerComponent::OnCollisionExit, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("hurt", "audio/hurt.wav");
}

void PlayerComponent::OnCollisionEnter(const Event& event) {
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);
	
	if (iString_Compate(actor->tag, "ground")) {
		contacts.push_back(actor);
	}

	if (iString_Compate(actor->tag, "enemy")) {
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("hurt");
	}

	std::cout << actor->tag << std::endl;
}

void PlayerComponent::OnCollisionExit(const Event& event) {
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (iString_Compate(actor->tag, "ground")) {
		contacts.remove(actor);
	}
}

bool PlayerComponent::Write(const rapidjson::Value& value) const{
	return false;
}

bool PlayerComponent::Read(const rapidjson::Value& value){
	JSON_READ(value, speed);
	return true;
}
