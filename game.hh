#pragma once
#include <string>

namespace game {

	class thing {
	public:
		std::string name;
		int thing_id;
	};

	class character {
	public:
		int mapx;
		int mapy;
		float health;
		float hit_dmg;
		float max_hp;
		std::string name;
	};
}
