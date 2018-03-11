#pragma once
#include "stdafx.h"
#include <vector>
#include "ComponentListener.h"
#include "BaseComponentManager.h"
#include "OrderedIndexMap.h"
#include <bitset>
#include "HashedArrayTree.h"
#include "VectorStore.h"
namespace ecsa {

	template<typename T>
	class ComponentManager : public BaseComponentManager {
	public:
		ComponentManager(int initCapacity, std::vector<std::bitset<MAX_COMPONENT_COUNT>> &aspectVector, int componentId);
		~ComponentManager();
		T& get(int e);
		T& operator[](int e);
		template<typename... Ts>
		T& create(int e, Ts&&... ts);
		void destroy(int e) override;
		

	private:

		COMPONENT_STORE<T> components;
	};


	template<typename T>
	ComponentManager<T>::ComponentManager(int initCapacity, std::vector<std::bitset<MAX_COMPONENT_COUNT>> &aspectVector, int componentId) 
		: BaseComponentManager( initCapacity, aspectVector, componentId){
		//components.reserve(initCapacity);
	}

	template<typename T>
	ComponentManager<T>::~ComponentManager() {
	}

	template<typename T>
	template<typename... Ts>
	T& ComponentManager<T>::create(int e, Ts&&... ts) {

		//new (components + e) T(ts...);
		
		
		components.emplace(e, std::forward<Ts>(ts)...);

		if (e >= entities.size()) {
			entities.resize((e + 1), false);
		}

		entities[e] = true;

		for (ComponentListener* listener : listeners) {
			listener->entityAdded(e);
		}

#if USE_ASPECTS
		aspects[e].set(componentId);
#endif

		return get(e);
	}

	template<typename T>
	inline T& ComponentManager<T>::get(int e) {
		return components[e];
	}

	template<typename T>
	void ComponentManager<T>::destroy(int e) {
		if (entities[e]) { //Lets not delete what inst there... 

			components.erase(e);
			entities[e] = false;

			for (ComponentListener* l : listeners) {
				l->entityRemoved(e);
			}
		}
	}

	template<typename T>
	inline T & ComponentManager<T>::operator[](int e)
	{
		return get(e);
	}

	
}