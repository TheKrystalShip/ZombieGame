#pragma once

#include "Level.h"
#include "Player.h"
#include "Bullet.h"

#include <Window.h>
#include <GLSLProgram.h>
#include <Camera2D.h>
#include <SpriteBatch.h>
#include <InputManager.h>

#include <vector>

enum class GAME_STATE
{
    PLAY,
    EXIT
};

class Zombie;

namespace TKSZG
{
    class MainGame
    {
    public:
        MainGame();
        ~MainGame();

        void run();

    private:
        void initSystems();
        void initLevel();
        void initShaders();
        void gameLoop();
        void updateAgents(float deltaTime);
        void updateBullets(float deltaTime);
        void checkVictory();
        void processInput();
        void drawGame();

        int _windowWidth;
        int _windowHeight;

        Toaster::Window _window;
        Toaster::GLSLProgram _textureProgram;
        Toaster::InputManager _inputManager;
        Toaster::Camera2D _camera;
        Toaster::SpriteBatch _agentSpriteBatch;
        std::vector<Level *> _levels;
        Player *_player;
        std::vector<Human *> _humans;
        std::vector<Zombie *> _zombies;
        std::vector<Bullet> _bullets;
        GAME_STATE _gameState;
        float _fps;
        int _currentLevel;

        int _humansKilled;
        int _zombiesKilled;
    };
}