#include "Light.h"

void Light::GUIExtras()
{
	ImGui::SeparatorText("Light Settings");

	ImGui::PushItemWidth(150.0f);
	ImGui::ColorPicker3("Colour", &colour.x, ImGuiColorEditFlags_DisplayRGB);
	ImGui::PopItemWidth();
	if (ImGui::DragFloat("Intensity", &intensity, 0.1f))
		if (intensity < 0.0f)
			intensity = 0.0f;
}

void Light::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram)
{

}

void Light::Update(float deltaTime)
{
	// Loop through all of This Object's Widgets.
	for (auto& curWidget : widgets)
	{
		// Update Position.
		curWidget->SetPosition(position);

		if (curWidget->GetWidgetType() == WidgetType::LIGHT)
		{
			// Cast the Widget to LightWidget.
			LightWidget* lightWidget{ static_cast<LightWidget*>(curWidget.get()) };

			// Update Widget Colour.
			lightWidget->SetColour(colour);
		}
	}
}

void Light::ShaderSetup(GLuint gShaderProgram)
{

}

Light::Light(std::string objectName = "Light", LightType lightType, glm::vec3 pos, glm::vec3 rot) : SceneObject(objectName, pos, rot)
{
	// Set Object Type.
	objectType = SceneObjectType::LIGHT;

	widgets.push_back(std::make_unique<LightWidget>(true, 0.25f));

	hasMesh = false;
	type = lightType;
}