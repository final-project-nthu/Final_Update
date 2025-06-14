#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>
#include <memory>


class WinScene final : public Engine::IScene {
private:
    float ticks;
    ALLEGRO_SAMPLE_ID bgmId;
    int score;
    std::string playerName;
    Engine::Label* nameLabel = nullptr;
    int finalScore = 0;
public:
    explicit WinScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void BackOnClick(int stage);
    void setscore(int s){score=s;}
    void OnKeyDown(int keyCode) override;
};

#endif   // WINSCENE_HPP
