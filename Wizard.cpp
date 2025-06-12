#include "Player/Wizard.hpp"
#include <allegro5/allegro.h>

// 呼叫Player的單張圖片建構子
Wizard::Wizard(const std::string& img, float x, float y, float speed)
    : Player(img, x, y, speed) {
}

void Wizard::Update(float deltaTime) {
    Player::Update(deltaTime);

    ALLEGRO_KEYBOARD_STATE keyState;
    al_get_keyboard_state(&keyState);

    if (al_key_down(&keyState, ALLEGRO_KEY_Q)) CastQ();
    if (al_key_down(&keyState, ALLEGRO_KEY_E)) CastE();
    if (al_key_down(&keyState, ALLEGRO_KEY_R)) CastR();
}

void Wizard::CastQ() {
    // Q skill placeholder
}

void Wizard::CastE() {
    // E skill placeholder
}

void Wizard::CastR() {
    // R skill placeholder
}
