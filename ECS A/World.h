#pragma once

#include <queue>
#include "stdafx.h"
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include "ComponentManager.h"
#include "BaseComponentManager.h"
#include <vector>
#include <bitset>
#include <set>
#include "System.h"
//using namespace std;

namespace ecsa {

	class Family;
	class FamilySubscription;

	typedef std::unordered_map<std::type_index, BaseComponentManager*> CompManagerMap;

	class World {
	public:
		World();
		~World();
		int create();
		void destroy(int entity);

		void update(float dt);

		void initialize();
		
		template<class T>
		void with();

		template<class T>
		ComponentManager<T>& getComponentManager(int initCapacity = 10);

		FamilySubscription& getSubscription(Family& family);

		std::vector<std::bitset<MAX_COMPONENT_COUNT>> aspects;
	private:

		int nextID;
		std::queue<int> purgatory;

		unsigned int nextCompId;
		CompManagerMap compManagerMap;

		std::vector<FamilySubscription*> subscriptions;

		std::vector<System*> systems;

		std::set<int> limbo; //entities to be deleted

		bool initialized;

		
	};

	template<class T>
	inline void World::with()
	{
		if (!initialized) {
			System* s = new T(*this);
			
			systems.emplace_back(s);
		}
	}

	template<class T>
	ComponentManager<T>& World::getComponentManager(int initCapacity)
	{
		if (compManagerMap.find(typeid(T)) == compManagerMap.end()) {
			compManagerMap[typeid(T)] = new ComponentManager<T>(initCapacity, aspects, nextCompId++);
		}

		return *static_cast<ComponentManager<T>*>(compManagerMap[typeid(T)]);
		/*consider reinterpret_cast
		** Safe here because we know manager can only be of type ComponentManager<T>.
		*/
	}

}