////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Game.cpp
///
/// \brief	Implements the game class. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2010 Sally Project
/// http://www.sally-project.org/
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU Lesser General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU Lesser General Public License for more details.
/// 
/// You should have received a copy of the GNU Lesser General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Game.h"

using namespace SallyAPI::Core;

LPDIRECT3DDEVICE9				CGame::m_pDirect3DDevice = NULL;
LPDIRECT3D9						CGame::m_pDirect3D = NULL;
LPDIRECT3DSURFACE9				CGame::m_pBackBuffer = NULL;
LPDIRECT3DSURFACE9				CGame::m_pRenderTarget = NULL;
SallyAPI::System::CLogger*		CGame::m_pLogger = NULL;
HWND							CGame::m_hWnd = NULL;
HINSTANCE						CGame::m_hInst = NULL;
LPD3DXSPRITE					CGame::m_pSpriteInterface = NULL;
int								CGame::m_iScreenWidth;
int								CGame::m_iScreenHeight;
std::string						CGame::m_strMediaFolder;
SallyAPI::Core::CCamera*		CGame::m_pCamera = NULL;
SallyAPI::Core::CCounter*		CGame::m_pCounter = NULL;
int								CGame::m_iDrawCount = 0;
int								CGame::m_iDrawCountNew = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \struct	DEVICE_LIST
///
/// \brief	List of devices. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

struct DEVICE_LIST
{
	GUID	guId;
	int		listId;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \struct	DEVICE_INFO
///
/// \brief	Information about the device. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

struct DEVICE_INFO
{
	int deviceId;
	int deviceType;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	INT_PTR CALLBACK DIEnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi,LPVOID pvRef)
///
/// \brief	Callback, called when the di enum devices. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	lpddi	The lpddi. 
/// \param	pvRef	The pv reference. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK DIEnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi,LPVOID pvRef)
{
	static int i = 0;

	std::vector<DEVICE_LIST>* deviceListVector = (std::vector<DEVICE_LIST> *)pvRef;
	DEVICE_LIST		deviceList;

	deviceList.guId = lpddi->guidInstance;
	deviceList.listId = i++;

	// Add the string to the combo box
	deviceListVector->push_back(deviceList);
	return DIENUM_CONTINUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LPDIRECT3DDEVICE9 CGame::GetDevice()
///
/// \brief	Gets the direct3d device. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The device. 
////////////////////////////////////////////////////////////////////////////////////////////////////

LPDIRECT3DDEVICE9 CGame::GetDevice()
{
	return m_pDirect3DDevice;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LPDIRECT3D9 CGame::GetD3D()
///
/// \brief	Gets the Direct3d. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The d 3 d. 
////////////////////////////////////////////////////////////////////////////////////////////////////

LPDIRECT3D9 CGame::GetD3D()
{
	return m_pDirect3D;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LPD3DXSPRITE CGame::GetSpriteInterface()
///
/// \brief	Gets the sprite interface. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The sprite interface. 
////////////////////////////////////////////////////////////////////////////////////////////////////

LPD3DXSPRITE CGame::GetSpriteInterface()
{
	return m_pSpriteInterface;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::System::CLogger* CGame::GetLogger()
///
/// \brief	Gets the logger. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the logger. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::System::CLogger* CGame::GetLogger()
{
	return m_pLogger;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Core::CCamera* CGame::GetCamera()
///
/// \brief	Gets the camera. 
///
/// \author	Christian Knobloch
/// \date	18.02.2011
///
/// \return	null if it fails, else the camera. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Core::CCamera* CGame::GetCamera()
{
	return m_pCamera;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Core::CCounter* CGame::GetCounter()
///
/// \brief	Gets the counter instance. 
///
/// \author	Christian Knobloch
/// \date	18.02.2011
///
/// \return	null if it fails, else the counter instance. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Core::CCounter* CGame::GetCounter()
{
	return m_pCounter;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	HWND CGame::GetHWnd()
///
/// \brief	Gets the HWND of the window. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The h window. 
////////////////////////////////////////////////////////////////////////////////////////////////////

HWND CGame::GetHWnd()
{
	return m_hWnd;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	HINSTANCE CGame::GetHInstance()
///
/// \brief	Gets the HInstance of the window. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The h instance. 
////////////////////////////////////////////////////////////////////////////////////////////////////

HINSTANCE CGame::GetHInstance()
{
	return m_hInst;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CGame::GetScreenWidth()
///
/// \brief	Gets the screen width. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The screen width. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CGame::GetScreenWidth()
{
	return m_iScreenWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CGame::GetScreenHeight()
///
/// \brief	Gets the screen height. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The screen height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CGame::GetScreenHeight()
{
	return m_iScreenHeight;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CGame::GetMediaFolder()
///
/// \brief	Gets the media folder. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The media folder. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CGame::GetMediaFolder()
{
	if (m_strMediaFolder.length() == 0)
	{
		m_strMediaFolder = SallyAPI::System::SystemHelper::GetModulePath();
	}
	return m_strMediaFolder;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGame::SetMediaFolder(const std::string& mediaFolder)
///
/// \brief	Sets a media folder. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	mediaFolder	Pathname of the media folder. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGame::SetMediaFolder(const std::string& mediaFolder)
{
	m_strMediaFolder = mediaFolder;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CGame::CGame()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CGame::CGame()
{
	m_pDirect3D					= NULL;
	m_pDirect3DDevice			= NULL;
	m_eDeviceState				= DEVICE_STATE_OK;

	// Input Devices
	m_pDirectInput				= NULL;
	m_pKeyboard					= NULL;
	m_pMouse					= NULL;
	m_pGamepad					= NULL;

	// Frames calculation
	m_dwFrames					= 0;
	m_dwFramesPerSecond			= 0;
	m_dwLastFramesPerSecondTime = 0;
	m_dwFramesPerSecondNew		= 0;

	m_iQuit						= 0;


	std::string loggerPath = CGame::GetMediaFolder();
	loggerPath.append("sally.log");

	// get loglevel
	std::string logLevelStr = SallyAPI::System::COption::GetPropertyStringStatic("sally", "logLevel", "Info");
	SallyAPI::System::LOG_LEVEL logLevel = SallyAPI::System::LOG_LEVEL_INFO;
	if (logLevelStr.compare("Debug") == 0)
		logLevel = SallyAPI::System::LOG_LEVEL_DEBUG;
	else if (logLevelStr.compare("Info") == 0)
		logLevel = SallyAPI::System::LOG_LEVEL_INFO;
	else if (logLevelStr.compare("Warning") == 0)
		logLevel = SallyAPI::System::LOG_LEVEL_WARNING;
	else if (logLevelStr.compare("Error") == 0)
		logLevel = SallyAPI::System::LOG_LEVEL_ERROR;

	bool logAppend = SallyAPI::System::COption::GetPropertyBoolStatic("sally", "logAppend", false);

	m_pLogger = new SallyAPI::System::CLogger(loggerPath, !logAppend, logLevel);
	m_pLogger->Info("Initialise Game Engine");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CGame::~CGame()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CGame::~CGame()
{
	m_pLogger->Info("Cleaning Up");

	SallyAPI::Core::CTextureManager::DeleteInstance();
	SallyAPI::Core::CFontManager::DeleteInstance();
	SallyAPI::Config::CConfig::DeleteInstance();

	SafeDelete(m_pCamera);
	SafeDelete(m_pCounter);

	SafeRelease(m_pSpriteInterface);
	SafeRelease(m_pDirect3DDevice);
	SafeRelease(m_pDirect3D);

	SafeDelete(m_pLogger);

	// shutdown pc?
	if (m_iQuit == 2)
	{
		HANDLE hToken; 
		TOKEN_PRIVILEGES tkp;

		// Get a token for this process. 
		OpenProcessToken(GetCurrentProcess(), 
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

		// Get the LUID for the shutdown privilege. 
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 

		tkp.PrivilegeCount = 1;  // one privilege to set    
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

		// Get the shutdown privilege for this process. 
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 

		// Shut down the system and force all applications to close. 
		ExitWindowsEx(EWX_SHUTDOWN| EWX_FORCE, 
			SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
			SHTDN_REASON_MINOR_UPGRADE |
			SHTDN_REASON_FLAG_PLANNED);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CGame::Initialise(HWND hWnd, HINSTANCE hInst)
///
/// \brief	Init the Game Engine and calls then the InitialiseEx. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	hWnd	Handle of the window. 
/// \param	hInst	The instance. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CGame::Initialise(HWND hWnd, HINSTANCE hInst)
{
	m_hWnd = hWnd;
	m_hInst = hInst;

	m_iDeviceId = SallyAPI::System::COption::GetPropertyIntStatic("GraphicDevice", "DeviceId", 0);
	m_bWindowd = ! (SallyAPI::System::COption::GetPropertyBoolStatic("GraphicDevice", "Fullscreen"));
	m_bMultiMonitor = SallyAPI::System::COption::GetPropertyBoolStatic("GraphicDevice", "MultiMonitor");
	m_strMonitorId = SallyAPI::System::COption::GetPropertyStringStatic("GraphicDevice", "MonitorId");
	m_MultiSampleType = (D3DMULTISAMPLE_TYPE) SallyAPI::System::COption::GetPropertyIntStatic("GraphicDevice", "MultiSamplingType");
	m_iScreenWidth = SallyAPI::System::COption::GetPropertyIntStatic("GraphicDevice", "Width");
	m_iScreenHeight = SallyAPI::System::COption::GetPropertyIntStatic("GraphicDevice", "Height");
	m_iScreenColorDepth = SallyAPI::System::COption::GetPropertyIntStatic("GraphicDevice", "Color");

	if ((m_iScreenWidth < 1024) && (m_iScreenWidth > 1920) &&
		(m_iScreenHeight < 600) && (m_iScreenHeight > 1200))
	{
		return false;
	}

	// if multimonitor and in fullscreen
	// than crate a window in the full screen size without border
	if ((m_bMultiMonitor) && (!m_bWindowd))
	{
		m_bWindowd = true;

		DEVMODE dm;
		// initialize the DEVMODE structure
		ZeroMemory(&dm, sizeof(dm));
		dm.dmSize = sizeof(dm);
		if (EnumDisplaySettings(m_strMonitorId.c_str(), ENUM_CURRENT_SETTINGS, &dm) == 0)
			return false;

		m_iWindowPosX = dm.dmPosition.x;
		m_iWindowPosY = dm.dmPosition.y;
		m_iWindowWidth = dm.dmPelsWidth;
		m_iWindowHeight = dm.dmPelsHeight;

		m_iScreenWidth = m_iWindowWidth;
		m_iScreenHeight = m_iWindowHeight;
	}
	else
	{
		m_bMultiMonitor = false;
	}

	//Initialise Direct3D
	if(!InitialiseDirect3D())
	{
		return false;
	}
	if (!InitialiseLights())
	{
		return false;
	}

	// setup Camera
	m_pCamera = new SallyAPI::Core::CCamera(m_iScreenWidth, m_iScreenHeight);
	// setup Counter
	m_pCounter = new SallyAPI::Core::CCounter();

	if(!InitialiseEx())
	{
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGame::GameLoop()
///
/// \brief	The Main Loop to Peek the Messages and Execute the Game. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGame::GameLoop()
{
	m_pLogger->Info("Starting Game Loop");

    //Enter the game loop
    MSG			msg; 
    BOOL		newMessage;

	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

    while((msg.message != WM_QUIT) && (!m_iQuit))
    {
        newMessage = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);

        if(newMessage)
        {
            //Process message
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
			// Check if the Device is Lost
			if (CheckDevice())
			{
				ExecuteGame();
			}
			else
			{
				TimerEvent();
			}
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CGame::CheckDevice()
///
/// \brief	check for lost device. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CGame::CheckDevice()
{
	HRESULT result = m_pDirect3DDevice->TestCooperativeLevel();
	switch(result)
	{
	case D3DERR_DRIVERINTERNALERROR:
		m_pLogger->Error("D3DERR_DRIVERINTERNALERROR");
		m_pLogger->Error(result);
		return false;
	case D3DERR_DEVICELOST:
		Sleep(200);

		m_pLogger->Debug("D3DERR_DEVICELOST");
		m_pLogger->Debug(result);

		if (m_eDeviceState != DEVICE_STATE_LOST)
			OnLostDevice();
		return false;
	case D3DERR_DEVICENOTRESET:
		Sleep(200);

		m_pLogger->Debug("D3DERR_DEVICENOTRESET");
		m_pLogger->Debug(result);

		result = m_pDirect3DDevice->Reset(&m_PresentParameters); 

		m_pLogger->Debug("m_pDirect3DDevice->Reset()");
		m_pLogger->Debug(result);

		if(FAILED(result))
		{
			switch (result)
			{
			case D3DERR_DEVICELOST:
				return false; 
			case D3DERR_DRIVERINTERNALERROR:
				return false;
			case D3DERR_OUTOFVIDEOMEMORY:
				return false;
			case E_OUTOFMEMORY:
				return false;
			}
		}
		OnRestoreDevice();
		return false;
	}
	if (m_eDeviceState == DEVICE_STATE_RESTORED)
	{
		OnCommandDeviceRestoreEnd();
		m_eDeviceState = DEVICE_STATE_OK;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CGame::OnLostDevice()
///
/// \brief	Executes the lost device action. 
///
/// \author	Christian Knobloch
/// \date	01.06.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CGame::OnLostDevice()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CTheme* theme = config->GetTheme();
	SallyAPI::Core::CFontManager* fontManager = SallyAPI::Core::CFontManager::GetInstance();

	if (m_pSpriteInterface != NULL)
	{
		HRESULT hr = m_pSpriteInterface->OnLostDevice();
		if (hr != S_OK)
		{
			m_pLogger->Error("CGame::OnLostDevice() m_pSpriteInterface->OnLostDevice()");
			m_pLogger->Error(hr);
			return false;
		}
	}
	if (!fontManager->OnLostDevice())
	{
		return false;
	}
	// send the message to the apps
	OnCommandDeviceLost();

	m_eDeviceState = DEVICE_STATE_LOST;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CGame::OnRestoreDevice()
///
/// \brief	Executes the restore device action. 
///
/// \author	Christian Knobloch
/// \date	01.06.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CGame::OnRestoreDevice()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CTheme* theme = config->GetTheme();
	SallyAPI::Core::CFontManager* fontManager = SallyAPI::Core::CFontManager::GetInstance();

	// if device lost is not called before
	if (m_eDeviceState != DEVICE_STATE_LOST)
		OnLostDevice();

	// send the app the device restored message
	OnCommandDeviceRestoreStart();

	InitialiseDeviceStates();

	if (m_pSpriteInterface != NULL)
	{
		HRESULT hr = m_pSpriteInterface->OnResetDevice();
		if (hr != S_OK)
		{
			m_pLogger->Error("CGame::OnLostDevice() m_pSpriteInterface->OnRestoreDevice()");
			m_pLogger->Error(hr);
			return false;
		}
	}
	if (!fontManager->OnResetDevice())
	{
		return false;
	}
	// send the message to the apps
	//OnCommandDeviceRestoreEnd(); // call this later

	m_eDeviceState = DEVICE_STATE_RESTORED;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGame::CheckDisplayMode()
///
/// \brief	Check display mode. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGame::CheckDisplayMode()
{
	UINT x;

	for(x = 0; x < m_pDirect3D->GetAdapterModeCount(m_iDeviceId, (D3DFORMAT) D3DFMT_X8R8G8B8); ++x)
	{
		m_pDirect3D->EnumAdapterModes(m_iDeviceId, (D3DFORMAT) D3DFMT_X8R8G8B8, x, &m_DisplayMode);
		if(m_DisplayMode.Width == m_iScreenWidth)
		{
			if(m_DisplayMode.Height == m_iScreenHeight)
			{
				if((m_DisplayMode.Format == D3DFMT_R5G6B5) || (m_DisplayMode.Format == D3DFMT_X1R5G5B5) 
					|| (m_DisplayMode.Format == D3DFMT_X4R4G4B4))
				{
					if(m_iScreenColorDepth == 16)
					{
						return;
					}
				}
				else if((m_DisplayMode.Format == D3DFMT_R8G8B8) || (m_DisplayMode.Format == D3DFMT_X8R8G8B8))
				{
					if(m_iScreenColorDepth == 32)
					{
						return;
					}
				}
			}
		}
	}

	m_DisplayMode.Format = D3DFMT_UNKNOWN;
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CGame::InitialisePresentationParameters()
///
/// \brief	Initialises the presentation parameters. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CGame::InitialisePresentationParameters()
{
	//Create a structure to hold the settings for our device
	ZeroMemory(&m_PresentParameters, sizeof(m_PresentParameters));

	m_PresentParameters.Windowed = m_bWindowd;
	m_PresentParameters.BackBufferCount = 1;
	m_PresentParameters.BackBufferFormat = m_DisplayMode.Format;
	m_PresentParameters.BackBufferWidth = m_DisplayMode.Width;
	m_PresentParameters.BackBufferHeight = m_DisplayMode.Height;
	m_PresentParameters.hDeviceWindow = m_hWnd;
	m_PresentParameters.SwapEffect = D3DSWAPEFFECT_FLIP;
	m_PresentParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_PresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // ToDo:Check
	//m_PresentParameters.MultiSampleType = m_MultiSampleType;

	//Select the best depth buffer, select 32, 24 or 16 bit
	if(m_pDirect3D->CheckDeviceFormat(m_iDeviceId, D3DDEVTYPE_HAL, m_DisplayMode.Format, 
		D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32) == D3D_OK)
	{
		m_PresentParameters.AutoDepthStencilFormat = D3DFMT_D32;
		m_PresentParameters.EnableAutoDepthStencil = TRUE;

		m_pLogger->Info("32bit depth buffer selected");
		return true;
	}
	else if(m_pDirect3D->CheckDeviceFormat(m_iDeviceId, D3DDEVTYPE_HAL, m_DisplayMode.Format, 
		D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8) == D3D_OK)
	{
		m_PresentParameters.AutoDepthStencilFormat = D3DFMT_D24X8;
		m_PresentParameters.EnableAutoDepthStencil = TRUE;

		m_pLogger->Info("24bit depth buffer selected");
		return true;
	}
	else if(m_pDirect3D->CheckDeviceFormat(m_iDeviceId, D3DDEVTYPE_HAL, m_DisplayMode.Format,
		D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16) == D3D_OK)
	{
		m_PresentParameters.AutoDepthStencilFormat = D3DFMT_D16;
		m_PresentParameters.EnableAutoDepthStencil = TRUE;

		m_pLogger->Info("16bit depth buffer selected");
		return true;
	}
	m_PresentParameters.EnableAutoDepthStencil = FALSE;
	m_pLogger->Error("Unable to select depth buffer.");
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CGame::InitialiseDisplayMode()
///
/// \brief	Initialises the display mode. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CGame::InitialiseDisplayMode()
{
	m_pLogger->Info("InitialiseDisplayMode() starting");
	if (m_bMultiMonitor)
	{
		MoveWindow(m_hWnd, m_iWindowPosX, m_iWindowPosY, m_iWindowWidth, m_iWindowHeight, TRUE);
	}
	else if (m_bWindowd)
	{
		m_pDirect3D->GetAdapterDisplayMode(m_iDeviceId, &m_DisplayMode);

		int winWidth = m_iScreenWidth;
		int winHeight = m_iScreenHeight;

		RECT rect;
		GetWindowRect(m_hWnd, &rect);
		MoveWindow(m_hWnd, (GetSystemMetrics(SM_CXSCREEN) - winWidth) / 2, (GetSystemMetrics(SM_CYSCREEN) - winHeight) / 2, winWidth, winHeight, TRUE);

		GetClientRect(m_hWnd, &rect);
		winWidth += (winWidth - rect.right);
		winHeight += (winHeight - rect.bottom);

		GetWindowRect(m_hWnd, &rect);
		MoveWindow(m_hWnd, (GetSystemMetrics(SM_CXSCREEN) - winWidth) / 2, (GetSystemMetrics(SM_CYSCREEN) - winHeight) / 2, winWidth, winHeight, TRUE);
	}	

	CheckDisplayMode();
	
	if(m_DisplayMode.Format == D3DFMT_UNKNOWN)
	{
		m_pLogger->Error("Unable to select back buffer format.");
		return false;
	}

	//Width x Height x 32bit has been selected
	m_DisplayMode.Width = m_iScreenWidth;
	m_DisplayMode.Height = m_iScreenHeight;

	m_pLogger->Info("InitialiseDisplayMode() done");
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CGame::InitialiseDirect3D()
///
/// \brief	Initialises the direct 3 d. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CGame::InitialiseDirect3D()
{
	m_pLogger->Info("InitialiseDirect3D() starting");

	m_fWidthCorrection = (float) m_iScreenWidth / 640;
	m_fHeightCorrection = (float) m_iScreenHeight / 480;

	SallyAPI::Core::CBaseObject::Init(m_fWidthCorrection, m_fHeightCorrection);

    m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
    if(m_pDirect3D == NULL)
    {
		m_pLogger->Error("Unable to create DirectX9 interface.");
        return false;
    }

	if (!InitialiseDisplayMode())
	{
		return false;
	}
	if (!InitialisePresentationParameters())
	{
		return false;
	}

// 	D3DCAPS9 d3dCaps; 
// 	m_pDirect3D->GetDeviceCaps(m_iDeviceId, D3DDEVTYPE_HAL, &d3dCaps); 
// 
// 	if( d3dCaps.VertexProcessingCaps != 0 ) 
// 		m_iVertexProcessing |= D3DCREATE_HARDWARE_VERTEXPROCESSING; 
// 	else 
// 		m_iVertexProcessing |= D3DCREATE_SOFTWARE_VERTEXPROCESSING; 

	//Create a Direct3D device.
    if(FAILED(m_pDirect3D->CreateDevice(m_iDeviceId, D3DDEVTYPE_HAL, m_hWnd, 
				D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
				&m_PresentParameters, &m_pDirect3DDevice)))
    {
		m_pLogger->Error("Unable to create 3D device.");
        return false;
    }


	// Create the SpriteInterface
	if(FAILED(D3DXCreateSprite(m_pDirect3DDevice, &m_pSpriteInterface)))
	{
		m_pLogger->Error("Unable to Sprite Interface.");
		return false;
	}

	if (!InitialiseDeviceStates())
	{
		return false;
	}
	m_pLogger->Info("InitialiseDirect3D() done");
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CGame::InitialiseLights()
///
/// \brief	Initialises the lights. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CGame::InitialiseLights()
{
	m_pLogger->Info("InitialiseLights() starting");

	D3DLIGHT9 d3dLight;

	//Initialize the light structure.
	ZeroMemory(&d3dLight, sizeof(D3DLIGHT9));

	d3dLight.Type = D3DLIGHT_POINT;

	d3dLight.Position.x = 0.0f;
	d3dLight.Position.y = 10.0f;
	d3dLight.Position.z = 0.0f;

	d3dLight.Attenuation0 = 1.0f; 
	d3dLight.Attenuation1 = 0.0f; 
	d3dLight.Attenuation2 = 0.0f; 
	d3dLight.Range = 1000.0f;	

	d3dLight.Diffuse.r = 1.0f;
	d3dLight.Diffuse.g = 1.0f;
	d3dLight.Diffuse.b = 1.0f;

	d3dLight.Ambient.r = 0.0f;
	d3dLight.Ambient.g = 0.0f;
	d3dLight.Ambient.b = 0.0f;

	d3dLight.Specular.r = 0.0f;
	d3dLight.Specular.g	= 0.0f;
	d3dLight.Specular.b	= 0.0f;

	//Assign the point light to our device in poisition (index) 0
	if(FAILED(m_pDirect3DDevice->SetLight(0, &d3dLight)))
	{
		m_pLogger->Error("SetLight Failed");
		return false;
	}

	//Enable our point light in position (index) 0
	if(FAILED(m_pDirect3DDevice->LightEnable(0, TRUE)))
	{
		m_pLogger->Error("LightEnable Failed");
		return false;
	}

	//Turn on lighting
	if(FAILED(m_pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
	{
		m_pLogger->Error("SetRenderState: D3DRS_LIGHTING Failed");
		return false;
	}

	//Set ambient light level
	if(FAILED(m_pDirect3DDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(60, 60, 60))))	
	{
		m_pLogger->Error("SetRenderState: D3DRS_AMBIENT Failed");
		return false;
	}

	m_pLogger->Info("InitialiseLights() done");
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CGame::InitialiseDeviceStates()
///
/// \brief	Initialises the device states. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CGame::InitialiseDeviceStates()
{
	m_pLogger->Info("InitialiseDeviceStates() starting");

	//Set fill state. Possible values: D3DFILL_POINT, D3DFILL_WIREFRAME, D3DFILL_SOLID
	if(FAILED(m_pDirect3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID)))
	{
		m_pLogger->Error("SetRenderState: D3DRS_FILLMODE Failed");
		return false;
	}

	if(FAILED(m_pDirect3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE)))
	{
		m_pLogger->Error("SetRenderState: D3DRS_NORMALIZENORMALS Failed");
		return false;
	}

	if(FAILED(m_pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,  TRUE)))
	{
		m_pLogger->Error("SetRenderState: D3DRS_ALPHABLENDENABLE Failed");
		return false;
	}

	if(FAILED(m_pDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA)))
	{
		m_pLogger->Error("SetRenderState: D3DRS_SRCBLEND Failed");
		return false;
	}
	
	if(FAILED(m_pDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA)))
	{
		m_pLogger->Error("SetRenderState: D3DRS_DESTBLEND Failed");
		return false;
	}

	if(FAILED(m_pDirect3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE)))
	{
		m_pLogger->Error("SetTextureStageState: D3DTSS_ALPHAOP Failed");
		return false;
	}

	m_pLogger->Info("InitialiseDeviceStates() done");
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGame::ExecuteGame()
///
/// \brief	Executes the Game Actions and calls the Render Method. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGame::ExecuteGame()
{
	ExecuteGameEx();

	// time to Render the next Frame?
	if (m_pCounter->RenderNextFrame())
	{
		m_pCounter->CalculateElapsedTime();
		Render();
		m_pCounter->CalculateNextFrame();
	}
	else
	{
		Sleep(1);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGame::Render()
///
/// \brief	The core Render Methods (calculates the FPS).
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGame::Render()
{
	if(m_pDirect3DDevice == NULL)
		return;

	if(!m_iQuit)
	{
		RenderEx();
		//Count Frames
		++m_dwFrames;

		//Count Frames per Second
		DWORD dwDuration = (timeGetTime() - m_dwLastFramesPerSecondTime);
		if (dwDuration > 1000)
		{
			m_dwFramesPerSecond = m_dwFramesPerSecondNew;
			m_dwFramesPerSecondNew = 0;
			m_dwLastFramesPerSecondTime = timeGetTime();

			m_iDrawCount = m_iDrawCountNew;
			m_iDrawCountNew = 0;
		}			
		else
		{
			++m_dwFramesPerSecondNew;
		}
	}
}

bool CGame::StartRenderToTexture(LPDIRECT3DTEXTURE9 pRenderTexture)
{
	pRenderTexture->GetSurfaceLevel(0, &m_pRenderTarget);

	m_pDirect3DDevice->GetRenderTarget(0, &m_pBackBuffer);
	m_pDirect3DDevice->SetRenderTarget(0, m_pRenderTarget);

	if(!SUCCEEDED(m_pDirect3DDevice->BeginScene()))
		return false;

	m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	if (m_pSpriteInterface != NULL)
		m_pSpriteInterface->Begin(D3DXSPRITE_ALPHABLEND);

	m_pCamera->SetupGUI2DCamera();

	return true;
}

bool CGame::EndRenderToTexture()
{
	if (m_pSpriteInterface != NULL)
		m_pSpriteInterface->End();

	m_pDirect3DDevice->EndScene();
	m_pDirect3DDevice->SetRenderTarget(0, m_pBackBuffer);

	SafeRelease(m_pRenderTarget);
	SafeRelease(m_pBackBuffer);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGame::ExecuteGameEx()
///
/// \brief	Executes the game ex operation. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGame::ExecuteGameEx()
{
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CGame::InitialiseEx()
///
/// \brief	Initialises the ex. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CGame::InitialiseEx()
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGame::IncreaseDrawCount()
///
/// \brief	Increase draw count. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGame::IncreaseDrawCount()
{
	++m_iDrawCountNew;
}