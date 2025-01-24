#include "BulletManager.h"
#include <iostream>


BulletManager::BulletManager(FrameworkClass* InFrameworkPtr)
{
	FrameworkPtr = InFrameworkPtr;

	if (!FrameworkPtr || !FrameworkPtr->Window)
	{
		return;
	}

	// Creating thread for fire performing
	std::thread FireThread([&]()
		{
			while (FrameworkPtr->Window->isOpen())
			{
				if (Mouse::isButtonPressed(Mouse::Left))
				{
					Int32 CurrentTime = FrameworkPtr->Clock.getElapsedTime().asMilliseconds();

					if (CurrentTime - LastShootTime > ShootDiapason)
					{
						LastShootTime = CurrentTime;
						Fire(FrameworkPtr->ShipSprite.getPosition(), FrameworkPtr->MousePosition - FrameworkPtr->ShipSprite.getPosition(), 15, FrameworkPtr->Clock.getElapsedTime().asMilliseconds(), 3000);
					}
				}
			}
		});
	FireThread.detach();
}

BulletManager::~BulletManager()
{

}

void BulletManager::Update(float Time)
{
	if (!FrameworkPtr)
	{
		return;
	}

	Int32 CurrentTime = FrameworkPtr->Clock.getElapsedTime().asMilliseconds();

	// Replacing bullets from queue to BulletsRenderArray
	if (BulletsArrayQueue.size() > 0)
	{
		for (int i = 0; i < BulletsArrayQueue.size(); ++i)
		{
			BulletsRenderArray.push_back(BulletsArrayQueue[i]);
		}

		BulletsArrayQueue.clear();
	}

	// Performing BulletsRenderArray
	for (int i = 0; i < BulletsRenderArray.size(); ++i)
	{
		// Destroy bullet according to life time
		if (CurrentTime - BulletsRenderArray[i]->Time > BulletsRenderArray[i]->LifeTime)
		{
			std::vector<std::shared_ptr<BulletData>>::iterator BulletsArrayIt = BulletsRenderArray.begin();
			BulletsRenderArray.erase(BulletsArrayIt + i);
			continue;
		}

		// Move bullet according to rotation
		Vector2f BulletPosition = BulletsRenderArray[i]->BulletSprite->getPosition();
		double CurrentAngle = BulletsRenderArray[i]->BulletSprite->getRotation();
		BulletPosition.x += BulletsRenderArray[i]->Speed * cos((CurrentAngle)*M_PI / 180);
		BulletPosition.y += BulletsRenderArray[i]->Speed * sin((CurrentAngle)*M_PI / 180);

		// Reflect bullet from the map walls
		if (BulletPosition.x > FrameworkPtr->ScreenWidth || BulletPosition.x < 0)
		{
			CurrentAngle = 180 - CurrentAngle;
		}
		else if (BulletPosition.y > FrameworkPtr->ScreenHeight || BulletPosition.y < 0)
		{
			CurrentAngle *= -1;
		}
		else
		{
			BulletsRenderArray[i]->BulletSprite->setPosition(BulletPosition);
		}
		BulletsRenderArray[i]->BulletSprite->setRotation(CurrentAngle);

		// Draw bullet
		FrameworkPtr->Window->draw(*BulletsRenderArray[i]->BulletSprite);
	}
}

void BulletManager::Fire(Vector2f Position, Vector2f Direction, float Speed, float Time, float LifeTime)
{
	BulletsArrayQueue.push_back(std::make_shared<BulletData>(Position, Direction, Speed, Time, LifeTime));
}
