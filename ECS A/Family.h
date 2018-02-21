#pragma once
#include <vector>
#include <typeindex>
#include <typeinfo>
#include "World.h"
#include "ComponentManager.h"
#include "ComponentListener.h"

namespace ecsa {

	class Family {
	public:
		Family(World& world) : world(world) {
		};

		virtual bool has(int e) = 0;
		virtual void registerListener(ComponentListener* listener) = 0;

		virtual bool operator==(const Family& other) = 0;

	protected:
		World & world;
	};


	class AllFamily: public Family {
	public:
		template<class... types>
		static AllFamily get(World& world) {
			return AllFamily(world, {&world.getComponentManager<types>()...});
		}

		bool has(int e) override;
		void registerListener(ComponentListener* listener);

		bool operator==(const Family& other) override;

	//private:
		AllFamily(World& world, std::initializer_list<BaseComponentManager*> il);
#if USE_ASPECTS
		std::bitset<MAX_COMPONENT_COUNT> aspect;
#endif
		std::vector<BaseComponentManager*> managers;

	};


	//base and
	inline bool varAnd(bool b) {
		return b;
	}
	//rec and. (hopefully inline)
	inline bool varAnd(bool arg1, bool args...) {
		return arg1 && varAnd(args);
	}



}
