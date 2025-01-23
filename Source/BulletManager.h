#pragma once

#include "Framework.h"

#define M_PI 3.14159265358979323846
class FrameworkClass;
using namespace sf;


struct BulletData
{
	std::unique_ptr<Texture> BulletTexture;
	std::unique_ptr<Sprite> BulletSprite;
	Vector2f Direction;
	float Speed;
	float Time;
	float LifeTime;

	BulletData(Vector2f InPosition, Vector2f InDirection, float InSpeed, float InTime, float InLifeTime)
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

	int GetBulletsCount() { return BulletsArray.size(); }

private:

	FrameworkClass* FrameworkPtr = nullptr;
	std::vector<std::unique_ptr<BulletData>> BulletsArray;
};
