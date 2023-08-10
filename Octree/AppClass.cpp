#include "AppClass.h"

using namespace Simplex;

void Application::InitVariables(void)
{	
#ifdef DEBUG
	uint uInstances = 800;
#else
	uint uInstances = 1600;
#endif

	int nSquare = static_cast<int>(std::sqrt(uInstances));
	m_uObjects = nSquare * nSquare;
	uint uIndex = -1;
	for (int i = 0; i < nSquare; i++)
	{
		for (int j = 0; j < nSquare; j++)
		{
			uIndex++;
			m_pEntityMngr->AddEntity("Minecraft\\Cube.obj");
			vector3 v3Position = vector3(glm::sphericalRand(32.0f)); 
			matrix4 m4Position = glm::translate(v3Position);
			m_pEntityMngr->SetModelMatrix(m4Position);
		}
	}

	m_pCameraMngr->SetPositionTargetAndUp(
			vector3(0.0f, 0.0f, 100.0f), 
			vector3(0.0f, 0.0f, 99.0f),	
			AXIS_Y);					

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); 
	m_uOctantLevels = 0;
	m_pRoot = new MyOctant(m_uOctantLevels, 5);
	m_pEntityMngr->Update();
}

void Application::Update(void)
{
	m_pSystem->Update();
	ArcBall();
	CameraRotation();
	m_pEntityMngr->Update();
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}

void Application::Display(void)
{
	ClearScreen();

	if (showOctree)
	{
		if (m_uOctantID == -1)
		{
			m_pRoot->Display();
		}
		else
		{
			m_pRoot->Display(m_uOctantID);
		}
	}

	
	m_pMeshMngr->AddSkyboxToRenderList();
	m_uRenderCallCount = m_pMeshMngr->Render();
	m_pMeshMngr->ClearRenderList();
	
	DrawGUI();
	
	m_pWindow->display();
}

void Application::Release(void)
{
	ShutdownGUI();
}