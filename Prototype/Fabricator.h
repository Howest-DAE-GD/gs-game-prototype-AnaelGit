#include "Buildings.h"


#pragma once
class Fabricator : public Buildings
{

public:

	//	constructor / destructor
	Fabricator(BuildingTypes buildingType, int posX, int posY, int width, int height);

	//	functions
	virtual void Update() override;
	virtual void Draw(int posX, int posY, int tileSize) const override;

	virtual Buildings::Items TakeOutputItem() override;

	void ValidInputs(Buildings::Items& firstInput, Buildings::Items& secondInput);
	void InputItem(Buildings::Items whichItem);

	void SetRecipe(Items whichItem);
	void ChangeShowInterface(bool show);

	void GetInputBufferNeeds(bool& needsFirstInput,bool& needsSecondInput);
	bool GetShowInterface() const;
	Buildings::Items GetSelectedRecipe() const;

private:
	Color4f m_Color{ Color4f(0.2f, 0.2f, 0.9f, 1.0f) };

	Storage m_Input[2]					{ { Buildings::Items::empty, 0 }, { Buildings::Items::empty, 0 } };
	Storage m_Output					{ Buildings::Items::empty, 0 };

	Buildings::Items m_SelectedRecipe		{Items::empty};
	bool m_ShowInterface		{false};

	int m_TickTimer						{};
};
