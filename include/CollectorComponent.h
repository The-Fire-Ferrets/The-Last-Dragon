#ifndef COLLECTORCOMPONENT_H
#define COLLECTORCOMPONENT_H

#include "Constants.h"
#include "ActorComponent.h"
#include "ActorFactory.h"

class CollectorComponent : public ActorComponent {

	public:
		CollectorComponent();
		bool Init(pugi::xml_node* elem) override;
		void PostInit(void) override;

		void update(float time) override;
		void reset(void) override;
		void restart(void) override;
		void quit(void) override;

		void setVases(int b);
		int getVases(void);
		
		static ComponentId id;
		ComponentId getId(void) override;
		
	private:
		static ActorComponent* create();
		int vases;
		static int instances;
		static const bool registered;
};

#endif
