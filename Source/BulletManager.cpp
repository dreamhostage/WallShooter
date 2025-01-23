#include "BulletManager.h"
#include <iostream>


BulletManager::BulletManager(FrameworkClass* InFrameworkPtr)
{
	FrameworkPtr = InFrameworkPtr;
}

BulletManager::~BulletManager()
{

}

void BulletManager::Update(float Time)
{
	for (int i = 0; i < BulletsArray.size(); ++i)
	{
		Vector2f BulletPosition = BulletsArray[i]->BulletSprite->getPosition();
		double CurrentAngle = BulletsArray[i]->BulletSprite->getRotation();

		BulletPosition.x += BulletsArray[i]->Speed * cos((CurrentAngle)*M_PI / 180);
		BulletPosition.y += BulletsArray[i]->Speed * sin((CurrentAngle)*M_PI / 180);

		// BulletPosition.x > FrameworkPtr->ScreenWidth || BulletPosition.y > FrameworkPtr->ScreenHeight || BulletPosition.x < 0 || BulletPosition.y < 0

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
			BulletsArray[i]->BulletSprite->setPosition(BulletPosition);
		}

		BulletsArray[i]->BulletSprite->setRotation(CurrentAngle);

		FrameworkPtr->Window->draw(*BulletsArray[i]->BulletSprite);
	}
}

void BulletManager::Fire(Vector2f Position, Vector2f Direction, float Speed, float Time, float LifeTime)
{
	BulletsArray.push_back(std::make_unique<BulletData>(Position, Direction, Speed, Time, LifeTime));
}
