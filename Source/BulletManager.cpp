#include "BulletManager.h"
#include <iostream>
#include <random>


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
	if (BulletsArrayQueue.size() > 0 && SpawnMutex.try_lock())
	{
		for (int i = 0; i < BulletsArrayQueue.size(); ++i)
		{
			BulletsRenderArray.push_back(BulletsArrayQueue[i]);
		}

		BulletsArrayQueue.clear();
		SpawnMutex.unlock();
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

void BulletManager::SpawnBulletsCount(int Count)
{
	Int32 CurrentTime = FrameworkPtr->Clock.getElapsedTime().asMilliseconds();

	if (CurrentTime - LastTestSpawnTime > TestSpawnDiapason)
	{
		LastTestSpawnTime = CurrentTime;
	}
	else
	{
		return;
	}

	std::thread FireThread([&]()
		{
			if (SpawnMutex.try_lock())
			{
				std::random_device rd;
				std::mt19937 gen(rd());
				Vector2f RandomPosition;
				Vector2f RandomDirection;
				std::uniform_int_distribution<> PositionXDistr(0, FrameworkPtr->ScreenWidth);
				std::uniform_int_distribution<> PositionYDistr(0, FrameworkPtr->ScreenHeight);

				for (int i = 0; i < Count; ++i)
				{
					RandomPosition.x = PositionXDistr(gen);
					RandomPosition.y = PositionYDistr(gen);
					RandomDirection.x = PositionXDistr(gen);
					RandomDirection.y = PositionYDistr(gen);

					BulletsArrayQueue.push_back(std::make_shared<BulletData>(RandomPosition, RandomDirection, 15, CurrentTime, 10000));
				}

				SpawnMutex.unlock();
			}
		});
	FireThread.detach();
}