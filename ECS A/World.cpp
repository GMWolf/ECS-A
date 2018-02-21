#include "stdafx.h"
#include "World.h"
#include "System.h"
#include <iostream>
#include "Family.h"
#include "FamilySubscription.h"
namespace ecsa {
	World::World() : initialized(false)
	{
		nextID = 0;
		nextCompId = 0;
	}

	World::~World()
	{
		//TODO: Clean all this mess
	}
	
	int World::create()
	{
		int id;
		if (purgatory.empty()) {
			id = nextID++;
		}
		else {
			int e = purgatory.front();
			purgatory.pop();
			id = e;
		}

		if (id >= aspects.size()) {
			aspects.resize(id + 1);
		}

		aspects[id].reset();

		return id;
	}

	void World::destroy(int entity) {
		limbo.insert(entity);
	}

	void World::update(float delta) {
		//update all systems
		for (System* system : systems) {
			system->update(delta);
		}

		//deal with limbo
		if (!limbo.empty()) {
			for (auto c : compManagerMap) {
				c.second->entitiesDeleted(limbo);
			}
			for (int e : limbo) {
				//recycle id
				purgatory.push(e);
			}

			limbo.clear();
		}

		//Update subscriptions
#if LATE_SUBSCRIPTIONS_RESOLVE
		for (FamilySubscription* sub : subscriptions) {
			sub->update();
		}
#endif //LATE_SUBSCRIPTIONS
	}

	void World::initialize() {
		if (!initialized) {
			for (System* system : systems) {
				system->initialize();
			}
			initialized = true;
		}
	}

	FamilySubscription & World::getSubscription(Family & family)
	{
		FamilySubscription* sub = new FamilySubscription(family);
		subscriptions.push_back(sub);
		return *sub;
	}

}