#include "pch.h"
#include "Player.h"
#include <string>


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

