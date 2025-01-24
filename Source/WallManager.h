#pragma once

#include "Framework.h"
#include <unordered_map>
#include <vector>
#include <cmath>

class FrameworkClass;
using namespace sf;

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------

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
    std::unordered_map<Cell, std::vector<RectangleShape*>, CellHash> grid;

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
    void Insert(RectangleShape* Rectangle)
    {
        Cell cell = getCell(Rectangle->getPosition());
        grid[cell].push_back(Rectangle);
    }

    void removeCell(const Vector2f& position)
    {
        Cell cell = getCell(position);
        grid.erase(cell); // Remove the cell and its contents
    }

    // Get all objects near a position within a given radius
    void query(const Vector2f& position, float radius, std::vector<RectangleShape*>& result) const
    {
        Cell centerCell = getCell(position);

        int range = static_cast<int>(std::ceil(radius / cellSize));

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
                        // Check if the object is within the radius
                        float distance = std::sqrt(std::pow(obj->getPosition().x - position.x, 2) + std::pow(obj->getPosition().y - position.y, 2));
                        if (distance <= radius) 
                        {
                            result.push_back(obj);
                        }
                    }
                }
            }
        }
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
    void SpawnWalls(int Count);
	int GetWallsCount() { return WallsRenderArray.size(); }
    void GetNearbyWalls(const Vector2f& position, float radius, std::vector<RectangleShape*>& result);
    void RemoveWallFromHashGrid(const Vector2f& position);

private:

	FrameworkClass* FrameworkPtr = nullptr;
	std::vector<std::unique_ptr<RectangleShape>> WallsRenderArray;
    HashGrid HashGridPanel;
};