#pragma once
#include "BaseGame.h"
#include "Transform.h"
#include <vector>
#include "Grid.h"

const Point2f G_WINDOW_SIZE{ 1080.f, 800.f };

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseWheel( const SDL_MouseWheelEvent& e ) override;

private:

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	// my functions
	void OrePopulation();
	void KeyInputHandeling();


	// my variables

	Point2f m_CameraPos							{};
	int m_CameraSpeed							{5};
	Point2f m_MousePosInGrid					{};
	Point2f m_MousePos{};

	float m_ZoomLevel							{0.5};
	Transform m_GridTransformer					{};

	struct KeyMemo
	{
		bool up;
		bool down;
		bool left;
		bool right;
		bool leftClick;
		bool rightClick;
	} m_PrestedKeys								{};

	Grid* m_GridPtr								{ new Grid() };

	const int M_NBR_OF_UPDATES_A_SEC			{ 60 };
	float m_Timer								{};
	int m_CappedUpdate							{};
};