#pragma once

#include "Bullet.h"

#include <Toaster/AudioEngine.h>

#include <GLM/glm.hpp>

#include <vector>
#include <string>

namespace TKSZG
{
    class Gun
    {
    public:
        Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed, Toaster::SoundEffect fireEffect);
        ~Gun();

        void update(bool isMouseDown, const glm::vec2 &position, const glm::vec2 &direction, std::vector<Bullet> &bullets, float deltaTime);

    private:
        void fire(const glm::vec2 &direction, const glm::vec2 &position, std::vector<Bullet> &bullets);

        std::string _name;
        int _fireRate;
        int _bulletsPerShot;
        float _spread;

        float _bulletDamage;
        float _bulletSpeed;

        float _frameCounter;

        Toaster::SoundEffect _fireEffect;
    };
}
