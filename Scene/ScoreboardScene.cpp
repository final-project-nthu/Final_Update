#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <fstream>      
#include <sstream>      
#include <vector>
#include <utility>      
#include <algorithm>    

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "ScoreboardScene.hpp"
#include "PlayScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

void ScoreboardScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;

    AddNewObject(new Engine::Image("stage-select/scoreboard_background.jpg", 0, 0,
    w, h, 0, 0));

    btn = new Engine::ImageButton("stage-select/rock.jpg", "stage-select/floor.png", halfW - 650, halfH / 2 + 400, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::PrevPageOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Prev_Page", "pirulen.ttf", 48, halfW - 450, halfH / 2 + 450, 255, 255, 255, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/rock.jpg", "stage-select/floor.png", halfW - 200, halfH / 2 + 400, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::BackOnClick, this, 4));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH / 2 + 450, 255, 255, 255, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/rock.jpg", "stage-select/floor.png", halfW + 250, halfH / 2 + 400, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::NextPageOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Next_Page", "pirulen.ttf", 48, halfW + 450, halfH / 2 + 450, 255, 255, 255, 255, 0.5, 0.5));

    AddNewObject(new Engine::Label("Scoreboard", "pirulen.ttf", 48, halfW, halfH / 2 - 150, 0, 255, 0, 255, 0.5, 0.5));

    bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);

    // 讀取 scoreboard.txt
    std::ifstream fin("C:/Users/USER/Downloads/2025_I2P2_TowerDefense-main/2025_I2P2_TowerDefense-main/Resource/scoreboard.txt");
    std::string line;
    scores.clear();

    while (std::getline(fin, line)) {
        std::istringstream iss(line);
        std::string name, timePart1, timePart2;
        int score;
        if (iss >> name >> score >> timePart1 >> timePart2) {
            std::string timestamp = timePart1 + " " + timePart2;
            scores.emplace_back(name, score, timestamp);
        }
    }
    fin.close();

    // 按分數排序
    std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
        return std::get<1>(a) > std::get<1>(b);
    });

    currentPage = 0;
    DisplayCurrentPage();
}

void ScoreboardScene::DisplayCurrentPage() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    // 清除之前的 Label 文字（只保留固定按鈕與標題）
    objects.remove_if([](const std::pair<bool, Engine::IObject*>& obj) {
        auto label = dynamic_cast<Engine::Label*>(obj.second);
        return label && label->Text != "Scoreboard" &&
                        label->Text != "Prev_Page" &&
                        label->Text != "Back" &&
                        label->Text != "Next_Page";
    });

    // 顯示目前頁的 5 筆資料
    int start = currentPage * itemsPerPage;
    int end = std::min(start + itemsPerPage, static_cast<int>(scores.size()));

    for (int i = start; i < end; ++i) {
        const auto& entry = scores[i];
        std::string displayText = std::to_string(i + 1) + ". " +
                                  std::get<0>(entry) + " - " +
                                  std::to_string(std::get<1>(entry)) + " - " +
                                  std::get<2>(entry);
        AddNewObject(new Engine::Label(displayText, "pirulen.ttf", 24,
            halfW, halfH / 2 - 50 + (i - start) * 50, 255, 255, 0, 255, 0.5, 0.5));
    }
}

void ScoreboardScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void ScoreboardScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}
void ScoreboardScene::NextPageOnClick(int stage) {
    int totalPages = (scores.size() + itemsPerPage - 1) / itemsPerPage;
    if (currentPage + 1 < totalPages) {
        currentPage++;
        DisplayCurrentPage();
    }
}
void ScoreboardScene::PrevPageOnClick(int stage) {
    if (currentPage > 0) {
        currentPage--;
        DisplayCurrentPage();
    }
}
void ScoreboardScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}
void ScoreboardScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}