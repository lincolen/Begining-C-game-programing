

#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include<cassert>
#include <vector>
#include <sstream>




using namespace sf;

enum side { LEFT, RIGHT, NONE, BOTH };

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

 void setTextOriginToCenter(Text & text) {
   FloatRect textRect = text.getLocalBounds();
   text.setOrigin(textRect.width / 2, textRect.height / 2);
 }

 class Axe : public SpriteHolder {
 public:
   Axe(const Texture & textureAxe);
   void moveRight();
   void moveLeft();

 private:
   //line up axe with tree
   const float AXE_POSITION_LEFT = 700;
   const float AXE_POISITON_RIGHT = 1075;
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

class Score {
  public:
    void reset();
    void increment();
    int getScore() const;

 private:
   int _score = 0;
};

class Player : public SpriteHolder {
public:
  Player(const Texture & texturePlayer);
  void reset();
  void update(const Time & deltaTime, Score & score);
  void setAxe(Axe & axe) {
    _axe = &axe;
  }
  side getSide() const{
    return _playerSide;
  }

private:
  side _playerSide = side::LEFT;
  bool _acceptInput = true;
  Axe * _axe = nullptr;

  void chopRight();
  void chopLeft();
};

class Log : public SpriteHolder {
 public:
   Log(const Texture & textureLog);
   void flyRight();
   void flyLeft();
   void update(const Time & deltaTime) {
     if (active) {
       setPosition(sprite.getPosition().x + speedX * deltaTime.asSeconds(), sprite.getPosition().y + speedY * deltaTime.asSeconds());
     }
   }

 private:
   bool active = false;
   float speedX = 1000;
   float speedY = -1500;
};


class TimeBar{
 private:
    RectangleShape _bar; 
    const float timeBarStartWidth = 400;
    const float timeBarDefaultHeight = 80;
    const Color _color = Color::Red;
    float timeRemaining = 6.0f;
    float timeBarConsumedPerSecond = timeBarStartWidth / timeRemaining;

 public:
  TimeBar()  {
    _bar.setSize(Vector2f(timeBarStartWidth, timeBarDefaultHeight));
    _bar.setFillColor(_color);
    _bar.setPosition(1920 / 2 - timeBarStartWidth / 2, 980);
  }

  void draw(RenderWindow & window) {
    window.draw(_bar);
  }

  void reset() {
    _bar.setSize(Vector2f(timeBarStartWidth, _bar.getSize().y));
  }

  void update(Time deltaTime) {
    _bar.setSize(Vector2f(_bar.getSize().x - timeBarConsumedPerSecond * deltaTime.asSeconds(), _bar.getSize().y));
  }

  void increment(const int score) {
    _bar.setSize(Vector2f(_bar.getSize().x + 0.15 * timeBarConsumedPerSecond * (2.0 / score), _bar.getSize().y));
  }

  bool timeIsUp() {
    return _bar.getSize().x <= 0.0f;
  }


};


void updateBranches(const int seed, Sprite * branches, side * branchPositions);
int main();
void setUpBranches(Sprite * branches, side * branchPositions);

const int NUMBER_OF_BRANCHES = 6;





int main()
{
	//create a video mode object
	VideoMode vm(1920, 1080);
	//create and open a window for the game 
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
	window.setMouseCursorVisible(false);
	//create a texture to hold a graphic on the GPU
  Texture textureAxe;
  Texture textureBackground;
  Texture textureBee;
  Texture textureBranch;
  Texture textureCloud;
  Texture texturePlayer;
  Texture textureLog;
  Texture textureRIP;
  Texture textureTree;
	

	Font font;
	
	//Load a graphic into the texture
  textureAxe.loadFromFile("graphics/axe.png");
	textureBackground.loadFromFile("graphics/background.png");
  textureBranch.loadFromFile("graphics/branch.png");
	textureTree.loadFromFile("graphics/tree.png");
	textureBee.loadFromFile("graphics/bee.png");
	textureCloud.loadFromFile("graphics/cloud.png");
  texturePlayer.loadFromFile("graphics/player.png");
  textureLog.loadFromFile("graphics/log.png");
  textureRIP.loadFromFile("graphics/rip.png");
  font.loadFromFile("Fonts/KOMIKAP_.ttf");
	  
	/****************
	staging area
	****************/
  Score score;
	Sprite spriteBackground;
	Sprite spriteTree;
  Sprite spriteRIP;
	Bee bee{ textureBee };
	const int numberOfClouds = 3;
	std::vector<Cloud> clouds(numberOfClouds, Cloud(textureCloud));
	Text startGameText;
	Text scoreText;
  TimeBar timeBar;
  Sprite branches[NUMBER_OF_BRANCHES];
  side branchPositions[NUMBER_OF_BRANCHES];
  Player player(texturePlayer);
  Axe axe(textureAxe);
  player.setAxe(axe);
  Log log(textureLog);

	//attach texture to sprite
  for (int i = 0; i < NUMBER_OF_BRANCHES; ++i) {
    branches[i].setTexture(textureBranch);
    FloatRect branchSize = branches[i].getLocalBounds();
    branches[i].setOrigin(Vector2f(branchSize.width / 2, branchSize.height / 2));

  }
	spriteBackground.setTexture(textureBackground);
	spriteTree.setTexture(textureTree);
  spriteRIP.setTexture(textureRIP);

	//timeBar
	

	//set up text
	startGameText.setString("Press Enter to start!");
	startGameText.setCharacterSize(75);
	startGameText.setFillColor(Color::White);
	startGameText.setFont(font);
	//set origin to center
  setTextOriginToCenter(startGameText);
	startGameText.setPosition(vm.width / 2, vm.height / 2);

	scoreText.setString("Score = ");
	scoreText.setCharacterSize(100);
	scoreText.setFillColor(Color::White);
	scoreText.setFont(font);
	scoreText.setPosition(20, 20);

	
	//set the Background to the screen 
	spriteBackground.setPosition(0, 0);
	spriteTree.setPosition(810, 0);
  spriteRIP.setPosition(600, 2000);



	sf::Clock clock;
	bool isGamePaused = true;


  for (int i = 0; i < NUMBER_OF_BRANCHES; ++i) {
    branchPositions[i] = side::NONE;
  }

  int scoreLastCycle = score.getScore();
  auto isChoped = [&]() {return !(score.getScore() == scoreLastCycle);};
	//main loop
	while (window.isOpen()) {
		/********************************
		handle the player input
		********************************/
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}
		if (isGamePaused == true && Keyboard::isKeyPressed(Keyboard::Enter)) {
			isGamePaused = false;
      timeBar.reset();
      score.reset();
    //remove branches  
      for (int i = 0; i < NUMBER_OF_BRANCHES; ++i) {
        branchPositions[i] = side::NONE;
      }
      //remove graveStone
      spriteRIP.setPosition(675, 2000);
      
      player.reset();

		}
		/********************************
		update the scene
		********************************/
		Time deltaTime = clock.restart();
		if (!isGamePaused) {
      player.update(deltaTime, score);
      if (isChoped()) {
        timeBar.increment(score.getScore()); //increase time
        player.getSide() == side::LEFT ? log.flyRight() : log.flyLeft(); //send log flying
        updateBranches((int)time(0), branches, branchPositions);

      }
      log.update(deltaTime);
      //move bee
			bee.update(deltaTime);
      //move clouds
			for (Cloud & cloud : clouds) {
				cloud.update(deltaTime);
			}
      //shrink time
      timeBar.update(deltaTime);
      //timeup
      if (timeBar.timeIsUp()) {
        isGamePaused = true;
        startGameText.setString("Time is up!");
        setTextOriginToCenter(startGameText);
      }
		}
		scoreText.setString("score =" + score.getScore());

    setUpBranches(branches, branchPositions);
    scoreLastCycle = score.getScore();

		/********************************
		Draw the scene
		********************************/
		window.clear();

    //darw background
		window.draw(spriteBackground);

		//draw clouds
		for (Cloud & cloud : clouds) {
			cloud.draw(window);
		}

    //draw tree
		window.draw(spriteTree);

    //draw bee
		bee.draw(window);

    //draw branches
    for (int i = 0; i < NUMBER_OF_BRANCHES; ++i) {
      window.draw(branches[i]);
    }

    player.draw(window);
    axe.draw(window);
    window.draw(spriteRIP);
    log.draw(window);

    //draw text
		window.draw(scoreText);
		if (isGamePaused) {
			window.draw(startGameText);
    }
    else {
      timeBar.draw(window);
    }



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

void setUpBranches(Sprite * branches, side * branchPositions) {
  for (int i = 0; i < NUMBER_OF_BRANCHES; ++i) {
    float height = i * 150;
    switch (branchPositions[i]) {
      case side::LEFT:
        branches[i].setPosition(610, height);
        branches[i].setRotation(180);
        break;
      case side::RIGHT:
        branches[i].setPosition(1330, height);
        branches[i].setRotation(0);
      default:
        //hides branch
        branches[i].setPosition(-3000, height);
    }
  }

}

void updateBranches(const int seed, Sprite *branches, side * branchPositions) {
  //move branches down 
  for (int i = 0; i < NUMBER_OF_BRANCHES - 1; ++i) {
    branchPositions[i] = branchPositions[i + 1];
  }
  srand(seed);
  int r = rand() % 5;
  if (r == 0) {
    branchPositions[NUMBER_OF_BRANCHES - 1] = side::LEFT;
  }else if (r <= 1) {
    branchPositions[NUMBER_OF_BRANCHES - 1] = side::RIGHT;
  }
  else {
    branchPositions[NUMBER_OF_BRANCHES - 1] = side::NONE;
  }
}

Player::Player(const Texture & texturePlayer) : SpriteHolder{texturePlayer}
{
  sprite.setPosition(580, 720);
}

void Player::reset()
{
  setPosition(580, 720);
  _playerSide = side::LEFT;
  _acceptInput = true;
}

void Player::update(const Time & deltaTime, Score & currentScore)
{
  assert(_axe != nullptr && "The axe has no been set!");
  if (!_acceptInput) return;
  if (Keyboard::isKeyPressed(Keyboard::Right)) {
    chopRight();
    currentScore.increment();
    _acceptInput = false;
  }
  else if (Keyboard::isKeyPressed(Keyboard::Left)) {
    chopLeft();
    currentScore.increment();
    _acceptInput = false;
  }
}

void Player::chopRight()
{
  _playerSide = side::RIGHT;
  _axe->moveRight();
  setPosition(1200, 720);
}

void Player::chopLeft()
{
  _playerSide = side::LEFT;
  _axe->moveLeft();
  setPosition(580, 720);
}

Axe::Axe(const Texture & textureAxe) : SpriteHolder{ textureAxe }
{
  setPosition(700, 830);
}

void Axe::moveRight()
{
  setPosition(AXE_POISITON_RIGHT, this->sprite.getPosition().y);
}

void Axe::moveLeft()
{
  setPosition(AXE_POSITION_LEFT, this->sprite.getPosition().y);
}

Log::Log(const Texture & textureLog) : SpriteHolder{textureLog}
{
  setPosition(810, 720);
}

void Log::flyRight()
{
  setPosition(810, 720);
  speedX = 5000;
  active = true;
}

void Log::flyLeft()
{
  setPosition(810, 720);
  speedX = -5000;
  active = true;
}

void Score::reset()
{
  _score = 0;
}

void Score::increment()
{
  ++_score;
}

int Score::getScore() const
{
  return _score;
}


