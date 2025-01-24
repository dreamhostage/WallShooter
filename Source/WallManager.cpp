#include "WallManager.h"
#include <random>


WallManager::WallManager(FrameworkClass* InFrameworkPtr)
{
	FrameworkPtr = InFrameworkPtr;

	if (!FrameworkPtr || !FrameworkPtr->Window)
	{
		return;
	}

	HashGridPanel = HashGrid(/*FrameworkPtr->ScreenWidth * FrameworkPtr->ScreenHeight*/ 10);
	SpawnWalls(10000);
}

WallManager::~WallManager()
{

}

void WallManager::AddWall(Vector2f InPosition)
{
	Vector2f TextureSize = Vector2f(50.f, 5.f);
	WallsRenderArray.push_back(std::make_unique<RectangleShape>(TextureSize));
	WallsRenderArray[WallsRenderArray.size() - 1]->setFillColor(Color::Red);
	WallsRenderArray[WallsRenderArray.size() - 1]->setPosition(InPosition);
	HashGridPanel.Insert(WallsRenderArray[WallsRenderArray.size() - 1].get());
}

void WallManager::Update(float Time)
{
	if (!FrameworkPtr)
	{
		return;
	}

	for (int i = 0; i < WallsRenderArray.size(); ++i)
	{
		FrameworkPtr->Window->draw(*WallsRenderArray[i]);
	}
}

void WallManager::GetNearbyWalls(const Vector2f& position, float radius, std::vector<RectangleShape*>& result)
{
	HashGridPanel.query(position, radius, result);
}

void WallManager::RemoveWallFromHashGrid(const Vector2f& position)
{
	HashGridPanel.removeCell(position);
}

void WallManager::SpawnWalls(int Count)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	Vector2f RandomPosition;
	std::uniform_int_distribution<> PositionXDistr(0, FrameworkPtr->ScreenWidth);
	std::uniform_int_distribution<> PositionYDistr(0, FrameworkPtr->ScreenHeight);

	for (int i = 0; i < Count; ++i)
	{
		RandomPosition.x = PositionXDistr(gen);
		RandomPosition.y = PositionYDistr(gen);

		AddWall(RandomPosition);
	}
}