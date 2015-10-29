#include "CraftableComponent.h"
#include "CraftEvent.h"
#include "CraftView.h"

//Unique instance id among instances of the same component
int CraftableComponent::instances = -1;
//Unique component id
ComponentId CraftableComponent::id = "CraftableComponent";
//Registers component with factory
const bool CraftableComponent::registered = ActorFactory::registerComponent(&id, &create);

/** Returns the id shared by all components of this type
 **
 **/
ComponentId CraftableComponent::getId(void) const {
    return id;
}

/** Returns a reference to the components constructor
 **
 **/
ActorComponent* CraftableComponent::create() { 
    //update the instance count
    instances++;
    return new CraftableComponent();
}

/** Constructor
 ** Sets up unique instance ID
 **/
CraftableComponent::CraftableComponent(void) {
    instance = instances;
}

/** Destructor
 **
 **/
CraftableComponent::~CraftableComponent() {
	elements.clear();
}

/** Initializer
 ** elem : node pointing to section of XML configuration holding more attribute defaults to setup
 ** Sets up additional attribute defaults
 **/
bool CraftableComponent::Init(pugi::xml_node* elem) {	
    //Iterate over the component's attributes
    for (pugi::xml_node tool = elem->first_child(); tool; tool = tool.next_sibling()) {
        for (pugi::xml_attribute attr = tool.first_attribute(); attr; attr = attr.next_attribute()) {
            if (!strcmp(attr.name(), "Type")) {
                type = attr.value();
				elements.push_back(type);
            }
        }
    }
    return true;
}

/** Renders component
 ** window: current game render window
 **/
void CraftableComponent::render(sf::RenderWindow *window, bool minimap) {
	
}
/** Final Initilizer
 ** Setups up additional attributes based on game configuration
 **/
bool CraftableComponent::PostInit(pugi::xml_node* elem) {
	if (elem != NULL)
	Init(elem);
    owner->addDelegate(CraftEvent::event_type);
	return true;
}

/** Updates the component's attributes
 ** time: current game time
 **/
void CraftableComponent::update(float time) {

}

/** Receives event when the actor is being contacted by another actor and responds by accordingly
 **
 **/
void CraftableComponent::update(EventInterfacePtr e) {
    EventType event_type = e->getEventType();
    StrongActorPtr sender = CraftView::getFlower(e->getSender());
	StrongActorPtr receiver = CraftView::getFlower(e->getReceiver());


	// item crafted
    if (event_type == CraftEvent::event_type && this->owner == receiver) {
		// get a pointer to the sender's colletable component
		std::shared_ptr<ActorComponent> ac;
		std::shared_ptr<CraftableComponent> cc;

		ac = sender->components[CraftableComponent::id];
		cc = std::dynamic_pointer_cast<CraftableComponent>(ac);

		// deep copy all elements
		this->elements.insert(this->elements.end(), cc->elements.begin(), cc->elements.end());

		CraftView::removeFlower(sender);

		// update the reference counter, delete if counter goes to 0
		sender.reset();
    }
}

/** Reset the component
 **
 **/
void CraftableComponent::reset(void) {

}

/** Restart the component
 **
 **/
void CraftableComponent::restart(void) {

}

/** Cleans up after quiting
 **
 **/
void CraftableComponent::quit(void) {

}
