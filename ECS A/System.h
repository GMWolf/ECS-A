#pragma once
namespace ecsa {

	class World;

	class System
	{
	public:
		System(World& world) : world(world){
		};
		virtual ~System() {
		};

		virtual void initialize() {

		}

		virtual void update(float dt) {

		}

	protected:
		World& world;
	};

}

