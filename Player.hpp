#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include "Engine/Sprite.hpp"

class PlayScene;

class Player : public Engine::Sprite {
protected:
    PlayScene* getPlayScene();
    float baseY;  // 角色原始Y座標（不包含震動offset）
    float timeSinceStart = 0.0f; // 用來累積時間做震動用
    float hitTimer = 0;            // 碰撞後紅色持續時間
    float knockbackX = 0, knockbackY = 0; // 擊退方向與力量
    float CollisionRadius;

public:
    float speed;
    bool Enabled = true;
    Player(std::string img, float x, float y, float speed);
    void Update(float deltaTime) override;
    void Draw() const override;
    Engine::Point GetPosition() const;
    bool facingLeft = false;
    void HitByEnemy(float fromX, float fromY); //受傷
    float GetCollisionRadius() const;
    float damageCooldown = 0.0f;  

};

#endif // PLAYER_HPP
