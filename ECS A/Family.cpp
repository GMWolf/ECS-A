#include "stdafx.h"
#include "Family.h"

namespace ecsa {
	
	bool Family::has(int e)
	{
		return false;
	}



	bool AllFamily::has(int e)
	{
#if USE_ASPECTS
		return (aspect & world.aspects[e]) == aspect;
#else
		for (int i = 0; i < managers.size(); i++) {
			if (!managers[i]->has(e)) {
				return false;
			}
		}
		return true;
#endif
	}

	void AllFamily::registerListener(ComponentListener * listener)
	{
		for (int i = 0; i < managers.size(); i++) {
			managers[i]->addListener(listener);
		}
	}

	bool AllFamily::operator==(const Family & other)
	{
		if (typeid(other) == typeid(*this)) {
			return managers == ((AllFamily&)other).managers;
		}
		return false;
	}


	AllFamily::AllFamily(World& world, std::initializer_list<BaseComponentManager*> il) : Family(world) {
#if USE_ASPECTS
		aspect.reset();
#endif
		for (BaseComponentManager* i : il) {
#if USE_ASPECTS
			aspect.set(i->componentId);
#endif
			managers.push_back(i);
		}
	}
}