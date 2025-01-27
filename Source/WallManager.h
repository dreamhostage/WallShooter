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

	void AddWall(Vector2f InPosition);
	void Update(float Time);
    void SpawnWalls(int Count, bool bForceSpawn = false);
	int GetWallsCount() { return WallsRenderArray.size(); }
    bool CheckWallsCollision(float radius, RectangleShape* BulletRectangle);
    void RemoveWallFromHashGrid(const Vector2f& position);
    const Vector2f& GetWallSize() { return WallSize; }
    void ClearAllData();

    //---
    void SpawnWalls2(int Count, bool bForceSpawn = false);
    void Update2(float Time);
    //---

private:

	FrameworkClass* FrameworkPtr = nullptr;
    //---
    void SetWallInvisible(int InWallId);
    std::unique_ptr<VertexArray> WallsArray;
    int WallsCount = 1000;
    const float WallWidth = 50.0f;
    const float WallHeight = 5.0f;
    //---
	std::vector<std::unique_ptr<WallData>> WallsRenderArray;
    std::unique_ptr<HashGrid> HashGridPanel;
    Vector2f WallSize = Vector2f(50.f, 5.f);
    Int32 LastTestSpawnTime = 0;
    Int32 TestSpawnDiapason = 1000;
};
