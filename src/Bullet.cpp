#include "Bullet.h"
#include "Human.h"
#include "Zombie.h"
#include "Agent.h"
#include "Level.h"

#include <Toaster/SpriteBatch.h>
#include <Toaster/ResourceManager.h>

#include <GLM/glm.hpp>

#include <vector>

namespace TKSZG
{
    Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed) : _speed(speed),
                                                                                         _damage(damage),
                                                                                         _position(position),
                                                                                         _direction(direction)
    {
    }

    Bullet::~Bullet()
    {
    }

    bool Bullet::update(const std::vector<std::string> &levelData, float deltaTime)
    {
        _position += _direction * _speed * deltaTime;

        return collideWithWorld(levelData);
    }

    void Bullet::draw(Toaster::SpriteBatch &spriteBatch)
    {
        glm::vec4 destRect(_position.x + BULLET_RADIUS, _position.y + BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);
        const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
        Toaster::ColorRGBA8 color(75, 75, 75, 255);

        spriteBatch.draw(destRect, uvRect, Toaster::ResourceManager::getTexture("assets/textures/circle.png").id, 0.0f, color);
    }

    bool Bullet::collideWithAgent(Agent *agent)
    {
        const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

        glm::vec2 centerPosA = _position;
        glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

        glm::vec2 distVec = centerPosA - centerPosB;
        float distance = glm::length(distVec);

        float collisionDepth = MIN_DISTANCE - distance;

        return (collisionDepth > 0);
    }

    bool Bullet::collideWithWorld(const std::vector<std::string> &levelData)
    {
        glm::ivec2 gridPosition;

        gridPosition.x = floor(_position.x / (float)TILE_WIDTH);
        gridPosition.y = floor(_position.y / (float)TILE_WIDTH);

        // Out of bounds check
        if (gridPosition.x < 0 || gridPosition.x >= levelData[0].size() ||
            gridPosition.y < 0 || gridPosition.y >= levelData.size())
        {
            return true;
        }

        // Collision with walls
        return (levelData[gridPosition.y][gridPosition.x] != '.');
    }
}
