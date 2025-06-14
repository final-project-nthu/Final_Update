#include <functional>
#include <string>
#include <fstream>
#include <ctime>
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "Scene/WinScene.hpp"
#include "Scene/PlayScene.hpp"

void WinScene::Initialize() {
    ticks = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH, 0, 0, 0.5, 0.5));
    AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 - 10, 255, 255, 255, 255, 0.5, 0.5));

    std::string scoreText = "Score: " + std::to_string(score);
    AddNewObject(new Engine::Label(scoreText, "pirulen.ttf", 36, halfW, halfH / 4+40, 255, 255, 255, 255, 0.5, 0.5));

    nameLabel = new Engine::Label("", "pirulen.ttf", 28, halfW, halfH / 4 + 80, 255, 255, 0, 255, 0.5, 0.5);

    AddNewObject(nameLabel);

    Engine::ImageButton* btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
    bgmId = AudioHelper::PlayAudio("win.wav");
}

void WinScene::OnKeyDown(int keyCode) {
    if (keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z) {
        if (playerName.length() < 12) {
            char c = 'A' + (keyCode - ALLEGRO_KEY_A);
            playerName += c;
        }
    } else if (keyCode == ALLEGRO_KEY_BACKSPACE && !playerName.empty()) {
        playerName.pop_back();
    } else if (keyCode == ALLEGRO_KEY_ENTER) {
        int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
        int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
        AddNewObject(new Engine::Label("Register successfully", "pirulen.ttf", 28,
            w / 2, h / 2 + 230, 0, 255, 0, 255, 0.5, 0.5));

    }

    if (nameLabel)
        nameLabel->Text = playerName;
}



void WinScene::Terminate() {
    IScene::Terminate();
    AudioHelper::StopBGM(bgmId);
}

void WinScene::Update(float deltaTime) {
    ticks += deltaTime;
    if (ticks > 4 && ticks < 100 &&
        dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
        ticks = 100;
        bgmId = AudioHelper::PlayBGM("happy.ogg");
    }
}

void WinScene::BackOnClick(int stage) {
    std::ofstream fout("C:/Users/USER/Downloads/2025_I2P2_TowerDefense-main/2025_I2P2_TowerDefense-main/Resource/scoreboard.txt", std::ios::app);
    if (fout.is_open()) {
        if (playerName.empty()) playerName = "Player";

        // 取得目前時間
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);
        char timeBuffer[64];
        std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", localTime);

        // 寫入名字、分數與時間
        fout << playerName << " " << score << " " << timeBuffer << "\n";
        fout.close();
    }
    playerName.clear();
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
