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

    if (velocity.x < 0) flipX = true;
    else if (velocity.x > 0) flipX = false;

    Position.x += velocity.x + knockbackX;
    baseY += velocity.y + knockbackY;

    knockbackX *= 0.85f;
    knockbackY *= 0.85f;

    Position.x = std::clamp(Position.x, 0.0f, mapWidth);
    baseY = std::clamp(baseY, 0.0f, mapHeight);

    float vibrationAmplitude = 3.5f;
    float vibrationFrequency = 0.6f;
    Position.y = baseY + vibrationAmplitude * sinf(timeSinceStart * 2 * 3.14159f * vibrationFrequency);

    // 滑鼠點擊射擊
    static bool prevLeftButtonDown = false;
    ALLEGRO_MOUSE_STATE mouseState;
    al_get_mouse_state(&mouseState);
    bool currLeftButtonDown = (mouseState.buttons & 1) != 0;

    if (currLeftButtonDown && !prevLeftButtonDown) {
        Engine::Point mousePos(mouseState.x, mouseState.y);
        Engine::Point direction = (mousePos - Position).Normalize();
        float bulletSpeed = 600;
        scene->BulletGroup->AddNewObject(new FreezeBullet(Position, direction, bulletSpeed, 0));
    }
    prevLeftButtonDown = currLeftButtonDown;

    // 判斷碰撞敵人
    for (auto& obj : scene->EnemyGroup->GetObjects()) {
        Enemy* enemy = dynamic_cast<Enemy*>(obj);  // 確保轉型成功
        if (!enemy) continue;

        float dx = Position.x - enemy->Position.x;
        float dy = baseY - enemy->Position.y;
        float dist = std::sqrt(dx * dx + dy * dy);
        float combinedRadius = enemy->GetCollisionRadius() + this->GetCollisionRadius();

        if (dist < combinedRadius) {
            HitByEnemy(enemy->Position.x, enemy->Position.y);
        }
    }

    if (hitTimer > 0) {
        hitTimer -= deltaTime;
        Tint = al_map_rgba(255, 128, 128, 255);
    } else {
        Tint = al_map_rgba(255, 255, 255, 255);
    }
}

void Player::HitByEnemy(float fromX, float fromY) {

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
