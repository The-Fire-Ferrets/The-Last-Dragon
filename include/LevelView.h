#ifndef LEVELVIEW_H
#define LEVELVIEW_H

#include "Constants.h"
#include "ActorFactory.h"
#include "AI.h"

class LevelView  { friend class AI;
	friend class CraftView;
	private:
		static const int size;
		static int num_actors;
		static sf::Texture background_texture;
		static sf::Texture edge_texture;
		static sf::Sprite background;
		static sf::Sprite edge;
		static std::string name;
		static sf::Clock level_clock;
		static sf::Text timer;
		static std::string timer_string;
		static sf::Font font;
		static sf::Vector2f timer_position;
		static sf::View gameView;
		static sf::View minimapView;
		static sf::Sprite minimap_border;
		static int duration;
		static sf::SoundBuffer buffer;
		static sf::Sound sound;
		static int view_state;
		static sf::Text commentary;
		static std::vector<sf::Vector2f> commentary_positions;
		static std::vector<std::string> commentary_strings;
		static std::string fitStringToCommentaryBox(std::string str);

	public:
		static std::vector<StrongActorPtr> actorList;	
		static StrongActorPtr player;
		static std::string getName(void);
		static int getNumActors(void);
		static void start();
		static void quit(void);
		static StrongActorPtr getActor(int instance);
		static void removeActor(int instance);
		static void cleanUp(void);

		static void Create(const char* resource, int* state, int flowers[]);
		static void update(sf::RenderWindow *window, int* state, float time);
		static void update(EventInterfacePtr e);
		static void render(sf::RenderWindow *window);
};

#endif
