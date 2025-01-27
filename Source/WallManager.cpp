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

	HashGridPanel = std::make_unique<HashGrid>(20);
	// SpawnWalls(5, true);
	SpawnWalls2(1000, true);
}

void WallManager::AddWall(Vector2f InPosition)
{
	Vector2f TextureSize = Vector2f(50.f, 5.f);
	WallsRenderArray.push_back(std::make_unique<WallData>(InPosition, WallSize));
	HashGridPanel->Insert(WallsRenderArray[WallsRenderArray.size() - 1].get());
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
			HashGridPanel->RemoveCell(WallsRenderArray[i]->Rectangle->getPosition());
			std::vector<std::unique_ptr<WallData>>::iterator BulletsArrayIt = WallsRenderArray.begin();
			WallsRenderArray.erase(BulletsArrayIt + i);
			continue;
		}

		FrameworkPtr->Window->draw(*WallsRenderArray[i]->Rectangle.get());
	}
}

bool WallManager::CheckWallsCollision(float radius, RectangleShape* BulletRectangle)
{
	int WallId = HashGridPanel->CheckCollision(radius, BulletRectangle);

	if (WallId == -1)
	{
		return false;
	}
	else
	{
		SetWallInvisible(WallId);
		return true;
	}
}

void WallManager::RemoveWallFromHashGrid(const Vector2f& position)
{
	HashGridPanel->RemoveCell(position);
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
	HashGridPanel->ClearData();
}

//---
void WallManager::SpawnWalls2(int InCount, bool bForceSpawn)
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
	WallsCount = InCount;
	WallsArray.reset();
	WallsArray = std::make_unique<VertexArray>(sf::Quads, 4 * WallsCount);

	//---
	int WallId = 0;
	for (int y = WallSize.y / 2; y < FrameworkPtr->ScreenHeight - WallSize.y; y += 20)
	{
		for (int x = WallSize.x / 2; x < FrameworkPtr->ScreenWidth - WallSize.x; x += 60)
		{
			if (WallId >= InCount)
			{
				return;
			}

			// Set positions for a rectangle
			(*WallsArray.get())[4 * WallId].position = sf::Vector2f(x, y);                        // Top-left
			(*WallsArray.get())[4 * WallId + 1].position = sf::Vector2f(x + WallWidth, y);        // Top-right
			(*WallsArray.get())[4 * WallId + 2].position = sf::Vector2f(x + WallWidth, y + WallHeight); // Bottom-right
			(*WallsArray.get())[4 * WallId + 3].position = sf::Vector2f(x, y + WallHeight);       // Bottom-left

			// Set colors
			Color color = Color::Red;
			(*WallsArray.get())[4 * WallId].color = color;
			(*WallsArray.get())[4 * WallId + 1].color = color;
			(*WallsArray.get())[4 * WallId + 2].color = color;
			(*WallsArray.get())[4 * WallId + 3].color = color;
			++WallId;

			Vector2f Position = Vector2f(x + 25, y + 3);
			WallsRenderArray.push_back(std::make_unique<WallData>(Position, WallSize));
			WallsRenderArray[WallsRenderArray.size() - 1].get()->WallId = WallId;
			HashGridPanel->Insert(WallsRenderArray[WallsRenderArray.size() - 1].get());
		}
	}
}

void WallManager::Update2(float Time)
{
	if (!FrameworkPtr)
	{
		return;
	}

	if (WallsArray.get())
	{
		FrameworkPtr->Window->draw(*WallsArray.get());
	}
}

void WallManager::SetWallInvisible(int InWallId)
{
	InWallId -= 1;
	Color color = Color::Green;
	color.a = 0;
	(*WallsArray.get())[4 * InWallId].color = color;
	(*WallsArray.get())[4 * InWallId + 1].color = color;
	(*WallsArray.get())[4 * InWallId + 2].color = color;
	(*WallsArray.get())[4 * InWallId + 3].color = color;
	RemoveWallFromHashGrid(Vector2f((*WallsArray.get())[4 * InWallId].position.x + 25, (*WallsArray.get())[4 * InWallId].position.y + 3));
}

//---