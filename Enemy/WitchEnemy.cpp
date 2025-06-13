#include "WitchEnemy.hpp"
#include "Enemy/SkeletonEnemy.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/GameEngine.hpp"
#include "Enemy.hpp"
#include <cmath>

WitchEnemy::WitchEnemy(float x, float y)
    : Enemy("play/witch.png", x, y, 1, 50, 100, 10), // radius, speed, HP, money
      summonRange(300), summonCooldown(6.0f), summonTimer(0) {
    // 不做額外設定，沿用父類別 Enemy 的 pathfinding 行為
}

void WitchEnemy::Update(float deltaTime) {
    // ✅ 先讓父類別執行它原本的 pathfinding 和移動邏輯
    Enemy::Update(deltaTime);

    // 🧙‍♀️ 巫師的召喚邏輯
    PlayScene* scene = getPlayScene();
    if (!scene) return;

    Engine::Point playerPos = scene->GetPlayerPosition();  // 你需要在 PlayScene 實作這個函式
    Engine::Point vec = playerPos - Position;
    float distance = vec.Magnitude();

    summonTimer += deltaTime;

    if (distance <= summonRange && summonTimer >= summonCooldown) {
        Summon();
        summonTimer = 0;
    }
}

void WitchEnemy::Summon() {
   // 在左右兩邊產生 PlaneEnemy
    PlayScene* scene = getPlayScene();
    const float offset = 1; // 兩側的偏移量

    int gridX = static_cast<int>(Position.x / PlayScene::BlockSize);
    int gridY = static_cast<int>(Position.y / PlayScene::BlockSize);

    if (gridX - 1 >= 0) {
        auto* e1 = new SkeletonEnemy((gridX - 1) * PlayScene::BlockSize + PlayScene::BlockSize / 2,
                                gridY * PlayScene::BlockSize + PlayScene::BlockSize / 2);
        e1->UpdatePath(scene->mapDistance);
        scene->EnemyGroup->AddNewObject(e1);
    }

    if (gridX + 1 < PlayScene::MapWidth) {
        auto* e2 = new SkeletonEnemy((gridX + 1) * PlayScene::BlockSize + PlayScene::BlockSize / 2,
                                gridY * PlayScene::BlockSize + PlayScene::BlockSize / 2);
        e2->UpdatePath(scene->mapDistance);
        scene->EnemyGroup->AddNewObject(e2);
    }
}

void WitchEnemy::Draw() const {
    int w = GetBitmapWidth();
    int h = GetBitmapHeight();
    float drawX = Position.x - Anchor.x * w * scale;
    float drawY = Position.y - Anchor.y * h * scale;

    al_draw_scaled_bitmap(
        bmp.get(),         // bitmap 指標
        0, 0, w, h,         // 原圖範圍
        drawX, drawY,       // 畫在哪
        w * scale, h * scale, // 縮放大小
        0);                // 無翻轉
}

