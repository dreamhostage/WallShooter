#pragma once
#include "Framework.h"
#include "WallManager.h"
#include <unordered_map>
#include <memory>

using namespace sf;


// Representation of walls data inside the hash grid
struct WallData
{
    std::unique_ptr<RectangleShape> Rectangle;
    bool bDestroyed = false;
    int WallId = 0;

    WallData(const Vector2f& InPosition, const Vector2f& WallSize);
};

// 2D grid cell
struct Cell
{
    int x, y;

    bool operator==(const Cell& other) const
    {
        return x == other.x && y == other.y;
    }
};

// Hash function for Cell
struct CellHash
{
    std::size_t operator()(const Cell& cell) const
    {
        return std::hash<int>()(cell.x) ^ (std::hash<int>()(cell.y) << 1);
    }
};

// Hash grid class
class HashGrid
{

public:

    HashGrid(float InCellSize, const Vector2f& InWallSize) { CellSize = InCellSize; WallSize = InWallSize; }
    void Insert(WallData* WallDataPtr);
    void RemoveCell(const Vector2f& position);
    int CheckCollision(float radius, RectangleShape* BulletRectangle) const;
    void ClearData();
    int GetGridObjectsCount() { return Grid.size(); }

private:

    float CellSize;
    Vector2f WallSize;
    std::unordered_map<Cell, std::vector<WallData*>, CellHash> Grid;
    Cell GetCell(const Vector2f& position) const;
};