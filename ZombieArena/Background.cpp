#include"pch.h"
#include"Background.h"
#include<SFML/Graphics.hpp>


namespace background {
  void createBackgroung(VertexArray & vertexArray, const IntRect & arena)
  {
    
    
    const int VERTS_IN_QUAD = 4;

    int worldWidth = arena.width / TILE_SIZE;
    int worldHeight = arena.height / TILE_SIZE;

    //what type of primitive are we using
    vertexArray.setPrimitiveType(PrimitiveType::Quads);
    //set the size of the vertex array 
    vertexArray.resize(worldWidth * worldHeight * VERTS_IN_QUAD);

    //Position vertices
    for (int w = 0, currentVertex = 0; w < worldWidth; ++w) {
      for (int h = 0; h < worldHeight; ++h, currentVertex += VERTS_IN_QUAD) {
        vertexArray[currentVertex + 0].position = Vector2f(TILE_SIZE * w, TILE_SIZE * h);
        vertexArray[currentVertex + 1].position = Vector2f(TILE_SIZE * w + TILE_SIZE, TILE_SIZE * h);
        vertexArray[currentVertex + 2].position = Vector2f(TILE_SIZE * w + TILE_SIZE, TILE_SIZE * h + TILE_SIZE);
        vertexArray[currentVertex + 3].position = Vector2f(TILE_SIZE * w, TILE_SIZE * h + TILE_SIZE);
      }
    }

    
    //assgin textures
    auto isBorder = [worldWidth, worldHeight](int w, int h) {
      return
        h == 0 ||
        h == worldHeight - 1 ||
        w == 0 ||
        w == worldWidth - 1;
    };
    srand((int)time(0));

    for (int w = 0, currentVertex = 0; w < worldWidth; ++w) {
      for (int h = 0; h < worldHeight; ++h,currentVertex += VERTS_IN_QUAD) {
        float verticalOffset;
        //texture at the border
        if (isBorder(w, h)) {
          verticalOffset = TILE_SIZE * WALL;
        }else{
          //random tile type
          int tileNumber = rand() % (TILE_TYPES - 1);
          verticalOffset = TILE_SIZE * tileNumber;              
        }
        vertexArray[currentVertex + 0].texCoords = Vector2f(0, verticalOffset);
        vertexArray[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, verticalOffset);
        vertexArray[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, verticalOffset + TILE_SIZE);
        vertexArray[currentVertex + 3].texCoords = Vector2f(0, verticalOffset + TILE_SIZE);
      }
    }
   



  }
}