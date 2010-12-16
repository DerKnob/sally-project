////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\SallyApp.h
///
/// \brief	Declares the sally application class. 
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

#pragma once
#include "Define.h"
#include "Player.h"
#include "WindowLoading.h"
#include "MainWindow.h"
#include "LoadingMainWindow.h"
#include "Console.h"
#include "Mesh.h"

struct TVertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 otherPos;
	D3DXVECTOR4 texOffset;
	D3DXVECTOR3 thickness;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL |
		D3DFVF_TEX2 | // D3DFVF_TEX2 specifies we have two sets of texture coordinates.
		D3DFVF_TEXCOORDSIZE4(0) | // This specifies that the first (0th) tex coord set has size 4 floats.
		D3DFVF_TEXCOORDSIZE3(1); // Specifies that second tex coord set has size 2 floats.
};

class CSallyApp :
	public SallyAPI::Core::CGame
{
protected:
	ULONG_PTR			m_gdiplusToken;

	CWindowLoading*		m_pGUILoading;
	CMainWindow*		m_pGUIMainWindow;
	CConsole*			m_pConsole;

	CWindow**					m_ppGUICurrent;
	CMesh*						m_pBackgroundMesh;
	LPD3DXEFFECT				m_pEffect;
	SallyAPI::Core::CTexture*	m_pEffectTexture;
	D3DXVECTOR3					m_aLines[2];
	FLOAT						m_fThickness;

	CLoadingMainWindowThread	m_tLoadingMainWindow;

	// Status Management
	virtual bool InitialiseEx();
	virtual void ExecuteGameEx();

	void LoadingDone();

	// Render
	virtual void OnCommandDeviceRestoreStart();
	virtual void OnCommandDeviceRestoreEnd();
	virtual void OnCommandDeviceLost();
	virtual void TimerEvent();
	virtual void RenderEx();
	void		RenderDebug();
	void		RenderGUI();
	void		Render3DBackground();
	void		Render3DForground();
	void		RenderLine(D3DXVECTOR3& v0, D3DXVECTOR3& v1);
public:
	CSallyApp();
	virtual ~CSallyApp();

	int		GetQuit();

	bool	KeyDown(int c);
	bool	CharInputPressed(char c);
	bool	ProcessMouseDoubleClick(int x, int y);
	bool	ProcessMouseUp(int x, int y);
	bool	ProcessMouseDown(int x, int y);
	bool	ProcessMouseMove(int x, int y);

	void	ProcessVoiceCommand();
	void	SendMessageToChilds(int messageId, SallyAPI::GUI::SendMessage::CParameterBase *messageParamter);
};
