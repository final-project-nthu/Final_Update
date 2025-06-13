#ifndef BOSSENEMY_HPP
#define BOSSENEMY_HPP
#include "Enemy.hpp"

class BossEnemy : public Enemy {
public:
    BossEnemy(int x, int y);
    void Update(float deltaTime) override; // 覆寫 Update 以檢查血量
protected:
    bool berserk; // 是否進入暴衝模式
};

#endif   // BOSSENEMY_HPP
