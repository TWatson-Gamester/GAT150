#include "TileMap.h"
#include "Engine.h"

namespace gn {
	void TileMap::Create(){
		for (int i = 0; i < tiles.size(); i++) {
			int index = tiles[i];
			if (index == 0) continue;

			auto actor = ObjectFactory::Instance().Create<Actor>(tilenames[index]);
			if (actor) {
				int x = i % numTilesX;
				int y = i / numTilesX;
				actor->transform.position = start + (gn::Vector2{ x, y } * size);
				scene->AddActor(std::move(actor));
			}
		}
	}

	bool TileMap::Write(const rapidjson::Value& value) const{
		return false;
	}

	bool TileMap::Read(const rapidjson::Value& value){
		JSON_READ(value, numTilesX);
		JSON_READ(value, numTilesY);
		JSON_READ(value, start);
		JSON_READ(value, size);
		JSON_READ(value, tilenames);
		JSON_READ(value, tiles);

		return true;
	}
}