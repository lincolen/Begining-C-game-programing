#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;


namespace background {
  const int TILE_SIZE = 50;
  const int TILE_TYPES = 4;
  enum Tile {BRICK1 = 0, GRASS, BRICK2, WALL};
  const std::string textureLocation = "graphics/background_sheet.png";

  void createBackgroung(VertexArray & vertexArray, const IntRect & arena);
}