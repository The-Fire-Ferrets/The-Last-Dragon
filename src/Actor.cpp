#include "Actor.h"

//unique instance id among actors
int Actor::instances = 0;

/** Gets the current actors unique instance ID
 **
**/
/** Returns the actors unique ID
int Actor::getInstance(void) {
	return instance;
}

 **
**/
ActorId Actor::getId(void) {
	return id;
}

/** Constructor
 ** actorid : unique id for each actor
 ** Sets some attributes to their defaults
**/
Actor::Actor(void) {
	instance = instances++;
	state = 0;
}

/** Initializer
 ** elem : node pointing to section of XML configuration holding more attribute defaults to setup
 ** Sets up additional attribute defaults
**/
bool Actor::Init(pugi::xml_node* elem) {
	for (pugi::xml_attribute attr = elem->first_attribute(); attr; attr = attr.next_attribute()) {
		if (!strcmp(attr.name(),"Type"))
  			id = attr.value(); 
		else if (!strcmp(attr.name(),"Texture"))
  			texture_filename = attr.value();
		else if (!strcmp(attr.name(),"Sprite"))
			sprite_filename = attr.value();
		else {
			std::cout << "Actor::Init: Failed to initialize" << std::endl;
			return false;
		}
	}
	
	boundary = new sf::FloatRect();
	return true;
}

/** Destructor
**/
Actor::~Actor(void) {

}

/** Final Initializer
 ** Setups up additional attributes based on game configuration
**/
void Actor::PostInit(pugi::xml_node* elem) {
	char* temp;
	for (pugi::xml_node tool = elem->first_child(); tool; tool = tool.next_sibling()) {
		for (pugi::xml_attribute attr = tool.first_attribute(); attr; attr = attr.next_attribute()) {
			if (!strcmp(attr.name(),"X")) {
				position.x = std::strtol(attr.value(), &temp, 10);
				if (*temp != '\0') {
					std::cout << "Actor::PostInit: Failed to post-initialize: Error reading attribute for " << attr.name() << std::endl;
				}
			}
			else if (!strcmp(attr.name(),"Y")) {
				position.y = std::strtol(attr.value(), &temp, 10);
				if (*temp != '\0') {
					std::cout << "Actor::PostInit: Failed to post-initialize: Error reading attribute for " << attr.name() << std::endl;
				}
			}
			else if (!strcmp(attr.name(),"Width")) {
				size.x = std::strtol(attr.value(), &temp, 10);
				if (*temp != '\0') {
					std::cout << "Actor::PostInit: Failed to post-initialize: Error reading attribute for " << attr.name() << std::endl;
				}
			}
			else if (!strcmp(attr.name(),"Height")) {
				size.y = std::strtol(attr.value(), &temp, 10);
				if (*temp != '\0') {
					std::cout << "Actor::PostInit: Failed to post-initialize: Error reading attribute for " << attr.name() << std::endl;
				}
			}
		}
	}

	boundary = new sf::FloatRect(position.x, position.y, size.x, size.y);
	texture.loadFromFile(("./assets/textures/" + texture_filename).c_str());
	sprite = sf::Sprite(texture, sf::IntRect(position.x, position.y, size.x, size.y));
	sprite.setPosition(position);
}


/** Moves the actor a certain distance based on the current game time
 ** time: current game time
 ** Prevents actors from moving past the top and bottom boundaries
 ** if the obstacle pointer is set (by a physics component o fthe actor or another actor after contact) it prevents the actors from moving into each other
**/
void Actor::move(float time) {	
	//Move Actor
	sprite.setPosition(position);
}

/** Updates each of the actor's components
 ** time: current game time
**/
void Actor::update(float time) {
	for (ActorComponents::iterator it = components.begin(); it != components.end(); ++it) {
		(it->second)->update(time);
	}
}

/** Renders each of the actors components
 ** window: current game render window
**/
void Actor::render(sf::RenderWindow *window) {
	window->draw(sprite);
}

/** Reset each of the actors components after scoring
 **
**/
void Actor::reset(void) {
	for (ActorComponents::iterator it = components.begin(); it != components.end(); ++it) {
		(it->second)->reset();
	}
}

/** Restarts each of the actors components after someone wins
 **
**/
void Actor::restart(void) {
	for (ActorComponents::iterator it = components.begin(); it != components.end(); ++it) {
		(it->second)->restart();
	}
}

/** Cleans up each of the actors components after quiting
 **
**/
void Actor::quit(void) {
	for (ActorComponents::iterator it = components.begin(); it != components.end(); ++it) {
		(it->second)->quit();
	}
}

/** Adds a component to the actor
 ** component: pointer to ActorComponent ot be added
**/
void Actor::AddComponent(StrongActorComponentPtr component) {
	components.insert({component->getId(), component});
}

/** Updates the actors boundaries to be used by a PhysicsComponent
 ** 
**/
void Actor::updateBoundary(void) {
	*boundary = sf::FloatRect(position.x, position.y, size.x, size.y);
}


// Mutators and accesors

/** returns the actors boundary
 **
**/
sf::FloatRect* Actor::getBoundary(void) {
	return boundary;
}

/** returns the actors state
 **
**/
int Actor::getState(void) {
	return state;
}

/** sets the actors state
 ** actorState: new state to have actor in
**/
void Actor::setState(int actorState) {
	state = actorState;
}

/** Returns the last game state the actor was aware of
 **
**/
int* Actor::getGameState(void) {
	return game_state;
}

/** Makes the actor aware of the current game state
 **
**/
void Actor::setGameState(int state) {
	*game_state = state;
}

/** Changes the game state
 **
**/
void Actor::resetGameState(int* state) {
	game_state = state;
}

/** Sets the height and width dimensions of actor
 ** sz: holds height and width data
**/
void Actor::setSize(sf::Vector2f sz) {
	size = sz;
	updateBoundary();
}

/** Returns the actors size
 **
**/
sf::Vector2f Actor::getSize() {
	return size;
}

/** Reutrns the actor's current position
 **
**/
sf::Vector2f Actor::getPosition(void) {
	return position;
}

/** Sets the actors to the new position
 **
**/
void Actor::setPosition(sf::Vector2f pos) {
	position = pos;
	updateBoundary();
}

/** Set the actors direction
 **
**/
void Actor::setDirection(int dir) {
	direction = dir;
}

/** Return the actors current direction
 **
**/
int Actor::getDirection(void) {
	return direction;
}

/** Return the actors instance
 **
**/
int Actor::getInstance(void) {
	return instance;
}
