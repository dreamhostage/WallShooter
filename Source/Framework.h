#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#define M_PI 3.14159265358979323846
using namespace sf;


class FrameworkClass
{

public:

    FrameworkClass();
    void Run();

private:

    // Render variables
    std::unique_ptr<RenderWindow> Window;
    View View;
    Vector2f ViewCenter;
    ContextSettings RenderSettings;
    Vector2f MousePosition;
    const unsigned int ScreenHeight = VideoMode::getDesktopMode().height;
    const unsigned int ScreenWidth = VideoMode::getDesktopMode().width;

    // FPS variables
    float CurrentFPS;
    Clock Clock = sf::Clock::Clock();
    Time PreviousTime = Clock.getElapsedTime();
    Time CurrentTime;
    Text FPSText;
    Font Font;
    const Int32 FPSChangingTimeMls = 500;
    Int32 LastFPSChangingTime = 0;
    Int32 CurrentMlsTime = 0;

    // Sprites
    Sprite CursorSprite;
    Sprite ScreenSprite;
    Sprite ShipSprite;
    // Textures
    Texture CursorTexture;
    Texture ScreenTexture;
    Texture ShipTexture;
    Vector2u TextureSize;
};