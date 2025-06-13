#ifndef ROBOTENEMY_HPP
#define ROBOTENEMY_HPP
#include "Enemy.hpp"

class RobotEnemy : public Enemy {
public:
    RobotEnemy(int x, int y);
    void Update(float deltaTime) override; // 覆寫 Update 以檢查血量
    float scale=0.3;
protected:
    bool berserk; // 是否進入暴衝模式
    void Draw() const override;
};

#endif   // BOSSENEMY_HPP
