#ifndef __APPLICATIONCLASS_H_
#define __APPLICATIONCLASS_H_

#include "Definitions.h"

#include "ControllerConfiguration.h"
#include "imgui\ImGuiObject.h"

#include "MyOctant.h"

namespace Simplex
{

class Application
{
public:
	MyEntityManager* m_pEntityMngr = nullptr; 
	uint m_uOctantID = -1; 
	uint m_uObjects = 0; 
	uint m_uOctantLevels = 0; 
	MyOctant* m_pRoot = nullptr;
	bool showOctree = true; 

private:
	String m_sProgrammer = "Israel Anthony - israelanthonyjr@gmail.com";

	static ImGuiObject gui; 
	bool m_bGUI_Main = true; 
	bool m_bGUI_Console = true; 
	bool m_bGUI_Test = false; 
	bool m_bGUI_Controller = false; 

	uint m_uRenderCallCount = 0; 
	uint m_uControllerCount = 0; 

	bool m_bFocused = true; 

	float m_fCameraMovementSpeed = 0.1f; 

	vector3 m_v3MousePos = vector3(); 
	bool m_bFirstPersonCamera = false;
	bool m_bArcBall = false;
	quaternion m_qArcBall; 

	vector4 m_v4ClearColor; //Color of the scene
	bool m_bRunning = false; 
	bool m_bModifier = false; //is shift pressed?

	sf::Window* m_pWindow = nullptr; 
	SystemSingleton* m_pSystem = nullptr; 
	LightManager* m_pLightMngr = nullptr; 
	MeshManager* m_pMeshMngr = nullptr; 
	CameraManager* m_pCameraMngr = nullptr; 
	
	ControllerInput* m_pController[8]; 
	uint m_uActiveController = 0; 

	sf::SoundBuffer m_soundBuffer; 
	sf::Sound m_sound; 
	sf::Music m_soundBGM; 

public:
#pragma region Constructor / Run / Destructor

	Application();

	/*
	USAGE: Initializes the window and rendering context
	ARGUMENTS:
	-	String a_sApplicationName -> Name of the window
	-	int size -> formated size, relate to BTO_RESOLUTIONS
	-	bool a_bFullscreen = false -> is the window fullscreen?
	-	bool a_bBorderless = false -> is the window borderless?
	*/
	void Init(String a_sApplicationName = "Octree Example", int a_uSize = BTO_RESOLUTIONS::RES_C_1920x1080_16x9_FULLHD,
		bool a_bFullscreen = false, bool a_bBorderless = false);

	/*
	USAGE: Initializes the window and rendering context
	ARGUMENTS:
	-	String a_sApplicationName = "" -> Name of the window 
	-	uint a_nWidth -> Window Width
	-	uint a_nHeight -> Window Height
	-	bool a_bFullscreen -> is the window fullscreen?
	-	bool a_bBorderless -> is the window borderless?
	*/
	void Init(String a_sApplicationName, uint a_uWidth, uint a_uHeight, bool a_bFullscreen, bool a_bBorderless);

	/*
	USAGE: Runs the main loop of the application
	*/
	void Run(void);

	/*
	USAGE: Destructor
	*/
	~Application(void);
#pragma endregion

private:
#pragma region Initialization / Release
	/*
	USAGE: Initialize the window
	ARGUMENTS: String a_sWindowName = "GLFW" -> Window name
	*/
	void InitWindow(String a_sWindowName = "Octree Example");

	/*
	USAGE: Initializes the application's member variables and the entities used for the Octree demonstration
	*/
	void InitVariables(void);

	/*
	USAGE: Reads the configuration of the application from a file
	*/
	void ReadConfig(void);

	/*
	USAGE: Writes the configuration of the application to a file
	*/
	void WriteConfig(void);

	/*
	USAGE: Releases the application
	*/
	void Release(void);
#pragma endregion

#pragma region Main Loop
	/*
	USAGE: Updates the scene
	*/
	void Update(void);

	/*
	USAGE: Displays the scene
	*/
	void Display(void);

	/*
	USAGE: Clears the OpenGL screen using the specified color
	ARGUMENTS: vector4 a_v4ClearColor = vector4(-1.0f) -> Color to clear the screen with
	*/
	void ClearScreen(vector4 a_v4ClearColor = vector4(-1.0f));

	/*
	USAGE: Initialize the controllers generically
	*/
	void InitControllers(void);

	/*
	USAGE: Releases the controllers
	*/
	void ReleaseControllers(void);
#pragma endregion

#pragma region Application Controls
	/*
	USAGE: Manage constant keyboard state
	*/
	void ProcessKeyboard(void);

	/*
	USAGE: Manage constant keyboard state
	*/
	void ProcessJoystick(void);

	/*
	USAGE: Process the arcball of the scene, rotating an object in the center of it	a_fSensitivity is
	a factor of change
	ARGUMENTS: float a_fSensitivity = 0.1f -> indicates how fast the arcball is going to change
	*/
	void ArcBall(float a_fSensitivity = 0.1f);

	/*
	USAGE: Manages the rotation of the camera a_fSpeed is a factor of change
	ARGUMENTS: float a_fSpeed = 0.005f
	*/
	void CameraRotation(float a_fSpeed = 0.005f);
#pragma endregion

#pragma region Process Events
	/*
	USAGE: Resizes the window
	*/
	void ResizeWindow(void);

	/*
	USAGE: Manage the response of key presses
	*/
	void ProcessKeyPressed(sf::Event a_event);

	/*
	USAGE: Manage the response of key releases
	*/
	void ProcessKeyReleased(sf::Event a_event);

	/*
	USAGE: Manage the response of mouse movement
	*/
	void ProcessMouseMovement(sf::Event a_event);

	/*
	USAGE: Manage the response of mouse key presses
	*/
	void ProcessMousePressed(sf::Event a_event);

	/*
	USAGE: Manage the response of mouse key release
	*/
	void ProcessMouseReleased(sf::Event a_event);

	/*
	USAGE: Manage the response of mouse scrolling
	*/
	void ProcessMouseScroll(sf::Event a_event);

	/*
	USAGE: Manage the response of joystick just connected
	*/
	void ProcessJoystickConnected(uint nController = 0);

	/*
	USAGE: Manage the response of joystick key presses
	*/
	void ProcessJoystickPressed(sf::Event a_event);

	/*
	USAGE: Manage the response of joystick key release
	*/
	void ProcessJoystickReleased(sf::Event a_event);

	/*
	USAGE: Manage the response of joystick axis movement
	*/
	void ProcessJoystickMoved(sf::Event a_event);
#pragma endregion

#pragma region GUI
	/*
	USAGE: Initialize the GUI
	*/
	void InitIMGUI(void);

	/*
	USAGE: Draw the GUI elements
	*/
	void DrawGUI(void);

	/*
	USAGE: Releases the GUI
	*/
	void ShutdownGUI(void);
	/*
	USAGE: Render the GUI lists
	*/
	static void RenderDrawLists(ImDrawData* draw_data);

	/*
	USAGE: Creates fonts for the GUI
	*/
	bool CreateFontsTexture(void);

	/*
	USAGE: Create OpenGL resources for the GUI
	*/
	bool CreateDeviceObjects(void);

	/*
	USAGE: creates a new frame for the GUI
	*/
	void NewFrame(void);
#pragma endregion

#pragma region The Rule of Three
	/*
	USAGE: copy constructor, private so it does not let object copy
	ARGUMENTS: GLFWApp const& input -> object to copy 
	*/
	Application(Application const& input);

	/*
	USAGE: copy assignment, private so it does not let object copy
	ARGUMENTS: GLFWApp const& input -> object to copy
	*/
	Application& operator=(Application const& input);
#pragma endregion
};

} 
#endif //__APPLICATIONCLASS_H_