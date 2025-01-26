#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "BulletManager.h"
#include "WallManager.h"
#include "HashGridTool.h"

#define M_PI 3.14159265358979323846
using namespace sf;


class FrameworkClass
{
    friend class BulletManager;
    friend class WallManager;

public:

    FrameworkClass();
    void UpdateScaleSettings(int InScale);
    void Run();

private:

    // Render variables
    std::unique_ptr<RenderWindow> Window;
    View View;
    Vector2f ViewCenter;
    ContextSettings RenderSettings;
    Vector2f MousePosition;
    unsigned int ScreenHeight;
    unsigned int ScreenWidth;
    unsigned int ScreenScale = 1;

    // FPS data variables
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

    // Bullet Manager
    std::unique_ptr<BulletManager> BulletManagerPtr;

    // Wall Manager
    std::unique_ptr<WallManager> WallManagerPtr;

    // Other
    Text BulletsCountText;
    Text WallsCountText;
    Int32 LastShootTime = 0;
    Int32 ShootDiapason = 100;
};