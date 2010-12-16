////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	PaintBoard\PaintArea.cpp
///
/// \brief	Implements the paint area class. 
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

#include "PaintArea.h"

CPaintArea::CPaintArea(SallyAPI::GUI::CAppBase* parent, int x, int y, int width, int height)
	:SallyAPI::GUI::CForm(parent, x, y, width, height, 0), m_pPicture(NULL), m_iMouseDownXOld(-1), m_iMouseDownYOld(-1),
	m_pAppBase(parent), m_pSurface(NULL), m_eDrawTool(DRAW_BRUSH)
{
	m_strPaintFile = SallyAPI::System::SallyHelper::GetMediaDirectory(m_pAppBase);
	m_strPaintFile.append("paintFile01.bmp");

	LoadImage(m_strPaintFile);

	m_pImageBox = new SallyAPI::GUI::CImageBox(this, 0, 0, width, height);
	m_pImageBox->SetPicture(m_pPicture);
	this->AddChild(m_pImageBox);
}

CPaintArea::~CPaintArea()
{
	SafeDelete(m_pPicture);
}

void CPaintArea::SaveConfig()
{
	HDC hdc = GetHDC();
	dc2bitmap(hdc, m_iWidth, m_iHeight, m_strPaintFile.c_str());
	ReleaseHDC(hdc);
}

bool CPaintArea::ProcessMouseMove(int x, int y)
{
	if (m_bMouseDown)
	{
		HDC hdc = GetHDC();

		Gdiplus::Graphics graphics(hdc);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

		Gdiplus::Pen      pen(m_cColor);
		pen.SetWidth(m_pAppBase->GetPropertyInt("penSize"));
		graphics.DrawLine(&pen, m_iMouseDownXOld, m_iMouseDownYOld, x, y);

		ReleaseHDC(hdc);

		m_iMouseDownXOld = x;
		m_iMouseDownYOld = y;
	}

	return SallyAPI::GUI::CForm::ProcessMouseMove(x, y);
}

bool CPaintArea::ProcessMouseUp(int x, int y)
{
	m_iMouseDownXOld = -1;
	m_iMouseDownYOld = -1;

	return SallyAPI::GUI::CForm::ProcessMouseMove(x, y);
}

bool CPaintArea::ProcessMouseDown(int x, int y)
{
	m_iMouseDownXOld = x;
	m_iMouseDownYOld = y;

	if (m_eDrawTool == DRAW_BRUSH)
	{
		std::string color = m_pAppBase->GetPropertyString("color");

		if (color.compare("white") == 0)
		{
			m_cColor = Gdiplus::Color(255, 255, 255);
		}
		else if (color.compare("black") == 0)
		{
			m_cColor = Gdiplus::Color(0, 0, 0);
		}
		else if (color.compare("green") == 0)
		{
			m_cColor = Gdiplus::Color(0, 255, 0);
		}
		else if (color.compare("red") == 0)
		{
			m_cColor = Gdiplus::Color(255, 0, 0);
		}
		else if (color.compare("blue") == 0)
		{
			m_cColor = Gdiplus::Color(0, 0, 255);
		}
	}
	else if (m_eDrawTool == DRAW_ERASER)
	{
		m_cColor = Gdiplus::Color(255, 221, 0);
	}

	return SallyAPI::GUI::CForm::ProcessMouseDown(x, y);
}

void CPaintArea::LoadImage(const std::string& fileName)
{
	m_pPicture = new SallyAPI::GUI::CPicture();
	m_pPicture->CreateEmptyD3DFormat(m_iWidth, m_iHeight, D3DFMT_R8G8B8);

	ClearPaintArea();

	if (SallyAPI::File::FileHelper::FileExists(fileName))
	{
		int			filenameSize;
		LPWSTR		filenameWSTR;

		// File
		filenameSize = MultiByteToWideChar(CP_THREAD_ACP, MB_PRECOMPOSED, fileName.c_str(), -1, 0, 0);
		filenameWSTR = (LPWSTR) malloc(filenameSize * sizeof(LPWSTR));

		MultiByteToWideChar(CP_THREAD_ACP, MB_PRECOMPOSED, fileName.c_str(), -1, filenameWSTR, filenameSize);

		Gdiplus::Bitmap imageBitmap(filenameWSTR);
		free (filenameWSTR);

		int width = imageBitmap.GetWidth();
		int height = imageBitmap.GetHeight();

		HDC hdc = GetHDC();

		Gdiplus::Graphics graphics(hdc);
		graphics.DrawImage(&imageBitmap, 0, 0, width, height);
		ReleaseHDC(hdc);
	}
}

HDC CPaintArea::GetHDC()
{
	if (m_pSurface != NULL)
		return NULL;

	SallyAPI::Core::CTexture* texture = m_pPicture->GetTexture();
	LPDIRECT3DTEXTURE9 d3dTexture = texture->GetTexture();

	D3DSURFACE_DESC pDesc;
	d3dTexture->GetLevelDesc(0, &pDesc);
	d3dTexture->GetSurfaceLevel(0, &m_pSurface);

	HDC hdc;
	m_pSurface->GetDC(&hdc);

	return hdc;
}

void CPaintArea::ReleaseHDC(HDC hdc)
{
	m_pSurface->ReleaseDC(hdc);
	m_pSurface->Release();

	m_pSurface = NULL;
}

int CPaintArea::dc2bitmap(HDC hdc, int width, int height, const char* filename) 
{
	HDC hdc2; 
	HBITMAP aBmp; 
	BITMAPINFO bi; 
	HGDIOBJ OldObj; 
	void *dibvalues; 
	HANDLE fileHandle; 

	BITMAPFILEHEADER bmfh; 
	BITMAPINFOHEADER bmih; 
	DWORD bytes_write; 
	DWORD bytes_written; 

	hdc2=CreateCompatibleDC(hdc); 

	ZeroMemory(&bmih,sizeof(BITMAPINFOHEADER)); 
	bmih.biSize=sizeof(BITMAPINFOHEADER); 
	bmih.biHeight=height; 
	bmih.biWidth=width; 
	bmih.biPlanes=1; 
	bmih.biBitCount=24; 
	bmih.biCompression=BI_RGB; 
	bmih.biSizeImage = ((((bmih.biWidth * bmih.biBitCount) + 31) & ~31) >> 3) * bmih.biHeight; 
	bmih.biXPelsPerMeter = 0; 
	bmih.biYPelsPerMeter = 0; 
	bmih.biClrImportant = 0; 
	//bmih.biSizeImage=(3*bmih.biHeight*bmih.biWidth); 
	//bmih.biSizeImage = 0; 


	bi.bmiHeader=bmih; 

	aBmp=CreateDIBSection(hdc,&bi,DIB_RGB_COLORS,(void**)&dibvalues,NULL,NULL); 

	if (aBmp==NULL) 
	{ 
		OutputDebugString("CreateDIBSection failed!\n"); 
		return 0; 
	} 

	OldObj=SelectObject(hdc2,aBmp); 
	BitBlt(hdc2,0,0,width,height,hdc,0,0,SRCCOPY); 

	ZeroMemory(&bmfh,sizeof(BITMAPFILEHEADER)); 
	bmfh.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER); 
	bmfh.bfSize=(3*bmih.biHeight*bmih.biWidth)+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER); 
	bmfh.bfType=0x4d42; 
	bmfh.bfReserved1 = 0; 
	bmfh.bfReserved2 = 0; 

	fileHandle=CreateFile(filename,GENERIC_READ | GENERIC_WRITE,(DWORD)0,NULL, 
		CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL); 
	if (fileHandle==INVALID_HANDLE_VALUE) 
	{ 
		OutputDebugString("CreateFile failed!\n"); 
		return 0; 
	} 

	// Write the BITMAPFILEHEADER 
	bytes_write=sizeof(BITMAPFILEHEADER); 
	if (!WriteFile(fileHandle,(void*)&bmfh,bytes_write,&bytes_written,NULL)) 
	{ 
		OutputDebugString("WriteFile failed!\n"); 
		return 0; 
	} 


	//Write the BITMAPINFOHEADER 
	bytes_write=sizeof(BITMAPINFOHEADER); 
	if (!WriteFile(fileHandle,(void*)&bmih,bytes_write,&bytes_written,NULL)) 
	{ 
		OutputDebugString("WriteFile failed!\n"); 
		return 0; 
	} 

	//Write the Color Index Array??? 
	bytes_write=bmih.biSizeImage;//3*bmih.biHeight*bmih.biWidth; 
	if (!WriteFile(fileHandle,(void*)dibvalues,bytes_write,&bytes_written,NULL)) 
	{ 
		OutputDebugString("WriteFile failed!\n"); 
		return 0; 
	} 

	CloseHandle(fileHandle); 

	DeleteObject(SelectObject(hdc2,OldObj)); 
	DeleteDC(hdc2); 

	return 1; 
}

void CPaintArea::ClearPaintArea()
{
	HDC hdc = GetHDC();

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 221, 0));

	graphics.FillRectangle(&brush, 0, 0, m_iWidth, m_iHeight);

	ReleaseHDC(hdc);
}

void CPaintArea::SetDrawTool(DRAW_TOOLS drawTool)
{
	m_eDrawTool = drawTool;
	m_pAppBase->SetPropertyInt("drawTool", (int) drawTool);
}