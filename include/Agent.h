#pragma once

#include <Toaster/SpriteBatch.h>

#include <GLM/glm.hpp>

#include <string>
#include <vector>

const float AGENT_WIDTH = 60.0F;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

namespace TKSZG
{
    class Human;
    class Zombie;

    class Agent
    {
    public:
        Agent();
        virtual ~Agent();

        virtual void update(const std::vector<std::string> &levelData,
                            std::vector<Human *> &humans,
                            std::vector<Zombie *> &zombies,
                            float deltaTime) = 0;

        bool collideWithLevel(const std::vector<std::string> &levelData);

        bool collideWithAgent(Agent *agent);

        void draw(Toaster::SpriteBatch &_spriteBatch);

        // Return true if agent dies
        bool applyDamage(float damage);

        const glm::vec2 getPosition() const { return _position; }

    protected:
        void checkTilePosition(const std::vector<std::string> &levelData,
                               std::vector<glm::vec2> &collideTilePositions,
                               float x,
                               float y);

        void collideWithTile(glm::vec2 tilePos);

        glm::vec2 _position;
        glm::vec2 _direction = glm::vec2(1.0f, 0.0f);
        Toaster::ColorRGBA8 _color;
        float _speed;
        float _health;
        GLuint _textureId;
    };
}
