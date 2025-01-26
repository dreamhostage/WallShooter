#include "WallManager.h"
#include <random>
#include <iostream>


WallManager::WallManager(FrameworkClass* InFrameworkPtr)
{
	FrameworkPtr = InFrameworkPtr;

	if (!FrameworkPtr || !FrameworkPtr->Window)
	{
		return;
	}

	HashGridPanel = HashGrid(5);
	SpawnWalls(1000, true);
}

WallManager::~WallManager()
{

}

void WallManager::AddWall(Vector2f InPosition)
{
	Vector2f TextureSize = Vector2f(50.f, 5.f);
	WallsRenderArray.push_back(std::make_unique<WallData>(InPosition, WallSize));
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
		if (WallsRenderArray[i]->bDestroyed)
		{
			HashGridPanel.removeCell(WallsRenderArray[i]->Rectangle->getPosition());
			std::vector<std::unique_ptr<WallData>>::iterator BulletsArrayIt = WallsRenderArray.begin();
			WallsRenderArray.erase(BulletsArrayIt + i);
			continue;
		}

		FrameworkPtr->Window->draw(*WallsRenderArray[i]->Rectangle.get());
	}
}

bool WallManager::CheckWallsCollision(const Vector2f& position, float radius, RectangleShape* BulletRectangle)
{
	return HashGridPanel.query(position, radius, BulletRectangle);
}

void WallManager::RemoveWallFromHashGrid(const Vector2f& position)
{
	HashGridPanel.removeCell(position);
}

void WallManager::SpawnWalls(int InCount, bool bForceSpawn)
{
	Int32 CurrentTime = FrameworkPtr->Clock.getElapsedTime().asMilliseconds();

	if (!bForceSpawn)
	{
		if (CurrentTime - LastTestSpawnTime > TestSpawnDiapason)
		{
			LastTestSpawnTime = CurrentTime;
		}
		else
		{
			return;
		}
	}

	ClearAllData();
	int Count = 0;
	for (int y = WallSize.y; y < FrameworkPtr->ScreenHeight; y += 20)
	{
		for (int x = WallSize.x; x < FrameworkPtr->ScreenWidth; x += 60)
		{
			if (Count >= InCount)
			{
				return;
			}

			Vector2f Position(x, y);
			AddWall(Position);
			++Count;
		}
	}
}

void WallManager::ClearAllData()
{
	WallsRenderArray.clear();
	HashGridPanel.ClearData();
}