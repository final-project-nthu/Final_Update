#ifndef DYYENEMY_HPP
#define DYYENEMY_HPP
#include "Enemy.hpp"

class DyyEnemy : public Enemy {
public:
   DyyEnemy(int x, int y);
protected:
    void OnExplode() override;
};

#endif   // DYYENEMY_HPP
