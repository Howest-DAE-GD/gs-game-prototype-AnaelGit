#include "pch.h"
#include "Miner.h"
#include <iostream>


Miner::Miner(BuildingTypes buildingType, int posX, int posY, int width, int height, Buildings::Items ressource)
	: Buildings(buildingType, posX, posY, width, height)
{
	m_Output.whichItem = ressource;
}

void Miner::Update()
{
	if (m_Output.whichItem != Buildings::Items::empty)						// if a miner doesn't have any ressources under it, it doesn't produce anything
	{
		++m_TickTimer;
		if (m_TickTimer > M_NBR_OF_TICKS_TO_MINE)
		{
			m_TickTimer -= M_NBR_OF_TICKS_TO_MINE;

			if (m_Output.quantity < M_MAX_NBR_OF_ITEMS)
			{
				++m_Output.quantity;
			}
		}
	}
}

void Miner::Draw(int posX, int posY, int tileSize) const
{
	utils::SetColor(m_Color);
	utils::FillRect( posX * tileSize + 8, posY * tileSize + 8, m_Width * tileSize - 16, m_Height * tileSize - 16);
}

Buildings::Items Miner::TakeOutputItem()
{
	if (m_Output.quantity > 0)
	{
		m_Output.quantity -= 1;
		return m_Output.whichItem;
	}
	else
	{
		return Buildings::Items::empty;
	}
}