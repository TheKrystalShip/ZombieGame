#include "Level.h"

#include <Toaster/Errors.h>
#include <Toaster/SpriteBatch.h>
#include <Toaster/ResourceManager.h>

#include <fstream>
#include <iostream>

namespace TKSZG
{
    Level::Level(const std::string &fileName)
    {
        std::ifstream file;
        file.open(fileName);

        if (file.fail())
            Toaster::fatalError("Failed to open " + fileName);

        // Ignore first string "Humans: " in file, take the nummber after it.
        std::string temp;
        file >> temp >> _numHumans;

        std::getline(file, temp); // Throw away the rest of the first line that's not part of the level.

        while (std::getline(file, temp))
            _levelData.push_back(temp);

        _spriteBatch.init();
        _spriteBatch.begin();

        glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

        for (size_t y = 0; y < _levelData.size(); y++)
        {
            for (size_t x = 0; x < _levelData[y].size(); x++)
            {
                char tile = _levelData[y][x];

                // Destination rect
                glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

                switch (tile)
                {
                case 'B': // Brick
                case 'R': // Red brick
                    _spriteBatch.draw(
                        destRect,
                        uvRect,
                        Toaster::ResourceManager::getTexture("assets/textures/red_bricks.png").id,
                        0.0f,
                        Toaster::ColorRGBA8(255, 255, 255));
                    break;

                case 'L': // Light bricks
                    _spriteBatch.draw(
                        destRect,
                        uvRect,
                        Toaster::ResourceManager::getTexture("assets/textures/light_bricks.png").id,
                        0.0f,
                        Toaster::ColorRGBA8(255, 255, 255));
                    break;

                case 'G': // Glass block
                    _spriteBatch.draw(
                        destRect,
                        uvRect,
                        Toaster::ResourceManager::getTexture("assets/textures/glass.png").id,
                        0.0f,
                        Toaster::ColorRGBA8(255, 255, 255));
                    break;

                case '@': // Player
                    _levelData[y][x] = '.'; // To avoid collision with itself
                    _playerStartPosition.x = x * TILE_WIDTH;
                    _playerStartPosition.y = y * TILE_WIDTH;
                    break;

                case 'Z': // Zombie
                    _levelData[y][x] = '.'; // To avoid collision with itself
                    _zombieStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
                    break;

                case '.': // Empty space
                    break;
                default:
                    std::printf("Unexpected symbol %c at (%lu,%lu)\n", tile, x, y);
                    break;
                }
            }
        }

        _spriteBatch.end();
    }

    Level::~Level()
    {
    }

    void Level::draw()
    {
        _spriteBatch.renderBatch();
    }
}
