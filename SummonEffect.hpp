#ifndef SUMMONEFFECT_HPP
#define SUMMONEFFECT_HPP

#include <string>
#include "Engine/Sprite.hpp"

class PlayScene;

class SummonEffect : public Engine::Sprite {
protected:
    float timeSpan;
    float alpha;
    float timer;
    PlayScene* getPlayScene();
    float scale=1;
public:
    SummonEffect(std::string img, float timeSpan, float x, float y, float scale = 1.0f);
    void Update(float deltaTime) override;
    void Draw() const override;
};
#endif  // SUMMONEFFECT_HPP
