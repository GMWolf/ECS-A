#pragma once
#include "ComponentListener.h"
#include <set>
#include "Family.h"
#include "Properties.h"


namespace ecsa {

	class FamilySubscription: public ComponentListener{
	public:
		FamilySubscription(Family& family);
		FamilySubscription(const FamilySubscription& fs) = delete; //NO!
		FamilySubscription& operator=(const FamilySubscription& fs) = delete;//No!
		~FamilySubscription();

		void entityAdded(int e) override;
		void entityRemoved(int e) override;

		auto begin() const{
			return entities.begin();
		}
		auto end() const{
			return entities.end();
		}

#if LATE_SUBSCRIPTIONS_RESOLVE
		void update();
#endif // LATE_SUBSCRIPTIONS

		
	private:
		Family& family;

		//Storing entities
#if LATE_SUBSCRIPTIONS_RESOLVE
		vector_bool dirty_entities;
		vector_bool entities_flag;
		std::vector<int> entities;
#else
		std::set<int> entities;
#endif 
		void updateEntity(int e);
		
		//Reordering the vector is less intensive than 
		//looping anch checking all entities every step
	};

}