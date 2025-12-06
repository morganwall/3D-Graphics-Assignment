#include "Keith Helpers.h"
#include "GUI.h"
#include "Objects/SceneObject.h"
#include "Objects/Cube.h"
#include "Objects/Terrain.h"
#include "Objects/Camera.h"
#include "Objects/Skybox.h"
#include "Objects/Lights/Light.h"
#include "Objects/Lights/DirectionalLight.h"
#include "Objects/Lights/PointLight.h"
#include "Objects/Lights/SpotLight.h"

glm::ivec2 wndSize{ 0, 0 };
GUI* gui{ nullptr };
GLuint gShaderProgram{ 0 };
GLuint gSkyboxShaderProgram{ 0 };
GLuint gWidgetShaderProgram{ 0 };
std::string wndTitle{ "3D Graphics" };
std::vector<SceneObject*> sceneObjects;
Camera* camera{ nullptr };
glm::vec2 mousePosOld{ 0.0f, 0.0f };
glm::vec2 mousePosBeforeMenu{ 0.0f, 0.0f }; // Position of the mouse just before the menu is opened. Is used to restore the same mouse position once the menu is closed.
bool firstMenuFrame{ true };
glm::vec4 clearColour{ 0.35f, 0.43f, 0.49f, 0.0f };
Light* light{ nullptr };
glm::mat4 projectionMatrix;

// Create Window & Init OpenGL Context. Returns a Pointer to the Newly Created Window.
GLFWwindow* InitGLFW()
{
	// Init GLFW.
	if (!glfwInit())
	{
		std::cerr << "[!] Failed to Initialise GLFW!\n";
		return nullptr;
	}
	std::cout << "[+] GLFW Initialised.\n";

	// Configure OpenGL.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Require OpenGL 4.6 or better.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use the core profile. Gets rid of old OpenGL 1 and 2 functions.
	glfwWindowHint(GLFW_SAMPLES, 4); // Enable 4x MSAA;
#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // Enables debug messages.
#endif

	// Get Main Monitor Size.
	GLFWmonitor* mainMonitor{ glfwGetPrimaryMonitor() }; // Pointer to main monitor object.
	const GLFWvidmode* videoMode{ glfwGetVideoMode(mainMonitor) }; // Monitor video mode.

	// Create Window.
	wndSize = { videoMode->width, videoMode->height };
	GLFWwindow* wnd{ glfwCreateWindow(videoMode->width, videoMode->height, wndTitle.c_str(), nullptr, nullptr) };
	if (!wnd)
	{
		std::cerr << "[!] Failed to Create GLFW Window!\n";
		glfwTerminate(); // Cleanup GLFW.
		return nullptr;
	}
	std::cout << "[+] GLFW Window Created.\n";

	// Maximise Window.
	glfwMaximizeWindow(wnd);

	// Assign OpenGL Window Context.
	glfwMakeContextCurrent(wnd);

	// Enable Anit-Aliasing.
	glEnable(GL_MULTISAMPLE);

	// Enable Depth Testing.
	glEnable(GL_DEPTH_TEST);

	// Enable Face Culling.
	glEnable(GL_CULL_FACE);

	// Disable VSync;
	glfwSwapInterval(0);

	std::cout << "[+] GLFW Initialised.\n";
	return wnd;
}

// Load OpenGL Functions.
bool InitGLEW()
{
	glewExperimental = true; // Required for Core Profile.

	// Init GLEW.
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "[!] Failed to Initialise GLEW!\n";
		return false;
	}

	std::cout << "[+] GLEW Initialised.\n";
	return true;
}

// Custom ImGui Theme. Credits: https://github.com/ocornut/imgui/issues/707#issuecomment-576867100
void StyleOldSteam()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.14f, 0.16f, 0.11f, 0.52f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.27f, 0.30f, 0.23f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.34f, 0.26f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.28f, 0.32f, 0.24f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.30f, 0.22f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.23f, 0.27f, 0.21f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
	colors[ImGuiCol_Button] = ImVec4(0.29f, 0.34f, 0.26f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
	colors[ImGuiCol_Header] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.42f, 0.31f, 0.6f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
	colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.11f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.54f, 0.57f, 0.51f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.19f, 0.23f, 0.18f, 0.00f); // grip invis
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.54f, 0.57f, 0.51f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.54f, 0.57f, 0.51f, 0.78f);
	colors[ImGuiCol_TabActive] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.78f, 0.28f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.73f, 0.67f, 0.24f, 1.00f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameBorderSize = 1.0f;
	style.WindowRounding = 0.0f;
	style.ChildRounding = 0.0f;
	style.FrameRounding = 0.0f;
	style.PopupRounding = 0.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 0.0f;
	style.WindowMenuButtonPosition = ImGuiDir_None; // Hides the menu button from each window.
}

// Initialise ImGui.
void InitImGui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Set Style.
	StyleOldSteam();

	// Enable Docking.
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup ImGui Bindings.
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	std::cout << "[+] ImGui Initialised.\n";
}

// OpenGL Debug Message Callback Function.
void SetupDebugMessageCallback()
{
#ifdef _DEBUG
	int flags{ 0 };
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		// Init Debug Output.
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(KeithHelpers::glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		std::cout << "[+] OpenGL Debug Message Callback Setup.\n";
	}
#endif
}

void CreateShaders()
{
	// ----- Default Shader - Start ------
	// Create General Purpose Shaders.
	GLuint vertexShader{ KeithHelpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/vertex_shader.vert") };
	GLuint fragmentShader{ KeithHelpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/fragment_shader.frag") };
	gShaderProgram = glCreateProgram();

	// Create and Attach Shader Program.
	glAttachShader(gShaderProgram, vertexShader);
	glAttachShader(gShaderProgram, fragmentShader);

	// Link Shaders.
	KeithHelpers::LinkProgramShaders(gShaderProgram);
	// ----- Default Shader - End ------

	// ----- Skybox Shader - Start ------
	// Create Skybox Shaders.
	GLuint skyboxVertexShader{ KeithHelpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/skybox.vert") };
	GLuint skyboxFragmentShader{ KeithHelpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/skybox.frag") };
	gSkyboxShaderProgram = glCreateProgram();

	// Create and Attach Shader Program.
	glAttachShader(gSkyboxShaderProgram, skyboxVertexShader);
	glAttachShader(gSkyboxShaderProgram, skyboxFragmentShader);

	// Link Shaders.
	KeithHelpers::LinkProgramShaders(gSkyboxShaderProgram);
	// ----- Skybox Shader - End ------

	// ----- Widget Shader - Start ------
	// Create Widget Shaders.
	GLuint widgetVertexShader{ KeithHelpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/widget.vert") };
	GLuint widgetFragmentShader{ KeithHelpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/widget.frag") };
	gWidgetShaderProgram = glCreateProgram();

	// Create and Attach Shader Program.
	glAttachShader(gWidgetShaderProgram, widgetVertexShader);
	glAttachShader(gWidgetShaderProgram, widgetFragmentShader);

	// Link Shaders.
	KeithHelpers::LinkProgramShaders(gWidgetShaderProgram);
	// ----- Skybox Shader - End ------

	// Delete Shader Objects.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(skyboxVertexShader);
	glDeleteShader(skyboxFragmentShader);

	std::cout << "[+] Shaders Created and Linked.\n";
}

float GetDeltaTime()
{
	static float lastTime{ 0.0f }; // Will store the time of the last frame.

	// Get Current Frame Time.
	float curTime{ static_cast<float>(glfwGetTime()) };

	// Calculate Delta Time.
	float deltaTime{ curTime - lastTime }; // Delta time is just the difference between the two frame times.

	// Update the Last Frame Time.
	lastTime = curTime;

	return deltaTime;
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	// Set Callback to ImGui. We'll lose ImGui interactability if we don't.
	if (ImGui::GetCurrentContext())
		ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

	// Calculate Mouse Distance Moved.
	glm::vec2 moveDif{ xpos - mousePosOld.x, ypos - mousePosOld.y };

	// Handle Mouse Move.
	ImGuiIO io{ ImGui::GetIO() };
	if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && !io.WantCaptureMouse)
		camera->MouseMove(moveDif, window, GetDeltaTime());

	// Store Old Mouse Position.
	mousePosOld = { xpos, ypos };
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Set Callback to ImGui. We'll lose ImGui interactability if we don't.
	if (ImGui::GetCurrentContext())
		ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

	// Check if ImGui Wants to Capture the Scroll, or if Our Menu is Open.
	if (ImGui::GetIO().WantCaptureMouse)
		return;

	ImGuiIO io{ ImGui::GetIO() };
	if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && !io.WantCaptureMouse)
		camera->ScrollInput((float)yoffset);
}

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// Set the New Window Size.
	wndSize = { width, height };

	// Set Projection Matrix.
	projectionMatrix = glm::perspective(glm::radians(camera->GetFOV()), (float)wndSize.x / (float)wndSize.y, camera->GetNearClip(), camera->GetFarClip());

	// Set Viewport.
	glViewport(0, 0, width, height);
}

void Render(GLFWwindow* window)
{
	// Clear Screen.
	glClearColor(clearColour.r, clearColour.g, clearColour.b, clearColour.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup Camera.
	projectionMatrix = glm::perspective(glm::radians(camera->GetFOV()), (float)wndSize.x / (float)wndSize.y, camera->GetNearClip(), camera->GetFarClip());

	// Loop through All Objects and Draw Them.
	for (auto& curObject : sceneObjects)
	{
		// Switch on Object Type and Draw the Object.
		switch (curObject->GetObjectType())
		{
		case SceneObjectType::SKYBOX:
			glUseProgram(gSkyboxShaderProgram);
			curObject->Draw(camera->GetViewMatrix(), projectionMatrix, gSkyboxShaderProgram);
			break;

		default:
			glUseProgram(gShaderProgram);
			curObject->Draw(camera->GetViewMatrix(), projectionMatrix, gShaderProgram);
			break;
		}
	}

	// Set Widget Depth Settings.
	glDepthFunc(GL_ALWAYS);
	glDepthMask(GL_FALSE);

	// Second Object Loop to Draw Widgets. Less Efficient, but I want to Draw Widgets Last.
	for (auto& curObject : sceneObjects)
	{
		// Don't Draw Widgets for Camera.
		if (curObject->GetObjectType() == SceneObjectType::CAMERA)
			continue;

		// Check if the Current Object is Selected.
		bool isSelected{ false };
		if (gui->GetSelectedObject() == curObject)
			isSelected = true;

		curObject->DrawWidgets(camera->GetViewMatrix(), projectionMatrix, gWidgetShaderProgram, isSelected);
	}

	// Restore Depth Settings.
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	// Create New ImGui Frame.
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Our GUI.
	gui->DrawGUI(window, clearColour);

	// Render GUI.
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Swap Buffers.
	glfwSwapBuffers(window);
}

void Update(float deltaTime)
{
	// Loop through All Objects and Update Them.
	for (auto& curObject : sceneObjects)
		curObject->Update(deltaTime);
}

void Input(GLFWwindow* window, float deltaTime) // TODO: Move to GUI class.
{
	// GUI Shortcuts.
	if (ImGui::IsKeyPressed(ImGuiKey_F1))
		gui->SetShowObjectsWindow(!gui->GetShowObjectsWindow());
	if (ImGui::IsKeyPressed(ImGuiKey_F2))
		gui->SetShowInspectorWindow(!gui->GetShowInspectorWindow());
	if (ImGui::IsKeyPressed(ImGuiKey_F3))
		gui->SetShowWorldSettingsWindow(!gui->GetShowWorldSettingsWindow());

	if (ImGui::IsKeyPressed(ImGuiKey_Delete))
		if (gui->GetSelectedObject())
			gui->DeleteObject(gui->GetSelectedObject());

	// Save the Pre-GUI Mouse Position.
	if (firstMenuFrame)
	{
		glm::dvec2 tempMousePos{ 0.0, 0.0 };
		glfwGetCursorPos(window, &tempMousePos.x, &tempMousePos.y);
		mousePosBeforeMenu = { (float)tempMousePos.x, (float)tempMousePos.y };
		firstMenuFrame = false;
	}

	// Change GLFW Input Mode.
	if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		camera->KeyButtonInput(window, deltaTime);
	}
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Cleanup(GLFWwindow* window)
{
	// Cleanup OpenGL Resources.
	glDeleteProgram(gShaderProgram);

	// Loop through SceneObjects.
	for (auto& curObject : sceneObjects)
	{
		curObject->Cleanup();
		delete curObject;
	}

	// Cleanup GLFW.
	glfwDestroyWindow(window);
	glfwTerminate();
}

int main()
{
	// Create Window and Init GLFW.
	GLFWwindow* window = InitGLFW();
	if (!window)
		return 1;

	// Init GLEW.
	if (!InitGLEW())
		return 1;

	// Init ImGui.
	InitImGui(window);

	// Setup Debug Message Callback.
	SetupDebugMessageCallback();

	// Set Initial Mouse Position.
	glm::ivec2 tempMousePos;
	glfwGetWindowSize(window, &tempMousePos.x, &tempMousePos.y);
	mousePosOld = tempMousePos;

	// Setup Callbacks.
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

	// Create Shaders.
	CreateShaders();

	// Create Menu.
	gui = new GUI(sceneObjects);

	// Create Camera.
	camera = new Camera();

	// Create Light.
	light = new DirectionalLight("Directional Light");
	//light = new PointLight("Point Light", { 0.0f, 10.0f, 0.0f });
	//light = new SpotLight("Spot Light", { 0.0f, 10.0f, 0.0f }, { -90.0f, 0.0f, 0.0f });

	// Create Objects.
	sceneObjects.push_back(camera);

	sceneObjects.push_back(new Cube("Cube", { -5.0f, 0.0f, 0.0f }));
	sceneObjects.push_back(new Cube("Cube", { 1.0f, 0.0f, 0.0f }));
	sceneObjects.push_back(new Terrain("Terrain"));
	sceneObjects.push_back(light);
	sceneObjects.push_back(new Skybox());

	// Main Loop.
	while (!glfwWindowShouldClose(window))
	{
		float deltaTime{ GetDeltaTime() };

		Input(window, deltaTime);

		Update(deltaTime);

		Render(window);

		glfwPollEvents(); // Poll for Window Event.
	}

	// Cleanup.
	Cleanup(window);

	return 0;
}

/*
TODO:
Add different icons to the light widget.
Add all parts of Phong shading. .SHOULD DO.
Add support for multiple lights at the same time.
Add random and perlin noise height option to the terrain.
Make objects and widgets clickable/selectable.
Add a move and rotation widget to selected objects.
Add loading and rendering 3D models, with support for textures. !REQUIRED!
	Maybe add hierarchical transformations, for example a rotating propellor on a plane.
Make objects cast shadows.
Make terrain texture tile. .SHOULD DO.
Add ambient light intensity slider and colour picker to world settings window.
Make the transform widgets only show for the selected object.
Add a Goto button in the inspector window, that moved the camera to the object, with an offset away, looking at the object.
*/