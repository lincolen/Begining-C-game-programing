

#include "pch.h"
//SMLF includes
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
//standard library
#include<cassert>
#include <vector>
#include <sstream>
#include<string>

using namespace sf;

enum side { LEFT, RIGHT, NONE, BOTH };
//base class for game objects represented using a sprite
 class SpriteHolder{
	protected:
	 Sprite sprite;

 public:
   //initilaize and set texture
	 SpriteHolder(const Texture & texture) {
		 sprite.setTexture(texture);
	 }
   //draw the sprite to the window
	 void draw(RenderWindow & window) {
		 window.draw(sprite);
	 }
   //set the position of sprite
   //when moving the sprite use move() instead
	 void setPosition(const int x, const int y) {
		 sprite.setPosition(x, y);
	 }

   //moves the sprite for the distance passed in the arguments
   inline void move(const int dx, const int dy) {
     setPosition(sprite.getPosition().x + dx, sprite.getPosition().y + dy);
   }

	 //should make virtual update and setup functions here 

 };

 //sets the origin points of the text to center
 void setTextOriginToCenter(Text & text) {
   FloatRect textRect = text.getLocalBounds();
   text.setOrigin(textRect.width / 2, textRect.height / 2);
 }

 class Axe : public SpriteHolder {
 public:
   Axe(const Texture & textureAxe);
   //move the axe to a right chop position
   void moveRight();
   //move the axe to a left chop position
   void moveLeft();
   //Hide the axe of the screen
   void hide();

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
//Class warper for score
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
  void update(const Time & deltaTime, Score & score, Window & window);
  //Set reference to axe obejct
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
   void update(const Time & deltaTime);

 private:
   bool active = false;
   float speedX = 0;
   float speedY = -2000;
};


class TimeBar{
 private:
    RectangleShape _bar; 
    const float timeBarStartWidth = 400.0f;
    const float timeBarDefaultHeight = 80.0f;
    const Color _color = Color::Red;
    const float startTime = 6.0f; //Time given to the player
    float timeRemaining = startTime; 
    const float timeBarConsumedPerSecond = timeBarStartWidth / startTime;

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
    _bar.setSize(Vector2f(timeBarStartWidth, timeBarDefaultHeight));
    timeRemaining = startTime;
  }

  void update(Time deltaTime) {
    timeRemaining -= deltaTime.asSeconds();
    _bar.setSize(Vector2f(timeBarConsumedPerSecond * timeRemaining, _bar.getSize().y));
   }
  //Add time current round when player chops
  //ajust formula to change the game difficulty 
  void increment(const int score) {
    assert (score > 0);
    //(should make private variables?)
    timeRemaining +=  0.15 * 2.0 / score;
  }

  bool timeIsUp() {
    return timeRemaining <= 0.0f;
  }


};

//moves the branches down the tree and places the branch at the top of the tree at a semi random position
void updateBranches(const int seed, Sprite * branches, side * branchPositions);
//update the branch positions
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
	
  //create font object
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

  //load font
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
  Text frameRateText;
  Text startGameText;
	Text scoreText;
  TimeBar timeBar;
  Sprite branches[NUMBER_OF_BRANCHES];
  side branchPositions[NUMBER_OF_BRANCHES];
  Player player(texturePlayer);
  Axe axe(textureAxe);
  player.setAxe(axe);
  Log log(textureLog);

	//attach texture to sprites
  for (int i = 0; i < NUMBER_OF_BRANCHES; ++i) {
    branches[i].setTexture(textureBranch);
    FloatRect branchSize = branches[i].getLocalBounds();
    branches[i].setOrigin(Vector2f(branchSize.width / 2, branchSize.height / 2));

  }
	spriteBackground.setTexture(textureBackground);
	spriteTree.setTexture(textureTree);
  spriteRIP.setTexture(textureRIP);

	//set up text
	startGameText.setString("Press Enter to start!");
	startGameText.setCharacterSize(75);
	startGameText.setFillColor(Color::White);
	startGameText.setFont(font);
  setTextOriginToCenter(startGameText); //set origin to center
	startGameText.setPosition(vm.width / 2, vm.height / 2); //place at the middle of the screen

	scoreText.setString("Score = ");
	scoreText.setCharacterSize(100);
	scoreText.setFillColor(Color::White);
	scoreText.setFont(font);
	scoreText.setPosition(20, 20); //set text to top left of the screen

  frameRateText.setString("FPS = ");
  frameRateText.setCharacterSize(30);
  frameRateText.setFillColor(Color::Green);
  frameRateText.setFont(font);
  frameRateText.setPosition(20, vm.height - 100);

  

  //setup sounds
  SoundBuffer chopBuffer;
  SoundBuffer deathBuffer;
  SoundBuffer outOfTimeBuffer;
  chopBuffer.loadFromFile("sounds/chop.wav");
  deathBuffer.loadFromFile("sounds/death.wav");
  outOfTimeBuffer.loadFromFile("sounds/out_of_time.wav");
  Sound soundChop;
  Sound soundDeath;
  Sound soundOutOfTime;
  soundChop.setBuffer(chopBuffer);
  soundDeath.setBuffer(deathBuffer);
  soundOutOfTime.setBuffer(outOfTimeBuffer);

	//set the Background to the screen 
	spriteBackground.setPosition(0, 0);
	spriteTree.setPosition(810, 0); //place tree
  spriteRIP.setPosition(600, 2000); //hide gravestone
  
  //Clock used to measure time between frames 
	sf::Clock clock;
	bool isGamePaused = true;
  //clear all branches initially
  for (int i = 0; i < NUMBER_OF_BRANCHES; ++i) {
    branchPositions[i] = side::NONE;
  }

  //used to check if the player chopped the tree
  int scoreLastCycle = score.getScore(); 
  auto isChoped = [&]() {return score.getScore() != scoreLastCycle;};
  float secondCounter = 0.0f; // used to determine if a second has elapsed
  int framesPerSecond = 0; //counts the number of frames every second averegeing out the fps display
  /*/////////////////////////
      main loop
  *//////////////////////////
	while (window.isOpen()) {
    Time deltaTime = clock.restart(); //get deltatime from last frame
    secondCounter += deltaTime.asSeconds();
    secondCounter = secondCounter >= 1.0f ? 0 : secondCounter;
		/********************************
		Exit game
		********************************/
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}
    /*/////////////////////
    Start the game
    *//////////////////////
    //start the game when the player presses enter
		if (isGamePaused == true && Keyboard::isKeyPressed(Keyboard::Enter)) {
			isGamePaused = false;
      timeBar.reset();
      score.reset();
      scoreLastCycle = score.getScore();
      //remove branches  
      for (int i = 0; i < NUMBER_OF_BRANCHES; ++i) {
        branchPositions[i] = side::NONE;
      }
      spriteRIP.setPosition(675, 2000); //remove graveStone
      player.reset();

		}
		/********************************
		update the scene
		********************************/
		if (!isGamePaused) {
      log.update(deltaTime);
      player.update(deltaTime, score, window);
      //handle chopping of tree
      if (isChoped()) {
        timeBar.increment(score.getScore()); //increase time
        player.getSide() == side::LEFT ? log.flyRight() : log.flyLeft(); //send log flying
        int seed = (int)time(0) + score.getScore(); // random seed for branch update
        updateBranches(seed, branches, branchPositions); 
        soundChop.play();
        scoreText.setString(std::string("score = ") + std::to_string(score.getScore())); //update score text
      }
			bee.update(deltaTime); //move bee
			for (Cloud & cloud : clouds) {
				cloud.update(deltaTime); //move clouds
			}
      //shrink time
      timeBar.update(deltaTime);
      //check for timeup
      if (timeBar.timeIsUp()) {
        isGamePaused = true;
        startGameText.setString("Time is up!");
        setTextOriginToCenter(startGameText);
        soundOutOfTime.play();
      }

      /*/////////////////////////
      handle Game over
      *//////////////////////////
      bool isGameOver = (branchPositions[NUMBER_OF_BRANCHES - 1] == player.getSide());
      if (isGameOver) {
        isGamePaused = true; //pause game
        spriteRIP.setPosition(525, 760); //position grave stone
        player.setPosition(2000, 660); //hide the player
        //handle gameover text
        startGameText.setString("SQUISHED!!");
        setTextOriginToCenter(startGameText);
        startGameText.setPosition(vm.width / 2, vm.height / 2); //locate to screen center
        soundDeath.play();
      }
		}
    //execute once per second
    ++framesPerSecond;
    if (secondCounter == 0) {
      frameRateText.setString(std::string("FPS = ") + std::to_string(framesPerSecond)); //update framerate text
      framesPerSecond = 0;
    }
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
    //draw player
    player.draw(window);
    //draw axe
    axe.draw(window);
    //draw gravestone
    window.draw(spriteRIP);
    //draw log
    log.draw(window);
    //draw text
		window.draw(scoreText);
    window.draw(frameRateText);
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

void setUpBranches(Sprite * branches, side * branchPositions) {
  for (int i = 0; i < NUMBER_OF_BRANCHES; ++i) {
    float height = 120 + i * 150;
    switch (branchPositions[i]) {
      case side::LEFT:
        branches[i].setPosition(610, height);
        branches[i].setRotation(180);
        break;
      case side::RIGHT:
        branches[i].setPosition(1330, height);
        branches[i].setRotation(0);
        break;
      default:
        //hides branch
        branches[i].setPosition(-3000, height);
    }
  }

}

void updateBranches(const int seed, Sprite *branches, side * branchPositions) {
  //move branches down 
  for (int i = NUMBER_OF_BRANCHES - 1; i > 0; --i) {
    branchPositions[i] = branchPositions[i - 1];
  }
  srand(seed);
  int r = rand() % 5;
  //position branch to the left
  if (r == 0) {
    branchPositions[0] = side::LEFT;
  //position branch to the right
  }else if (r <= 1) {
    branchPositions[0] = side::RIGHT;
  } //no branch
  else {
    branchPositions[0] = side::NONE;
  }
}

Player::Player(const Texture & texturePlayer) : SpriteHolder{texturePlayer}
{
  sprite.setPosition(580, 720); //start position
}

void Player::reset()
{
  setPosition(580, 720); //start position
  _playerSide = side::LEFT;
  _acceptInput = true;
}

void Player::update(const Time & deltaTime, Score & currentScore,  Window & window)
{
  assert(_axe != nullptr && "The axe has no been set!");

  //handle key being released
  Event event;
  while (window.pollEvent(event)) {
    if (event.type != Event::KeyReleased)
      break;
    _acceptInput = true;
    _axe->hide(); //hide axe
  }
  //handle key being pressed
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

void Axe::hide()
{
  setPosition(2000, sprite.getPosition().y);
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

void Log::update(const Time & deltaTime)
{
  active = sprite.getPosition().y < -100 ? false : active;
  if (active == false)
    return;

  move( speedX * deltaTime.asSeconds(),  speedY * deltaTime.asSeconds());
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