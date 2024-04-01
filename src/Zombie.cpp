#include "Zombie.h"
#include "Human.h"

#include <ResourceManager.h>

namespace TKSZG
{
    Zombie::Zombie()
    {
    }

    Zombie::~Zombie()
    {
    }

    void Zombie::init(float speed, glm::vec2 position)
    {
        _health = 150.0f;

        _speed = speed;
        _position = position;
        _color = Toaster::ColorRGBA8(255, 255, 255);
        _textureId = Toaster::ResourceManager::getTexture("assets/textures/zombie.png").id;
    }

    void Zombie::update(const std::vector<std::string> &levelData,
                        std::vector<Human *> &humans,
                        std::vector<Zombie *> &zombies,
                        float deltaTime)
    {
        Human *closesHuman = getClosestHuman(humans);

        if (closesHuman != nullptr)
        {
            _direction = glm::normalize(closesHuman->getPosition() - _position);
            _position += _direction * _speed * deltaTime;
        }

        collideWithLevel(levelData);
    }

    Human *Zombie::getClosestHuman(std::vector<Human *> humans)
    {
        Human *closestHuman = nullptr;
        float smallestDistance = 999999.0f;

        for (size_t i = 0; i < humans.size(); i++)
        {
            glm::vec2 distVec = humans[i]->getPosition() - _position;
            float distance = glm::length(distVec);

            if (distance < smallestDistance)
            {
                smallestDistance = distance;
                closestHuman = humans[i];
            }
        }

        return closestHuman;
    }
}
