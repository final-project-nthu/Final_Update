#include <string>
#include "RobotEnemy.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/GameEngine.hpp"
#include "Bullet/Bullet.hpp"
#include "Enemy.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/LOG.hpp"
#include "Scene/PlayScene.hpp"
#include "Turret/Turret.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/ExplosionEffect.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/IObject.hpp"
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Player/Player.hpp"
RobotEnemy::RobotEnemy(int x, int y) : Enemy("play/robot.png", x, y, 10, 10, 30, 50),berserk(false){
}

void RobotEnemy::Update(float deltaTime) {
    if (!berserk && hp <= 15) {
        berserk = true;
        speed *= 5;

        // 播放暴衝音效
        AudioHelper::PlaySample("shockwave.ogg");

        // 加入暴衝啟動時的綠色爆炸效果（可換成更炫的特效圖）
        getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/explosion-3.png", 0.3 , Position.x, Position.y));
    }

    // 每 0.2 秒加一個綠色殘影（簡化示範）
    if (berserk) {
        shadowCooldown -= deltaTime;
        if (shadowCooldown <= 0) {
            shadowCooldown = 0.2; // 控制殘影間隔
            getPlayScene()->EffectGroup->AddNewObject(new DirtyEffect("play/explosion-3.png", 0.3 , Position.x, Position.y));
        }
    }

    // 保留原邏輯
    Enemy::Update(deltaTime);

    PlayScene* scene = getPlayScene();
    if (!scene) return;

    Engine::Point playerPos = scene->GetPlayerPosition();
    Engine::Point vec = playerPos - Position;
    float distance = vec.Magnitude();

    if (distance > 0)
        Velocity = (vec / distance) * speed;
    else
        Velocity = Engine::Point(0, 0);

    Position = Position + Velocity * deltaTime;
    Rotation = atan2(Velocity.y, Velocity.x);
    Sprite::Update(deltaTime);
}

void RobotEnemy::Draw() const {
    int w = GetBitmapWidth();
    int h = GetBitmapHeight();
    float drawX = Position.x - Anchor.x * w * scale;
    float drawY = Position.y - Anchor.y * h * scale;

    if (berserk) {
        al_draw_tinted_scaled_bitmap(
            bmp.get(),
            al_map_rgb(0, 255, 0), // 綠色暴衝狀態
            0, 0, w, h,
            drawX, drawY,
            w * scale, h * scale,
            0);
    } else {
        al_draw_scaled_bitmap(
            bmp.get(), 0, 0, w, h,
            drawX, drawY,
            w * scale, h * scale,
            0);
    }                // 無翻轉
}
