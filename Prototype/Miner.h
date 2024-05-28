#include "Buildings.h"

#pragma once
class Miner : public Buildings
{

public:

	//	special variable types
	enum EnvironmentalTileTypes
	{
		empty = 0,
		NephirOre = 1,
		AzulOre = 2,
	};


	//	constructor / destructor
	Miner(BuildingTypes buildingType, int posX, int posY, int width, int height, Buildings::Items ressource);

	//	functions
	virtual void Update() override;
	virtual void Draw(int posX, int posY, int tileSize) const override;

	virtual Buildings::Items TakeOutputItem() override;

private:
	Color4f m_Color								{ Color4f(0.9f, 0.2f, 0.2f, 1.0f) };

	Storage m_Output							{ Buildings::Items::empty, 0 };
	int m_TickTimer								{};
	static const int M_NBR_OF_TICKS_TO_MINE		{ 30 };
	static const int M_MAX_NBR_OF_ITEMS			{ 10 };
};

