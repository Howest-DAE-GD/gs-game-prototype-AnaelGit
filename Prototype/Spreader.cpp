#include "pch.h"
#include "Spreader.h"
#include <iostream>

Spreader::Spreader(BuildingTypes buildingType, int posX, int posY, int width, int height)
	: Buildings(buildingType, posX, posY, width, height)
{
	//	done
}

void Spreader::Update()
{
	// doesn't have a update
}

void Spreader::Draw(int posX, int posY, int tileSize) const
{
	utils::SetColor(m_Color);
	utils::FillRect(posX * tileSize + 8, posY * tileSize + 8, m_Width * tileSize - 16, m_Height * tileSize - 16);
}

Buildings::Items Spreader::TakeOutputItem()
{
	return Buildings::Items::empty;		// the spreader doesn't have a output
}

void Spreader::ValidInputs(Buildings::Items& inputItem)
{
	if (m_Input.quantity == 0)
	{
		inputItem = m_ValidInput;
	}
	else
	{
		inputItem = m_Input.whichItem;
	}
}

void Spreader::InputItem(Buildings::Items whichItem)
{
	m_Input.whichItem = whichItem;
	m_Input.quantity += 1;
}

void Spreader::GetInputBufferNeeds(bool& inputNeeds)
{
	inputNeeds = (m_Input.quantity < M_MAX_NBR_OF_ITEMS);
}

bool Spreader::CanSpread()
{
	bool retval = false;

	if (m_Input.quantity > 0)
	{
		m_TickTimer++;
		if (m_TickTimer >= M_NBR_OF_TICKS_TO_SPREAD)
		{
			m_TickTimer -= M_NBR_OF_TICKS_TO_SPREAD;

			m_Input.quantity -= 1;
			if (m_Input.quantity == 0)
			{
				m_Input.whichItem = Buildings::Items::empty;
			}
			retval = true;
		}
	}
	return retval;
}