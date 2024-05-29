#include "pch.h"
#include "Grid.h"
#include <iostream>

Grid::Grid()
{
	Initialize();
	Populate();
	CorruptGrid();
}

Grid::~Grid()
{
	for (int Yindex{}; Yindex < m_GridHeight; ++Yindex)
	{
		for (int Xindex{}; Xindex < m_GridWidth; ++Xindex)
		{
			delete m_Grid[Xindex][Yindex];
			m_Grid[Xindex][Yindex] = nullptr;

			DeleteBuilding(Xindex, Yindex);
		}
	}

	delete m_CompactNephirRecipeText;
	delete m_AzulireRecipeText;
	delete m_CompositeRecipeText;
}

void Grid::Update()
{
	for (int index{}; index < int(m_CorruptionSpeed); ++index)
	{
		CorruptionSpread();
	}
	m_CorruptionSpeed += m_CorruptionSpreadAcceleration;
	std::cout << m_CorruptionSpeed << "\n";

	int nbrOfCorruptedTile = 0 ;

	for (int Yindex{}; Yindex < m_GridHeight; ++Yindex)
	{
		for (int Xindex{}; Xindex < m_GridWidth; ++Xindex)
		{
			//counts the number of corrupted tiles for win/lose conditions
			if (m_Grid[Xindex][Yindex]->GetCorruptionState() == Tile::CorruptionState::corrupted)
			{
				nbrOfCorruptedTile += 1;
			}


			if (m_GridBuildings[Xindex][Yindex] != nullptr)
			{
				Point2f bottomLeftPos{ m_GridBuildings[Xindex][Yindex]->GetBottomLeftPos() };

				if (Xindex == int(bottomLeftPos.x)
					&& Yindex == int(bottomLeftPos.y))
				{
					m_GridBuildings[Xindex][Yindex]->Update();	// update


					// fabricator need to look around itself for inputs
					if (m_GridBuildings[Xindex][Yindex]->GetBuidlingType() == Buildings::fabricator)
					{
						bool firstInputNeeds{ false };
						bool secondInputNeeds{ false };
						dynamic_cast<Fabricator*>(m_GridBuildings[Xindex][Yindex])->GetInputBufferNeeds(firstInputNeeds, secondInputNeeds);	// get what the fabricator needs by reference

						if (firstInputNeeds
							|| secondInputNeeds)
						{
							Buildings::Items firstInput{ Buildings::Items::empty };
							Buildings::Items secondInput{ Buildings::Items::empty };
							dynamic_cast<Fabricator*>(m_GridBuildings[Xindex][Yindex])->ValidInputs(firstInput, secondInput);				// get what items the fabricator wants by reference

							BigBuildingInputs(-1, -1, 0, 2, Xindex, Yindex, ConveyorBelt::Direction::right, firstInput, firstInputNeeds, secondInput, secondInputNeeds);
							BigBuildingInputs(3, 3, 0, 2, Xindex, Yindex, ConveyorBelt::Direction::left, firstInput, firstInputNeeds, secondInput, secondInputNeeds);
							BigBuildingInputs(0, 2, -1, -1, Xindex, Yindex, ConveyorBelt::Direction::up, firstInput, firstInputNeeds, secondInput, secondInputNeeds);
							BigBuildingInputs(0, 2, 3, 3, Xindex, Yindex, ConveyorBelt::Direction::down, firstInput, firstInputNeeds, secondInput, secondInputNeeds);
						}
					}

					// spreader need to look around itself for inputs
					if (m_GridBuildings[Xindex][Yindex]->GetBuidlingType() == Buildings::spreader)
					{
						bool tempNeeds{ false };
						dynamic_cast<Spreader*>(m_GridBuildings[Xindex][Yindex])->GetInputBufferNeeds(tempNeeds);

						if (tempNeeds)
						{
							Buildings::Items tempValidInput{ Buildings::Items::empty };
							dynamic_cast<Spreader*>(m_GridBuildings[Xindex][Yindex])->ValidInputs(tempValidInput);

							BigBuildingInputs(-1, -1, 0, 4, Xindex, Yindex, ConveyorBelt::Direction::right, tempValidInput, tempNeeds, tempValidInput, tempNeeds);
							BigBuildingInputs(5, 5, 0, 4, Xindex, Yindex, ConveyorBelt::Direction::left, tempValidInput, tempNeeds, tempValidInput, tempNeeds);
							BigBuildingInputs(0, 4, -1, -1, Xindex, Yindex, ConveyorBelt::Direction::up, tempValidInput, tempNeeds, tempValidInput, tempNeeds);
							BigBuildingInputs(0, 4, 5, 5, Xindex, Yindex, ConveyorBelt::Direction::down, tempValidInput, tempNeeds, tempValidInput, tempNeeds);
						}
					}
				}
			}
		}
	}

	for (int Yindex{}; Yindex < m_GridHeight; ++Yindex)
	{
		for (int Xindex{}; Xindex < m_GridWidth; ++Xindex)
		{
			if (m_GridBuildings[Xindex][Yindex] != nullptr)																//is there a building
			{
				// conveyor belt are special and need more that one update cycle to avoid direction dependency


				if (m_GridBuildings[Xindex][Yindex]->GetBuidlingType() == Buildings::conveyorBelt)						// is it a belt
				{
					if (dynamic_cast<ConveyorBelt*>(m_GridBuildings[Xindex][Yindex])->hasEmptyInput())					// does the belt have a empty slot for a new item ?
					{
						ConveyorBelt::Direction direction = (dynamic_cast<ConveyorBelt*>(m_GridBuildings[Xindex][Yindex])->GetDirection());	// what direction is it looking

						int lookupOffsetX{ 0 };
						int lookupOffsetY{ 0 };

						switch (direction)
						{
						case ConveyorBelt::Direction::up:
							lookupOffsetY = -1;
							break;

						case ConveyorBelt::Direction::right:
							lookupOffsetX = -1;
							break;

						case ConveyorBelt::Direction::down:
							lookupOffsetY = 1;
							break;

						case ConveyorBelt::Direction::left:
							lookupOffsetX = 1;
							break;
						}


						if (Xindex + lookupOffsetX >= 0																	// is the the position behind the belt even in the grid ?
							&& Xindex + lookupOffsetX < m_GridWidth
							&& Yindex + lookupOffsetY >= 0
							&& Yindex + lookupOffsetY < m_GridHeight)
						{
							if (m_GridBuildings[Xindex + lookupOffsetX][Yindex + lookupOffsetY] != nullptr)
							{
								if (m_GridBuildings[Xindex + lookupOffsetX][Yindex + lookupOffsetY]->GetBuidlingType() != Buildings::BuildingTypes::conveyorBelt						// is the thing behind the belt something else than a belt or a belt that isn't pointing in the opposit direction
									|| (m_GridBuildings[Xindex + lookupOffsetX][Yindex + lookupOffsetY]->GetBuidlingType() == Buildings::BuildingTypes::conveyorBelt
										&& ((direction + 2) % 4) != (dynamic_cast<ConveyorBelt*>(m_GridBuildings[Xindex + lookupOffsetX][Yindex + lookupOffsetY])->GetDirection())))
								{
									if (m_GridBuildings[Xindex + lookupOffsetX][Yindex + lookupOffsetY]->GetHasAnOutput(m_GridBuildings[Xindex + lookupOffsetX][Yindex + lookupOffsetY]->GetBuidlingType()))
									{
										Buildings::Items item = m_GridBuildings[Xindex + lookupOffsetX][Yindex + lookupOffsetY]->TakeOutputItem();
										if (not (item == Buildings::Items::empty))
										{
											dynamic_cast<ConveyorBelt*>(m_GridBuildings[Xindex][Yindex])->InputItem(item);
										}
									}
								}
							}
						}
					}
				}

				// Spreader spreading their cure
				if (m_GridBuildings[Xindex][Yindex]->GetBuidlingType() == Buildings::spreader)						// is it a spreader
				{
					Point2f bottomLeftPos{ m_GridBuildings[Xindex][Yindex]->GetBottomLeftPos() };

					if (Xindex == int(bottomLeftPos.x)
						&& Yindex == int(bottomLeftPos.y))
					{
						if (dynamic_cast<Spreader*>(m_GridBuildings[Xindex][Yindex])->CanSpread())
						{
							for (int spreadXTimes{}; spreadXTimes < 5; spreadXTimes++)
							{

								int randNbr = rand() % 360;

								bool hasYetTospread = true;
								int spreadRange{};
								float angleX = cos(randNbr);
								float angleY = sin(randNbr);
								int nbrOfTries{};
								while (hasYetTospread && nbrOfTries < 10)
								{
									if (Xindex + 2 + angleX * spreadRange >= 0																	// is the the position behind the belt even in the grid ?
										&& Xindex + 2 + angleX * spreadRange < m_GridWidth
										&& Yindex + 2 + angleY * spreadRange >= 0
										&& Yindex + 2 + angleY * spreadRange < m_GridHeight)
									{
										if (m_Grid[int(Xindex + 2 + angleX * spreadRange)][int(Yindex + 2 + angleY * spreadRange)]->GetCorruptionState() != Tile::CorruptionState::antiCorruption)
										{
											m_Grid[int(Xindex + 2 + angleX * spreadRange)][int(Yindex + 2 + angleY * spreadRange)]->ChangeTileCorruption(Tile::CorruptionState::antiCorruption);
											hasYetTospread = false;
										}

										++spreadRange;
									}
									else
									{
										randNbr = rand() % 360;
										angleX = cos(randNbr);
										angleY = sin(randNbr);
										spreadRange = 0;

										nbrOfTries++;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	// see if the game ended 
	if (nbrOfCorruptedTile == m_GridWidth * m_GridHeight)
	{
		m_GameEndState = GameEnds::losed;
	}
	if (nbrOfCorruptedTile == 0)
	{
		m_GameEndState = GameEnds::won;
	}

	// change text position
	if (m_IsCurrentlyShowingAFactoryInterface)
	{
		m_CompactNephirRecipeTextTrans.Position = Vector2f((m_FactoryWithInterfacePos.x + 1) * M_TILE_SIZE + 14, (m_FactoryWithInterfacePos.y + 3) * M_TILE_SIZE + 14);
		m_AzulireRecipeTextTrans.Position = Vector2f((m_FactoryWithInterfacePos.x + 1) * M_TILE_SIZE + 14, (m_FactoryWithInterfacePos.y + 2) * M_TILE_SIZE + 14);
		m_CompositeRecipeTextTrans.Position = Vector2f((m_FactoryWithInterfacePos.x + 1) * M_TILE_SIZE + 14, (m_FactoryWithInterfacePos.y + 1) * M_TILE_SIZE + 14);
	}
}

void Grid::Draw() const
{
	for (int Yindex{}; Yindex < m_GridHeight; ++Yindex)
	{
		for (int Xindex{}; Xindex < m_GridWidth; ++Xindex)
		{
			m_Grid[Xindex][Yindex]->Draw(Xindex, Yindex, M_TILE_SIZE);

			if ((Yindex % 2 == 0 && Xindex % 2 == 0)														//checkers the grid with white and low oppacity
				|| (Yindex % 2 == 1 && Xindex % 2 == 1))
			{
				utils::SetColor(Color4f{ 1.0f, 1.0f, 1.0f, 0.03f });
				utils::FillRect(Xindex * M_TILE_SIZE, Yindex * M_TILE_SIZE, M_TILE_SIZE, M_TILE_SIZE);		//grid outline
			}
		}
	}

	for (int Yindex{}; Yindex < m_GridHeight; ++Yindex)
	{
		for (int Xindex{}; Xindex < m_GridWidth; ++Xindex)
		{
			if (m_GridBuildings[Xindex][Yindex] != nullptr)
			{
				Point2f bottomLeftPos{ m_GridBuildings[Xindex][Yindex]->GetBottomLeftPos() };

				if (Xindex == int(bottomLeftPos.x)
					&& Yindex == int(bottomLeftPos.y))
				{
					m_GridBuildings[Xindex][Yindex]->Draw(Xindex, Yindex, M_TILE_SIZE);
				}
			}
		}
	}

	if (m_IsCurrentlyShowingAFactoryInterface)
	{
		utils::SetColor(Color4f(0.2f, 0.2f, 0.2f, 0.75f));
		utils::FillRect((m_FactoryWithInterfacePos.x + 1) * M_TILE_SIZE + 8, (m_FactoryWithInterfacePos.y + 1) * M_TILE_SIZE + 8, 8 * M_TILE_SIZE - 16, 3 * M_TILE_SIZE - 16);
		for (int index{ 1 }; index <= 3; index++)
		{
			utils::FillRect((m_FactoryWithInterfacePos.x + 1) * M_TILE_SIZE + 14, (m_FactoryWithInterfacePos.y + index) * M_TILE_SIZE + 14, 8 * M_TILE_SIZE - 28, M_TILE_SIZE - 28);
		}

		m_CompactNephirRecipeTextTrans.ApplyTransformation();
		m_CompactNephirRecipeText->Draw();
		m_CompactNephirRecipeTextTrans.ResetTransformation();

		m_AzulireRecipeTextTrans.ApplyTransformation();
		m_AzulireRecipeText->Draw();
		m_AzulireRecipeTextTrans.ResetTransformation();

		m_CompositeRecipeTextTrans.ApplyTransformation();
		m_CompositeRecipeText->Draw();
		m_CompositeRecipeTextTrans.ResetTransformation();
	}
}

void Grid::ChangeTile(int posX, int posY, Tile::EnvironmentalTileTypes environmentalTileType)
{
	m_Grid[posX][posY]->ChangeTile(environmentalTileType);
}

void Grid::ChangeSelectedBuilding(Buildings::BuildingTypes buildingType)
{
	m_SelectedBuilding = buildingType;
	if (buildingType == Buildings::conveyorBelt)
	{
		m_SelectedDirection = ConveyorBelt::Direction::up;
	}
}

void Grid::ChangeSelectedDirection(int incrementToDirection)
{
	if (m_SelectedDirection + incrementToDirection >= 0)
	{
		m_SelectedDirection = ConveyorBelt::Direction((m_SelectedDirection + incrementToDirection) % 4);
	}
	else
	{
		m_SelectedDirection = ConveyorBelt::Direction::left;
	}
}

void Grid::LeftClick(int posX, int posY, bool& disableLeftClickTemporarily)
{
	disableLeftClickTemporarily = true;
	if (m_IsCurrentlyShowingAFactoryInterface)
	{
		for (int Yindex{-3}; Yindex < 0; Yindex++)
		{
			for (int Xindex{-8}; Xindex < 0; Xindex++)
			{
				if (posX + Xindex == int(m_FactoryWithInterfacePos.x)
					&& posY + Yindex == int(m_FactoryWithInterfacePos.y))
				{
					switch (Yindex)
					{
					case -3:
						dynamic_cast<Fabricator*>(m_GridBuildings[int(m_FactoryWithInterfacePos.x)][int(m_FactoryWithInterfacePos.y)])->SetRecipe(Buildings::Items::CompactNephir);
						break;

					case -2:
						dynamic_cast<Fabricator*>(m_GridBuildings[int(m_FactoryWithInterfacePos.x)][int(m_FactoryWithInterfacePos.y)])->SetRecipe(Buildings::Items::Azulire);
						break;

					case -1:
						dynamic_cast<Fabricator*>(m_GridBuildings[int(m_FactoryWithInterfacePos.x)][int(m_FactoryWithInterfacePos.y)])->SetRecipe(Buildings::Items::Composite);
						break;
					}
				}
			}
		}

		dynamic_cast<Fabricator*>(m_GridBuildings[int(m_FactoryWithInterfacePos.x)][int(m_FactoryWithInterfacePos.y)])->ChangeShowInterface(false);
		m_IsCurrentlyShowingAFactoryInterface = false;
		m_FactoryWithInterfacePos = Point2f(m_GridWidth, m_GridHeight);
	}
	else
	{
		if (m_GridBuildings[posX][posY] != nullptr)
		{
			if (m_GridBuildings[posX][posY]->GetBuidlingType() == Buildings::BuildingTypes::fabricator)
			{
				dynamic_cast<Fabricator*>(m_GridBuildings[posX][posY])->ChangeShowInterface(true);
				m_IsCurrentlyShowingAFactoryInterface = true;
				m_FactoryWithInterfacePos = Point2f(posX, posY);
			}
			else
			{
				disableLeftClickTemporarily = false;
			}
		}
		else
		{
			PlaceBuilding(posX, posY);
			disableLeftClickTemporarily = false;
		}
	}
}

void Grid::PlaceBuilding(int posX, int posY)
{
	int width{ m_BuildingDimensionTable[m_SelectedBuilding].width };
	int height{ m_BuildingDimensionTable[m_SelectedBuilding].height };
	bool isThereEnoughPlace{ true };

	if (posX + width <= m_GridWidth															// is it in the array? 
		&& posY + height <= m_GridHeight)
	{
		for (int Yindex{}; Yindex < height; ++Yindex)										// is there anything obstructing the building's placement
		{
			for (int Xindex{}; Xindex < width; ++Xindex)
			{
				if (!(m_GridBuildings[posX + Xindex][posY + Yindex] == nullptr
					&& m_Grid[posX + Xindex][posY + Yindex]->GetCorruptionState() != Tile::CorruptionState::corrupted))
				{
					isThereEnoughPlace = false;
				}
			}
		}
	}
	else
	{
		isThereEnoughPlace = false;
	}


	if (isThereEnoughPlace)
	{
		Buildings* ptr{ nullptr };

		Buildings::Items ressource = Buildings::Items::empty;									// C2360 "initialization of 'ressource' is skipped by 'case' label",	 if only use for mine
		int coveredRessources[]{ 0,0,0 };	//[0]empty,[1]NephirOre,[2]AzulOre					// same error


		switch (m_SelectedBuilding)
		{
		case Buildings::miner:

			for (int Yindex{}; Yindex < height; ++Yindex)											// what is the mine covering?
			{
				for (int Xindex{}; Xindex < width; ++Xindex)
				{
					coveredRessources[m_Grid[posX + Xindex][posY + Yindex]->GetEnvironmentalTileType()] += 1;
				}
			}

			if (coveredRessources[1] == 0															// what is the mine overing the most?
				&& coveredRessources[2] == 0)
			{
				ressource = Buildings::Items::empty;
			}
			else if (coveredRessources[1] > coveredRessources[2])
			{
				ressource = Buildings::Items::NephirOre;
			}
			else
			{
				ressource = Buildings::Items::AzulOre;
			}

			ptr = new Miner(Buildings::BuildingTypes::miner, posX, posY, m_BuildingDimensionTable[m_SelectedBuilding].width, m_BuildingDimensionTable[m_SelectedBuilding].height, ressource);
			break;

		case Buildings::conveyorBelt:
			ptr = new ConveyorBelt(Buildings::BuildingTypes::conveyorBelt, posX, posY, m_BuildingDimensionTable[m_SelectedBuilding].width, m_BuildingDimensionTable[m_SelectedBuilding].height, m_SelectedDirection);
			break;

		case Buildings::fabricator:
			ptr = new Fabricator(Buildings::BuildingTypes::fabricator, posX, posY, m_BuildingDimensionTable[m_SelectedBuilding].width, m_BuildingDimensionTable[m_SelectedBuilding].height);
			break;

		case Buildings::spreader:
			ptr = new Spreader(Buildings::BuildingTypes::spreader, posX, posY, m_BuildingDimensionTable[m_SelectedBuilding].width, m_BuildingDimensionTable[m_SelectedBuilding].height);
			break;
		}

		for (int Yindex{}; Yindex < height; ++Yindex)
		{
			for (int Xindex{}; Xindex < width; ++Xindex)
			{
				m_GridBuildings[posX + Xindex][posY + Yindex] = ptr;
			}
		}
	}
}

void Grid::DeleteBuilding(int posX, int posY)
{
	if (m_GridBuildings[posX][posY] != nullptr)
	{
		if (m_IsCurrentlyShowingAFactoryInterface)
		{
			dynamic_cast<Fabricator*>(m_GridBuildings[int(m_FactoryWithInterfacePos.x)][int(m_FactoryWithInterfacePos.y)])->ChangeShowInterface(false);
			m_IsCurrentlyShowingAFactoryInterface = false;
			m_FactoryWithInterfacePos = Point2f(m_GridWidth, m_GridHeight);
		}


		Point2f bottomLeftPos{ m_GridBuildings[posX][posY]->GetBottomLeftPos() };
		Buildings* ptr{ m_GridBuildings[posX][posY] };

		int width{ m_BuildingDimensionTable[m_GridBuildings[posX][posY]->GetBuidlingType()].width };
		int height{ m_BuildingDimensionTable[m_GridBuildings[posX][posY]->GetBuidlingType()].height };

		for (int Yindex{}; Yindex < height; ++Yindex)
		{
			for (int Xindex{}; Xindex < width; ++Xindex)
			{
				m_GridBuildings[int(bottomLeftPos.x) + Xindex][int(bottomLeftPos.y) + Yindex] = nullptr;
			}
		}

		delete ptr;
	}
}

int Grid::GetGridWidth() const
{
	return m_GridWidth;
}

int Grid::GetGridHeight() const
{
	return m_GridHeight;
}

int Grid::GetM_TILE_SIZE() const
{
	return M_TILE_SIZE;
}

Grid::GameEnds Grid::GetGameEndState() const
{
	return m_GameEndState;
}

void Grid::CorruptionSpread()
{
	int RandPosX{ rand() % m_GridWidth };
	int RandPosY{ rand() % m_GridHeight };

	if (m_Grid[RandPosX][RandPosY]->GetCorruptionState() == Tile::CorruptionState::corrupted)
	{
		int RandVariationX{ (rand() % 7) - 3 };
		int RandVariationY{ (rand() % 7) - 3 };

		if (RandPosX + RandVariationX >= 0												// is it in the array?
			&& RandPosX + RandVariationX < m_GridWidth
			&& RandPosY + RandVariationY >= 0
			&& RandPosY + RandVariationY < m_GridHeight)
		{
			if (m_Grid[RandPosX + RandVariationX][RandPosY + RandVariationY]->GetCorruptionState() == Tile::CorruptionState::antiCorruption)
			{
				if ((rand() % 10) == 0)
				{
					m_Grid[RandPosX + RandVariationX][RandPosY + RandVariationY]->ChangeTileCorruption(Tile::CorruptionState::corrupted); // corrupt a tile (can already be corrupted)
					DeleteBuilding(RandPosX + RandVariationX, RandPosY + RandVariationY);
				}
			}
			else
			{
				m_Grid[RandPosX + RandVariationX][RandPosY + RandVariationY]->ChangeTileCorruption(Tile::CorruptionState::corrupted); // corrupt a tile (can already be corrupted)
				DeleteBuilding(RandPosX + RandVariationX, RandPosY + RandVariationY);
			}

		}
	}
}

void Grid::Initialize()
{
	for (int Yindex{}; Yindex < m_GridHeight; ++Yindex)
	{
		for (int Xindex{}; Xindex < m_GridWidth; ++Xindex)
		{
			m_Grid[Xindex][Yindex] = new Tile();
		}
	}
}

void Grid::Populate()
{
	for (int Yindex{}; Yindex < m_GridHeight; ++Yindex)
	{
		for (int Xindex{}; Xindex < m_GridWidth; ++Xindex)
		{
			int randNbr{ rand() % 2000 };
			if (randNbr < 4)
			{
				int randX = 0;
				int randY = 0;


				for (int nbrOfMergedPatches{}; nbrOfMergedPatches < 2; nbrOfMergedPatches++)
				{
					if (nbrOfMergedPatches > 0)
					{
						randX = (rand() % 7) - 3;
						randY = (rand() % 7) - 3;
					}

					for (int YPopulate{ -2 }; YPopulate < 3; ++YPopulate)								// populate around the squar 
					{
						for (int XPopulate{ -2 }; XPopulate < 3; ++XPopulate)
						{
							if (Yindex + YPopulate + randX >= 0												// is it in the array?
								&& Yindex + YPopulate + randX < m_GridHeight
								&& Xindex + XPopulate + randY >= 0
								&& Xindex + XPopulate + randY < m_GridWidth)
							{
								if (!(YPopulate == -2 && XPopulate == -2							// populate around the squar (except the corners)
									|| YPopulate == -2 && XPopulate == 2
									|| YPopulate == 2 && XPopulate == 2
									|| YPopulate == 2 && XPopulate == -2))
								{
									if (randNbr < 2)
									{
										m_Grid[Xindex + XPopulate + randY][Yindex + YPopulate + randX]->ChangeTile(Tile::EnvironmentalTileTypes::AzulOre);
									}
									else
									{
										m_Grid[Xindex + XPopulate + randY][Yindex + YPopulate + randX]->ChangeTile(Tile::EnvironmentalTileTypes::NephirOre);

									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void Grid::CorruptGrid()
{
	for (int Yindex{}; Yindex < m_GridHeight; ++Yindex)
	{
		for (int Xindex{}; Xindex < m_GridWidth; ++Xindex)
		{
			if ((Yindex == 0
				|| Yindex == m_GridHeight - 1)
				|| (Xindex == 0
					|| Xindex == m_GridWidth - 1))
			{
				m_Grid[Xindex][Yindex]->ChangeTileCorruption(Tile::CorruptionState::corrupted);
			}
		}
	}

	for (int index{}; index < 20000; ++index)
	{
		CorruptionSpread();
	}
}

void Grid::BigBuildingInputs(int lookupOffsetXStart, int lookupOffsetXEnd, int lookupOffsetYStart, int lookupOffsetYEnd, int Xindex, int Yindex, ConveyorBelt::Direction looksAwayOfTheFabricator, Buildings::Items firstInput, bool firstInputNeeds, Buildings::Items secondInput, bool secondInputNeeds)
{
	for (int lookupOffsetY{ lookupOffsetYStart }; lookupOffsetY <= lookupOffsetYEnd; ++lookupOffsetY)
	{
		for (int lookupOffsetX{ lookupOffsetXStart }; lookupOffsetX <= lookupOffsetXEnd; ++lookupOffsetX)
		{
			if (Xindex + lookupOffsetX >= 0																				// is the the position behind the belt even in the grid ?
				&& Xindex + lookupOffsetX < m_GridWidth
				&& Yindex + lookupOffsetY >= 0
				&& Yindex + lookupOffsetY < m_GridHeight)
			{
				if (m_GridBuildings[Xindex + lookupOffsetX][Yindex + lookupOffsetY] != nullptr)
				{
					if (m_GridBuildings[Xindex + lookupOffsetX][Yindex + lookupOffsetY]->GetBuidlingType() == Buildings::conveyorBelt)
					{
						if (dynamic_cast<ConveyorBelt*>(m_GridBuildings[Xindex + lookupOffsetX][Yindex + lookupOffsetY])->GetDirection() == looksAwayOfTheFabricator)		//direction
						{
							if (dynamic_cast<ConveyorBelt*>(m_GridBuildings[Xindex + lookupOffsetX][Yindex + lookupOffsetY])->GetOutput() == firstInput
								&& firstInputNeeds)
							{
								Buildings::Items item = m_GridBuildings[Xindex + lookupOffsetX][Yindex + lookupOffsetY]->TakeOutputItem();
								if (not (item == Buildings::Items::empty))
								{
									Buildings::BuildingTypes tempBuildingType = m_GridBuildings[Xindex][Yindex]->GetBuidlingType();

									switch (tempBuildingType)
									{
									case Buildings::BuildingTypes::fabricator:
										dynamic_cast<Fabricator*>(m_GridBuildings[Xindex][Yindex])->InputItem(item);
										break;

									case Buildings::BuildingTypes::spreader:
										dynamic_cast<Spreader*>(m_GridBuildings[Xindex][Yindex])->InputItem(item);
										break;
									}
								}
							}


							if (dynamic_cast<ConveyorBelt*>(m_GridBuildings[Xindex + lookupOffsetX][Yindex + lookupOffsetY])->GetOutput() == secondInput
								&& secondInputNeeds)
							{
								Buildings::Items item = m_GridBuildings[Xindex + lookupOffsetX][Yindex + lookupOffsetY]->TakeOutputItem();
								if (not (item == Buildings::Items::empty))
								{
									Buildings::BuildingTypes tempBuildingType = m_GridBuildings[Xindex][Yindex]->GetBuidlingType();

									switch (tempBuildingType)
									{
									case Buildings::BuildingTypes::fabricator:
										dynamic_cast<Fabricator*>(m_GridBuildings[Xindex][Yindex])->InputItem(item);
										break;

									case Buildings::BuildingTypes::spreader:
										dynamic_cast<Spreader*>(m_GridBuildings[Xindex][Yindex])->InputItem(item);
										break;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}