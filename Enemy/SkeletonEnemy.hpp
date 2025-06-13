#ifndef SKELETONENEMY_HPP
#define SKELETONENEMY_HPP
#include "Enemy.hpp"

class SkeletonEnemy : public Enemy {
public:
    SkeletonEnemy(int x, int y);
    void Draw() const override;
    float scale=0.2;
};

#endif   // SKELETONENEMY_HPP
