#pragma once

#include "Agent.h"

#include <GLM/glm.hpp>

namespace TKSZG
{
    class Human;

    class Zombie : public Agent
    {
    public:
        Zombie();
        ~Zombie();

        void init(float speed, glm::vec2 position);

        void update(const std::vector<std::string> &levelData,
                    std::vector<Human *> &humans,
                    std::vector<Zombie *> &zombies,
                    float deltaTime) override;

    private:
        Human *getClosestHuman(std::vector<Human *> humans);
    };
}
