#pragma once

#include <SpriteBatch.h>

#include <GLM/glm.hpp>

#include <string>
#include <vector>

namespace TKSZG
{
    class Agent;
    class Human;
    class Zombie;

    const int BULLET_RADIUS = 5;

    class Bullet
    {
    public:
        Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
        ~Bullet();

        bool update(const std::vector<std::string> &levelData, float deltaTime);

        void draw(Toaster::SpriteBatch &spriteBatch);

        bool collideWithAgent(Agent *agent);

        float getDamage() const { return _damage; }

    private:
        bool collideWithWorld(const std::vector<std::string> &levelData);

        float _speed;
        float _damage;
        glm::vec2 _position;
        glm::vec2 _direction;
    };
}
