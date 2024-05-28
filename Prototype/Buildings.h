#include "utils.h" 

#pragma once
class Buildings
{
public:

	//	special variable types
	enum BuildingTypes
	{
		miner = 0,
		conveyorBelt = 1,
		fabricator = 2,
	};

	enum Items
	{
						// see Recipe Table for actual recipe management
		CompactNephir = 0,		// 3 NephirOre						 to 1 CompactNephir
		Azulire = 1,			// 2 AzulOre and 1 NephirOre		 to 1 Azulir
		Composite = 2,			// 1 CompactNephir and 1 Azulir		 to 1 Composite

		empty = 3,
		NephirOre = 4,
		AzulOre = 5
	};

	struct HasAnOutput
	{
		HasAnOutput(BuildingTypes InBuildingType, bool InHasAOutput)
			: buildingType{ InBuildingType }, hasAnOutput{ InHasAOutput }
		{
			//done
		}

		BuildingTypes buildingType;
		bool hasAnOutput;
	};

	struct Storage
	{
		Storage(Items InWhichItem, int InQuantity)
			: whichItem{ InWhichItem }, quantity{InQuantity}
		{
			//	done
		}

		Items whichItem;
		int quantity;
	};

	struct Recipe
	{
		Recipe(Items IN_OutputedItem, Storage IN_InputOne, Storage IN_InputTwo, int IN_TicksToCraft)
			: outputedItem{ IN_OutputedItem }, inputOne{ IN_InputOne }, inputTwo{ IN_InputTwo }, ticksToCraft{ IN_TicksToCraft }
		{
			//	done
		}

		Items outputedItem;
		Storage inputOne;
		Storage inputTwo;
		int ticksToCraft;
	};


	//	constructor / destructor
	Buildings(BuildingTypes buildingTypeint, int posX, int posY, int width, int height);

	//	functions
	virtual void Update() = 0;
	virtual void Draw(int posX, int posY, int tileSize) const = 0;

	virtual Buildings::Items TakeOutputItem() = 0;

	Point2f GetBottomLeftPos() const;
	BuildingTypes GetBuidlingType() const;
	bool GetHasAnOutput(BuildingTypes builldingType) const;

protected:
	BuildingTypes m_Building				{};
	int m_BottomLeftPosX					{};
	int m_BottomLeftPosY					{};
	int m_Width								{};
	int m_Height							{};

	HasAnOutput m_HasAnOutputTable[3]			{	{miner, true},
												{conveyorBelt, true},
												{fabricator, true} };

	Recipe m_RecipeTable[3]						{ {CompactNephir , {empty , 0}, { NephirOre, 5}, 30 },
												  {Azulire , { AzulOre, 2}, { NephirOre, 1}, 30 },
												  {Composite , { CompactNephir, 1}, { Azulire, 1}, 300 } };
};

