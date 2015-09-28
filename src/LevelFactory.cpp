#include "LevelFactory.h"

//Total size of pointer arrays
const int LevelFactory::size = 20;
//Number of actors populating the level
int LevelFactory::num_actors = 0;
//Array holding pointers of actors populating the level
StrongActorPtr LevelFactory::actors[size];
//Holds filename of background to load
std::string LevelFactory::background;
//Holds level name
std::string LevelFactory::name;

/** Creates and populates a level and all its components based on XML configuration
 ** resource: filename for xml
 ** state: current game state
**/
void LevelFactory::CreateLevel(const char* resource, int* state) {
	//Reference to current location in Actor population array
	//Holds referenced to loaded XML file	
	num_actors = 0;
	pugi::xml_document doc;
	
	//Error check to see if file was loaded correctly
	pugi::xml_parse_result result;
	std::string resource_str(resource);
    	if (!(result = doc.load_file(("./assets/levels/" + resource_str + ".xml").c_str()))) {
		std::cout << "LevelFactory::CreateLevel(...): Failed to load" << std::endl;
		std::cout << "Filename: " << resource << " Load result: " << result.description() << std::endl;
	}

	//Used to iterate over XML file to get attributes
	pugi::xml_node tools = doc.child(resource);
	for (pugi::xml_attribute attr = tools.first_attribute(); attr; attr = attr.next_attribute()) {
		if (!strcmp(attr.name(), "Name"))
			name = attr.value();
		else if (!strcmp(attr.name(), "Background"))
			background = attr.value();
	}

	//Iterates over XML to get components to add
	for (pugi::xml_node tool = tools.first_child(); tool; tool = tool.next_sibling()) {
		actors[num_actors] = ActorFactory::CreateActor(tool.name(), state);
		actors[num_actors++]->PostInit(&tool);
	}
}

std::string LevelFactory::getName(void) {
	return name;
}

int LevelFactory::getNumActors(void) {
	return num_actors;
}

