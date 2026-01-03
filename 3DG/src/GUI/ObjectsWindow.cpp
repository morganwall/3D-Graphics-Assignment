#include "ObjectsWindow.h"
#include "../Objects/SceneObject.h"
#include "../Objects/Cube.h"
#include "../Objects/Terrain.h"
#include "../Objects/Skybox.h"
#include "../Objects/Lights/Light.h"
#include "../Objects/Lights/DirectionalLight.h"
#include "../Objects/Lights/PointLight.h"
#include "../Objects/Lights/SpotLight.h"

extern Light* light;

void ObjectsWindow::DeleteObject(SceneObject* target)
{
	// Stop Camera Deletion.
	if (!target || target->GetObjectType() == SceneObjectType::CAMERA)
		return;

	// Check if we're Deleting the Light.
	if (target == light)
		light = nullptr;

	// Reset Selection if this Object is Currently Selected.
	if (selectedObject == target)
		selectedObject = nullptr;

	// Find the object in the Scene Objects Vector.
	auto inVec{ std::find(sceneObjects->begin(), sceneObjects->end(), target) };
	if (inVec != sceneObjects->end()) // Check if we found the selected object in the scene objects vector.
	{
		// Cleanup Object.
		target->Cleanup();

		// Remove Object from Scene Objects Vector.
		sceneObjects->erase(inVec);

		// Delete Object.
		delete target;
	}
}

void ObjectsWindow::Draw()
{
	if (!active)
		return;

	ImGui::Begin("Objects", nullptr, ImGuiWindowFlags_NoCollapse);

	ImGui::Text("Scene Objects");
	ImVec2 newObjectButtonSize{ 50.0f, 20.0f };
	ImGui::SameLine(ImGui::GetWindowWidth() - newObjectButtonSize.x - 20.0f);
	if (ImGui::Button("+", newObjectButtonSize))
		ImGui::OpenPopup("NewObjectPopup");

	// Popup for Creating a New Object.
	if (ImGui::BeginPopup("NewObjectPopup"))
	{
		ImGui::Text("New Scene Object");

		ImGui::Separator();

		if (ImGui::MenuItem("Cube"))
			sceneObjects->push_back(new Cube());
		if (ImGui::MenuItem("Terrain"))
			sceneObjects->push_back(new Terrain());
		// Check that We Don't Alreadty Have a Light in the Scene. Only One Light is Allowed.
		if (!light)
		{
			if (ImGui::MenuItem("Directional Light"))
			{
				light = new DirectionalLight();
				sceneObjects->push_back(light);
			}

			if (ImGui::MenuItem("Point Light"))
			{
				light = new PointLight();
				sceneObjects->push_back(light);
			}

			if (ImGui::MenuItem("Spot Light"))
			{
				light = new SpotLight();
				light->SetRotation({ -90.0f, 0.0f, 0.0f }); // Points the spot light down by default.
				sceneObjects->push_back(light);
			}
		}
		else
		{
			ImGui::TextDisabled("Directional Light (Max Lights Reached)");
			ImGui::TextDisabled("Point Light (Max Lights Reached)");
			ImGui::TextDisabled("Spot Light (Max Lights Reached)");
		}

		if (ImGui::MenuItem("Skybox"))
			sceneObjects->push_back(new Skybox());

		ImGui::EndPopup();
	}

	ImGui::Separator();

	ImGui::BeginChild("Objects", { 0, 0 }, true);

	unsigned int id{ 0 }; // ID for ImGui elements.

	SceneObject* toDelete{ nullptr }; // Used to store a Scene Object that should be deleted.

	// Loop through Scene Objects.
	for (auto& curObject : *sceneObjects)
	{
		bool isSelected{ false };
		if (curObject == selectedObject)
			isSelected = true;
		else
			isSelected = false;

		// Push Object Pointer as ImGui ID.
		ImGui::PushID(curObject);

		// Create Object Selectable.
		if (ImGui::Selectable(curObject->GetName().c_str(), isSelected))
			selectedObject = curObject;

		// Change Background Colour on Selection.
		if (isSelected)
			ImGui::SetItemDefaultFocus();

		// Right Click Menu.
		if (curObject->GetObjectType() != SceneObjectType::CAMERA) // Make sure we're not right clicking on a Camera object. I don't want the camera to be deleteable.
		{
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete"))
					toDelete = curObject;

				ImGui::EndPopup();
			}
		}

		ImGui::PopID();
	}

	ImGui::EndChild();

	// Delete Object.
	if (toDelete)
		DeleteObject(toDelete);

	ImGui::End();
}

ObjectsWindow::ObjectsWindow(bool isActive, std::vector<SceneObject*>* objects) : Window(isActive), sceneObjects(objects)
{

}

ObjectsWindow::~ObjectsWindow()
{

}