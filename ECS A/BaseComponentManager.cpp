#include "stdafx.h"
#include "BaseComponentManager.h"
#include <iostream>
namespace ecsa {


	BaseComponentManager::BaseComponentManager(unsigned int initial_capacity, std::vector<std::bitset<MAX_COMPONENT_COUNT>> &aspectVector, int componentId)
		: entities(initial_capacity, false), aspects(aspectVector), componentId(componentId)
	{
		//std::cout << "ComponentId: " << componentId << std::endl;
	}

	BaseComponentManager::~BaseComponentManager()
	{
	}

	bool BaseComponentManager::has(int e) {
		return (e < entities.size()) && entities[e];
	}

	void BaseComponentManager::addListener(ComponentListener * listener)
	{
		listeners.emplace_back(listener);
	}

	void BaseComponentManager::entitiesDeleted(const std::set<int>& deleted)
	{
		for (int e : deleted) {
			if (e < entities.size()) {
				if (entities[e]) {
					entities[e] = false;
					for (ComponentListener* l : listeners) {
						l->entityRemoved(e);
					}
				}
			}
		}
	}

}