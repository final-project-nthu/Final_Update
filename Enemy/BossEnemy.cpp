#include <string>
#include "BossEnemy.hpp"

BossEnemy::BossEnemy(int x, int y) : Enemy("play/enemy-5.png", x, y, 10, 50, 30, 50),berserk(false){
}

// 覆寫 Update，檢查是否進入暴衝
void BossEnemy::Update(float deltaTime) {
    if (!berserk && hp <= 15) {
        berserk = true;
        speed *= 3;
    }
    Enemy::Update(deltaTime); // 保留原本的移動與行為
}