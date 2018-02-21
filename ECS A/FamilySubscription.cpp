#include "stdafx.h"
#include "FamilySubscription.h"
#include <iostream>

namespace ecsa {
	FamilySubscription::FamilySubscription(Family& family) : family(family)
	{
		family.registerListener(this);
	}


	FamilySubscription::~FamilySubscription()
	{
	}

	void FamilySubscription::entityAdded(int e) {
		updateEntity(e);
	}
	void FamilySubscription::entityRemoved(int e)
	{
		updateEntity(e);
	}

#if LATE_SUBSCRIPTIONS_RESOLVE
	void FamilySubscription::update()
	{
		if (!dirty_entities.empty()) {
			for (int e = 0; e < dirty_entities.size(); e++) {
				if (dirty_entities[e]) {
					if (e >= entities_flag.size()) {
						entities_flag.resize(e + 1, false);
					}

					entities_flag[e] = family.has(e);
				}
			}

			dirty_entities.clear();
			
			entities.clear();
			for (int e = 0; e < entities_flag.size(); e++) {
				if (entities_flag[e]) {
					entities.emplace_back(e);
				}
			}

		}
	}

	void FamilySubscription::updateEntity(int e)
	{
		if (e >= dirty_entities.size()) {
			dirty_entities.resize(e + 1, false);
		}

		dirty_entities[e] = true;
	}
#else


	void FamilySubscription::updateEntity(int e)
	{
		if (family.has(e)) {
			entities.insert(e);
		}
		else {
			entities.erase(e);
		}
	}
#endif //LATE_SUBSCRIPTIONS

	
}