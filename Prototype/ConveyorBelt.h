#include "Buildings.h"

#pragma once
class ConveyorBelt : public Buildings
{

public:

	//	special variables types
	enum Direction																		// changed the order for the clockwise / counter-clockwise rotation
	{
		up,
		right,
		down,
		left
	};


	//	constructor / destructor
	ConveyorBelt(BuildingTypes buildingType, int posX, int posY, int width, int height, Direction direction);

	//	functions
	virtual void Update() override;
	virtual void Draw(int posX, int posY, int tileSize) const override;

	virtual Buildings::Items TakeOutputItem() override;

	bool hasEmptyInput() const;
	void InputItem(Buildings::Items whichItem);
	void MoveInputToOutput();

	Direction GetDirection() const;
	Items GetOutput() const ;

private:
	Color4f m_ConveyorColor							{ Color4f(0.2f, 0.2f, 0.2f, 1.0f) };
	Color4f m_ArrowColor							{ Color4f(0.6f, 0.6f, 0.6f, 1.0f) };
	Direction m_Direction							{ up };

	Storage m_Input									{ Buildings::Items::empty, 0 };
	Storage m_Output								{ Buildings::Items::empty, 0 };
	//int m_TickTimer{};
	//static const int M_NBR_OF_TICKS_TO_Transfer		{ 30 };
};
