#ifndef SCOREBOARDSCENE_HPP
#define SCOREBOARDSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include <tuple>
#include <vector>
#include "Engine/IScene.hpp"

class ScoreboardScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    std::vector<std::tuple<std::string, int, std::string>> scores;
    int currentPage = 0;
    const int itemsPerPage = 5;

public:
    explicit ScoreboardScene() = default;
    void Initialize() override;
    void Terminate() override;
    void PrevPageOnClick(int stage);
    void NextPageOnClick(int stage);
    void BackOnClick(int stage);
    void DisplayCurrentPage();

    void BGMSlideOnValueChanged(float value);
    void SFXSlideOnValueChanged(float value);
};

#endif   // STAGESELECTSCENE_HPP
