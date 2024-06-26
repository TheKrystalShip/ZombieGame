#include "Gun.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/rotate_vector.hpp>

#include <ctime>
#include <random>

namespace TKSZG
{
    Gun::Gun(std::string name, int fireRate, int bulletsPerShot,
             float spread, float bulletDamage, float bulletSpeed, Toaster::SoundEffect fireEffect) : _name(name),
                                                                                                     _fireRate(fireRate),
                                                                                                     _bulletsPerShot(bulletsPerShot),
                                                                                                     _spread(spread),
                                                                                                     _bulletDamage(bulletDamage),
                                                                                                     _bulletSpeed(bulletSpeed),
                                                                                                     _frameCounter(0.0f),
                                                                                                     _fireEffect(fireEffect)
    {
    }

    Gun::~Gun()
    {
    }

    void Gun::update(bool isMouseDown, const glm::vec2 &position, const glm::vec2 &direction, std::vector<Bullet> &bullets, float deltaTime)
    {
        _frameCounter += 1.0f * deltaTime;

        if (_frameCounter >= _fireRate && isMouseDown)
        {
            fire(direction, position, bullets);
            _frameCounter = 0;
        }
    }

    void Gun::fire(const glm::vec2 &direction, const glm::vec2 &position, std::vector<Bullet> &bullets)
    {
        static std::mt19937 randomEngine(time(nullptr));
        std::uniform_real_distribution<float> randSpread(-_spread, _spread);

        _fireEffect.play();

        for (int i = 0; i < _bulletsPerShot; i++)
        {
            bullets.emplace_back(position, glm::rotate(direction, glm::radians((randSpread(randomEngine)))), _bulletDamage, _bulletSpeed);
        }
    }
}
