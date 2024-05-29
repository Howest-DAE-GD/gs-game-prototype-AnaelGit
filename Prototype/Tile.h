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

	enum CorruptionState
	{
		nothing,
		corrupted,
		antiCorruption
	};


	//	constructor / destructor
	Tile();
	Tile(EnvironmentalTileTypes environmentalTileType, Tile::CorruptionState corruptedState);
	~Tile();


	//	functions every cycle
	void Draw(int posX, int posY, int tileSize) const;

	//	functions when called
	void ChangeTile(EnvironmentalTileTypes environmentalTileType);
	void ChangeTileCorruption(Tile::CorruptionState corruptionState);

	EnvironmentalTileTypes GetEnvironmentalTileType() const;
	Color4f GetColor() const;
	CorruptionState GetCorruptionState() const;
	

private:
	void ChangeToCorrectColor();

	EnvironmentalTileTypes m_EnvironmentalTileType		{ empty };
	CorruptionState m_CorruptionState					{ CorruptionState::nothing };
	Color4f m_Color										{ Color4f(0.2f, 0.6f, 0.2f, 1.f) };
};

