#pragma once
#include "BaseGame.h"
#include "Transform.h"
#include "Texture.h"
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

	Point2f m_CameraPos							{-3200,-3200};
	int m_CameraSpeed							{5};
	Point2f m_MousePosInGrid					{};
	Point2f m_MousePos{};

	float m_ZoomLevel							{1.0};
	Transform m_GridTransformer					{};

	struct KeyMemo
	{
		bool up;
		bool down;
		bool left;
		bool right;

		bool z;
		bool q;
		bool s;
		bool d;

		bool leftClick;
		bool rightClick;
		bool shift;
	} m_PrestedKeys								{};

	Grid* m_GridPtr								{ new Grid() };

	const int M_NBR_OF_UPDATES_A_SEC			{ 60 };
	float m_Timer								{};
	int m_CappedUpdate							{};

	const std::string m_TextPath{ "Roboto-Regular.ttf" };
	const Texture* m_PausedText = new Texture("PAUSED", m_TextPath, 50, Color4f{ 0.1f, 0.1f, 0.1f, 1.0f });
	Transform m_PausedTextTrans {m_PausedTextTrans.Position = Vector2f( - m_PausedText->GetWidth() / 2 + G_WINDOW_SIZE.x / 2, G_WINDOW_SIZE.y * 1 / 3)};
	const Texture* m_WonText = new Texture("<YOU WIN>", m_TextPath, 50, Color4f{ 0.1f, 0.1f, 0.1f, 1.0f });
	Transform m_WonTextTrans{ m_WonTextTrans.Position = Vector2f(-m_PausedText->GetWidth() / 2 + G_WINDOW_SIZE.x / 2, G_WINDOW_SIZE.y * 2 / 3) };
	const Texture* m_LosedText = new Texture("<YOU LOSE>", m_TextPath, 50, Color4f{ 0.1f, 0.1f, 0.1f, 1.0f });
	Transform m_LosedTextTrans{ m_LosedTextTrans.Position = Vector2f(-m_PausedText->GetWidth() / 2 + G_WINDOW_SIZE.x / 2, G_WINDOW_SIZE.y * 2 / 3) };
	const Texture* m_RecipeTreeTEX = new Texture("CorrCureRecipeTree.png");
	Transform m_RecipeTreeTEXTrans{ m_RecipeTreeTEXTrans.Position = Vector2f( 0.8 * ( - m_RecipeTreeTEX->GetWidth() / 2) + G_WINDOW_SIZE.x / 2, 0.8  * (-m_RecipeTreeTEX->GetHeight() / 2) + G_WINDOW_SIZE.y / 2)};

	bool m_ShowRecipeTree						{ false };

	bool m_IsGamePaused							{ false };

	bool m_IsChoosingCorruptionSpeed			{ true };


	const Texture* m_MinerSelectedText = new Texture("miner", m_TextPath, 30, Color4f{ 0.9f, 0.9f, 0.9f, 1.0f });
	const Texture* m_ConveyorBeltSelectedText = new Texture("conveyor belt ", m_TextPath, 30, Color4f{ 0.9f, 0.9f, 0.9f, 1.0f });
	const Texture* m_FabricatorSelectedText = new Texture("fabricator", m_TextPath, 30, Color4f{ 0.9f, 0.9f, 0.9f, 1.0f });
	const Texture* m_SpreaderSelectedText = new Texture("spreader", m_TextPath, 30, Color4f{ 0.9f, 0.9f, 0.9f, 1.0f });

	const Texture* m_BeltUpText = new Texture("up", m_TextPath, 30, Color4f{ 0.9f, 0.9f, 0.9f, 1.0f });
	const Texture* m_BeltDownText = new Texture("down", m_TextPath, 30, Color4f{ 0.9f, 0.9f, 0.9f, 1.0f });
	const Texture* m_BeltLeftText = new Texture("left", m_TextPath, 30, Color4f{ 0.9f, 0.9f, 0.9f, 1.0f });
	const Texture* m_BeltRightText = new Texture("right", m_TextPath, 30, Color4f{ 0.9f, 0.9f, 0.9f, 1.0f });
};