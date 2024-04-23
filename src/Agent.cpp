#include "Agent.h"
#include "Level.h"

#include <Toaster/ResourceManager.h>
#include <Toaster/SpriteBatch.h>

#include <GLM/glm.hpp>

#include <algorithm>
#include <vector>

namespace TKSZG
{
    Agent::Agent()
    {
    }

    Agent::~Agent()
    {
    }

    bool Agent::collideWithLevel(const std::vector<std::string> &levelData)
    {
        std::vector<glm::vec2> collideTilePositions;

        // Check the 4 corners
        // First corner
        checkTilePosition(levelData, collideTilePositions, _position.x, _position.y);
        // Second corner
        checkTilePosition(levelData, collideTilePositions, _position.x + AGENT_WIDTH, _position.y);
        // Third corner
        checkTilePosition(levelData, collideTilePositions, _position.x, _position.y + AGENT_WIDTH);
        // Forth corner
        checkTilePosition(levelData, collideTilePositions, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH);

        if (collideTilePositions.size() == 0)
            return false;

        // Do collision
        for (size_t i = 0; i < collideTilePositions.size(); i++)
            collideWithTile(collideTilePositions[i]);

        return true;
    }

    bool Agent::collideWithAgent(Agent *agent)
    {
        const float MIN_DISTANCE = AGENT_RADIUS * 2;

        glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);
        glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

        glm::vec2 distVec = centerPosA - centerPosB;
        float distance = glm::length(distVec);

        float collisionDepth = MIN_DISTANCE - distance;

        if (collisionDepth > 0)
        {
            glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;

            _position += collisionDepthVec / 2.0f;
            agent->_position -= collisionDepthVec / 2.0f;
            return true;
        }

        return false;
    }

    void Agent::checkTilePosition(const std::vector<std::string> &levelData,
                                  std::vector<glm::vec2> &collideTilePosition,
                                  float x,
                                  float y)
    {
        glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH), floor(y / (float)TILE_WIDTH));

        // If we are outsize of the world just return
        if (cornerPos.x < 0 || cornerPos.x >= levelData[0].size() ||
            cornerPos.y < 0 || cornerPos.y >= levelData.size())
        {
            return;
        }

        // Check collision
        if (levelData[cornerPos.y][cornerPos.x] != '.')
            collideTilePosition.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2));
    }

    // AABB collission
    void Agent::collideWithTile(glm::vec2 tilePosition)
    {
        const float TILE_RADIUS = (float)TILE_WIDTH / 2;
        const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

        glm::vec2 centerPlayerPos = _position + glm::vec2(AGENT_RADIUS);
        glm::vec2 distVec = centerPlayerPos - tilePosition;

        float xDepth = MIN_DISTANCE - std::abs(distVec.x);
        float yDepth = MIN_DISTANCE - std::abs(distVec.y);

        // if true
        if (xDepth > 0 && yDepth > 0)
        {
            if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f))
            {
                if (distVec.x < 0)
                {
                    _position.x -= xDepth;
                }
                else
                {
                    _position.x += xDepth;
                }
            }
            else
            {
                if (distVec.y < 0)
                {
                    _position.y -= yDepth;
                }
                else
                {
                    _position.y += yDepth;
                }
            }
        }
    }

    void Agent::draw(Toaster::SpriteBatch &spriteBatch)
    {
        glm::vec4 destRect(_position.x, _position.y, AGENT_WIDTH, AGENT_WIDTH);
        spriteBatch.draw(destRect, {0.0f, 0.0f, 1.0f, 1.0f}, _textureId, 1.0f, _color, _direction);
    }

    bool Agent::applyDamage(float damage)
    {
        _health -= damage;

        return (_health <= 0);
    }
}
