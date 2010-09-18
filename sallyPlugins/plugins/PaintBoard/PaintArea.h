////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	PaintBoard\PaintArea.h
///
/// \brief	Declares the paint area class. 
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

#pragma once
#include "Define.h"
#include <sallyAPI\sallyAPI.h>
#include <GdiPlus.h>

enum DRAW_TOOLS {DRAW_BRUSH, DRAW_ERASER};

class CPaintArea :
	public SallyAPI::GUI::CForm
{
private:
	DRAW_TOOLS							m_eDrawTool;
	Gdiplus::Color						m_cColor;
	SallyAPI::GUI::CAppBase*			m_pAppBase;
	SallyAPI::GUI::CPicture*			m_pPicture;
	SallyAPI::GUI::CImageBox*			m_pImageBox;
	int									m_iMouseDownXOld;
	int									m_iMouseDownYOld;
	std::string							m_strPaintFile;
	IDirect3DSurface9*					m_pSurface;

	void	ReleaseHDC(HDC hdc);
	HDC		GetHDC();
	void	LoadImage(const std::string& fileName);
	int		dc2bitmap(HDC hdc, int width, int height, const char* filename);

	virtual bool	ProcessMouseMove(int x, int y);
	virtual bool	ProcessMouseDown(int x, int y);
	virtual bool	ProcessMouseUp(int x, int y);
public:
	CPaintArea(SallyAPI::GUI::CAppBase* parent, int x, int y, int width, int height);
	virtual ~CPaintArea();

	void	ClearPaintArea();
	void	SetDrawTool(DRAW_TOOLS drawTool);

	void	SaveConfig();
};
