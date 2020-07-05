// HelloSFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Background.h"
#include "Player.h"
#include "Zombie.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include<forward_list> 


using namespace sf;

bool pickPowerUp(Player & player);

int main()
{
  enum GameState {
    PAUSED, LEVELING_UP, GAME_OVER, PLAYING
  };
  //default game state
  GameState gameState = GameState::GAME_OVER;


  //Get resultion and create a window
  Vector2f resolution;
  resolution.x = VideoMode::getDesktopMode().width;
  resolution.y = VideoMode::getDesktopMode().height;

  RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);

  //Main view
  View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

  //clock
  Clock clock;
  Time gameTimeTotal; // how long has the game been in the playing state

  //mouse coordinates
  Vector2f mouseWorldPosition;
  Vector2i mouseScreenPosition;

  //player
  Player player;

  //background
  VertexArray backgroundVertexArray;
  Texture textureBackground;
  textureBackground.loadFromFile("graphics/background_sheet.png");


  //Arena boundries
  IntRect arena;

  //Enemys
  std::forward_list<Zombie> horde;
  

  //Main game loop
  while (window.isOpen()) {
    /*******************
    Handle input
    **********************/
    //handle events by polling
    Event event;
    while (window.pollEvent(event)) {
      if (event.type == Event::KeyPressed)
      {
        if(event.key.code == Keyboard::Enter)
        {
          switch (gameState) 
          {
            //pause the game
            case GameState::PLAYING:
              gameState = GameState::PAUSED;
              break;
              //restart the game
            case GameState::PAUSED:
              gameState = GameState::PLAYING;
              clock.restart();
              break;
              //start a new game
            case GameState::GAME_OVER:
              gameState = GameState::LEVELING_UP;
              break;
          }
        }
      }
    }
      
    //close the application
    if(Keyboard::isKeyPressed(Keyboard::Escape))
      window.close();

    //Pick a powerup
    if (gameState == GameState::LEVELING_UP) {
      if (pickPowerUp(player)) {
      //prepare the level
        gameState = GameState::PLAYING;
        arena.width = 500;
        arena.height = 500;
        arena.left = 0;
        arena.top = 0;
        background::createBackgroung(backgroundVertexArray, arena);

        //spawn player
        player.spawn(arena, resolution, background::TILE_SIZE);
        clock.restart();

        //spawn enemy
        horde = createHorde(2, arena);
      }
    }
      
      
      /*********************
      update the scene
      ********************/
      if (gameState == GameState::PLAYING) {
        //get delta time 
        Time deltaTime = clock.restart();
        gameTimeTotal += deltaTime;
        //where is the mouse pointer
        mouseScreenPosition = Mouse::getPosition(); 
        //get mouse poistion in world cordinates
        mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);
        //update player
        player.update(deltaTime.asSeconds(), mouseScreenPosition);

        for (auto zombie = horde.begin(); zombie != horde.end(); ++zombie) {
          zombie->update(deltaTime.asSeconds(), player.getCenter());
        }

        //center main camera on player
        mainView.setCenter(player.getCenter()); 
      }

      /***************
      Draw the scene
      ****************/
      if (gameState == GameState::PLAYING) {
        window.clear();
        //Render main view
        window.setView(mainView);
        //draw background
        window.draw(backgroundVertexArray, &textureBackground);
        //draw player
        player.draw(window);


        //draw enemy
        for (auto zombie = horde.begin(); zombie != horde.end(); ++zombie) {
          zombie->draw(window);
        }
      }

      if (gameState == GameState::LEVELING_UP) {
      }
      if(gameState == GameState::GAME_OVER){
      }
      if(gameState == GameState::PAUSED){
      }

      //display scene
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

inline bool pickPowerUp(Player & player)
{
  if (Keyboard::isKeyPressed(Keyboard::Num1)) {
    return true;
  }
  if (Keyboard::isKeyPressed(Keyboard::Num2)) {
    return true;
  }
  if (Keyboard::isKeyPressed(Keyboard::Num3)) {
    return true;
  }

  return false;
}
