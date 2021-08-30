#include "PickupComponent.h"

bool PickupComponent::Write(const rapidjson::Value& value) const{
	return false;
}

bool PickupComponent::Read(const rapidjson::Value& value){
	return true;
}

void PickupComponent::Update(){
}
