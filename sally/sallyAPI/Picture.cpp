////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Picture.cpp
///
/// \brief	Implements the picture class. 
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

#include "Picture.h"

using namespace SallyAPI::GUI;

#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CPicture::CPicture()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CPicture::CPicture()
	: m_pTexture(NULL), m_pVertexBuffer(NULL), m_iAlphaBlending(255)
{
	CreateVertexBuffer();
	m_fAngleX = 0.0;
	m_fAngleY = 0.0;
	m_fAngleZ = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CPicture::~CPicture()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CPicture::~CPicture()
{
	CleanUp();
	SafeRelease(m_pVertexBuffer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPicture::CleanUp()
///
/// \brief	Clean up. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPicture::CleanUp()
{
	SallyAPI::Core::CTextureManager* textureManager = SallyAPI::Core::CTextureManager::GetInstance();
	textureManager->RemoveTextureByTexture(m_pTexture);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPicture::SetRotationAngleY(float angle)
///
/// \brief	Sets a rotation angle y coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	angle	The angle. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPicture::SetRotationAngleY(float angle)
{
	m_fAngleY = angle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPicture::SetRotationAngleX(float angle)
///
/// \brief	Sets a rotation angle x coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	angle	The angle. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPicture::SetRotationAngleX(float angle)
{
	m_fAngleX = angle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPicture::SetRotationAngleZ(float angle)
///
/// \brief	Sets a rotation angle z coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	angle	The angle. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPicture::SetRotationAngleZ(float angle)
{
	m_fAngleZ = angle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPicture::SetAlphaBlending(int alphaBlending)
///
/// \brief	Sets an alpha blending. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	alphaBlending	The alpha blending. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPicture::SetAlphaBlending(int alphaBlending)
{
	m_iAlphaBlending = alphaBlending;
	PANEL_CUSTOMVERTEX* pVertices = NULL;
	m_pVertexBuffer->Lock(0, 4 * sizeof(PANEL_CUSTOMVERTEX), (void**)&pVertices, 0);
	if (pVertices == NULL)
		return;

	DWORD m_dwColour = D3DCOLOR_ARGB(m_iAlphaBlending, 255, 255, 255);

	//Set all the vertices to selected colour
	pVertices[0].colour = m_dwColour;
	pVertices[1].colour = m_dwColour;
	pVertices[2].colour = m_dwColour;
	pVertices[3].colour = m_dwColour;

	m_pVertexBuffer->Unlock();
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPicture::SetTexture(SallyAPI::Core::CTexture* texture)
///
/// \brief	Sets a texture. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	texture	If non-null, the texture. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPicture::SetTexture(SallyAPI::Core::CTexture* texture)
{
	CleanUp();

	m_pTexture = texture;
	m_iWidth = m_pTexture->GetWidth();
	m_iHeight = m_pTexture->GetHeight();

	UpdateVertices();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Core::CTexture* CPicture::GetTexture()
///
/// \brief	Gets the texture. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the texture. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Core::CTexture* CPicture::GetTexture()
{
	return m_pTexture;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPicture::UpdateDimensions()
///
/// \brief	Updates the width and hight information. 
///
/// \author	Christian Knobloch
/// \date	21.05.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPicture::UpdateDimensions()
{
	if (m_pTexture != NULL)
	{
		m_iWidth = m_pTexture->GetWidth();
		m_iHeight = m_pTexture->GetHeight();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CPicture::GetWidth()
///
/// \brief	Gets the width. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The width. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CPicture::GetWidth()
{
	return m_iWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CPicture::GetHeight()
///
/// \brief	Gets the height. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CPicture::GetHeight()
{
	return m_iHeight;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPicture::SetWidth(int width)
///
/// \brief	Sets a width. 
///
/// \author	Christian Knobloch
/// \date	21.05.2010
///
/// \param	width	The width. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPicture::SetWidth(int width)
{
	m_iWidth = width;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPicture::SetHeight(int height)
///
/// \brief	Sets a height. 
///
/// \author	Christian Knobloch
/// \date	21.05.2010
///
/// \param	height	The height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPicture::SetHeight(int height)
{
	m_iHeight = height;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPicture::CreateVertexBuffer()
///
/// \brief	Creates the vertex buffer. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPicture::CreateVertexBuffer()
{
	LPDIRECT3DDEVICE9 pD3DDevice = SallyAPI::Core::CGame::GetDevice();

	//Create the vertex buffer from our device.
	pD3DDevice->CreateVertexBuffer(4 * sizeof(PANEL_CUSTOMVERTEX),
		0, PANEL_D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPicture::UpdateVertices()
///
/// \brief	Updates the vertices. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPicture::UpdateVertices()
{
	PANEL_CUSTOMVERTEX* pVertices = NULL;
	m_pVertexBuffer->Lock(0, 4 * sizeof(PANEL_CUSTOMVERTEX), (void**)&pVertices, 0);
	if (pVertices == NULL)
		return;

	DWORD m_dwColour = D3DCOLOR_ARGB(m_iAlphaBlending, 255, 255, 255);

	//Set all the vertices to selected colour
	pVertices[0].colour = m_dwColour;
	pVertices[1].colour = m_dwColour;
	pVertices[2].colour = m_dwColour;
	pVertices[3].colour = m_dwColour;
	
	if ((m_fAngleY == 0) && (m_fAngleX == 0) && (m_fAngleZ == 0))
	{
		// render2D
		//Set the positions of the vertices
		pVertices[0].x = 0;
		pVertices[0].y = (float) m_iHeight;

		pVertices[1].x = 0;
		pVertices[1].y = 0;

		pVertices[2].x = (float) m_iWidth;
		pVertices[2].y = (float) m_iHeight;

		pVertices[3].x = (float) m_iWidth;
		pVertices[3].y = 0;

		pVertices[0].z = 1.0f;
		pVertices[1].z = 1.0f;
		pVertices[2].z = 1.0f; 
		pVertices[3].z = 1.0f;


		//Set the texture coordinates of the vertices
		pVertices[0].u = 0.0f;
		pVertices[0].v = 1.0f;

		pVertices[1].u = 0.0f;
		pVertices[1].v = 0.0f;

		pVertices[2].u = 1.0f;
		pVertices[2].v = 1.0f;

		pVertices[3].u = 1.0f;
		pVertices[3].v = 0.0f;

		m_pVertexBuffer->Unlock();
	}
	else
	{
		// render3D
		//Set the positions of the vertices
		pVertices[0].x = 0;
		pVertices[0].y = (float) m_iHeight;

		pVertices[1].x = 0;
		pVertices[1].y = 0;

		pVertices[2].x = (float) m_iWidth;
		pVertices[2].y = (float) m_iHeight;

		pVertices[3].x = (float) m_iWidth;
		pVertices[3].y = 0;

		pVertices[0].z = 1.0f;
		pVertices[1].z = 1.0f;
		pVertices[2].z = 1.0f; 
		pVertices[3].z = 1.0f;


		//Set the texture coordinates of the vertices
		pVertices[0].u = 0.0f;
		pVertices[0].v = 0.0f;

		pVertices[1].u = 0.0f;
		pVertices[1].v = 1.0f;

		pVertices[2].u = 1.0f;
		pVertices[2].v = 0.0f;

		pVertices[3].u = 1.0f;
		pVertices[3].v = 1.0f;

		m_pVertexBuffer->Unlock();
	}
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPicture::MoveTo(int x, int y)
///
/// \brief	Move to. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPicture::MoveTo(int x, int y)
{
	LPDIRECT3DDEVICE9 pD3DDevice = SallyAPI::Core::CGame::GetDevice();

	if ((m_fAngleY == 0) && (m_fAngleX == 0) && (m_fAngleZ == 0))
	{
		// render2D
		//x and y specify the top left corner of the panel in screen coordinates
		D3DXMATRIX matMove;
		D3DXMatrixTranslation(&matMove, (float)x, (float)y, 0.0f);

		pD3DDevice->SetTransform(D3DTS_WORLD, &matMove);
	}
	else
	{
		Rotate(x,y);
	}
// 	else
// 	{
// 		D3DXMATRIX matMove;
// 		float xCod = (float)x-(WINDOW_WIDTH/2) - 1;
// 		float yCod = (float)-y+(WINDOW_HEIGHT/2 - m_iHeight) + 1;
// 		D3DXMatrixTranslation(&matMove, xCod, yCod, 0.0f);
// 		pD3DDevice->SetTransform(D3DTS_WORLD, &matMove);
// 	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPicture::Rotate(int x, int y)
///
/// \brief	Rotates. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPicture::Rotate(int x, int y)
{
	LPDIRECT3DDEVICE9 pD3DDevice = SallyAPI::Core::CGame::GetDevice();

	D3DXMATRIX matRotationUserX, matRotationUserY, matRotationUserZ, matGlobalRotation;
	D3DXMATRIX matMoveRight20, matMoveLeft20, matMoveUp20, matMoveDown20;
	D3DXMATRIX matTransformation1, matTransformation2, matTransformation3, matTransformation4;

	D3DXMATRIX matMove, matRotate, matShip, matMove2;

	D3DXMatrixRotationAxis(&matRotationUserX, &D3DXVECTOR3(20.0f, 0.0f, 0.0f), m_fAngleX);
	D3DXMatrixRotationAxis(&matRotationUserY, &D3DXVECTOR3(0.0f, 20.0f, 0.0f), m_fAngleY);
	D3DXMatrixRotationAxis(&matRotationUserZ, &D3DXVECTOR3(0.0f, 0.0f, 20.0f), m_fAngleZ);
	D3DXMatrixTranslation(&matMove,-(((float) m_iWidth) / 2), -(((float) m_iHeight) / 2) , 0.0f);
	D3DXMatrixMultiply(&matGlobalRotation,&matMove,&(matRotationUserX * matRotationUserY *matRotationUserZ));
	D3DXMatrixTranslation(&matMove2, (float)x-(WINDOW_WIDTH/2)+(((float) m_iWidth) / 2), (float)-y+(WINDOW_HEIGHT/2 - m_iHeight)+(((float) m_iHeight) / 2), 0.0f);
	D3DXMatrixMultiply(&matTransformation2,&matGlobalRotation,&matMove2);
	pD3DDevice->SetTransform(D3DTS_WORLD,&matTransformation2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CPicture::LoadTexture(const std::string& fileName, bool createMipMaps)
///
/// \brief	Loads a texture. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fileName		Filename of the file. 
/// \param	createMipMaps	true to create mip maps. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CPicture::LoadTexture(const std::string& fileName, bool createMipMaps)
{
	CleanUp();

	SallyAPI::Core::CTextureManager* textureManager = SallyAPI::Core::CTextureManager::GetInstance();

	m_pTexture = textureManager->LoadTexture(fileName, createMipMaps);

	if (m_pTexture != NULL)
	{
		m_iHeight = m_pTexture->GetHeight();
		m_iWidth = m_pTexture->GetWidth();

		UpdateVertices();
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CPicture::LoadTextureOverGDI(const std::string& fileName, int maxWidth, int maxHeight)
///
/// \brief	Loads a texture over gdi. 
///
/// \author	Christian Knobloch
/// \date	05.05.2010
///
/// \param	fileName	Filename of the file. 
/// \param	maxWidth	The max width. If -1 than use source width.
/// \param	maxHeight	The max height. If -1 than use source height. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CPicture::LoadTextureOverGDI(const std::string& fileName, int maxWidth, int maxHeight)
{
	CleanUp();

	int						filenameSize;
	LPWSTR					filenameWSTR;
	Gdiplus::Bitmap*		pThumbnail = NULL;
	Gdiplus::PixelFormat	pixelFormat;

	// File
	filenameSize = MultiByteToWideChar(CP_THREAD_ACP, MB_PRECOMPOSED, fileName.c_str(), -1, 0, 0);
	filenameWSTR = (LPWSTR) malloc(filenameSize * sizeof(LPWSTR));

	MultiByteToWideChar(CP_THREAD_ACP, MB_PRECOMPOSED, fileName.c_str(), -1, filenameWSTR, filenameSize);

	// load with gdi++
	Gdiplus::Bitmap imageBitmap(filenameWSTR);
	free (filenameWSTR);

	if ((maxWidth != -1) && (maxHeight != -1))
	{
		if (maxWidth == -1)
			maxWidth = imageBitmap.GetWidth();
		if (maxHeight == -1)
			maxHeight = imageBitmap.GetHeight();

		float iWidthRelation = (float) imageBitmap.GetWidth() / maxWidth;
		float iHeightRelation = (float) imageBitmap.GetHeight() / maxHeight;
		if (imageBitmap.GetWidth() > imageBitmap.GetHeight())
		{
			maxHeight = (long) (imageBitmap.GetHeight() / iWidthRelation);
		}
		else
		{
			maxWidth = (long) (imageBitmap.GetWidth() / iHeightRelation);
		}

		if ((maxWidth <= 128) && (maxHeight <= 128))
		{
			pThumbnail = (Gdiplus::Bitmap*) imageBitmap.GetThumbnailImage(maxWidth, maxHeight, NULL, NULL);
			pixelFormat = pThumbnail->GetPixelFormat();
		}
		else
		{
			pixelFormat = imageBitmap.GetPixelFormat();
		}
	}
	else
	{
		maxWidth = imageBitmap.GetWidth();
		maxHeight = imageBitmap.GetHeight();
		pixelFormat = imageBitmap.GetPixelFormat();
	}

	if (!this->CreateEmptyPixelFormat(maxWidth, maxHeight, pixelFormat))
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error("CPicture::LoadTextureOverGDI() CreateEmptyPixelFormat failed");
		SafeDelete(pThumbnail);
		return false;
	}

	IDirect3DSurface9* surface = NULL;
	SallyAPI::Core::CTexture* texture = this->GetTexture();
	if (texture == NULL)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error("CPicture::LoadTextureOverGDI() CTexture->GetTexture failed");
		SafeDelete(pThumbnail);
		return false;
	}

	LPDIRECT3DTEXTURE9 d3dTexture = texture->GetTexture();
	if (d3dTexture == NULL)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error("CPicture::LoadTextureOverGDI() LPDIRECT3DTEXTURE9->GetTexture failed");
		SafeDelete(pThumbnail);
		return false;
	}

	D3DSURFACE_DESC pDesc;
	d3dTexture->GetLevelDesc(0, &pDesc);
	d3dTexture->GetSurfaceLevel(0, &surface);

	HDC hdc;
	surface->GetDC(&hdc);

	if (pThumbnail == NULL)
	{
		Gdiplus::Graphics graphics(hdc);
		graphics.DrawImage(&imageBitmap, 0, 0, pDesc.Width, pDesc.Height);
	}
	else
	{
		Gdiplus::Graphics graphics(hdc);
		graphics.DrawImage(pThumbnail, 0, 0, pDesc.Width, pDesc.Height);
		/*
		Gdiplus::Graphics graphics(pThumbnail);
		HDC dcThumbnail = graphics.GetHDC();

		::BitBlt(hdc, 0, 0, pDesc.Width, pDesc.Height, dcThumbnail,
			0, 0, pThumbnail->GetWidth(), pThumbnail->GetHeight(), SRCCOPY);

		graphics.ReleaseHDC(dcThumbnail);
		*/
	}

	surface->ReleaseDC(hdc);
	surface->Release();

	SafeDelete(pThumbnail);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CPicture::CreateEmptyPixelFormat(int iWidth, int iHeight,
/// Gdiplus::PixelFormat pixelFormat)
///
/// \brief	Creates an empty pixel format. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	iWidth		The width. 
/// \param	iHeight		The height. 
/// \param	pixelFormat	The pixel format. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CPicture::CreateEmptyPixelFormat(int iWidth, int iHeight, Gdiplus::PixelFormat pixelFormat)
{
	D3DFORMAT d3dFormat;

	switch (pixelFormat)
	{
	case PixelFormat24bppRGB:
		d3dFormat = D3DFMT_R8G8B8;
		break;
	case PixelFormat32bppARGB:
		d3dFormat = D3DFMT_A8R8G8B8;
		break;
	case PixelFormat32bppRGB:
		d3dFormat = D3DFMT_R8G8B8;
		break;
	case PixelFormat32bppPARGB:
		d3dFormat = D3DFMT_A8R8G8B8;
		break;
	case PixelFormat48bppRGB:
		d3dFormat = D3DFMT_R8G8B8;
		break;
	case PixelFormat32bppCMYK:
		d3dFormat = D3DFMT_R8G8B8;
		break;
	default:
		d3dFormat = D3DFMT_R8G8B8;
		break;
	}

	return CreateEmptyD3DFormat(iWidth, iHeight, d3dFormat);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CPicture::CreateEmptyD3DFormat(int iWidth, int iHeight, D3DFORMAT d3dFormat)
///
/// \brief	Creates an empty d 3 d format. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	iWidth		The width. 
/// \param	iHeight		The height. 
/// \param	d3dFormat	The 3d format. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CPicture::CreateEmptyD3DFormat(int iWidth, int iHeight, D3DFORMAT d3dFormat)
{
	SallyAPI::Core::CTextureManager* textureManager = SallyAPI::Core::CTextureManager::GetInstance();

	m_pTexture = textureManager->CreateEmptyTexture(iWidth, iHeight, d3dFormat);

	if (m_pTexture != NULL)
	{
		m_iWidth = m_pTexture->GetWidth();
		m_iHeight = m_pTexture->GetHeight();

		UpdateVertices();
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CPicture::CreateEmptyAsRenderTargetD3DFormat(int iWidth, int iHeight,
/// D3DFORMAT d3dFormat)
///
/// \brief	Creates an empty as render target d 3 d format. 
///
/// \author	Christian Knobloch
/// \date	27.04.2010
///
/// \param	iWidth		The width. 
/// \param	iHeight		The height. 
/// \param	d3dFormat	The 3d format. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CPicture::CreateEmptyAsRenderTargetD3DFormat(int iWidth, int iHeight, D3DFORMAT d3dFormat)
{
	SallyAPI::Core::CTextureManager* textureManager = SallyAPI::Core::CTextureManager::GetInstance();

	m_pTexture = textureManager->CreateEmptyTextureAsRenderTarget(iWidth, iHeight, d3dFormat);

	if (m_pTexture != NULL)
	{
		m_iWidth = m_pTexture->GetWidth();
		m_iHeight = m_pTexture->GetHeight();

		UpdateVertices();
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPicture::Draw(int x, int y)
///
/// \brief	Draws. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

/*****************************************************************************************************/

void CPicture::Draw(int x, int y)
{
	LPDIRECT3DDEVICE9 pD3DDevice = SallyAPI::Core::CGame::GetDevice();
	SallyAPI::Core::CCamera* camera = SallyAPI::Core::CGame::GetCamera();

	if ((m_fAngleY == 0) && (m_fAngleX == 0) && (m_fAngleZ == 0))
		camera->SetupGUI2DCamera();
	else
		camera->SetupGUI3DCamera();

	MoveTo(x, y);

	pD3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(PANEL_CUSTOMVERTEX));
	pD3DDevice->SetFVF(PANEL_D3DFVF_CUSTOMVERTEX);

	if(m_pTexture != NULL)
	{
		pD3DDevice->SetTexture(0, m_pTexture->GetTexture());
	}
	else
	{
		pD3DDevice->SetTexture(0, NULL);
	}
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	SallyAPI::Core::CGame::IncreaseDrawCount();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPicture::Draw(int x, int y, int width, int height)
///
/// \brief	Draws. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x		The x coordinate. 
/// \param	y		The y coordinate. 
/// \param	width	The width. 
/// \param	height	The height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPicture::Draw(int x, int y, int width, int height)
{
	SallyAPI::Core::CCamera* camera = SallyAPI::Core::CGame::GetCamera();

	if ((m_fAngleY == 0) && (m_fAngleX == 0) && (m_fAngleZ == 0))
		camera->SetupGUI2DCamera();
	else
		camera->SetupGUI3DCamera();

	int iWidthTemp = m_iWidth;
	int iHeightTemp = m_iHeight;

	m_iWidth = width;
	m_iHeight = height;

	if ((iWidthTemp != width) || (iHeightTemp != height))
		UpdateVertices();

	LPDIRECT3DDEVICE9 pD3DDevice = SallyAPI::Core::CGame::GetDevice();

	MoveTo(x, y);

	pD3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(PANEL_CUSTOMVERTEX));
	pD3DDevice->SetFVF(PANEL_D3DFVF_CUSTOMVERTEX);

	if(m_pTexture != NULL)
	{
		pD3DDevice->SetTexture(0, m_pTexture->GetTexture());
	}
	else
	{
		pD3DDevice->SetTexture(0, NULL);
	}

	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	m_iWidth = iWidthTemp;
	m_iHeight = iHeightTemp;
	if ((iWidthTemp != width) || (iHeightTemp != height))
		UpdateVertices();

	SallyAPI::Core::CGame::IncreaseDrawCount();
}