#include "pch.h"
#include "Game.h"
#include "utils.h"
#include <iostream>

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	std::cout << "Hi and welcome to \"CorrCure\"!\n\n";
	std::cout << "First of all, the controls:\n";
	std::cout << " - p, to pause/unpause\n";
	std::cout << " - the arrowkeys to move around\n";
	std::cout << " - the scrollwheel to zoom in/out\n";
	std::cout << " - right click to destroy a building\n";
	std::cout << " - left click to build a building\n";
	std::cout << "   left click can also be used on a fabricator to change its recipe\n";
	std::cout << "   for example: CompactNephir { NephirOre, 5 } 0.5sec\n";
	std::cout << "   what does it make {inputs needed and in what quantity} and craft time\n";
	std::cout << " - 1 to 4, to select a building (1-miner / 2-conveyor belt / 3-Fabricator / 4-Spreader)\n";
	std::cout << " - r, to rotate the conveyor belt clockwise, must be done before placing it (hold shift at the same time for counter clockwise)\n";
	std::cout << " - h, to show/hide a image demonstrating the most basic setup\n\n";

	std::cout << "extra info: coneyor belt work by taking from what's behind them and buildings take from conveyor belt that point to them\n\n\n";

	std::cout << "What the color of the tile mean:\n";
	std::cout << " - Green just means that there is nothing noteworthy (think of it as grass)\n";
	std::cout << " - Dark purple is for corruption\n";
	std::cout << " - Light blue is azul ore\n";
	std::cout << " - Yellow is nephir ore\n";
	std::cout << " - A tile with a very light blue tint means that you successfully spread some composite\n";
	std::cout << "   (keep in mind that a tile with compite isn't immune to corruption, only resistant to it (10% corruption rate))\n";
	std::cout << " - the color of the buildings is just for the prototype\n\n";

	std::cout << "Now what is it about?\n";
	std::cout << "This world is being corrupted and you will have to make a composite and spread it to counter the corruption's own spread.\n";
	std::cout << "But you can't make composite from scratch, intermidiate items are needed.\n";
	std::cout << "Extract ores, build machines and stop the corruption fast because the longer you take the faster it spreads!\n";
	std::cout << "I wish you good luck.\n\n";

	m_RecipeTreeTEXTrans.Scale = Vector2f(0.5, 0.5);
}

void Game::Cleanup( )
{
	delete m_GridPtr;
	m_GridPtr = nullptr;

	delete m_PausedText;
	delete m_WonText;
	delete m_LosedText;
	delete m_RecipeTreeTEX;
}

void Game::Update( float elapsedSec )
{
	KeyInputHandeling();


	if (m_IsGamePaused == false
		&& m_GridPtr->GetGameEndState() == Grid::GameEnds::stillPlaying)
	{
		m_Timer += elapsedSec;

		if (m_Timer >= 1.f / M_NBR_OF_UPDATES_A_SEC)					// updates the grid at a steady 1.f / M_NBR_OF_UPDATES_A_SEC a second (60 by default)
		{
			m_Timer -= 1.f / M_NBR_OF_UPDATES_A_SEC;

			m_GridPtr->Update();
		}
	}
}

void Game::Draw( ) const
{
	ClearBackground( );

	m_GridTransformer.ApplyTransformation();
	
	m_GridPtr->Draw();

	m_GridTransformer.ResetTransformation();


	if (m_ShowRecipeTree)
	{
		utils::SetColor(Color4f(0.5f, 0.5f, 0.5f, 0.4f));
		utils::FillRect(Rectf(0, 0, G_WINDOW_SIZE.x, G_WINDOW_SIZE.y));

		m_RecipeTreeTEXTrans.ApplyTransformation();
		m_RecipeTreeTEX->Draw();
		m_RecipeTreeTEXTrans.ResetTransformation();
	}

	if (m_IsGamePaused)
	{
		utils::SetColor(Color4f(0.5f, 0.5f, 0.5f, 0.3f));
		utils::FillRect(Rectf(0, G_WINDOW_SIZE.y * 1 / 3, G_WINDOW_SIZE.x, m_PausedText->GetHeight()));

		m_PausedTextTrans.ApplyTransformation();
		m_PausedText->Draw();
		m_PausedTextTrans.ResetTransformation();
	}

	if (m_GridPtr->GetGameEndState() == Grid::GameEnds::won)
	{
		utils::SetColor(Color4f(0.3f, 0.9f, 0.3f, 0.3f));
		utils::FillRect(Rectf(0, G_WINDOW_SIZE.y * 2 / 3, G_WINDOW_SIZE.x, m_PausedText->GetHeight()));

		m_WonTextTrans.ApplyTransformation();
		m_WonText->Draw();
		m_WonTextTrans.ResetTransformation();
	}
	else if (m_GridPtr->GetGameEndState() == Grid::GameEnds::losed)
	{
		utils::SetColor(Color4f(0.9f, 0.3f, 0.3f, 0.3f));
		utils::FillRect(Rectf(0, G_WINDOW_SIZE.y * 2 / 3, G_WINDOW_SIZE.x, m_PausedText->GetHeight()));

		m_LosedTextTrans.ApplyTransformation();
		m_LosedText->Draw();
		m_LosedTextTrans.ResetTransformation();
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	switch(e.keysym.sym)
	{
	case SDLK_UP:
		m_PrestedKeys.up = true;
		break;

	case SDLK_DOWN:
		m_PrestedKeys.down = true;
		break;

	case SDLK_LEFT:
		m_PrestedKeys.left = true;
		break;

	case SDLK_RIGHT:
		m_PrestedKeys.right = true;
		break;
	case SDLK_r:
		if (m_PrestedKeys.shift)
		{
			m_GridPtr->ChangeSelectedDirection(-1);
		}
		else
		{
			m_GridPtr->ChangeSelectedDirection(1);
		}
		break;
	case SDLK_p:
		if (m_GridPtr->GetGameEndState() == Grid::GameEnds::stillPlaying)
		{
			m_IsGamePaused = (m_IsGamePaused + 1) % 2;
		}
		break;
	case SDLK_1:
		m_GridPtr->ChangeSelectedBuilding(Buildings::miner);
		break;
	case SDLK_2:
		m_GridPtr->ChangeSelectedBuilding(Buildings::conveyorBelt);
		break;
	case SDLK_3:
		m_GridPtr->ChangeSelectedBuilding(Buildings::fabricator);
		break;
	case SDLK_4:
		m_GridPtr->ChangeSelectedBuilding(Buildings::spreader);
		break;
	case SDLK_LSHIFT:
		m_PrestedKeys.shift = true;
		break;
	case SDLK_h:
		if (m_GridPtr->GetGameEndState() == Grid::GameEnds::stillPlaying)
		{
			m_ShowRecipeTree = (m_ShowRecipeTree + 1) % 2;
		}
		break;
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch (e.keysym.sym)
	{
	case SDLK_UP:
		m_PrestedKeys.up = false;
		break;

	case SDLK_DOWN:
		m_PrestedKeys.down = false;
		break;

	case SDLK_LEFT:
		m_PrestedKeys.left = false;
		break;

	case SDLK_RIGHT:
		m_PrestedKeys.right = false;
		break;
	case SDLK_LSHIFT:
		m_PrestedKeys.shift = false;
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	m_MousePos.x = e.x;
	m_MousePos.y = e.y;
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		m_PrestedKeys.leftClick = true;
		break;
	case SDL_BUTTON_RIGHT:
		m_PrestedKeys.rightClick = true;
		break;
	}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		m_PrestedKeys.leftClick = false;
		break;
	case SDL_BUTTON_RIGHT:
		m_PrestedKeys.rightClick = false;
		break;
	}
}

void Game::ProcessMouseWheel(const SDL_MouseWheelEvent& e)
{
	if (e.preciseY > 0
		&& m_ZoomLevel + 0.1 <= 1.5)
	{
		m_ZoomLevel += 0.1f;
	}
	else if (e.preciseY < 0
			&& m_ZoomLevel - 0.1 >= 0.09)													//0.09 is because floats... the max dezoom is 0.1
	{
		m_ZoomLevel -= 0.1f;
	}

	m_GridTransformer.Scale = Vector2f(m_ZoomLevel, m_ZoomLevel);
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}


// Game function
void Game::KeyInputHandeling()
{
	// movment keys
	if (m_PrestedKeys.up)																				//those aren't if else!
	{
		if (m_CameraPos.y > -m_GridPtr->GetGridHeight() * m_GridPtr->GetM_TILE_SIZE() )													// margin at the grid border
		{
			m_CameraPos.y -= m_CameraSpeed / m_ZoomLevel;
		}
	}

	if (m_PrestedKeys.down)
	{
		if (m_CameraPos.y < 0)
		{
			m_CameraPos.y += m_CameraSpeed / m_ZoomLevel;
		}
	}

	if (m_PrestedKeys.left)
	{
		if (m_CameraPos.x < 0)
		{
			m_CameraPos.x += m_CameraSpeed / m_ZoomLevel;
		}
	}

	if (m_PrestedKeys.right)
	{
		if (m_CameraPos.x > -m_GridPtr->GetGridWidth() * m_GridPtr->GetM_TILE_SIZE() )
		{
			m_CameraPos.x -= m_CameraSpeed / m_ZoomLevel;
		}
	}

	m_GridTransformer.Position = Vector2f(m_CameraPos.x * m_ZoomLevel + G_WINDOW_SIZE.x / 2.f, m_CameraPos.y * m_ZoomLevel + G_WINDOW_SIZE.y / 2.f);



	// mouse handeling
	// Building / Deconstructing
	if (m_IsGamePaused == false
		&& m_GridPtr->GetGameEndState() == Grid::GameEnds::stillPlaying)
	{
		m_MousePosInGrid.x = int(((m_MousePos.x - (m_CameraPos.x * m_ZoomLevel + G_WINDOW_SIZE.x / 2.f)) / m_ZoomLevel) / m_GridPtr->GetM_TILE_SIZE());
		m_MousePosInGrid.y = int(((m_MousePos.y - (m_CameraPos.y * m_ZoomLevel + G_WINDOW_SIZE.y / 2.f)) / m_ZoomLevel) / m_GridPtr->GetM_TILE_SIZE());

		if (m_MousePosInGrid.x >= 0																		// has the mouse clicked inside the grid
			&& m_MousePosInGrid.x < m_GridPtr->GetGridWidth()
			&& m_MousePosInGrid.y >= 0
			&& m_MousePosInGrid.y < m_GridPtr->GetGridHeight())
		{
			if (m_PrestedKeys.leftClick)
			{
				bool disableLeftClickTemporarily = true;
				m_GridPtr->LeftClick(int(m_MousePosInGrid.x), int(m_MousePosInGrid.y), disableLeftClickTemporarily);

				if (disableLeftClickTemporarily == true)
				{
					m_PrestedKeys.leftClick = false;
				}
			}
			else if (m_PrestedKeys.rightClick)
			{
				m_GridPtr->DeleteBuilding(int(m_MousePosInGrid.x), int(m_MousePosInGrid.y));
			}
			//std::cout << m_MousePosInGrid.x << ", " << m_MousePosInGrid.y << std::endl;
		}
	}
}