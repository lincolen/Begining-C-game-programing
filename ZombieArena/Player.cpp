#include "pch.h"
#include "Player.h"
#include <string>
#include <algorithm> // min, max
#include <math.h> //PI


bool Player::isVulnerable(const Time & currentTime) const
{
  return currentTime.asMilliseconds() - _lastHit.asMilliseconds() > 200;
}

Player::Player() : _speed{ START_SPEED }, _health{ START_HEALTH }, _maxHealth{ START_HEALTH } {
  //associte texture to sprite
  if (!(_texture.loadFromFile("Graphics/player.png"))) {
    throw("Player texture not found");
  }
  _sprite.setTexture(_texture);

  //set sprite origin to center
  IntRect playerSize = _sprite.getTextureRect();
  _sprite.setOrigin(playerSize.width / 2, playerSize.height / 2);
}

void Player::spawn(IntRect & arena, Vector2f & resolution, int tileSize)
{
  //place player in center of the arena
  _position.x = arena.width / 2;
  _position.y = arena.height / 2;

  //copy the details of the arena
  _arena = arena;
  _tileSize = tileSize;
  _resoultion = resolution;
}

void Player::resetPlayerState()
{
  _speed = START_SPEED;
  _health = START_HEALTH;
  _maxHealth = START_HEALTH;
}

bool Player::hit(const Time & timeHit)
{
  if (isVulnerable(timeHit)) {
    _lastHit = timeHit;
    _health -= 10;
    return true;
  }

  return false;
}

Time Player::getLastHitTime() const 
{
  return _lastHit;
}

FloatRect Player::getPoitsion() const
{
  return _sprite.getGlobalBounds();
}

Vector2f Player::getCenter() const
{
  return _position;
}

float Player::getRotation() const
{
  return _sprite.getRotation();
}

Sprite Player::getSprite() const
{
  return _sprite;
}

void Player::move(const Vector2f & moveVector)
{
  _position += moveVector;
  _position.x = clamp(_position.x, (float)_arena.left + _tileSize, float(_arena.left + _arena.width - _tileSize));
  _position.y = clamp(_position.y, (float)_arena.top + _tileSize, float(_arena.top + _arena.height -_tileSize));
}

void Player::update(float elapsedTime, Vector2i mousePosition)
{
  //Handle movement
  Vector2f movementVector(0, 0);
  
  const Vector2f UP(0, -1), DOWN(0, 1), LEFT(-1, 0), RIGHT(1,0);
  movementVector += UP * (float) Keyboard::isKeyPressed(Keyboard::W);
  movementVector += DOWN * (float)Keyboard::isKeyPressed(Keyboard::S);
  movementVector += RIGHT * (float)Keyboard::isKeyPressed(Keyboard::D);
  movementVector += LEFT * (float)Keyboard::isKeyPressed(Keyboard::A);
  move(movementVector * _speed * elapsedTime);
  _sprite.setPosition(_position);

  //angle the player
  //mouse position reletive to screen center
  float angle = (atan2(mousePosition.y - _resoultion.y / 2, mousePosition.x - _resoultion.x / 2) * 180) / PI;
  _sprite.setRotation(angle);
}

void Player::upgradeSpeed()
{
  //20% speed upgrade
  _speed += START_SPEED * 0.2;
}

void Player::heal(const int amount)
{
  assert(amount >= 0 && "argument out of range");
  _health += amount;
  _health = std::min(_health, _maxHealth);
}

void Player::increaseMaxHealth()
{
  //20% max health increase
  _maxHealth += START_HEALTH * 0.2;
}

bool Player::isDead() const
{
  return _health > 0;
}

void Player::draw(RenderWindow & window) const
{
  window.draw(this->getSprite());
}

