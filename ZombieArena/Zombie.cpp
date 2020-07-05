#include"pch.h"
#include "Zombie.h"
#include "SFML/Graphics.hpp"
#include "Background.h" // tileSize
#include <iostream>
#include <math.h> // abs
 
#pragma region Base class

void Zombie::draw(RenderWindow & window) const
{
  window.draw(_sprite);
}

void Zombie::damageHit()
{
  _health--;

  if (isDead()) {
    _sprite.setTexture(bloodSplater);
  }
}

void Zombie::update(const float deltaTime, const Vector2f playerLocation)
{
  //move the zombie towerds the player
  const Vector2f UP(0.0f, -1.0f) , DOWN(0.0f, 1.0f), LEFT(-1.0f, 0.0f), RIGHT(1.0f, 0.0f);
  
  Vector2f  moveDirection(0.0f, 0.0f);
  if (playerLocation.x > _position.x) {
    moveDirection += RIGHT;
  }
  if (playerLocation.x < _position.x) {
    moveDirection += LEFT;
  }
  if (playerLocation.y > _position.y) {
    moveDirection += DOWN;
  }
  if (playerLocation.y < _position.y) {
    moveDirection += UP;
  }

  move(deltaTime * _speed * moveDirection);

  //rotate zombie to face the player
  const float RADIAN_TO_DEG = 180 / PI;
  float angle = atan2(playerLocation.y - _position.y, playerLocation.x - _position.x) * RADIAN_TO_DEG;
  _sprite.setRotation(angle);

}

inline Zombie::Zombie(const float maxSpeed, const float maxHealth, const Texture & texture, const Vector2f & position)
{
  //initilize speed modified by variance
  float speedModifier = (rand() % (int)_MAX_SPEED_VARIANCE) + _SPEED_VARIANCE_OFFSET;
  speedModifier /= 100.0f;
  _speed = maxSpeed * speedModifier;

  _health = maxHealth;
  _sprite.setTexture(texture);
  _position = position;
  _sprite.setPosition(position.x, position.y);
  
  auto textureSize = texture.getSize();
  _sprite.setOrigin(textureSize.x /2, textureSize.y / 2); // set sprite origin to center
}

void Zombie::move(const Vector2f & transpose)
{
  _position += transpose;
  _sprite.setPosition(_position);
  
}

sf::Texture MyLoadBloodTextureFromFile() {
  Texture bloodTexture;
  if (!bloodTexture.loadFromFile("graphics/bloater.png")) {
    throw("Texture File not found!");
  }
  return bloodTexture;
}

sf::Texture Zombie::bloodSplater = MyLoadBloodTextureFromFile();

#pragma endregion 

#pragma region Bloater 

sf::Texture Bloater::loadTextureFromFile() {
  Texture BloaterTexture;
  if (!BloaterTexture.loadFromFile("graphics/bloater.png")) {
    throw("Texture not file not found!");
  }
  return BloaterTexture;
}

Texture Bloater::_texture = Bloater::loadTextureFromFile();

Bloater::Bloater(const Vector2f & position) : Zombie(_MAX_SPEED, _MAX_HEALTH, _texture, position) {

}

const float Bloater::_MAX_SPEED = 40;
const float Bloater::_MAX_HEALTH = 5;
#pragma endregion

#pragma region Chaser 

Texture Chaser::loadTextureFromFile() {
  Texture ChaserTexture;
  if (!ChaserTexture.loadFromFile("graphics/Chaser.png")) {
    throw("Texture not file not found!");
  }
  return ChaserTexture;
}

Texture Chaser::_texture = Chaser::loadTextureFromFile();

Chaser::Chaser(const Vector2f & position) : Zombie(_MAX_SPEED, _MAX_HEALTH, _texture, position) {
}

const float Chaser::_MAX_SPEED = 80;
const float Chaser::_MAX_HEALTH = 1;

#pragma endregion

#pragma region Crawler 

Texture Crawler::loadTextureFromFile() {
  Texture CrawlerTexture;
  if (!CrawlerTexture.loadFromFile("graphics/Crawler.png")) {
    throw("Texture not file not found!");
  }
  return CrawlerTexture;
}

Texture Crawler::_texture = Crawler::loadTextureFromFile();

Crawler::Crawler::Crawler(const Vector2f & position) : Zombie(_MAX_SPEED, _MAX_HEALTH, _texture) {

}

const float Crawler::_MAX_SPEED = 20;
const float Crawler::_MAX_HEALTH = 3;

#pragma endregion

std::forward_list<Zombie>  createHorde(const unsigned int numZombies,const IntRect & arena)
{
  std::forward_list<Zombie> horde;
  //spawnable size of the arena
  const int wallOffset = background::TILE_SIZE / 2;
  int maxX = arena.left + arena.width - wallOffset;
  int minX = arena.left + wallOffset ;
  int maxY = arena.top + arena.height - wallOffset;
  int minY = arena.top +  wallOffset;

  srand((int)time(0));

  enum Side {
    TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 3
  } side;

  int numOfTypes = 3;
  enum EnemyType
  {
    CRAWLER = 0, BLOATER, CHASER
  } type;

  for (int i = 0; i < numZombies; ++i) {

    //decide spawn position
    Vector2f initialPosition;

    side = (Side)(rand() % 4);


    switch (side) {
      case Side::TOP:
        initialPosition.y = minY;
        initialPosition.x = minX + (rand() % std::abs(maxX - minX));
        break;
      case Side::BOTTOM:
        initialPosition.y = maxY;
        initialPosition.x = minX + (rand() % std::abs(maxX - minX));
        break;
      case Side::LEFT:
        initialPosition.x = minX;
        initialPosition.y = minY + (rand() % std::abs(maxY - minY));
        break;
      case Side::RIGHT:
        initialPosition.x = maxX;
        initialPosition.y = minY + (rand() % std::abs(maxY - minY));
        break;
      default:
        throw("unrecognized side");
        break;
    }
    
    std::cout << "span range " << std::abs(maxY - minY);
    std::cout << "initial position " << initialPosition.x << " " << initialPosition.y << std::endl;
    //spawn a zombie of a random type
    type = (EnemyType) (rand() % numOfTypes);
    switch (type)
    {
      case EnemyType::BLOATER:
        horde.push_front(Bloater(initialPosition));
        break;
      case EnemyType::CHASER:
        horde.push_front(Chaser(initialPosition));
        break;
      case EnemyType::CRAWLER:
        horde.push_front(Crawler(initialPosition));
        break;
      default:
        throw("Type not recognized");
        break;
    }
  }

  return horde;
}
