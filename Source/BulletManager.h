#pragma once

#include "Framework.h"
#include <mutex>

#define M_PI 3.14159265358979323846
class FrameworkClass;
using namespace sf;


struct BulletData
{
	std::unique_ptr<Texture> BulletTexture;
	std::unique_ptr<Sprite> BulletSprite;
	Vector2f Direction;
	float Speed;
	Int32 Time;
	Int32 LifeTime;

	BulletData(Vector2f InPosition, Vector2f InDirection, float InSpeed, Int32 InTime, Int32 InLifeTime)
	{
		BulletTexture = std::make_unique<Texture>();
		BulletSprite = std::make_unique<Sprite>();
		BulletTexture->loadFromFile("Images/bullet.png");
		Vector2u TextureSize;
		TextureSize = BulletTexture->getSize();
		BulletSprite->setTexture(*BulletTexture);
		BulletSprite->setOrigin(TextureSize.x / 2, TextureSize.y / 2);
		BulletSprite->setPosition(InPosition);
		Direction = InDirection;
		Speed = InSpeed;
		Time = InTime;
		LifeTime = InLifeTime;

		float ShipRotation = (atan2(InDirection.y, InDirection.x)) * 180 / M_PI;
		BulletSprite->setRotation(ShipRotation);
	}
};

class BulletManager
{

public:

	BulletManager(FrameworkClass* InFrameworkPtr);
	~BulletManager();

	void Update(float Time);
	void Fire(Vector2f Position, Vector2f Direction, float Speed, float Time, float LifeTime);

	int GetBulletsCount() { return BulletsRenderArray.size(); }

private:

	FrameworkClass* FrameworkPtr = nullptr;
	std::vector<std::shared_ptr<BulletData>> BulletsRenderArray;
	std::vector<std::shared_ptr<BulletData>> BulletsArrayQueue;
	Int32 LastShootTime = 0;
	Int32 ShootDiapason = 10;
};
