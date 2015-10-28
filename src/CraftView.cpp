#include "CraftView.h"

//Total size of pointer arrays
const int CraftView::size = 20;

// Total number of flowers
int CraftView::fireFlowers = 0;
int CraftView::waterFlowers = 0;
int CraftView::earthFlowers = 0;
int CraftView::airFlowers = 0;
int CraftView::totalFlowers;
std::string CraftView::flower_str;
sf::Text CraftView::flower_text;

// holds list of flowers actor has collected
std::string CraftView::flower_list[size];
std::vector<StrongActorPtr> CraftView::actorList;

// Holds all of sprites to be drawn
sf::Sprite CraftView::sprites[size];
// Holds positions of said sprites
sf::Vector2f CraftView::positions[size];
// Size of sprites
sf::Vector2f CraftView::sizes[size];
// Texture of sprites
sf::Texture CraftView:: textures[size];

//Holds background texture
sf::Texture CraftView::background_texture;
//Holds background
sf::Sprite CraftView::background;
// Holds map sprite
sf::Sprite CraftView::map;
// Hold recipe book sprite
sf::Sprite CraftView::book;
// Holds sprite for box 1 and 2 of crafting table
sf::Sprite CraftView::box1Sprite;
sf::Sprite CraftView::box2Sprite;
//Hold text font
sf::Font CraftView::font;
// Holds text "craftable companion" will be speaking
sf::Text CraftView::text;
sf::Vector2f CraftView::text_pos;

sf::RectangleShape CraftView::backlay;
sf::RectangleShape CraftView::craftButton;
sf::Text CraftView::button_text;

bool CraftView::pressed;
// Used to determine if there is an item in both boxes of crafting table
bool CraftView::box1;
bool CraftView::box2;

/** Creates and populates a level and all its components based on XML configuration
 ** resource: filename for xml
 ** state: current game state
 **/
void CraftView::Create(const char* resource, int* state) {
    //Reference to current location in Actor population array
    //Holds referenced to loaded XML file	
    totalFlowers = 0;
    pugi::xml_document doc;

    //Error check to see if file was loaded correctly
    pugi::xml_parse_result result;
    std::string resource_str(resource);
    if (!(result = doc.load_file(("./assets/" + resource_str + ".xml").c_str()))) {
        std::cout << "CraftView::Create(...): Failed to load" << std::endl;
        std::cout << "Filename: " << resource << " Load result: " << result.description() << std::endl;
    }

    //Used to iterate over XML file to get attributes for this display -- currently none but background
    pugi::xml_node tools = doc.child(resource);
    char* temp;	
    
    for (pugi::xml_attribute attr = tools.first_attribute(); attr; attr = attr.next_attribute()) {
        // Background to hold general backdrop image, + static image of "crafting companion" 
        if (!strcmp(attr.name(), "Background")) {
            background_texture.loadFromFile(("./assets/backgrounds/" + (std::string)attr.value()).c_str());
            background = sf::Sprite(background_texture, sf::IntRect(0, 0, Configuration::getWindowWidth(),  Configuration::getWindowHeight()));
            background.setPosition(sf::Vector2f(0,0));
        }
        // Font for general text on screen
        else if (!strcmp(attr.name(), "Font")) {
            font.loadFromFile(("./assets/" + (std::string)attr.value()).c_str());
            text.setFont(font);
	        flower_text.setFont(font);
        }
        // Size of dialogue text
        else if (!strcmp(attr.name(), "Text_Size")) {
            text.setCharacterSize(std::strtol(attr.value(), &temp, 10));
	    flower_text.setCharacterSize(std::strtol(attr.value(), &temp, 10));
            if (*temp != '\0') {
                std::cout << "CraftView::Create: Error reading attribute for " << attr.name() << std::endl;
            }
        }
        // Pos of dialogue text
        else if (!strcmp(attr.name(), "Text_X")) {
            text_pos.x = (std::strtol(attr.value(), &temp, 10));
            if (*temp != '\0') {
                std::cout << "CraftView::Create: Error reading attribute for " << attr.name() << std::endl;
            }
        }
        // Pos of dialogue text
        else if (!strcmp(attr.name(), "Text_Y")) {
            text_pos.y = (std::strtol(attr.value(), &temp, 10));
            if (*temp != '\0') {
                std::cout << "CraftView::Create: Error reading attribute for " << attr.name() << std::endl;
            }
        }
        // texture and position of map icon so it can be interacted with 
        else if (!strcmp(attr.name(), "Map")) {
            sf::Texture texture;
    	    if (!texture.loadFromFile(("./assets/sprites/" + (std::string)attr.value()).c_str())){
    		  std::cout << "CraftView::Create: Failed to load " << attr.value();
    	    }
    	    map = sf::Sprite(texture, sf::IntRect(0, 0, Configuration::getWindowWidth()/26.6, Configuration::getWindowHeight()/26.6));
    	    map.setPosition(Configuration::getWindowWidth()/1.05,Configuration::getWindowHeight()/40);
        }
        else if(!strcmp(attr.name(), "Book")) {
	    sf::Texture texture;
	    if (!texture.loadFromFile(("./assets/sprites/" + (std::string)attr.value()).c_str())){
    		  std::cout << "CraftView::Create: Failed to load " << attr.value();
    	    }
    	    book = sf::Sprite(texture, sf::IntRect(0, 0, Configuration::getWindowWidth()/26.6, Configuration::getWindowHeight()/26.6));
	    book.setPosition(Configuration::getWindowWidth()/40,Configuration::getWindowHeight()/1.06);
	}
    }
    
    //Iterates over XML to get components to add, primarily sprites to populate the screen with 
    int i = 0;
    for (pugi::xml_node tool = tools.first_child(); tool; tool = tool.next_sibling(), i++) {
	  for (pugi::xml_attribute attr = tool.first_attribute(); attr; attr = attr.next_attribute()){
	      if (!strcmp(attr.name(), "Sprite")) {
		  if (!textures[i].loadFromFile(("./assets/sprites/" + (std::string)attr.value()).c_str())){
			std::cout << "CraftView::Create: Failed to load " << attr.value();
		  }
	      }
	      // pos of sprite
	      else if (!strcmp(attr.name(), "X")){
		  positions[i].x = std::strtol(attr.value(), &temp, 10);
		  if (*temp != '\0'){
		      std::cout << "CraftView::Create: Error reading attribute for " << tool.name() << std::endl;
		  }
	      }
	      else if (!strcmp(attr.name(), "Y")) {
		  positions[i].y =std::strtol(attr.value(), &temp, 10);
		  if (*temp != '\0') {
		    std::cout << "CraftView::Create: Error reading attribute for " << tool.name() << std::endl;
		  }
	      }
	      else if (!strcmp(attr.name(), "Width")){
		sizes[i].x = std::strtol(attr.value(), &temp, 10);
		if (*temp != '\0') {
		  std::cout << "CraftView::PostInit: Failed to post-initialize: Error reading attribute for " << attr.name() << std::endl;
		}
	      }
	      else if (!strcmp(attr.name(),"Height")) {
		sizes[i].y = std::strtol(attr.value(), &temp, 10);
		if (*temp != '\0') {
		  std::cout << "CraftView::PostInit: Failed to post-initialize: Error reading attribute for " << attr.name() << std::endl;
		}
	      }
	  }
    }
    
    // Setting dialogue text position
    text.setPosition(text_pos);
    text.setColor(sf::Color::Blue);
    flower_text.setPosition(200,180);
    flower_text.setColor(sf::Color::Black);
    
    // Backlay set off to the side to allow space for item select screens to the left
    backlay.setPosition(Configuration::getWindowWidth()/6.6, Configuration::getWindowHeight()/1.4);
    backlay.setOutlineColor(sf::Color::Black);
    backlay.setFillColor(sf::Color::White);
    backlay.setSize(sf::Vector2f(Configuration::getWindowWidth()/1.3,Configuration::getWindowHeight()/4));
    backlay.setOutlineThickness(5);
    
    button_text.setString("Craft");
    craftButton.setPosition(190, 170);
    craftButton.setFillColor(sf::Color::White);
    craftButton.setOutlineColor(sf::Color::Black);
    craftButton.setOutlineThickness(5);
    craftButton.setSize(sf::Vector2f(Configuration::getWindowWidth()/10, Configuration::getWindowHeight()/15));
    
    // render sprites on screen
    for (int count = 0; count < i; count++){
    	  sprites[count] = sf::Sprite(textures[count], sf::IntRect(0,0, textures[count].getSize().x, textures[count].getSize().y));
	  sprites[count].setScale(sizes[count].x/(textures[count].getSize()).x, sizes[count].y/(textures[count].getSize()).y);
	  sprites[count].setPosition(positions[count]);
    }
    
    // Checks player for current inventory, updates.
    // Accessing this info from LevelView::Player's CollectorComponent.
    if (LevelView::player != NULL){
      StrongActorPtr player = LevelView::player;
      StrongActorComponentPtr ac = player->components[CollectorComponent::id];
      std::shared_ptr<CollectorComponent> cc = std::dynamic_pointer_cast<CollectorComponent>(ac);
      std::vector<StrongActorPtr> flowers = cc->getFlowers();

      // add flowers to persistent list to make available to other classes
      CraftView::actorList.insert(CraftView::actorList.end(), flowers.begin(), flowers.end());
      
      // iterate through player's inventory to update inventory on screen
      for (int i=0; i < flowers.size() ; i++){
	// here, flowerList is a vector full of StrongActorPtrs. determine the id of each strongactorptr 
	// to determine if it is a fire flower, water flower, air flower, or earth flower
	  if (flowers[i]->getId() == "FireFlower"){
	      fireFlowers++;
	  }
	  else if (flowers[i]->getId() == "WaterFlower"){
	      waterFlowers++;
	  }
	  else if (flowers[i]->getId() == "AirFlower"){
	      airFlowers++;
	  }
	  else if (flowers[i]->getId() == "EarthFlower"){
	      earthFlowers++;
	  }
	  
	  //restore player's vases now that it's cleared space in inventory
	  totalFlowers++;
	  cc->setVases(cc->getVases()+1);
	  removeFlower(flowers[i]);
      }
    }
}

int CraftView::getNumFlowers(void) {
    return totalFlowers;
}

/** Handles events happening in craft view and listens for user input.
 **
 **/
void CraftView::update(sf::RenderWindow *window, int* state) {
  
      // Anticipates clicking in different areas of the screen 
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed) {
        pressed = true;
        const sf::Vector2i pos = sf::Mouse::getPosition(*window);
        if (map.getGlobalBounds().contains(pos.x, pos.y)) {
		LevelView::player->reset();
		  *state = 0;
        }
        
        // checking to see if flowers clicked on
        for (int i = 0; i <= 3; i++){
	    // player attempts to use this item for crafting, but only if one of the two boxes are filled
	    if (sprites[i].getGlobalBounds().contains(pos.x,pos.y)){
	      if (i == 0 && fireFlowers != 0 && (box1 == false || box2 == false)){
		fireFlowers--;
	      }
	      else if (i == 1 && waterFlowers !=0 && (box1 == false || box2 == false)){
		waterFlowers--;
	      }
	      else if (i == 2 && earthFlowers != 0 && (box1 == false || box2 == false)){
		earthFlowers--;
	      }
	      else if (i == 3 && airFlowers != 0 && (box1 == false || box2 == false)){
		airFlowers--;
	      }
	      if (box1 == false){
		 // draw sprite in box 1 
		  box1 = true;
		  box1Sprite = sprites[i];
          // scale the sprite up in size
          //box1Sprite.setScale(sprites[i].width*4, sprites[i].height*4);
		  box1Sprite.setPosition(150,100);
		}
	      else{
		 // draw sprite in box 2 
		box2 = true;
		box2Sprite = sprites[i];
		box2Sprite.setPosition(210, 100);
	      }
	    }
	}
	
	// Check here for the 'Craft' button to have been clicked and two flowers have been selected to be filled
	if (craftButton.getGlobalBounds().contains(pos.x, pos.y) && box1 == true && box2 == true){
	   //if (!EventManagerInterface::get()->queueEvent(new ContactEvent(0, owner->getInstance(), other_actor->getInstance())) )
             //           std::cout << "CraftView::update: Unable to queue event" << std::endl;
	   box1 = false;
	   box2 = false;
	   // Clear sprite image, add newly combined sprite to inventory?
	}

    // Check to see if flowers within the craft box are clicked to return them to player inventory
    if (box1Sprite.getGlobalBounds().contains(pos.x,pos.y)){
        std::cout << "CraftView::Update: Removing flower from craft table";
    }

    if (box2Sprite.getGlobalBounds().contains(pos.x,pos.y)){
        std::cout << "CraftView::Update: Removing flower from craft table";
    }
    }
      
      else if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left))) {
        pressed = false;
      }
      

}


/** Checks for events and update accordingly
 **
 **/
void CraftView::update(EventInterfacePtr e) {

}

/** Renders the backdrop and menu selects onto the window, as well as option to
 ** head back to map.
 **
 **/
void CraftView::render(sf::RenderWindow *window) {
   /////////// Going to put this somewhere else in the code to make it cleaner, just haven't figured out where yet

    std::string str = "Welcome back, Phil! You have " + std::to_string(totalFlowers) + " flowers!\nTo craft them, click on their icons."; 
    text.setString(str);

    //Update graphics	
    window->draw(background);
    window->draw(backlay);
    window->draw(text);
    window->draw(map);
    window->draw(book);
    window->draw(box1Sprite);
    window->draw(box2Sprite);
    window->draw(craftButton);
    window->draw(flower_text);
    
    for (int i = 0; i <= 3; i++){
	window->draw(sprites[i]);
	if (i == 0){
	  flower_str = "x " + std::to_string(fireFlowers);
	  flower_text.setPosition(sprites[i].getLocalBounds().left+60, sprites[i].getLocalBounds().top+20);
	}
	else if (i ==1){
	  flower_str = "x " + std::to_string(waterFlowers);
	  flower_text.setPosition(sprites[i].getLocalBounds().left+60, sprites[i].getLocalBounds().top+80);
	}
	else if (i == 2){
	  flower_str = "x " + std::to_string(earthFlowers);
	  flower_text.setPosition(sprites[i].getLocalBounds().left+60, sprites[i].getLocalBounds().top+140);
	}
	else if (i == 3){
	  flower_str = "x " + std::to_string(airFlowers);
	  flower_text.setPosition(sprites[i].getLocalBounds().left+60, sprites[i].getLocalBounds().top+200);
	}
	flower_text.setString(flower_str);
	window->draw(flower_text);
    }
    
}


/** Clean up level after completion
 **
 **/
void CraftView::cleanUp(void) {

            }

/** Quit level
 **
 **/
void CraftView::quit(void) {
    cleanUp();
}

/** Returns a pointer to flower instance, if present
 **
 **/
StrongActorPtr CraftView::getFlower(int instance) {
	for (auto it = CraftView::actorList.begin(); it != CraftView::actorList.end(); it++) {
		if ((*it)->getInstance() == instance) {
			return *it;
		}
	}
	return nullptr;
}

/** Removes the given flower, if present
 ** TODO: does not modify counts of WEFA flowers or the flower_list
 **/
bool CraftView::removeFlower(StrongActorPtr flower) {
	for (auto it = CraftView::actorList.begin(); it != CraftView::actorList.end(); it++) {
		if ( *(*it) == (*flower) ) {
			CraftView::actorList.erase(it);
			return true;
		}
	}

	return false;
}
