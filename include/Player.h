#pragma once

#include "Human.h"

#include <Toaster/Camera2D.h>
#include <Toaster/InputManager.h>

namespace TKSZG
{
    class Zombie;
    class Gun;
    class Bullet;

    class Player : public Human
    {
    public:
        Player();
        ~Player();

        void init(float speed, glm::vec2 position, Toaster::InputManager *inputManager, Toaster::Camera2D *camera, std::vector<Bullet> *bullets);

        void addGun(Gun *gun);

        void update(const std::vector<std::string> &levelData,
                    std::vector<Human *> &humans,
                    std::vector<Zombie *> &zombies,
                    float deltaTime) override;

    private:
        Toaster::InputManager *_inputManager;
        Toaster::Camera2D *_camera;
        std::vector<Gun *> _guns;
        int _currentGunIndex;
        std::vector<Bullet> *_bullets;
    };
}
