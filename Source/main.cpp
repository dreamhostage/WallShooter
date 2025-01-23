#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <math.h>
#include <stdio.h>
#include <memory>

#include <iostream>
#include <iterator>
#include <list>
#include <queue>
#include <vector>

#define M_PI 3.14159265358979323846
using namespace sf;

int main(int argc, char* argv[])
{
    // Render variables
    std::unique_ptr<RenderWindow> Window;
    View View;
    Vector2f ViewCenter;
    ContextSettings RenderSettings;
    RenderSettings.antialiasingLevel = 8;
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
    FPSText.setCharacterSize(40);
    Font.loadFromFile("Images/18949.ttf");
    FPSText.setFont(Font);
    FPSText.setPosition(ScreenWidth - 200, 50);

    // Sprites
    Sprite CursorSprite;
    Sprite ScreenSprite;
    Sprite ShipSprite;
    // Textures
    Texture CursorTexture;
    Texture ScreenTexture;
    Texture ShipTexture;

    // Textures & Sprites settings
    Vector2u TextureSize;
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
    Window = std::make_unique<RenderWindow>(VideoMode(ScreenWidth, ScreenHeight), "WallShooter", Style::Fullscreen, RenderSettings);
    ViewCenter.x = ScreenWidth / 2;
    ViewCenter.y = ScreenHeight / 2;
    View = Window->getView();
    View.setCenter(ViewCenter);
    Window->setFramerateLimit(90);
    Window->setView(View);
    Window->setMouseCursorVisible(false);

    Vector2f MousePosition;

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

            if (CurrentFPS >= 90)
            {
                FPSText.setFillColor(Color::Green);
            }
            else
            {
                FPSText.setFillColor(Color::Red);
            }
        }
    }

    return 0;
}
