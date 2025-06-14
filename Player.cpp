#include "Player.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/GameEngine.hpp"
#include <allegro5/allegro.h>
#include <cmath>
#include <algorithm>
#include <allegro5/allegro_primitives.h>
#include "Bullet/FreezeBullet.hpp"
#include "Enemy/Enemy.hpp"

PlayScene* Player::getPlayScene() {
    return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

Player::Player(std::string img, float x, float y, float speed)
    : Engine::Sprite(img, x, y), speed(speed), baseY(y), CollisionRadius(16.0f) {}



void Player::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    timeSinceStart += deltaTime;

    if (damageCooldown > 0) damageCooldown -= deltaTime;

    PlayScene* scene = getPlayScene();
    const float mapWidth = PlayScene::MapWidth * PlayScene::BlockSize;
    const float mapHeight = PlayScene::MapHeight * PlayScene::BlockSize;

    ALLEGRO_KEYBOARD_STATE keyState;
    al_get_keyboard_state(&keyState);

    Engine::Point velocity(0, 0);
    if (al_key_down(&keyState, ALLEGRO_KEY_W)) velocity.y -= speed;
    if (al_key_down(&keyState, ALLEGRO_KEY_S)) velocity.y += speed;
    if (al_key_down(&keyState, ALLEGRO_KEY_A)) velocity.x -= speed;
    if (al_key_down(&keyState, ALLEGRO_KEY_D)) velocity.x += speed;

    if (velocity.Magnitude() > 0)
        velocity = velocity.Normalize() * speed * deltaTime;

    // 角色翻轉判斷
    if (velocity.x < 0) flipX = true;
    else if (velocity.x > 0) flipX = false;

    // 計算嘗試移動的新位置（含擊退）
    float tryX = Position.x + velocity.x + knockbackX;
    float tryY = baseY + velocity.y + knockbackY;

    // 先處理 X 軸移動與碰撞
    int gridX = static_cast<int>(tryX / PlayScene::BlockSize);
    int gridY_curr = static_cast<int>(baseY / PlayScene::BlockSize);
    if (gridX >= 0 && gridX < PlayScene::MapWidth &&
        gridY_curr >= 0 && gridY_curr < PlayScene::MapHeight &&
        scene->mapState[gridY_curr][gridX] == PlayScene::TILE_DIRT) {
        Position.x = tryX;
    }

    // 再處理 Y 軸移動與碰撞
    int gridY = static_cast<int>(tryY / PlayScene::BlockSize);
    int gridX_curr = static_cast<int>(Position.x / PlayScene::BlockSize);
    if (gridY >= 0 && gridY < PlayScene::MapHeight &&
        gridX_curr >= 0 && gridX_curr < PlayScene::MapWidth &&
        scene->mapState[gridY][gridX_curr] == PlayScene::TILE_DIRT) {
        baseY = tryY;
    }

    // 擊退阻力衰減
    knockbackX *= 0.85f;
    knockbackY *= 0.85f;

    // 限制不出地圖邊界
    Position.x = std::clamp(Position.x, 0.0f, mapWidth);
    baseY = std::clamp(baseY, 0.0f, mapHeight);

    // 角色垂直抖動效果
    float vibrationAmplitude = 3.5f;
    float vibrationFrequency = 0.6f;
    Position.y = baseY + vibrationAmplitude * sinf(timeSinceStart * 2 * 3.14159f * vibrationFrequency);

    // 滑鼠點擊射擊
    static bool prevLeftButtonDown = false;
    ALLEGRO_MOUSE_STATE mouseState;
    al_get_mouse_state(&mouseState);
    bool currLeftButtonDown = (mouseState.buttons & 1) != 0;

    // if (currLeftButtonDown && !prevLeftButtonDown) {
    //     Engine::Point mousePos(mouseState.x, mouseState.y);
    //     Engine::Point direction = (mousePos - Position).Normalize();
    //     float bulletSpeed = 600;
    //     scene->BulletGroup->AddNewObject(new FreezeBullet(Position, direction, bulletSpeed, 0));

    //     if (scene->enchantmentActive) {
    //         float chance = 0.3f; // 30% 機率雙發
    //         if (((float)rand() / RAND_MAX) < chance) {
    //             // 旋轉方向偏移約 10 度 (0.1745 弧度)
    //             float offsetAngle = 0.1745f;

    //             // 方向向量旋轉函式
    //             auto rotateVector = [](const Engine::Point& vec, float angle) -> Engine::Point {
    //                 return Engine::Point(
    //                     vec.x * cos(angle) - vec.y * sin(angle),
    //                     vec.x * sin(angle) + vec.y * cos(angle)
    //                 );
    //             };

    //             Engine::Point offsetDir = rotateVector(direction, offsetAngle);

    //             // 發射第二顆子彈
    //             scene->BulletGroup->AddNewObject(new FreezeBullet(Position, offsetDir, bulletSpeed, 0));
    //         }
    //     }
    // }

    if (currLeftButtonDown && !prevLeftButtonDown) {
    Engine::Point mousePos(mouseState.x, mouseState.y);
    Engine::Point direction = (mousePos - Position).Normalize();
    float bulletSpeed = 600;
    scene->BulletGroup->AddNewObject(new FreezeBullet(Position, direction, bulletSpeed, 0));

    if (scene->enchantmentLevel > 0) {
        // 以基礎30%機率，每等級機率加倍：0.3 * 2^(enchantmentLevel-1)
        float baseChance = 0.3f;
        float chance = baseChance * (1 << (scene->enchantmentLevel - 1));
        if (chance > 1.0f) chance = 1.0f;  // 機率最大為100%

        if (((float)rand() / RAND_MAX) < chance) {
            float offsetAngle = 0.1745f; // 約10度

            auto rotateVector = [](const Engine::Point& vec, float angle) -> Engine::Point {
                return Engine::Point(
                    vec.x * cos(angle) - vec.y * sin(angle),
                    vec.x * sin(angle) + vec.y * cos(angle)
                );
            };

            Engine::Point offsetDir = rotateVector(direction, offsetAngle);
            scene->BulletGroup->AddNewObject(new FreezeBullet(Position, offsetDir, bulletSpeed, 0));
        }
    }
}


    prevLeftButtonDown = currLeftButtonDown;

    // 碰撞敵人判斷
    for (auto& obj : scene->EnemyGroup->GetObjects()) {
        Enemy* enemy = dynamic_cast<Enemy*>(obj);
        if (!enemy) continue;

        float dx = Position.x - enemy->Position.x;
        float dy = baseY - enemy->Position.y;
        float dist = std::sqrt(dx * dx + dy * dy);
        float combinedRadius = enemy->GetCollisionRadius() + this->GetCollisionRadius();

        if (dist < combinedRadius) {
            HitByEnemy(enemy->Position.x, enemy->Position.y);
        }
    }

    // 受傷時顯示紅色
    if (hitTimer > 0) {
        hitTimer -= deltaTime;
        Tint = al_map_rgba(255, 128, 128, 255);
    }
    else {
        Tint = al_map_rgba(255, 255, 255, 255);
    }
}


void Player::HitByEnemy(float fromX, float fromY) {
    if (damageCooldown > 0) return; 

    damageCooldown = 1.0f; 
    hitTimer = 0.2f;

    Engine::Point dir = Engine::Point(Position.x - fromX, baseY - fromY).Normalize();
    float knockbackStrength = 300;
    knockbackX = dir.x * knockbackStrength * 0.016f;
    knockbackY = dir.y * knockbackStrength * 0.016f;

    PlayScene* scene = getPlayScene();
    scene->Hit();
}


void Player::Draw() const {
    float shadowX = Position.x - 2;
    float shadowY = baseY + 60;
    float shadowWidth = Size.x * 0.6f;
    float shadowHeight = Size.y * 0.15f;
    al_draw_filled_ellipse(shadowX, shadowY, shadowWidth / 2, shadowHeight / 2,
                           al_map_rgba(0, 0, 0, 100));

    Sprite::Draw();
}

Engine::Point Player::GetPosition() const {
    return Position;
}

float Player::GetCollisionRadius() const {
    return CollisionRadius;
}
