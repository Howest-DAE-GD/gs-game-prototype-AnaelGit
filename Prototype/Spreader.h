#include "Buildings.h"


#pragma once
class Spreader : public Buildings
{
public:

	//	constructor / destructor
	Spreader(BuildingTypes buildingType, int posX, int posY, int width, int height);


	//	functions
	virtual void Update() override;
	virtual void Draw(int posX, int posY, int tileSize) const override;

	virtual Buildings::Items TakeOutputItem() override;

	void ValidInputs(Buildings::Items& inputItem);
	void InputItem(Buildings::Items whichItem);
	void GetInputBufferNeeds(bool& inputNeeds);

	bool CanSpread();

private:
	Color4f m_Color									{ Color4f(0.68f, 0.89f, 1.0f, 1.0f) };

	Storage m_Input									{ Buildings::Items::empty, 0 };
	int m_TickTimer									{};
	static const int M_NBR_OF_TICKS_TO_SPREAD		{ 60 };
	static const int M_MAX_NBR_OF_ITEMS				{ 10 };
	static const Buildings::Items m_ValidInput		{ Buildings::Items::Composite };
};


