#include <allegro5/base.h>
#include <allegro5/color.h>

#include "SummonEffect.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/GameEngine.hpp"

PlayScene* SummonEffect::getPlayScene() {
    return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

SummonEffect::SummonEffect(std::string img, float timeSpan, float x, float y, float scale)
    : Sprite(img, x, y), timeSpan(timeSpan), alpha(1.0f), timer(0.0f) {
    this->scale = scale;
    this->Rotation = 0.0f;  // 固定角度，不旋轉
}

void SummonEffect::Update(float deltaTime) {
    timer += deltaTime;
    alpha = 1.0f - (timer / timeSpan);
    if (alpha <= 0) {
        getPlayScene()->EffectGroup->RemoveObject(objectIterator);
        return;
    }
    // 更新透明度
    unsigned char r, g, b, a = static_cast<unsigned char>(alpha * 255);
    al_unmap_rgb(Tint, &r, &g, &b);
    Tint = al_map_rgba(r, g, b, a);
    Sprite::Update(deltaTime);
}

void SummonEffect::Draw() const{
    int w = GetBitmapWidth();
    int h = GetBitmapHeight();
    float drawX = Position.x - Anchor.x * w * scale;
    float drawY = Position.y - Anchor.y * h * scale;

    al_draw_tinted_scaled_bitmap(
        bmp.get(), Tint,       // 使用 Tint 來處理透明度
        0, 0, w, h,             // 原圖範圍
        drawX, drawY,           // 畫的位置
        w * scale, h * scale,   // 縮放大小
        0                       // 無翻轉
    );
}