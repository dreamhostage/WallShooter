#include "HashGridTool.h"


WallData::WallData(const Vector2f& InPosition, const Vector2f& WallSize)
{
    Rectangle = std::make_unique<RectangleShape>(WallSize);
    Rectangle->setPosition(InPosition);
    Rectangle->setFillColor(Color::Red);
    Rectangle->setOrigin(WallSize.x / 2, WallSize.y / 2);
}

// Convert BulletPosition to Grid cell
Cell HashGrid::GetCell(const Vector2f& BulletPosition) const
{
    return Cell
    {
        static_cast<int>(std::floor(BulletPosition.x / CellSize)),
        static_cast<int>(std::floor(BulletPosition.y / CellSize))
    };
}

// Add Cell to the hash Grid
void HashGrid::Insert(WallData* WallDataPtr)
{
    Cell Cell = GetCell(WallDataPtr->Rectangle->getPosition());
    Grid[Cell].push_back(WallDataPtr);
}

// Remove Cell from hash Grid
void HashGrid::RemoveCell(const Vector2f& BulletPosition)
{
    Cell Cell = GetCell(BulletPosition);
    Grid.erase(Cell); // Remove the cell and its contents
}

// Check nearby Cells for collisions with bullet
bool HashGrid::CheckCollision(float Radius, RectangleShape* BulletRectangle) const
{
    Vector2f BulletPosition = BulletRectangle->getPosition();
    Cell CenterCell = GetCell(BulletPosition);

    int Range = static_cast<int>(std::ceil(Radius / CellSize));

    // Check all neighboring cells in the Radius
    for (int dx = -Range; dx <= Range; ++dx)
    {
        for (int dy = -Range; dy <= Range; ++dy)
        {
            Cell NeighborCell = { CenterCell.x + dx, CenterCell.y + dy };

            if (Grid.find(NeighborCell) != Grid.end())
            {
                for (const auto& NearByWall : Grid.at(NeighborCell))
                {
                    if (NearByWall && NearByWall->Rectangle && !NearByWall->bDestroyed)
                    {
                        // Check if the wall is within the Radius
                        float Distance = std::sqrt(std::pow(NearByWall->Rectangle->getPosition().x - BulletPosition.x, 2) + std::pow(NearByWall->Rectangle->getPosition().y - BulletPosition.y, 2));
                        if (Distance <= Radius)
                        {
                            if (NearByWall->Rectangle->getGlobalBounds().intersects(BulletRectangle->getGlobalBounds()))
                            {
                                NearByWall->bDestroyed = true;
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

void HashGrid::ClearData()
{
    Grid.clear();
}
