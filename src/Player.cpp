#include "Player.h"
#include "Gun.h"
#include "Bullet.h"

#include <InputManager.h>
#include <Camera2D.h>

#include <SDL2/SDL.h>
#include <GLM/glm.hpp>

#include <vector>

namespace TKSZG
{
    Player::Player() : _currentGunIndex(-1)
    {
    }

    Player::~Player()
    {
    }

    void Player::init(float speed, glm::vec2 position, Toaster::InputManager *inputManager, Toaster::Camera2D *camera, std::vector<Bullet> *bullets)
    {
        _health = 150.0f;
        _speed = speed;
        _position = position;
        _color = Toaster::ColorRGBA8(0, 128, 255);
        _inputManager = inputManager;
        _camera = camera;
        _bullets = bullets;
    }

    void Player::addGun(Gun *gun)
    {
        _guns.push_back(gun);

        // If no gun equiped, automatically equip it
        if (_currentGunIndex == -1)
            _currentGunIndex = 0;
    }

    void Player::update(const std::vector<std::string> &levelData,
                        std::vector<Human *> &humans,
                        std::vector<Zombie *> &zombies,
                        float deltaTime)
    {
        // Movement
        if (_inputManager->isKeyDown(SDLK_w))
            _position.y += _speed * deltaTime;
        if (_inputManager->isKeyDown(SDLK_s))
            _position.y -= _speed * deltaTime;
        if (_inputManager->isKeyDown(SDLK_a))
            _position.x -= _speed * deltaTime;
        if (_inputManager->isKeyDown(SDLK_d))
            _position.x += _speed * deltaTime;

        // Gun switching
        if (_inputManager->isKeyDown(SDLK_1) && _guns.size() > 0)
            _currentGunIndex = 0;
        if (_inputManager->isKeyDown(SDLK_2) && _guns.size() >= 1)
            _currentGunIndex = 1;
        if (_inputManager->isKeyDown(SDLK_3) && _guns.size() >= 2)
            _currentGunIndex = 2;

        if (_currentGunIndex != -1)
        {
            glm::vec2 mouseCoords = _inputManager->getMouseCoords();
            mouseCoords = _camera->convertScreenToWorld(mouseCoords);

            glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);
            glm::vec2 direction = glm::normalize(mouseCoords - centerPosition);

            _guns[_currentGunIndex]->update(
                _inputManager->isKeyDown(SDL_BUTTON_LEFT),
                centerPosition,
                direction,
                *_bullets,
                deltaTime);
        }

        collideWithLevel(levelData);
    }
}
