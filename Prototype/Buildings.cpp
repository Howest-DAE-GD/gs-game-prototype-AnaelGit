#include "pch.h"
#include "Buildings.h"

Buildings::Buildings(BuildingTypes buildingType, int posX, int posY, int width, int height)
	: m_Building{ buildingType }, m_BottomLeftPosX{ posX }, m_BottomLeftPosY{ posY }, m_Width{ width }, m_Height{ height }
{
	//done
}

void Buildings::Update()
{
	// is overriden
}

void Buildings::Draw(int posX, int posY, int tileSize) const
{
	// is overriden
}

Point2f Buildings::GetBottomLeftPos() const
{
	return Point2f(m_BottomLeftPosX, m_BottomLeftPosY);
}

Buildings::BuildingTypes Buildings::GetBuidlingType() const
{
	return m_Building;
}

bool Buildings::GetHasAnOutput(BuildingTypes builldingType) const
{
	return m_HasAnOutputTable[builldingType].hasAnOutput;
}