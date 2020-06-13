#pragma once

#include<SFML/Graphics.hpp>
#include<cassert>

using namespace sf;

class Player {
private:
  const float START_SPEED = 200;
  const int START_HEALTH = 100;
  Vector2f _position;
  Sprite _sprite;
  Texture _texture;
  //screen resultion
  Vector2f _resoultion;
  //size of the arena
  IntRect _arena;
  //Size of a single tile
  int _tileSize;
  Vector2f _moveDirection; //the direction the player is moving in
  int _health;
  int _maxHealth;
  Time _lastHit; // when was the player last hit
  float _speed; //pixels per second

  bool isVulnerable(const Time & currentTime) const;
  //bool isSetup();

public:
  Player();
  void spawn(IntRect & arena, Vector2f & resolution, int tileSize);

  //Resets the player object to default state
  //call this at the end of every game
  void resetPlayerState();

  //Player being hit by a zombie
  bool hit(const Time & timeHit);

  //How long ago was the player last hit
  Time getLastHitTime() const;

  //Get player poistion
  FloatRect getPoitsion() const;

  //Get the center of player
  Vector2f getCenter() const;

  //Get player rotation
  float getRotation() const;

  //Get player sprite
  Sprite getSprite() const;

  //Move the player
  void move(const Vector2f & moveVector);

  //Update player
  //called once per frame
  void update(float elapsedTime, Vector2i mousePosition);

  //Give the player a speed boost
  void upgradeSpeed();

  //Heal player
  void heal(const int amount);

  //increase the max health
  void increaseMaxHealth();

  //Returns true of the player died
  bool isDead() const;

  //draw the player to the window
  void draw(RenderWindow & window) const;

};