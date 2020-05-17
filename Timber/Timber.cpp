

#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>




using namespace sf;

 class SpriteHolder{
	protected:
	 Sprite sprite;

 public:
	 SpriteHolder(const Texture & texture) {
		 sprite.setTexture(texture);
	 }

	 void draw(RenderWindow & window) {
		 window.draw(sprite);
	 }
	 void setPosition(const int x, const int y) {
		 sprite.setPosition(x, y);
	 }

	 //should make virtual update and setup functions here 

 };

class Bee : public SpriteHolder{
	private:
		bool _isMoving;
		float _speed;
		void setup() {
			srand((int)time(0) * 10 );
			//setup the bee
			const int minSpeed = 150;
			const int speedRange = 400;
			setSpeed((rand() % speedRange) + minSpeed);
			const int minHeight = 500;
			const int heightRange = 500;
			setPosition(2000, (rand() % heightRange) + minHeight  );
			setActive(true);
		}

	public:
		Bee(const Texture & beeTexture, float speed = 0.0) : SpriteHolder{ beeTexture },  _speed { speed }, _isMoving{ false } {
			sprite.setPosition(300, 300);
		}

		bool isMoving() {
			return _isMoving;
		}

		void setSpeed(const float newSpeed) {
			_speed = newSpeed;
		}

		void setActive(bool isActive) {
			_isMoving = isActive;
		}

		void update(Time deltaTime) {
			if (isMoving()) {
				sprite.move(-(_speed * deltaTime.asSeconds()), 0);
				bool isOffscreen = sprite.getPosition().x < -100;
				if (isOffscreen) {
					setActive(false);
				}
			}
			else {
				setup();
			}
		}

};

class Cloud : public SpriteHolder {
	private:
		bool _isActive;
		float _speed;
		//used to track the number of clouds in the secene to generte random seeds when initilazing cloud behivior;
		static int numberOfClouds;
		
		void setup() {
			//seed variance for setting up multiple clouds at the same time
			static int seedVariance = 1;
			seedVariance = (seedVariance) % numberOfClouds + 1;
			srand(int (time(0) * seedVariance));
			const int minSpeed = 20;
		    const int speedRange = 200;
			_speed = rand() % speedRange + minSpeed;
			 const int minHeight = 0;
			 const int heightRange = 250;
			sprite.setPosition(-300, rand() % heightRange + minHeight);
			_isActive = true;
		}

	public:
		Cloud(const Texture & textureCloud, float speed = 0.0f) : SpriteHolder{ textureCloud }, _speed(speed) {
			_isActive = false;
			++Cloud::numberOfClouds;
		}
		//copy constructor
		Cloud(const Cloud & rhs) : SpriteHolder{ SpriteHolder(rhs) }, _speed{ rhs._speed }, _isActive{ rhs._isActive } {
			++Cloud::numberOfClouds;
		};

		~Cloud() {
			--numberOfClouds;
		}
		bool isActive() {
			return _isActive;
		}

		void update(const Time & deltaTime){
			if (isActive()) {
				sprite.move(_speed * deltaTime.asSeconds(),0);
				bool isOffscreen = sprite.getPosition().x > 1920;
				if (isOffscreen){
					_isActive = false;
				}
			}
			else {
				setup();
			}
		}
};

int Cloud::numberOfClouds = 0;

int main()
{
	//create a video mode object
	VideoMode vm(1920, 1080);
	//create and open a window for the game 
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
	window.setMouseCursorVisible(false);
	//create a texture to hold a graphic on the GPU
	Texture textureBackground;
	Texture textureTree;
	Texture textureBee;
	Texture textureCloud;
	//Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");
	textureTree.loadFromFile("graphics/tree.png");
	textureBee.loadFromFile("graphics/bee.png");
	textureCloud.loadFromFile("graphics/cloud.png");
	  
	//create a sprite
	Sprite spriteBackground;
	Sprite spriteTree;
	Bee bee{ textureBee };
	const int numberOfClouds = 3;
	std::vector<Cloud> clouds(numberOfClouds, Cloud(textureCloud));
	//attach texture to sprite
	spriteBackground.setTexture(textureBackground);
	spriteTree.setTexture(textureTree);
	
	//set the Background to the screen 
	spriteBackground.setPosition(0, 0);
	spriteTree.setPosition(810, 0);

	//set random seed


	sf::Clock clock;




	//main loop
	while (window.isOpen()) {
		/********************************
		handle the player input
		********************************/
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}
		/********************************
		update the scene
		********************************/
		Time deltaTime = clock.restart();
		//move bee
		bee.update(deltaTime);
		for (Cloud & cloud : clouds) {
			cloud.update(deltaTime);
		}


		/********************************
		Draw the scene
		********************************/
		window.clear();
		window.draw(spriteBackground);
		//draw clouds
		for (Cloud & cloud : clouds) {
			cloud.draw(window);
		}
		window.draw(spriteTree);
		bee.draw(window);



		//draw everything
		//show everthing
		window.display();
	}

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
