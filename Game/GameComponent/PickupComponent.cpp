#include "PickupComponent.h"

PickupComponent::~PickupComponent()
{
	owner->scene->engine->Get<gn::EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<gn::EventSystem>()->Unsubscribe("collision_exit", owner);
}

bool PickupComponent::Write(const rapidjson::Value& value) const{
	return false;
}

bool PickupComponent::Read(const rapidjson::Value& value){
	return true;
}

void PickupComponent::Update(){
}
