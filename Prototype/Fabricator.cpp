#include "pch.h"
#include "Fabricator.h"
#include <iostream>

Fabricator::Fabricator(BuildingTypes buildingType, int posX, int posY, int width, int height)
	: Buildings(buildingType, posX, posY, width, height)
{
	m_Input[0].whichItem = m_RecipeTable[SelectedRecipe].inputOne.whichItem;
	m_Input[1].whichItem = m_RecipeTable[SelectedRecipe].inputTwo.whichItem;
}

void Fabricator::Update()
{
	if (m_Output.quantity < 10)
	{
		if (m_Input[0].whichItem == m_RecipeTable[SelectedRecipe].inputOne.whichItem
			&& m_Input[0].quantity >= m_RecipeTable[SelectedRecipe].inputOne.quantity
			&& m_Input[1].whichItem == m_RecipeTable[SelectedRecipe].inputTwo.whichItem
			&& m_Input[1].quantity >= m_RecipeTable[SelectedRecipe].inputTwo.quantity)
		{
			++m_TickTimer;

			if (m_TickTimer > m_RecipeTable[SelectedRecipe].ticksToCraft)
			{
				m_TickTimer -= m_RecipeTable[SelectedRecipe].ticksToCraft;

				m_Input[0].quantity -= m_RecipeTable[SelectedRecipe].inputOne.quantity;
				m_Input[1].quantity -= m_RecipeTable[SelectedRecipe].inputTwo.quantity;

				m_Output.whichItem = SelectedRecipe;
				m_Output.quantity += 1;

				//std::cout << "crafted " << m_Output.quantity << " of " << SelectedRecipe << "\n";

				//std::cout << "Selected recipe: " << SelectedRecipe <<
				//	"\nInput needed: " << m_RecipeTable[SelectedRecipe].inputOne.quantity << " of " << m_RecipeTable[SelectedRecipe].inputOne.whichItem << ", " <<
				//						  m_RecipeTable[SelectedRecipe].inputTwo.quantity << " of " << m_RecipeTable[SelectedRecipe].inputTwo.whichItem <<
				//	"\nNumber of ticks to craft: " << m_RecipeTable[SelectedRecipe].ticksToCraft << "\n\n";


				//std::cout << m_Input[0].quantity << " of " << m_Input[0].whichItem << "\n";
				//std::cout << m_Input[1].quantity << " of " << m_Input[1].whichItem << "\n";
				//std::cout << m_Output.quantity << " of " << m_Output.whichItem << "\n\n\n";
			}
			else
			{
				//std::cout << "crafting . . . \n";
			}
		}
		else
		{
			//std::cout << "inputs are misssing\n";
		}
	}
}

void Fabricator::Draw(int posX, int posY, int tileSize) const
{
	utils::SetColor(m_Color);
	utils::FillRect(posX * tileSize + 8, posY * tileSize + 8, m_Width * tileSize - 16, m_Height * tileSize - 16);
}

Buildings::Items Fabricator::TakeOutputItem()
{
	if (m_Output.quantity > 0)
	{
		m_Output.quantity -= 1;
		return m_Output.whichItem;
	}
	else if (m_Output.quantity == 0)
	{
		return Buildings::Items::empty;
	}
}

void Fabricator::ValidInputs(Buildings::Items& firstInput, Buildings::Items& secondInput)
{
	firstInput = m_RecipeTable[SelectedRecipe].inputOne.whichItem;
	secondInput = m_RecipeTable[SelectedRecipe].inputTwo.whichItem;
}

void Fabricator::InputItem(Buildings::Items whichItem)
{
	if (whichItem == m_RecipeTable[SelectedRecipe].inputOne.whichItem)
	{
		m_Input[0].quantity += 1;
	}
	else if (whichItem == m_RecipeTable[SelectedRecipe].inputTwo.whichItem)
	{
		m_Input[1].quantity += 1;
	}
}

void Fabricator::SetRecipe(Items whichItem)
{
	SelectedRecipe = whichItem;

	m_Input[0].quantity = 0;
	m_Input[1].quantity = 0;

	m_Input[0].whichItem = m_RecipeTable[SelectedRecipe].inputOne.whichItem;
	m_Input[1].whichItem = m_RecipeTable[SelectedRecipe].inputTwo.whichItem;
}

void Fabricator::ChangeShowInterface(bool show)
{
	m_ShowInterface = show;
}

void Fabricator::GetInputBufferNeeds(bool& needsFirstInput, bool& needsSecondInput)
{
	needsFirstInput = (m_Input[0].quantity < 5 * m_RecipeTable[SelectedRecipe].inputOne.quantity);
	needsSecondInput = (m_Input[1].quantity < 5 * m_RecipeTable[SelectedRecipe].inputTwo.quantity);
}

bool Fabricator::GetShowInterface() const
{
	return m_ShowInterface;
}