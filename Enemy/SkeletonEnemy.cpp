#include <string>
#include "SkeletonEnemy.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/GameEngine.hpp"
#include "Enemy.hpp"
#include <cmath>
SkeletonEnemy::SkeletonEnemy(int x, int y) : Enemy("play/skeleton.png", x, y, 50, 50, 5, 5){
}

void SkeletonEnemy::Draw() const {
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