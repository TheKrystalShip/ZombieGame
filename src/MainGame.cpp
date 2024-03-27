#include "MainGame.h"
#include "Human.h"
#include "Player.h"
#include "Zombie.h"
#include "Gun.h"

#include <Toaster.h>
#include <Timing.h>
#include <Errors.h>

#include <SDL2/SDL.h>

#include <random>
#include <ctime>
#include <iostream>

namespace TKSZG
{
    const float PLAYER_SPEED = 10.0f;
    const float HUMAN_SPEED = 1.0f;
    const float ZOMBIE_SPEED = 1.3f;

    MainGame::MainGame() : _windowWidth(1920),
                           _windowHeight(1080),
                           _player(nullptr),
                           _gameState(GAME_STATE::PLAY),
                           _fps(60.0f),
                           _currentLevel(0),
                           _humansKilled(0),
                           _zombiesKilled(0),
                           _inputManager()
    {
    }

    MainGame::~MainGame()
    {
        for (auto &l : _levels)
            delete l;

        for (auto &h : _humans)
            delete h;

        for (auto &z : _zombies)
            delete z;
    }

    void MainGame::run()
    {
        initSystems();
        initLevel();
        gameLoop();
    }

    void MainGame::initSystems()
    {
        Toaster::init();

        _window.setVsync(true);
        _window.create("ZombieGame", _windowWidth, _windowHeight);
        glClearColor(0.75f, 0.75f, 0.75f, 1.0f);

        initShaders();

        _agentSpriteBatch.init();

        _camera.init(_windowWidth, _windowHeight);
    }

    void MainGame::initLevel()
    {
        // Level 1
        _levels.push_back(new Level("assets/levels/level1.txt"));

        _player = new Player();
        _player->init(PLAYER_SPEED, _levels[_currentLevel]->getPlayerStartPos(), &_inputManager, &_camera, &_bullets);

        _humans.push_back(_player);

        std::mt19937 randomEngine;
        randomEngine.seed(time(nullptr));
        std::uniform_int_distribution<int> randomPosX(2.0f, _levels[_currentLevel]->getWidth() - 2);
        std::uniform_int_distribution<int> randomPosY(2.0f, _levels[_currentLevel]->getHeight() - 2);

        // Add humans
        for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++)
        {
            _humans.push_back(new Human);
            _humans.back()->init(HUMAN_SPEED, glm::vec2(randomPosX(randomEngine) * TILE_WIDTH, randomPosY(randomEngine) * TILE_WIDTH));
        }

        // Add zombies
        const std::vector<glm::vec2> &zombiePositions = _levels[_currentLevel]->getZomobieStartPositions();
        for (size_t i = 0; i < zombiePositions.size(); i++)
        {
            _zombies.push_back(new Zombie);
            _zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
        }

        // Give player some guns
        const float BULLET_SPEED = 20.0f;
        _player->addGun(new Gun("Magnum", 10, 1, 5.0f, 30.0f, BULLET_SPEED));
        _player->addGun(new Gun("Shotgun", 30, 12, 20.0f, 25.0f, BULLET_SPEED));
        _player->addGun(new Gun("MP5", 3, 1, 10.0f, 10.0f, BULLET_SPEED));
    }

    void MainGame::initShaders()
    {
        _textureProgram.compileShaders("assets/shaders/textureShading.vert", "assets/shaders/textureShading.frag");
        _textureProgram.addAttribute("vertexPosition");
        _textureProgram.addAttribute("vertexColor");
        _textureProgram.addAttribute("vertexUV");
        _textureProgram.linkShaders();
    }

    void MainGame::gameLoop()
    {
        const float MS_PER_SECOND = 1000.0f;
        const float FPS_LIMIT = 60.0f;
        const float DESIRED_FPS = 60.0f;
        const float DESIDED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
        const float MAX_DELTATIME = 1.0f;
        const int MAX_PHYSICS_STEPS = 6;

        Toaster::FpsLimiter fpsLimiter;
        fpsLimiter.setMaxFps(FPS_LIMIT);

        const float CAMERA_SCALE = 1.0f / 1.2f;
        _camera.setScale(CAMERA_SCALE);

        // Delta time calculation
        float previousTicks = SDL_GetTicks();

        while (_gameState == GAME_STATE::PLAY)
        {
            fpsLimiter.begin();

            float newTicks = SDL_GetTicks();
            float frameTime = newTicks - previousTicks;
            previousTicks = newTicks;

            float totalDeltaTime = frameTime / DESIDED_FRAMETIME;

            checkVictory();

            _inputManager.update();

            processInput();

            int i = 0;
            while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
            {
                float deltaTime = std::min(totalDeltaTime, MAX_DELTATIME);
                updateAgents(deltaTime);
                updateBullets(deltaTime);

                totalDeltaTime -= deltaTime;
                i++;
            }

            _camera.setPosition(_player->getPosition());
            _camera.update();

            drawGame();

            _fps = fpsLimiter.end();
        }
    }

    void MainGame::updateAgents(float deltaTime)
    {
        // Update Human collisions
        for (size_t i = 0; i < _humans.size(); i++)
        {
            _humans[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);

            for (size_t j = i + 1; j < _humans.size(); j++)
            {
                _humans[i]->collideWithAgent(_humans[j]);
            }
        }

        // Update Zombie collisions
        for (size_t i = 0; i < _zombies.size(); i++)
        {
            // Update
            _zombies[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);

            // Collide with other zombies
            for (size_t j = i + 1; j < _zombies.size(); j++)
            {
                _zombies[i]->collideWithAgent(_zombies[j]);
            }

            // Collide with humans
            for (size_t j = 1; j < _humans.size(); j++)
            {
                if (_zombies[i]->collideWithAgent(_humans[j]))
                {
                    // Add new zombie
                    _zombies.push_back(new Zombie);
                    _zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());

                    // Delete human
                    delete _humans[j];
                    _humans[j] = _humans.back();
                    _humans.pop_back();
                }
            }

            // Collide with Player
            if (_zombies[i]->collideWithAgent(_player))
            {
                Toaster::fatalError("YOU LOSE");
            }
        }
    }

    void MainGame::updateBullets(float deltaTime)
    {
        // World collision
        for (size_t i = 0; i < _bullets.size();)
        {
            if (_bullets[i].update(_levels[_currentLevel]->getLevelData(), deltaTime))
            {
                _bullets[i] = _bullets.back();
                _bullets.pop_back();
            }
            else
            {
                i++;
            }
        }

        bool wasBulletRemoved;

        // Agent collision
        for (size_t i = 0; i < _bullets.size(); i++)
        {
            wasBulletRemoved = false;
            // Zombie collision
            for (size_t j = 0; j < _zombies.size();)
            {
                if (_bullets[i].collideWithAgent(_zombies[j]))
                {
                    // Damage zombie, check if dead
                    if (_zombies[j]->applyDamage(_bullets[i].getDamage()))
                    {
                        delete _zombies[j];
                        _zombies[j] = _zombies.back();
                        _zombies.pop_back();
                        _zombiesKilled++;
                    }
                    else
                    {
                        j++;
                    }

                    _bullets[i] = _bullets.back();
                    _bullets.pop_back();
                    // Make sure we don't skip a bullet
                    wasBulletRemoved = true;
                    // Since bullet hit, skip rest of iterations
                    break;
                }
                else
                {
                    j++;
                }
            }

            // Only check collision with humans if the bullet still exists
            if (!wasBulletRemoved)
                for (size_t j = 1; j < _humans.size();) // Player is _humans[0], start J at 1
                {
                    if (_bullets[i].collideWithAgent(_humans[j]))
                    {
                        // Damage human, check if dead
                        if (_humans[j]->applyDamage(_bullets[i].getDamage()))
                        {
                            delete _humans[j];
                            _humans[j] = _humans.back();
                            _humans.pop_back();
                            _humansKilled++;
                        }
                        else
                        {
                            j++;
                        }

                        _bullets[i] = _bullets.back();
                        _bullets.pop_back();
                        // Make sure we don't skip a bullet
                        wasBulletRemoved = true;
                        // Since bullet hit, skip rest of iterations
                        break;
                    }
                    else
                    {
                        j++;
                    }
                }
        }
    }

    void MainGame::checkVictory()
    {
        if (!_zombies.empty())
            return;

        std::printf("*** You win! ***\nYou killed %d humans and %d zombies\nThere are %ld/%d civilians remaining",
                    _humansKilled,
                    _zombiesKilled,
                    _humans.size(),
                    _levels[_currentLevel]->getNumHumans());

        Toaster::fatalError("-");
    }

    void MainGame::processInput()
    {
        SDL_Event e;

        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                _gameState = GAME_STATE::EXIT;
                break;
            case SDL_MOUSEMOTION:
                _inputManager.setMouseCoords(e.motion.x, e.motion.y);
                break;

            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    _gameState = GAME_STATE::EXIT;
                    break;
                }
                else
                {
                    _inputManager.pressKey(e.key.keysym.sym);
                }
                break;

            case SDL_KEYUP:
                _inputManager.releaseKey(e.key.keysym.sym);
                break;

            case SDL_MOUSEBUTTONDOWN:
                _inputManager.pressKey(e.button.button);
                break;

            case SDL_MOUSEBUTTONUP:
                _inputManager.releaseKey(e.button.button);
                break;

            default:
                break;
            }
        }
    }

    void MainGame::drawGame()
    {
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _textureProgram.bind();

        // Draw here
        glActiveTexture(GL_TEXTURE0);

        // Make sure the shader uses texture 0
        GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
        glUniform1i(textureUniform, 0);

        // Grab the camera matrix
        glm::mat4 projectionMatrix = _camera.getCameraMatrix();
        GLint pUniform = _textureProgram.getUniformLocation("P");
        glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

        // Draw the level
        _levels[_currentLevel]->draw();

        _agentSpriteBatch.begin();

        // Draw the humans
        for (auto &h : _humans)
            if (_camera.isBoxInView(h->getPosition(), glm::vec2(AGENT_RADIUS * 2.0f)))
                h->draw(_agentSpriteBatch);

        // Draw zombies
        for (auto &z : _zombies)
            if (_camera.isBoxInView(z->getPosition(), glm::vec2(AGENT_RADIUS * 2.0f)))
                z->draw(_agentSpriteBatch);

        // Draw bullets
        for (auto &b : _bullets)
            b.draw(_agentSpriteBatch);

        _agentSpriteBatch.end();

        _agentSpriteBatch.renderBatch();

        _textureProgram.unbind();
        _window.swapBuffer();
    }
}
