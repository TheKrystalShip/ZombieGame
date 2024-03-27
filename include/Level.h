#pragma once

#include <SpriteBatch.h>
#include <ResourceManager.h>

#include <GLM/glm.hpp>

#include <string>
#include <vector>

const int TILE_WIDTH = 64;

namespace TKSZG
{
    class Level
    {
    public:
        Level(const std::string &fileName);
        ~Level();

        void draw();

        int getWidth() const { return _levelData[0].size(); }
        int getHeight() const { return _levelData.size(); }
        glm::vec2 getPlayerStartPos() const { return _playerStartPosition; }
        const std::vector<glm::vec2> &getZomobieStartPositions() const { return _zombieStartPositions; }
        const std::vector<std::string> &getLevelData() const { return _levelData; }
        int getNumHumans() const { return _numHumans; }

    private:
        std::vector<std::string> _levelData;
        int _numHumans;
        Toaster::SpriteBatch _spriteBatch;

        glm::vec2 _playerStartPosition;
        std::vector<glm::vec2> _zombieStartPositions;
    };
}
