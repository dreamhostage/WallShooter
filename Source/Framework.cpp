#include "Framework.h"

#include <vector>
#include <thread>


FrameworkClass::FrameworkClass()
{
    // Screen size settings
    ScreenHeight = VideoMode::getDesktopMode().height * ScreenScale;
    ScreenWidth = VideoMode::getDesktopMode().width * ScreenScale;

    // Textures & Sprites settings
    CursorTexture.loadFromFile("Images/aim.png");
    ScreenTexture.loadFromFile("Images/screen.png");
    ShipTexture.loadFromFile("Images/ship.png");

    CursorSprite.setTexture(CursorTexture);
    ScreenSprite.setTexture(ScreenTexture);
    ShipSprite.setTexture(ShipTexture);

    TextureSize = CursorTexture.getSize();
    CursorSprite.setOrigin(TextureSize.x / 2, TextureSize.y / 2);
    TextureSize = ScreenTexture.getSize();
    ScreenSprite.setOrigin(TextureSize.x / 2, TextureSize.y / 2);
    ScreenSprite.setPosition(ScreenWidth / 2, ScreenHeight / 2);
    ScreenSprite.setScale(ScreenWidth / ScreenSprite.getLocalBounds().width, ScreenHeight / ScreenSprite.getLocalBounds().height);
    TextureSize = ShipTexture.getSize();
    ShipSprite.setOrigin(TextureSize.x / 2, TextureSize.y / 2);
    ShipSprite.setPosition(ScreenWidth / 2, ScreenHeight - 100);

    // Window & render settings
    RenderSettings.antialiasingLevel = 0;
    Window = std::make_unique<RenderWindow>(VideoMode(ScreenWidth, ScreenHeight), "WallShooter", Style::Fullscreen, RenderSettings);
    ViewCenter.x = ScreenWidth / 2;
    ViewCenter.y = ScreenHeight / 2;
    View = Window->getView();
    View.setCenter(ViewCenter);
    View.setSize(Vector2f(ScreenWidth, ScreenHeight));
    Window->setFramerateLimit(90);
    Window->setView(View);
    Window->setMouseCursorVisible(false);
    Window->setVerticalSyncEnabled(true);

    // FPS settings
    FPSText.setCharacterSize(40 * ScreenScale);
    Font.loadFromFile("Images/18949.ttf");
    FPSText.setFont(Font);
    FPSText.setString("FPS: XXX");
    FPSText.setPosition(ScreenWidth - FPSText.getGlobalBounds().width, FPSText.getGlobalBounds().height);

    // Bullet manager initialization
    BulletManagerPtr = std::make_unique<BulletManager>(this);

    // Wall manager initialization
    WallManagerPtr = std::make_unique<WallManager>(this);

    // BulletsCount text settings
    BulletsCountText.setCharacterSize(40 * ScreenScale);
    BulletsCountText.setFont(Font);
    BulletsCountText.setString("Bullets: X");
    BulletsCountText.setPosition(100, BulletsCountText.getGlobalBounds().height);
    FPSText.setFillColor(Color::White);

    // WallsCount text settings
    WallsCountText.setCharacterSize(40 * ScreenScale);
    WallsCountText.setFont(Font);
    WallsCountText.setString("Walls:   X");
    WallsCountText.setPosition(100, WallsCountText.getGlobalBounds().height * 2.5);
    FPSText.setFillColor(Color::White);
}

void FrameworkClass::Run()
{
    while (Window->isOpen())
    {
        Event event;
        MousePosition = Window->mapPixelToCoords(Mouse::getPosition(*Window));

        while (Window->pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                Window->close();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            exit(0);
        }

        // Test launching keys performing
        if (Keyboard::isKeyPressed(Keyboard::Num1))
        {
            if (ScreenScale != 2)
            {
                UpdateScaleSettings(2);
            }

            WallManagerPtr->SpawnWalls(10000);
        }

        if (Keyboard::isKeyPressed(Keyboard::Num2))
        {
            BulletManagerPtr->SpawnBulletsCount(10000);
        }

        if (Keyboard::isKeyPressed(Keyboard::Num3))
        {
            if (ScreenScale != 6)
            {
                UpdateScaleSettings(6);
            }

            WallManagerPtr->SpawnWalls2(100000);
        }

        if (Keyboard::isKeyPressed(Keyboard::Num4))
        {
            BulletManagerPtr->SpawnBulletsCount(100000);
        }

        // Display data
        Window->draw(ScreenSprite);
        Window->draw(CursorSprite);
        Window->draw(ShipSprite);
        // WallManagerPtr->Update(Clock.getElapsedTime().asMilliseconds());
        //---
        WallManagerPtr->Update2(Clock.getElapsedTime().asMilliseconds());
        //---
        BulletManagerPtr->Update(Clock.getElapsedTime().asMilliseconds());
        Window->draw(FPSText);
        Window->draw(BulletsCountText);
        Window->draw(WallsCountText);
        Window->display();
        Window->clear();

        // Calculating Ship rotation
        Vector2f DirectionVector = ShipSprite.getPosition() - MousePosition;
        float ShipRotation = (atan2(DirectionVector.y, DirectionVector.x)) * 180 / M_PI;
        ShipSprite.setRotation(ShipRotation);
        CursorSprite.setPosition(Window->mapPixelToCoords(Mouse::getPosition(*Window)));

        // FPS calculation
        CurrentTime = Clock.getElapsedTime();
        CurrentFPS = 1.0f / (CurrentTime.asSeconds() - PreviousTime.asSeconds());
        PreviousTime = CurrentTime;

        CurrentMlsTime = CurrentTime.asMilliseconds();
        if (CurrentMlsTime - LastFPSChangingTime > FPSChangingTimeMls)
        {
            LastFPSChangingTime = CurrentTime.asMilliseconds();
            FPSText.setString("FPS: " + std::to_string((int)CurrentFPS));

            if (CurrentFPS >= 0 && CurrentFPS < 30)
            {
                FPSText.setFillColor(Color::Red);
            }
            else if (CurrentFPS >= 30 && CurrentFPS < 60)
            {
                FPSText.setFillColor(Color::Yellow);
            }
            else
            {
                FPSText.setFillColor(Color::Green);
            }
        }

        // Bullets count text updating
        BulletsCountText.setString("Bullets: " + std::to_string(BulletManagerPtr->GetBulletsCount()));
        // Walls count text updating
        WallsCountText.setString("Walls:     " + std::to_string(WallManagerPtr->GetWallsCount()));
    }
}

void FrameworkClass::UpdateScaleSettings(int InScale)
{
    // Screen size settings
    ScreenScale = InScale;
    ScreenHeight = VideoMode::getDesktopMode().height * ScreenScale;
    ScreenWidth = VideoMode::getDesktopMode().width * ScreenScale;

    ScreenSprite.setPosition(ScreenWidth / 2, ScreenHeight / 2);
    ScreenSprite.setScale(ScreenWidth / ScreenSprite.getLocalBounds().width, ScreenHeight / ScreenSprite.getLocalBounds().height);
    ShipSprite.setPosition(ScreenWidth / 2, ScreenHeight - 100);
    Window->create(sf::VideoMode(ScreenWidth, ScreenHeight), "WallShooter", Style::Fullscreen, RenderSettings);
    Window->setFramerateLimit(90);
    Window->setMouseCursorVisible(false);
    ViewCenter.x = ScreenWidth / 2;
    ViewCenter.y = ScreenHeight / 2;
    View = Window->getView();
    View.setCenter(ViewCenter);
    View.setSize(Vector2f(ScreenWidth, ScreenHeight));
    Window->setView(View);
    Window->setVerticalSyncEnabled(true);

    FPSText.setCharacterSize(40 * ScreenScale);
    FPSText.setPosition(ScreenWidth - (FPSText.getGlobalBounds().width * 1.1), FPSText.getGlobalBounds().height);
    BulletsCountText.setCharacterSize(40 * ScreenScale);
    BulletsCountText.setPosition(100, BulletsCountText.getGlobalBounds().height);
    WallsCountText.setCharacterSize(40 * ScreenScale);
    WallsCountText.setPosition(100, WallsCountText.getGlobalBounds().height * 2.5);

    BulletManagerPtr->LaunchFirePerformingThread();
}
