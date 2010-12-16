////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Game.h
///
/// \brief	Declares the game class. 
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

#pragma once
#include "Define.h"
#include <d3dx9.h>
#include <dinput.h>
#include <vector>

namespace SallyAPI
{
	namespace Config
	{
		class CTheme;
		class CLanguageManager;
	}
}

namespace SallyAPI
{
	namespace Core
	{
		class CCamera;
	}
}

#include "Texture.h"
#include "TextureManager.h"
#include "Font.h"
#include "FontManager.h"
#include "Camera.h"
#include "Counter.h"
#include "Speech.h"
#include "GUIBaseObject.h"
#include "Config.h"
#include "StringHelper.h"
#include "PathHelper.h"
#include "Logger.h"
#include "Option.h"

namespace SallyAPI
{
	namespace Core
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \enum	DEVICE_STATE
		///
		/// \brief	Values that represent DEVICE_STATE. 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum DEVICE_STATE {DEVICE_STATE_OK, DEVICE_STATE_LOST, DEVICE_STATE_RESTORED};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CGame
		///
		/// \brief	Game. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CGame
		{
		protected:
			int			m_iDeviceId;
			int			m_iVertexProcessing;
			bool		m_bWindowd;
			bool		m_bMultiMonitor;
			std::string	m_strMonitorId;
			int			m_iWindowPosX;
			int			m_iWindowPosY;
			int			m_iWindowWidth;
			int			m_iWindowHeight;
			float		m_fWidthCorrection;
			float		m_fHeightCorrection;
			int			m_iScreenColorDepth;
			DEVICE_STATE		m_eDeviceState;
			D3DMULTISAMPLE_TYPE	m_MultiSampleType;

			static int		m_iScreenWidth;
			static int		m_iScreenHeight;
		protected:
			// Initialise
			bool	InitialiseLights();
			bool	InitialiseDirect3D();
			bool	InitialiseDeviceStates();

			// Create Device
			bool	CheckDevice();
			void	CheckDisplayMode();
			bool	InitialisePresentationParameters();
			bool	InitialiseDisplayMode();

			bool	OnLostDevice();
			bool	OnRestoreDevice();

			// The Game
			void	Render();
			void	ExecuteGame();
		protected:
			static std::string					m_strMediaFolder;
			static SallyAPI::System::CLogger*	m_pLogger;
			static SallyAPI::Core::CCamera*		m_pCamera;

			static LPDIRECT3DDEVICE9	m_pDirect3DDevice;
			static LPD3DXSPRITE			m_pSpriteInterface;
			static LPDIRECT3D9			m_pDirect3D;
			static LPDIRECT3DSURFACE9	m_pBackBuffer;
			static LPDIRECT3DSURFACE9	m_pRenderTarget;

			static HWND					m_hWnd;
			static HINSTANCE			m_hInst;

			D3DDISPLAYMODE				m_DisplayMode;
			D3DPRESENT_PARAMETERS		m_PresentParameters;

			// Input Devices
			LPDIRECTINPUT8				m_pDirectInput; 
			LPDIRECTINPUTDEVICE8		m_pKeyboard;
			LPDIRECTINPUTDEVICE8		m_pMouse; 
			LPDIRECTINPUTDEVICE8		m_pGamepad;

			// Frame calculation
			DWORD		m_dwFrames;
			DWORD		m_dwFramesPerSecond;
			DWORD		m_dwFramesPerSecondNew;
			DWORD		m_dwLastFramesPerSecondTime;

			// draw counter
			static int	m_iDrawCount;
			static int	m_iDrawCountNew;

			// Misc
			int			m_iQuit;

			SallyAPI::Core::CCounter	m_counter;
		protected:
			// Virtuelle Functionen
			virtual bool InitialiseEx();
			virtual void ExecuteGameEx();

			virtual void OnCommandDeviceRestoreStart() = 0;
			virtual void OnCommandDeviceRestoreEnd() = 0;
			virtual void OnCommandDeviceLost() = 0;
			virtual void TimerEvent() = 0;
			virtual void RenderEx() = 0;
		public:
			CGame();
			virtual ~CGame();

			static LPDIRECT3DDEVICE9			GetDevice();
			static LPDIRECT3D9					GetD3D();

			static SallyAPI::Core::CCamera*		GetCamera();
			static LPD3DXSPRITE					GetSpriteInterface();
			static SallyAPI::System::CLogger*	GetLogger();
			static HWND							GetHWnd();
			static HINSTANCE					GetHInstance();
			static int							GetScreenWidth();
			static int							GetScreenHeight();
			static std::string					GetMediaFolder();
			static void							SetMediaFolder(const std::string& mediaFolder);
			static void							IncreaseDrawCount();

			static bool StartRenderToTexture(LPDIRECT3DTEXTURE9 pRenderTexture);
			static bool EndRenderToTexture();

			void GameLoop();
			bool Initialise(HWND hWnd, HINSTANCE hInst);
		};
	}
}