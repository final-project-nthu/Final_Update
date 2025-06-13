#ifndef WITCHENEMY_HPP
#define WITCHENEMY_HPP

#include "Enemy.hpp"

class WitchEnemy : public Enemy {
private:
    float summonRange;     // 召喚範圍
    float keepDistance;    // 與玩家保持的距離
    float summonCooldown;  // 召喚冷卻時間
    float summonTimer;     // 召喚計時器
    float scale=0.08;
public:
    WitchEnemy(float x, float y);
    void Update(float deltaTime) override;
    void Summon();  // 召喚小怪
    void Draw() const override;
};

#endif
