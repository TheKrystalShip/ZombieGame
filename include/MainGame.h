#pragma once

#include "Bullet.h"
#include "Level.h"
#include "Player.h"

#include <Toaster/Camera2D.h>
#include <Toaster/GLSLProgram.h>
#include <Toaster/InputManager.h>
#include <Toaster/SpriteBatch.h>
#include <Toaster/SpriteFont.h>
#include <Toaster/Window.h>
#include <Toaster/AudioEngine.h>
#include <Toaster/ParticleEngine2D.h>
#include <Toaster/ParticleBatch2D.h>

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
        void drawHud();
        void addBlood(const glm::vec2 &position, int numParticles);

        int _windowWidth;
        int _windowHeight;

        Toaster::Window _window;

        Toaster::GLSLProgram _textureProgram;
        Toaster::InputManager _inputManager;

        Toaster::Camera2D _camera;
        Toaster::Camera2D _hudCamera;

        Toaster::SpriteBatch _agentSpriteBatch;
        Toaster::SpriteBatch _hudSpriteBatch;
        Toaster::SpriteFont *_spriteFont;
        Toaster::AudioEngine _audioEngine;

        Toaster::ParticleEngine2D _particleEngine;
        Toaster::ParticleBatch2D *_bloodParticleBatch;

        std::vector<Level *> _levels;
        int _currentLevel;

        Player *_player;

        std::vector<Human *> _humans;
        std::vector<Zombie *> _zombies;
        std::vector<Bullet> _bullets;

        GAME_STATE _gameState;

        float _fps;

        int _humansKilled;
        int _zombiesKilled;
    };
}
