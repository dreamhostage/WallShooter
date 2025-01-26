#pragma once

#include "Framework.h"
#include <unordered_map>
#include <vector>
#include <cmath>

class FrameworkClass;
using namespace sf;

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

struct WallData
{
    std::unique_ptr<RectangleShape> Rectangle;
    bool bDestroyed = false;

    WallData(const Vector2f& InPosition, const Vector2f& WallSize)
    {
        Rectangle = std::make_unique<RectangleShape>(WallSize);
        Rectangle->setPosition(InPosition);
        Rectangle->setFillColor(Color::Red);
        Rectangle->setOrigin(WallSize.x / 2, WallSize.y / 2);
    }
};

// Hash function for 2D grid cells
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
private:
    float cellSize;
    std::unordered_map<Cell, std::vector<WallData*>, CellHash> grid;

    // Convert position to grid cell
    Cell getCell(const Vector2f& position) const 
    {
        return Cell
        {
            static_cast<int>(std::floor(position.x / cellSize)),
            static_cast<int>(std::floor(position.y / cellSize))
        };
    }

public:
    HashGrid() {}
    HashGrid(float cellSize) : cellSize(cellSize) {}

    // Add an object to the grid
    void Insert(WallData* WallDataPtr)
    {
        Cell cell = getCell(WallDataPtr->Rectangle->getPosition());
        grid[cell].push_back(WallDataPtr);
    }

    void removeCell(const Vector2f& position)
    {
        Cell cell = getCell(position);
        grid.erase(cell); // Remove the cell and its contents
    }

    // Get all objects near a position within a given radius
    bool query(const Vector2f& position, float radius, RectangleShape* BulletRectangle) const
    {
        Cell centerCell = getCell(position);

        int range = static_cast<int>(std::ceil(radius / cellSize));
        bool bWallDestroyed = false;

        // Check all neighboring cells in the radius
        for (int dx = -range; dx <= range; ++dx) 
        {
            for (int dy = -range; dy <= range; ++dy) 
            {
                Cell neighborCell = { centerCell.x + dx, centerCell.y + dy };

                if (grid.find(neighborCell) != grid.end()) 
                {
                    for (const auto& obj : grid.at(neighborCell)) 
                    {
                        if (obj && obj->Rectangle && !obj->bDestroyed)
                        {
                            // Check if the object is within the radius
                            float distance = std::sqrt(std::pow(obj->Rectangle->getPosition().x - position.x, 2) + std::pow(obj->Rectangle->getPosition().y - position.y, 2));
                            if (distance <= radius)
                            {
                                if (obj->Rectangle->getGlobalBounds().intersects(BulletRectangle->getGlobalBounds()))
                                {
                                    obj->bDestroyed = true;
                                    bWallDestroyed = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        return bWallDestroyed;
    }

    void ClearData()
    {
        grid.clear();
    }
};
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

class WallManager
{

public:

	WallManager(FrameworkClass* InFrameworkPtr);
	~WallManager();

	void AddWall(Vector2f InPosition);
	void Update(float Time);
    void SpawnWalls(int Count, bool bForceSpawn = false);
	int GetWallsCount() { return WallsRenderArray.size(); }
    bool CheckWallsCollision(const Vector2f& position, float radius, RectangleShape* BulletRectangle);
    void RemoveWallFromHashGrid(const Vector2f& position);
    const Vector2f& GetWallSize() { return WallSize; }
    void ClearAllData();

private:

	FrameworkClass* FrameworkPtr = nullptr;
	std::vector<std::unique_ptr<WallData>> WallsRenderArray;
    HashGrid HashGridPanel;
    Vector2f WallSize = Vector2f(50.f, 5.f);
    Int32 LastTestSpawnTime = 0;
    Int32 TestSpawnDiapason = 1000;
};