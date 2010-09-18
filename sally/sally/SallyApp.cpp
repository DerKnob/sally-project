////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\SallyApp.cpp
///
/// \brief	Implements the sally application class. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2010 Sally Project
/// http://www.sally-project.de/
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU General Public License for more details.
/// 
/// You should have received a copy of the GNU General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SallyApp.h"

CSallyApp::CSallyApp()
	:m_pGUILoading(NULL), m_pGUIMainWindow(NULL), m_pConsole(NULL), m_pEffect(NULL), m_pEffectTexture(NULL), m_ppGUICurrent(NULL)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

CSallyApp::~CSallyApp()
{
	m_pLogger->Debug(" - starting destroying CSallyApp");

	// first delete the speech output to kill the thread which is talking
	SallyAPI::Speech::CSpeechOutputManager::Deactivate();

	m_pLogger->Debug(" -- SpeechOutputManager deactivated");

	if (m_ppGUICurrent != NULL)
	{
		if ((*m_ppGUICurrent) != NULL)
			(*m_ppGUICurrent)->SaveConfig();
	}

	m_pLogger->Debug(" -- application config saved");

 	SafeDelete(m_pGUILoading);
 	SafeDelete(m_pGUIMainWindow);
 	SafeDelete(m_pConsole);

	m_pLogger->Debug(" -- windows deleted");
 
	SallyAPI::Speech::CSpeechOutputManager::DeleteInstance();
	SallyAPI::Core::COptionManager::DeleteInstance();
	SallyAPI::Scheduler::CSchedulerManager::DeleteInstance();
	SallyAPI::Community::CCommunityManager::DeleteInstance();
	SallyAPI::Community::CCommunityDB::DeleteInstance();
	SallyAPI::System::CNumberGenerator::DeleteInstance();
	SallyAPI::Sound::Volume::CVolumeManager::DeleteInstance();
	SallyAPI::System::CSmartThreadPoolManager::DeleteInstance();
	SallyAPI::Database::CDatabaseConnection::Destroy();

	m_pLogger->Debug(" -- singelton classes destroyed");
// 
// 	SafeRelease(m_pEffect);

	Gdiplus::GdiplusShutdown(m_gdiplusToken);

	m_pLogger->Debug(" -- gdiplus shutdown");
}

bool CSallyApp::InitialiseEx()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
	SallyAPI::System::COption* option = config->GetOption();
	SallyAPI::Config::CTheme* theme = config->GetTheme();

	theme->AddPicture("gui\\backgrounds\\loading.png", GUI_THEME_SALLY_LOADING, 0);

	// Set State = GS_MENU
	m_pGUILoading	= new CWindowLoading();
	m_ppGUICurrent	= (CWindow**) &m_pGUILoading;

	m_pGUILoading->SetStatus("Loading Sally...");
	m_pGUILoading->SetProcessbar(5);

	m_pConsole = new CConsole();
			
// 	std::string backgroundFilename = SallyAPI::System::SystemHelper::GetModulePath();
// 	backgroundFilename.append("objects\\tie.x");
//  	m_pBackgroundMesh = new CMesh(backgroundFilename);


	// background effect
// 	std::string backgroundEffectTexture = SallyAPI::System::SystemHelper::GetModulePath();
// 	backgroundEffectTexture.append("effects\\glow.png");
// 
// 	SallyAPI::Core::CTextureManager* textureManager = SallyAPI::Core::CTextureManager::GetInstance();
// 
// 	m_pEffectTexture = textureManager->LoadTexture(backgroundEffectTexture.c_str());
// 
// 	std::string backgroundEffect = SallyAPI::System::SystemHelper::GetModulePath();
// 	backgroundEffect.append("effects\\VolumeLines.fx");
// 	HRESULT hr2 = D3DXCreateEffectFromFile(m_pDirect3DDevice, backgroundEffect.c_str(), NULL, NULL, 0, NULL, &m_pEffect, NULL);
// 
// 	m_pEffect->SetTexture("lineTexture", m_pEffectTexture->GetTexture());
// 
// 	m_fThickness = 2.f;

// 	for(UINT i = 0; i < ARRAY_SIZE(m_aLines); ++i)
// 	{
// 		m_aLines[i].x = SallyAPI::System::CNumberGenerator::GetFloatNumber(-1, 1) * 5.f;
// 		m_aLines[i].y = SallyAPI::System::CNumberGenerator::GetFloatNumber(-1, 1) * 5.f;
// 		m_aLines[i].z = 100.f;
// 	}

// 	m_aLines[0].x = 0;
// 	m_aLines[0].y = 0;
// 	m_aLines[0].z = 100.f;
// 
// 	m_aLines[1].x = 5.f;
// 	m_aLines[1].y = 5.f;
// 	m_aLines[1].z = 100.f;

	return true;
}

void CSallyApp::ProcessVoiceCommand()
{
	(*m_ppGUICurrent)->SendMessageToParent(0, 0, MS_SALLY_NEW_VOICE_COMMAND);
}

void CSallyApp::ExecuteGameEx()
{
	CWindowMessage wm;

	for (wm = (*m_ppGUICurrent)->GetMessage(); wm.GetReporter() != 0 
		|| wm.GetMessageId()!= 0; wm = (*m_ppGUICurrent)->GetMessage())
	{
		switch (wm.GetMessageId())
		{
		case GUIM_START_LOADING:
			m_tLoadingMainWindow.SetValues(&m_pGUIMainWindow, m_pGUILoading);
			m_tLoadingMainWindow.Start();
			break;
		case GUIM_LOADING_DONE:
			LoadingDone();
			break;
		case GUIM_EXIT:
			ShowWindow(m_hWnd, SW_HIDE);
			m_iQuit = 1;
			break;
		case GUIM_SHUTDOWN:
			ShowWindow(m_hWnd, SW_HIDE);
			m_iQuit = 2;
			break;
		case GUIM_UPDATE:
			ShowWindow(m_hWnd, SW_HIDE);
			m_iQuit = 3;

			std::string modulePath = SallyAPI::System::SystemHelper::GetModulePath();
			modulePath.append("SallyConfig.exe");

			ShellExecute(0, "open", modulePath.c_str(), "update", 0, SW_SHOWNORMAL);
			break;
		}
	}
}

void CSallyApp::LoadingDone() 
{
	m_ppGUICurrent = (CWindow**) &m_pGUIMainWindow;

	// Cleanup old
	SafeDelete(m_pGUILoading);

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
	SallyAPI::System::COption* option = config->GetOption();
	SallyAPI::Config::CTheme* theme = config->GetTheme();

	theme->RemovePicture(GUI_THEME_SALLY_LOADING);
}

int CSallyApp::GetQuit()
{
	return m_iQuit;
}

bool CSallyApp::CharInputPressed(char c)
{
	return (*m_ppGUICurrent)->CharInputPressed(c);
}

bool CSallyApp::KeyDown(int c)
{
	return (*m_ppGUICurrent)->KeyDown(c);
}

bool CSallyApp::ProcessMouseMove(int x, int y)
{
	return (*m_ppGUICurrent)->ProcessMouseMove(x, y);
}

bool CSallyApp::ProcessMouseDoubleClick( int x, int y )
{
	return (*m_ppGUICurrent)->ProcessMouseDoubleClick(x, y);
}

bool CSallyApp::ProcessMouseUp(int x, int y)
{
	return (*m_ppGUICurrent)->ProcessMouseUp(x, y);
}

bool CSallyApp::ProcessMouseDown(int x, int y)
{
	return (*m_ppGUICurrent)->ProcessMouseDown(x, y);
}

void CSallyApp::SendMessageToChilds(int messageId, SallyAPI::GUI::SendMessage::CParameterBase *messageParamter)
{
	(*m_ppGUICurrent)->SendMessageToChilds(0, 0, messageId, messageParamter);
}

void CSallyApp::RenderEx()
{
	if(!m_iQuit)
	{
		// fire the timer event
		(*m_ppGUICurrent)->Timer(m_counter.GetElapsedTime());

		// update the render to texture files
		// ToDo: Update the Texture
		//(*m_ppGUICurrent)->Update();

		//Clear the back buffer and depth buffer
		HRESULT hr = m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		if (hr != D3D_OK)
		{
			m_pLogger->Error("Core: RenderEx() m_pDirect3DDevice->Clear()");
			m_pLogger->Error(hr);
		}

		//Begin the scene
		hr = m_pDirect3DDevice->BeginScene();
		if (hr != D3D_OK)
		{
			m_pLogger->Error("Core: RenderEx() m_pDirect3DDevice->BeginScene()");
			m_pLogger->Error(hr);
		}

		if (hr == D3D_OK)
		{
			//Render3DBackground();
			RenderGUI();
			//Render3DForground();
		}
		//End the scene
		hr = m_pDirect3DDevice->EndScene();
		if (hr != D3D_OK)
		{
			m_pLogger->Error("Core: RenderEx() m_pDirect3DDevice->EndScene()");
			m_pLogger->Error(hr);
		}

		// display Scene
		hr = m_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
		if (hr != D3D_OK)
		{
			m_pLogger->Error("Core: RenderEx() m_pDirect3DDevice->Present()");
			m_pLogger->Error(hr);
		}
	}
}

void CSallyApp::TimerEvent()
{
	(*m_ppGUICurrent)->Timer(m_counter.GetElapsedTime());
}

void CSallyApp::RenderGUI()
{
	if (m_pSpriteInterface != NULL)
		m_pSpriteInterface->Begin(D3DXSPRITE_ALPHABLEND);

	m_pCamera->SetupGUICamera();
 	
	(*m_ppGUICurrent)->Render();
	
	if (m_pSpriteInterface != NULL)
		m_pSpriteInterface->End();

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
	SallyAPI::System::COption* option = config->GetOption();

	if (option->GetPropertyBool("sally", "console", false))
		RenderDebug();
}

void CSallyApp::RenderDebug()
{
	m_pConsole->SetFrames(m_dwFramesPerSecond);
	m_pConsole->SetDrawCount(m_iDrawCount);
	m_pConsole->Render();
}

void CSallyApp::Render3DBackground()
{
	m_pCamera->Setup3DCamera();

	// We will not be using a viewing transformation, so the view matrix will be identity.
	m_pEffect->SetMatrix("mWV", &(m_pCamera->GetViewMatrix()));
	m_pEffect->SetMatrix("mWVP", &(m_pCamera->GetProjectionMatrix()));

	UINT passes = 0;
	if(SUCCEEDED(m_pEffect->Begin(&passes, 0)))
	{
		m_pEffect->BeginPass(0);
		m_pDirect3DDevice->SetFVF(TVertex::FVF);

		for(UINT i = 0; i < ARRAY_SIZE(m_aLines) / 2; ++i)
		{
			RenderLine(m_aLines[i*2], m_aLines[i*2 + 1]);
		}
		m_pEffect->EndPass();
		m_pEffect->End();
	}


	m_pCamera->Setup3DCamera();

	LPDIRECT3DDEVICE9 direct3DDevice = SallyAPI::Core::CGame::GetDevice();

	//Render our 3D objects
	D3DXMATRIX matMove;
	D3DXMATRIX matShip;
	D3DXMATRIX matRotateX, matRotateY, matRotateZ;

	//Create the transformation matrices
	D3DXMatrixRotationX(&matRotateX, timeGetTime()/1000.0f);
	D3DXMatrixRotationY(&matRotateY, timeGetTime()/1000.0f);
	D3DXMatrixRotationZ(&matRotateZ, timeGetTime()/1000.0f);
	D3DXMatrixTranslation(&matMove, 0.0f, 0.0f, 20.0f);

	D3DXMatrixMultiply(&matShip, &matRotateY, &matRotateX);
	D3DXMatrixMultiply(&matShip, &matShip, &matRotateZ);
	D3DXMatrixMultiply(&matShip, &matShip, &matMove);

	//Render our objects
	direct3DDevice->SetTransform(D3DTS_WORLD, &matShip);

	m_pBackgroundMesh->Render();
}

void CSallyApp::Render3DForground()
{
}


void CSallyApp::RenderLine(D3DXVECTOR3& v0, D3DXVECTOR3& v1)
{
	TVertex vrts[4];
	vrts[0].pos = v0;		vrts[0].otherPos = v1;
	vrts[1].pos = v1;		vrts[1].otherPos = v0;
	vrts[2].pos = v0;		vrts[2].otherPos = v1;
	vrts[3].pos = v1;		vrts[3].otherPos = v0;

	vrts[0].thickness = D3DXVECTOR3(-m_fThickness, 0.f, m_fThickness * 0.5f);
	vrts[1].thickness = D3DXVECTOR3(m_fThickness, 0.f, m_fThickness * 0.5f);
	vrts[2].thickness = D3DXVECTOR3(m_fThickness, 0.f, m_fThickness * 0.5f);
	vrts[3].thickness = D3DXVECTOR3(-m_fThickness, 0.f, m_fThickness * 0.5f);

	vrts[0].texOffset = D3DXVECTOR4(m_fThickness, m_fThickness, 0.f, 0.f);
	vrts[1].texOffset = D3DXVECTOR4(m_fThickness, m_fThickness, 0.25f, 0.f);
	vrts[2].texOffset = D3DXVECTOR4(m_fThickness, m_fThickness, 0.f, 0.25f);
	vrts[3].texOffset = D3DXVECTOR4(m_fThickness, m_fThickness, 0.25f, 0.25f);

	m_pDirect3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vrts, sizeof(TVertex));
}

void CSallyApp::OnCommandDeviceLost()
{
	(*m_ppGUICurrent)->SendMessageToParent(0, 0, MS_SALLY_APP_STOP_SCREENSAVER);
	(*m_ppGUICurrent)->SendMessageToChilds(0, 0, MS_SALLY_DEVICE_LOST);
}

void CSallyApp::OnCommandDeviceRestoreStart()
{
	m_pLogger->Warning("Device Restore Start");
	(*m_ppGUICurrent)->SendMessageToChilds(0, 0, MS_SALLY_DEVICE_RESTORE_START);
}

void CSallyApp::OnCommandDeviceRestoreEnd()
{
	m_pLogger->Warning("Device Restore End");
	(*m_ppGUICurrent)->SendMessageToChilds(0, 0, MS_SALLY_DEVICE_RESTORE_END);
}