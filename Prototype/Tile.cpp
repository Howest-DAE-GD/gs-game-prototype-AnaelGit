#include "pch.h"
#include "Tile.h"
#include <iostream>


Tile::Tile()
{
	//done
}

Tile::Tile(EnvironmentalTileTypes environmentalTileType, Tile::CorruptionState corruptedState)
	: m_EnvironmentalTileType{ environmentalTileType }, m_CorruptionState{ corruptedState  }
{
	ChangeToCorrectColor();
}

Tile::~Tile()
{

}

void Tile::Draw(int posX, int posY, int tileSize) const
{
	utils::SetColor(m_Color);
	utils::FillRect(posX * tileSize, posY * tileSize, tileSize, tileSize);

	if (m_CorruptionState == Tile::CorruptionState::antiCorruption)
	{
		utils::SetColor(Color4f(0.68f, 0.89f, 1.0f, 0.2f));
		utils::FillRect(posX * tileSize, posY * tileSize, tileSize, tileSize);
	}
}

void Tile::ChangeTile(EnvironmentalTileTypes environmentalTileType)
{
	m_EnvironmentalTileType = environmentalTileType;

	ChangeToCorrectColor();
}

void Tile::ChangeTileCorruption(Tile::CorruptionState corruptionState)
{
	m_CorruptionState = corruptionState;

	ChangeToCorrectColor();
}

Tile::EnvironmentalTileTypes Tile::GetEnvironmentalTileType() const
{
	return m_EnvironmentalTileType;
}

Color4f Tile::GetColor() const
{
	return m_Color;
}

Tile::CorruptionState Tile::GetCorruptionState() const
{
	return m_CorruptionState;
}

void Tile::ChangeToCorrectColor()
{
	if (m_CorruptionState == CorruptionState::corrupted)
	{
		m_Color = { Color4f(0.25, 0.0f, 0.5f, 1.0f) };
	}
	else														
	{
		switch (m_EnvironmentalTileType)
		{
		case empty:
			m_Color = { Color4f(0.2f, 0.6f, 0.2f, 1.f) };
			break;

		case AzulOre:
			m_Color = { Color4f(0.3f, 0.7f, 0.6f, 1.0f) };
			break;

		case NephirOre:
			m_Color = { Color4f(0.7f, 0.7f, 0.3f, 1.0f) };
			break;

		}
	}
}
