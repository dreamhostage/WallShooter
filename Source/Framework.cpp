#include "Framework.h"

#include <vector>
#include <thread>


FrameworkClass::FrameworkClass()
{
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
    Window->setFramerateLimit(90);
    Window->setView(View);
    Window->setMouseCursorVisible(false);

    // FPS settings
    FPSText.setCharacterSize(40);
    Font.loadFromFile("Images/18949.ttf");
    FPSText.setFont(Font);
    FPSText.setPosition(ScreenWidth - 200, 50);

    // Bullet manager initialization
    BulletManagerPtr = std::make_unique<BulletManager>(this);

    // BulletsCount text settings
    BulletsCountText.setCharacterSize(40);
    BulletsCountText.setFont(Font);
    BulletsCountText.setPosition(200, 50);
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

        //-------
        if (Keyboard::isKeyPressed(Keyboard::Num1))
        {
            BulletManagerPtr->SpawnBulletsCount(10000);
        }
        //-------

        // Render block
        Window->draw(ScreenSprite);
        Window->draw(CursorSprite);
        Window->draw(ShipSprite);
        BulletManagerPtr->Update(Clock.getElapsedTime().asMilliseconds());
        Window->draw(FPSText);
        Window->draw(BulletsCountText);
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

        // Bullets size updating
        BulletsCountText.setString("Bullets: " + std::to_string(BulletManagerPtr->GetBulletsCount()));
    }
}
