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

	HashGridPanel = std::make_unique<HashGrid>(20, WallSize);
	SpawnWalls(1000, true);
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

void WallManager::ClearAllData()
{
	WallsRenderArray.clear();
	HashGridPanel->ClearData();
	WallsArray.reset();
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
	WallsCount = InCount;
	WallsArray = std::make_unique<VertexArray>(sf::Quads, 4 * WallsCount);

	int WallId = 0;
	for (float y = WallSize.y; y < FrameworkPtr->ScreenHeight - WallSize.y; y += 20)
	{
		for (float x = WallSize.x; x < FrameworkPtr->ScreenWidth - WallSize.x; x += 60)
		{
			if (WallId >= InCount)
			{
				return;
			}

			// Set positions for a rectangle
			(*WallsArray.get())[4 * WallId].position = sf::Vector2f(x, y);                               // Top-left
			(*WallsArray.get())[4 * WallId + 1].position = sf::Vector2f(x + WallSize.x, y);              // Top-right
			(*WallsArray.get())[4 * WallId + 2].position = sf::Vector2f(x + WallSize.x, y + WallSize.y); // Bottom-right
			(*WallsArray.get())[4 * WallId + 3].position = sf::Vector2f(x, y + WallSize.y);              // Bottom-left

			// Set colors
			Color color = Color::Red;
			(*WallsArray.get())[4 * WallId].color = color;
			(*WallsArray.get())[4 * WallId + 1].color = color;
			(*WallsArray.get())[4 * WallId + 2].color = color;
			(*WallsArray.get())[4 * WallId + 3].color = color;

			Vector2f Position = Vector2f(x, y);
			WallsRenderArray.push_back(std::make_unique<WallData>(Position, WallSize));
			WallsRenderArray[WallsRenderArray.size() - 1].get()->WallId = WallId;
			HashGridPanel->Insert(WallsRenderArray[WallsRenderArray.size() - 1].get());

			++WallId;
		}
	}
}

void WallManager::Update(float Time)
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
	Color color = Color(0, 0, 0, 0);
	(*WallsArray.get())[4 * InWallId].color = color;
	(*WallsArray.get())[4 * InWallId + 1].color = color;
	(*WallsArray.get())[4 * InWallId + 2].color = color;
	(*WallsArray.get())[4 * InWallId + 3].color = color;
	RemoveWallFromHashGrid(Vector2f((*WallsArray.get())[4 * InWallId].position.x + WallSize.x / 2.f, (*WallsArray.get())[4 * InWallId].position.y + WallSize.y / 2.f));
}

int WallManager::GetWallsCount() 
{ 
	return HashGridPanel.get()->GetGridObjectsCount();
}