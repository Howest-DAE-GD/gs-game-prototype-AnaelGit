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

}

void Game::Cleanup( )
{
	delete m_GridPtr;
	m_GridPtr = nullptr;
}

void Game::Update( float elapsedSec )
{
	KeyInputHandeling();



	m_Timer += elapsedSec;

	if (m_Timer >= 1.f / M_NBR_OF_UPDATES_A_SEC)					// updates the grid at a steady 1.f / M_NBR_OF_UPDATES_A_SEC a second (60 by default)
	{
		m_Timer -= 1.f / M_NBR_OF_UPDATES_A_SEC;

		m_GridPtr->Update();
	}
}

void Game::Draw( ) const
{
	ClearBackground( );

	m_GridTransformer.ApplyTransformation();
	
	m_GridPtr->Draw();

	m_GridTransformer.ResetTransformation();
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
		m_GridPtr->ChangeSelectedDirection(1);
		break;
	case SDLK_a:
		m_GridPtr->ChangeSelectedBuilding(1);
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
	m_MousePosInGrid.x = int(((m_MousePos.x - (m_CameraPos.x * m_ZoomLevel + G_WINDOW_SIZE.x / 2.f)) / m_ZoomLevel) / m_GridPtr->GetM_TILE_SIZE() );
	m_MousePosInGrid.y = int(((m_MousePos.y - (m_CameraPos.y * m_ZoomLevel + G_WINDOW_SIZE.y / 2.f)) / m_ZoomLevel) / m_GridPtr->GetM_TILE_SIZE() );

	if (m_MousePosInGrid.x >= 0																		// has the mouse clicked inside the grid
		&& m_MousePosInGrid.x < m_GridPtr->GetGridWidth()
		&& m_MousePosInGrid.y >= 0
		&& m_MousePosInGrid.y < m_GridPtr->GetGridHeight() )
	{
		if (m_PrestedKeys.leftClick)
		{
			m_GridPtr->PlaceBuilding(int(m_MousePosInGrid.x), int(m_MousePosInGrid.y));
		}
		else if (m_PrestedKeys.rightClick)
		{
			m_GridPtr->DeleteBuilding(int(m_MousePosInGrid.x), int(m_MousePosInGrid.y));
		}
		//std::cout << m_MousePosInGrid.x << ", " << m_MousePosInGrid.y << std::endl;
	}
}