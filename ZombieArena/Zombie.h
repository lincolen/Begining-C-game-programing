#pragma once
#include "Player.h"
#include<SFML/Graphics.hpp>
#include<forward_list>
#include<cassert>

using namespace sf;


class Zombie{
public:
  inline bool isDead() const {
    return _health <= 0;
  }
  //draw zombie to windw
  void draw(RenderWindow & window) const;

  //Is hit by the player
  void damageHit();

  //did the player collide with a zombie
  friend bool isPressCollision(const Player player,const Zombie zombie);

  void update(const float deltaTime, const Vector2f playerLocation);

protected:
  Zombie(const float maxSpeed, const float maxHealth, const Texture & texture,const Vector2f & position = Vector2f(0.0f, 0.0f));

private:
  //transpose the zombie position by the given argument
  void move(const Vector2f & transpose);

  Vector2f _position;
  Sprite _sprite;
  float _health;
  float _speed;
  const float _MAX_SPEED_VARIANCE = 30; //Set between 0 AND 99
  const float _SPEED_VARIANCE_OFFSET = 100 - _MAX_SPEED_VARIANCE;

  static Texture bloodSplater;
};

std::forward_list<Zombie>  createHorde(const unsigned int numZombies, const IntRect & arena);


//Slow Bloater Zombie
class Bloater : public Zombie {
public:
  Bloater(const Vector2f & position = Vector2f(0.0f, 0.0f));

private:
  static const float _MAX_SPEED;
  static const float _MAX_HEALTH;
  static Texture _texture;
  static Texture loadTextureFromFile();
};

//Fast chaser type zombie
class Chaser : public Zombie {
public:
  Chaser(const Vector2f & position = Vector2f(0.0f, 0.0f));

private:
  static const float _MAX_SPEED;
  static const float _MAX_HEALTH;
  static Texture _texture;
  static Texture loadTextureFromFile();
};

//Normal crawler zombie
class Crawler : public Zombie {
public:
  Crawler(const Vector2f & position = Vector2f(0.0f, 0.0f));

private:
  static const float _MAX_SPEED;
  static const float _MAX_HEALTH;
  static Texture _texture;
  static Texture loadTextureFromFile();
};



