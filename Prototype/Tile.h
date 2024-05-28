#include "utils.h"

#pragma once
class Tile
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
	Tile();
	Tile(EnvironmentalTileTypes environmentalTileType, bool isCorrupted);
	~Tile();


	//	functions every cycle
	void Draw(int posX, int posY, int tileSize) const;

	//	functions when called
	void ChangeTile(EnvironmentalTileTypes environmentalTileType);
	void ChangeTileCorruption(bool isCorrupted);

	EnvironmentalTileTypes GetEnvironmentalTileType() const;
	Color4f GetColor() const;
	bool GetIsCorrupted() const;
	

private:
	void ChangeToCorrectColor();

	EnvironmentalTileTypes m_EnvironmentalTileType		{ empty };
	bool m_IsCorrupted									{ false };
	Color4f m_Color										{ Color4f(0.2f, 0.6f, 0.2f, 1.f) };
};

