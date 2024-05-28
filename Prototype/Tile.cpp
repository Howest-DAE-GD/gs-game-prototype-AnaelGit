#include "pch.h"
#include "Tile.h"
#include <iostream>


Tile::Tile()
{
	//done
}

Tile::Tile(EnvironmentalTileTypes environmentalTileType, bool isCorrupted)
	: m_EnvironmentalTileType{ environmentalTileType }, m_IsCorrupted{isCorrupted}
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
}

void Tile::ChangeTile(EnvironmentalTileTypes environmentalTileType)
{
	m_EnvironmentalTileType = environmentalTileType;

	ChangeToCorrectColor();
}

void Tile::ChangeTileCorruption(bool isCorrupted)
{
	m_IsCorrupted = isCorrupted;

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

bool Tile::GetIsCorrupted() const
{
	return m_IsCorrupted;
}

void Tile::ChangeToCorrectColor()
{
	if (m_IsCorrupted)
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
			m_Color = { Color4f(0.7f, 0.7f, 0.3f, 1.0f) };
			break;

		case NephirOre:
			m_Color = { Color4f(0.3f, 0.7f, 0.6f, 1.0f) };
			break;

		}
	}
}
