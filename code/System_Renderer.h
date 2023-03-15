#pragma once
#include <SFML/Graphics.hpp>

namespace Renderer 
{
    void Initialise(sf::RenderWindow&);
    sf::RenderWindow& getWindow();

    void Shutdown();
    void Update(const double&);
    void Queue(const sf::Drawable* s);
    void Render();
};