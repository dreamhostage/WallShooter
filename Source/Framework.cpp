#include "Framework.h"

#include <vector>


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
    RenderSettings.antialiasingLevel = 8;
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
}

void FrameworkClass::Run()
{
    while (Window->isOpen())
    {
        Event event;

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

        // Render block
        Window->draw(ScreenSprite);
        Window->draw(CursorSprite);
        Window->draw(ShipSprite);
        Window->draw(FPSText);
        Window->display();
        Window->clear();

        // Calculating Ship rotation
        MousePosition = Window->mapPixelToCoords(Mouse::getPosition(*Window));
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
    }
}