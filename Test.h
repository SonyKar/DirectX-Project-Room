#pragma once

#include "Scene.h"

class Test : public Scene
{
	int direction = 1;
	int maximum_step_direction = 300;
	int currentStep = 0;
public:
	Test(CXCamera* camera);
	void setup_view_matrix() override;
};

