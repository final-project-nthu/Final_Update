#include <string>
#include "RobotEnemy.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/GameEngine.hpp"
RobotEnemy::RobotEnemy(int x, int y) : Enemy("play/robot.png", x, y, 10, 50, 30, 50),berserk(false){
}

// 覆寫 Update，檢查是否進入暴衝
void RobotEnemy::Update(float deltaTime) {
    if (!berserk && hp <= 15) {
        berserk = true;
        speed *= 3;
    }
    Enemy::Update(deltaTime); // 保留原本的移動與行為

    PlayScene* scene = getPlayScene();
    if (!scene) return;
    
    Engine::Point playerPos = scene->GetPlayerPosition();  
    Engine::Point vec = playerPos - Position;
    float distance = vec.Magnitude();

    if (distance > 0) {
        Engine::Point normalized = vec / distance;
        Velocity = normalized * speed;
    } else {
        Velocity = Engine::Point(0, 0);
    }

    Position = Position + Velocity * deltaTime;
    Rotation = atan2(Velocity.y, Velocity.x);
    Sprite::Update(deltaTime);
}

void RobotEnemy::Draw() const {
    int w = GetBitmapWidth();
    int h = GetBitmapHeight();
    float drawX = Position.x - Anchor.x * w * scale;
    float drawY = Position.y - Anchor.y * h * scale;

    al_draw_scaled_bitmap(
        bmp.get(),         // bitmap 指標
        0, 0, w, h,         // 原圖範圍
        drawX, drawY,       // 畫在哪
        w * scale, h * scale, // 縮放大小
        1);                // 無翻轉
}
