#pragma once
#include<vector>
#include"ComponentListener.h"
#include<set>
#include<bitset>
#include "Properties.h"

namespace ecsa {

	class BaseComponentManager
	{
	public:
		BaseComponentManager(unsigned int initial_capacity, std::vector<std::bitset<MAX_COMPONENT_COUNT>> &aspectVector, int componentId);
		virtual ~BaseComponentManager();

		bool has(int e);
		void addListener(ComponentListener* listener);

		void entitiesDeleted(const std::set<int>& deleted);

		std::vector<std::bitset<MAX_COMPONENT_COUNT>>& aspects;
		int componentId;
		virtual void destroy(int e) = 0;
	protected:
		
		vector_bool entities;
		std::vector<ComponentListener*> listeners;
		
	};

}