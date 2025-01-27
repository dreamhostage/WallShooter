#pragma once

#include "Framework.h"
#include <unordered_map>
#include <vector>
#include <cmath>
#include <memory>

class FrameworkClass;
class HashGrid;
struct WallData;
using namespace sf;

class WallManager
{

public:

	WallManager(FrameworkClass* InFrameworkPtr);

    void Update(float Time);
    void SpawnWalls(int Count, bool bForceSpawn = false);
    int GetWallsCount();
    bool CheckWallsCollision(float radius, RectangleShape* BulletRectangle);
    void RemoveWallFromHashGrid(const Vector2f& position);
    const Vector2f& GetWallSize() { return WallSize; }
    void ClearAllData();
    void SetWallInvisible(int InWallId);

private:

	FrameworkClass* FrameworkPtr = nullptr;
    std::unique_ptr<VertexArray> WallsArray;
    int WallsCount = 1000;
    const float WallHeight = 5.0f;
	std::vector<std::unique_ptr<WallData>> WallsRenderArray;
    std::unique_ptr<HashGrid> HashGridPanel;
    Vector2f WallSize = Vector2f(50.f, 5.f);
    Int32 LastTestSpawnTime = 0;
    Int32 TestSpawnDiapason = 1000;
};
