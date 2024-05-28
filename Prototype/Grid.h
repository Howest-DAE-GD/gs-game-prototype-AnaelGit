#include "Tile.h"
#include <vector>
#include "utils.h"

#include "Miner.h"
#include "ConveyorBelt.h"
#include "Fabricator.h"


#pragma once
class Grid
{
public:

	//	special variable types
	struct BuildingDimensions
	{
		BuildingDimensions(Buildings::BuildingTypes InBuildingType, int InWidth, int InHeight)
			: buildingType{ InBuildingType }, width{ InWidth }, height{ InHeight }
		{
			//done
		}

		Buildings::BuildingTypes buildingType;
		int width;
		int height;
	};


	//	constructor / destructor
	Grid();
	~Grid();

	//	functions every cycle
	void Update();

	void Draw() const;

	//	functions when called

	void ChangeTile(int posX, int posY, Tile::EnvironmentalTileTypes environmentalTileType);

	//void ChangeSelectedBuilding(Buildings::BuildingTypes newSelection);
	void ChangeSelectedBuilding(int incrementToBuilding);
	void ChangeSelectedDirection(int incrementToDirection);

	void PlaceBuilding(int posX, int posY);
	void DeleteBuilding(int posX, int posY);

	int GetGridWidth() const;
	int GetGridHeight() const;
	int GetM_TILE_SIZE() const;

private:

	void CorruptionSpread();
	void Initialize();
	void Populate();
	void CorruptGrid();

	void FabricatorInputs(	int lookupOffsetXStart, int lookupOffsetXEnd,
							int lookupOffsetYStart, int lookupOffsetYEnd,
							int Xindex, int Yindex, ConveyorBelt::Direction looksAwayOfTheFabricator,
							Buildings::Items firstInput, bool firstInputNeeds,
							Buildings::Items secondInput, bool secondInputNeeds);

	static const int  m_GridWidth								{ 100 };
	static const int m_GridHeight								{ 100 };
	const int M_TILE_SIZE										{ 64 };
	Tile* m_Grid[m_GridWidth][m_GridHeight];
	Buildings* m_GridBuildings[m_GridWidth][m_GridHeight]		{};

	int m_CorruptionSpeed										{5};
	Buildings::BuildingTypes m_SelectedBuilding					{ Buildings::BuildingTypes::conveyorBelt};
	ConveyorBelt::Direction m_SelectedDirection					{ConveyorBelt::Direction::up};

	const BuildingDimensions m_BuildingDimensionTable[3]		{	{Buildings::miner,2,2},
																	{Buildings::conveyorBelt,1,1},
																	{Buildings::fabricator,3,3} };
};

