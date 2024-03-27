#include "Human.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/rotate_vector.hpp>

#include <random>

namespace TKSZG
{
    Human::Human() : _frames(0)
    {
    }

    Human::~Human()
    {
    }

    void Human::init(float speed, glm::vec2 pos)
    {
        static std::mt19937 randomEngine;
        static std::uniform_real_distribution<float> randomDir(-1.0f, 1.0f);

        _health = 20.0f;

        _color.r = 200;
        _color.g = 0;
        _color.b = 200;
        _color.a = 255;

        _speed = speed;
        _position = pos;
        _direction = glm::vec2(randomDir(randomEngine), randomDir(randomEngine));

        // Make sure direction isn't 0
        if (_direction.length() == 0)
        {
            _direction = glm::vec2(1.0f, 0.0f);
        }

        _direction = glm::normalize(_direction);
    }

    void Human::update(const std::vector<std::string> &levelData,
                       std::vector<Human *> &humans,
                       std::vector<Zombie *> &zombies,
                       float deltaTime)
    {
        static float rotationFactor = 40;
        static std::mt19937 randEng;
        static std::uniform_real_distribution<float> randRotate(-rotationFactor * PI_CONST / 180, rotationFactor * PI_CONST / 180);

        _position += _direction * _speed * deltaTime;

        if (_frames == 20)
        {
            _direction = glm::rotate(_direction, randRotate(randEng));
            _frames = 0;
        }

        if (collideWithLevel(levelData))
            _direction = glm::rotate(_direction, randRotate(randEng));

        _frames++;
    }
}
