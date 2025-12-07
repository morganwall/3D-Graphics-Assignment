#pragma once
#include "Window.h"
#include "glm/glm.hpp"

class WorldSettingsWindow : public Window
{
public:
	void Draw(glm::vec4& clearColour, float& ambientIntensity);

	WorldSettingsWindow(bool isActive = true);
	~WorldSettingsWindow();
};