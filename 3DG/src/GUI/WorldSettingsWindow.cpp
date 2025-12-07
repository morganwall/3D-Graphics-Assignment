#include "WorldSettingsWindow.h"

void WorldSettingsWindow::Draw(glm::vec4& clearColour, float& ambientIntensity)
{
	if (!active)
		return;

	ImGui::Begin("World Settings", nullptr, ImGuiWindowFlags_NoCollapse);

	ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate);
	ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);

	ImGui::Separator();

	ImGui::PushItemWidth(150.0f);
	ImGui::ColorPicker3("Clear Colour", &clearColour.x, ImGuiColorEditFlags_DisplayRGB);
	ImGui::PopItemWidth();

	ImGui::PushItemWidth(150.0f);
	if (ImGui::DragFloat("Ambient Light Intensity", &ambientIntensity, 0.05f))
		if (ambientIntensity < 0.0f)
			ambientIntensity = 0.0f;
	ImGui::PopItemWidth();

	ImGui::End();
}

WorldSettingsWindow::WorldSettingsWindow(bool isActive) : Window(isActive)
{

}

WorldSettingsWindow::~WorldSettingsWindow()
{

}