#include "Tile.h"
#include <vector>
#include "utils.h"

#include "Texture.h"
#include "Transform.h"

#include "Miner.h"
#include "ConveyorBelt.h"
#include "Fabricator.h"
#include "Spreader.h"


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

	enum GameEnds
	{
		stillPlaying,
		won,
		losed
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
	void ChangeSelectedBuilding(Buildings::BuildingTypes buildingType);
	void ChangeSelectedDirection(int incrementToDirection);

	void LeftClick(int posX, int posY, bool& isBuiling);
	void PlaceBuilding(int posX, int posY);
	void DeleteBuilding(int posX, int posY);

	int GetGridWidth() const;
	int GetGridHeight() const;
	int GetM_TILE_SIZE() const;
	GameEnds GetGameEndState() const;

private:

	void CorruptionSpread();
	void Initialize();
	void Populate();
	void CorruptGrid();

	void BigBuildingInputs(	int lookupOffsetXStart, int lookupOffsetXEnd,
							int lookupOffsetYStart, int lookupOffsetYEnd,
							int Xindex, int Yindex, ConveyorBelt::Direction looksAwayOfTheFabricator,
							Buildings::Items firstInput, bool firstInputNeeds,
							Buildings::Items secondInput, bool secondInputNeeds);

	static const int  m_GridWidth								{ 100 };
	static const int m_GridHeight								{ 100 };
	const int M_TILE_SIZE										{ 64 };
	Tile* m_Grid[m_GridWidth][m_GridHeight];
	Buildings* m_GridBuildings[m_GridWidth][m_GridHeight]		{};

	float m_CorruptionSpeed										{1};
	const float m_CorruptionSpreadAcceleration					{ 1/36000.f};
	Buildings::BuildingTypes m_SelectedBuilding					{ Buildings::BuildingTypes::miner};
	ConveyorBelt::Direction m_SelectedDirection					{ConveyorBelt::Direction::up};

	const BuildingDimensions m_BuildingDimensionTable[4]		{	{Buildings::miner, 2, 2},
																	{Buildings::conveyorBelt, 1, 1},
																	{Buildings::fabricator, 3, 3},
																	{Buildings::spreader, 5, 5} };
	GameEnds m_GameEndState											{ GameEnds::stillPlaying };

	bool m_IsCurrentlyShowingAFactoryInterface					{ false };
	Point2f m_FactoryWithInterfacePos							{ m_GridWidth, m_GridHeight };

	std::string m_TextPath{ "Roboto-Regular.ttf" };
	const Texture* m_CompactNephirRecipeText = new Texture("CompactNephir { NephirOre, 5 } 0.5sec", m_TextPath, 20, Color4f{ 0.9f, 0.9f, 0.9f, 1.0f });
	Transform m_CompactNephirRecipeTextTrans;
	const Texture* m_AzulireRecipeText = new Texture("Azulire { AzulOre, 2 and NephirOre, 1} 0.5sec", m_TextPath, 20, Color4f{ 0.9f, 0.9f, 0.9f, 1.0f });
	Transform m_AzulireRecipeTextTrans;
	const Texture* m_CompositeRecipeText = new Texture("Composite { CompactNephir, 1 and Azulire, 1} 2sec", m_TextPath, 20, Color4f{ 0.9f, 0.9f, 0.9f, 1.0f });
	Transform m_CompositeRecipeTextTrans;
};
