#ifndef WIZARD_HPP
#define WIZARD_HPP

#include "Player.hpp"
#include <string>

class Wizard : public Player {
public:
    
    Wizard(const std::string& img, float x, float y, float speed);
    void Update(float deltaTime) override;
    void CastQ();
    void CastE();
    void CastR();
};

#endif // WIZARD_HPP
