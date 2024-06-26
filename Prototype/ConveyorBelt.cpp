#include "pch.h"
#include "ConveyorBelt.h"
#include <iostream>

ConveyorBelt::ConveyorBelt(BuildingTypes buildingType, int posX, int posY, int width, int height, Direction direction)
	: Buildings(buildingType, posX, posY, width, height), m_Direction{direction}
{
	//	done
}

void ConveyorBelt::Update()
{
	MoveInputToOutput();
}

void ConveyorBelt::Draw(int posX, int posY, int tileSize) const
{
	switch (m_Direction)
	{
	case up:
		utils::SetColor(m_ConveyorColor);
		utils::FillRect(posX * tileSize + 8, posY * tileSize - 8, m_Width * tileSize - 16, m_Height * tileSize);

		utils::SetColor(m_ArrowColor);

		utils::DrawLine(posX * tileSize + 16, posY * tileSize + 22, posX * tileSize + 32, posY * tileSize + 50, 5);
		utils::DrawLine(posX * tileSize + 32, posY * tileSize + 50, posX * tileSize + 48, posY * tileSize + 22, 5);
		break;

	case down:
		utils::SetColor(m_ConveyorColor);
		utils::FillRect(posX * tileSize + 8, posY * tileSize + 8, m_Width * tileSize - 16, m_Height * tileSize);

		utils::SetColor(m_ArrowColor);

		utils::DrawLine(posX * tileSize + 16, posY * tileSize + 46, posX * tileSize + 32, posY * tileSize + 18, 5);
		utils::DrawLine(posX * tileSize + 32, posY * tileSize + 18, posX * tileSize + 48, posY * tileSize + 46, 5);
		break;

	case left:
		utils::SetColor(m_ConveyorColor);
		utils::FillRect(posX * tileSize + 8, posY * tileSize + 8, m_Width * tileSize, m_Height * tileSize - 16);

		utils::SetColor(m_ArrowColor);

		utils::DrawLine(posX * tileSize + 18, posY * tileSize + 32, posX * tileSize + 46, posY * tileSize + 48, 5);
		utils::DrawLine(posX * tileSize + 46, posY * tileSize + 16, posX * tileSize + 18, posY * tileSize + 32, 5);
		break;

	case right:
		utils::SetColor(m_ConveyorColor);
		utils::FillRect(posX * tileSize - 8, posY * tileSize + 8, m_Width * tileSize, m_Height * tileSize - 16);

		utils::SetColor(m_ArrowColor);

		utils::DrawLine(posX * tileSize + 22, posY * tileSize + 16, posX * tileSize + 50, posY * tileSize + 32, 5);
		utils::DrawLine(posX * tileSize + 50, posY * tileSize + 32, posX * tileSize + 22, posY * tileSize + 48, 5);
		break;
	}

	
	switch (m_Input.whichItem)
	{
	case Buildings::Items::empty:
		break;

	case Buildings::Items::NephirOre:
		utils::SetColor(Color4f(0.7f, 0.7f, 0.3f, 1.0f));
		utils::FillEllipse(posX * tileSize + 32, posY * tileSize + 32, 10, 10);
		break;

	case Buildings::Items::AzulOre:
		utils::SetColor(Color4f(0.3f, 0.7f, 0.6f, 1.0f));
		utils::FillEllipse(posX * tileSize + 32, posY * tileSize + 32, 10, 10);
		break;

	case Buildings::Items::Azulire:
		utils::SetColor(Color4f(0.4f, 0.4f, 0.8f, 1.0f));
		utils::FillEllipse(posX * tileSize + 32, posY * tileSize + 32, 10, 10);
		break;

	case Buildings::Items::CompactNephir:
		utils::SetColor(Color4f(0.8f, 0.9f, 0.4f, 1.0f));
		utils::FillEllipse(posX * tileSize + 32, posY * tileSize + 32, 10, 10);
		break;

	case Buildings::Items::Composite:
		utils::SetColor(Color4f(0.2f, 0.3f, 0.6f, 1.0f));
		utils::FillEllipse(posX * tileSize + 32, posY * tileSize + 32, 10, 10);
		break;
	}
}

Buildings::Items ConveyorBelt::TakeOutputItem()
{
	Buildings::Items temp = m_Output.whichItem;
	m_Output.whichItem = Buildings::Items::empty;
	return temp;
}

bool ConveyorBelt::hasEmptyInput() const
{
	return (m_Input.whichItem == Buildings::Items::empty);
}

void ConveyorBelt::InputItem(Buildings::Items whichItem)
{
	m_Input = Storage(whichItem, 1);
}

void ConveyorBelt::MoveInputToOutput()
{
	if (m_Output.whichItem == Buildings::Items::empty)
	{
		m_Output = m_Input;
		m_Input = Storage(Buildings::Items::empty, 0);
	}
}

ConveyorBelt::Direction ConveyorBelt::GetDirection() const
{
	return m_Direction;
}

Buildings::Items ConveyorBelt::GetOutput() const
{
	return m_Output.whichItem;
}