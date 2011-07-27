////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Texture.cpp
///
/// \brief	Implements the texture class. 
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

#include "Texture.h"

using namespace SallyAPI::Core;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CTexture::CTexture()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CTexture::CTexture()
	:m_Texture(NULL), m_iId(0), m_iWidth(0), m_iHeight(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CleanUp()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CTexture::~CTexture()
{
	CleanUp();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CTexture::SetTexture(LPDIRECT3DTEXTURE9 m_Texture)
///
/// \brief	Sets a texture. 
///
/// \author	Christian Knobloch
/// \date	03.12.2010
///
/// \param	m_Texture	The texture. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CTexture::SetTexture(LPDIRECT3DTEXTURE9 m_Texture)
{
	this->m_Texture = m_Texture;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CTexture::LoadTexture(const std::string& fileName, int id, bool createMipMaps)
///
/// \brief	Loads a texture. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fileName		Filename of the file. 
/// \param	id				The identifier. 
/// \param	createMipMaps	true to create mip maps. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CTexture::LoadTexture(const std::string& fileName, int id, bool createMipMaps)
{
	CleanUp();

	D3DXIMAGE_INFO info;

	UINT	mipMaps = D3DX_FROM_FILE;
	DWORD	mipFilter = D3DX_FILTER_NONE;
	if (createMipMaps)
	{
		mipMaps = 5;
		mipFilter = D3DX_FILTER_LINEAR | D3DX_FILTER_MIRROR;
	}


	// Try to Load the Texture
	HRESULT hr = D3DXCreateTextureFromFileEx(SallyAPI::Core::CGame::GetDevice(), fileName.c_str(), D3DX_DEFAULT,
		D3DX_DEFAULT, mipMaps, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_FILTER_BOX | D3DX_FILTER_MIRROR,
		mipFilter, 0, &info, 0, &m_Texture);

	if(hr != D3D_OK)
	{
		CleanUp();

		return false;
	}
	m_strFileName = fileName;
	m_iWidth = info.Width;
	m_iHeight = info.Height;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CTexture::CreateEmpty(int iWidth, int iHeight, int id, D3DFORMAT d3dFormat)
///
/// \brief	Creates an empty. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	iWidth		The width. 
/// \param	iHeight		The height. 
/// \param	id			The identifier. 
/// \param	d3dFormat	The 3d format. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CTexture::CreateEmpty(int iWidth, int iHeight, int id, D3DFORMAT d3dFormat)
{
	CleanUp();

	// Try to Load the Texture
	if(D3DXCreateTexture(SallyAPI::Core::CGame::GetDevice(), iWidth, iHeight, 1,
		0, d3dFormat, D3DPOOL_MANAGED, &m_Texture) != D3D_OK)
	{
		CleanUp();

		return false;
	}
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iId = id;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CTexture::CreateEmptyRenderTarget(int iWidth, int iHeight, int id,
/// D3DFORMAT d3dFormat)
///
/// \brief	Creates an empty render target. 
///
/// \author	Christian Knobloch
/// \date	01.06.2010
///
/// \param	iWidth		The width. 
/// \param	iHeight		The height. 
/// \param	id			The identifier. 
/// \param	d3dFormat	The 3d format. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CTexture::CreateEmptyRenderTarget(int iWidth, int iHeight, int id, D3DFORMAT d3dFormat)
{
	CleanUp();

	// Try to Load the Texture
	if(D3DXCreateTexture(SallyAPI::Core::CGame::GetDevice(), iWidth, iHeight, 1,
		D3DUSAGE_RENDERTARGET, d3dFormat, D3DPOOL_DEFAULT, &m_Texture) != D3D_OK)
	{
		CleanUp();

		return false;
	}
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iId = id;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CTexture::CleanUp()
///
/// \brief	Clean up. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CTexture::CleanUp()
{
	SafeRelease(m_Texture);

	m_iWidth = 0;
	m_iHeight = 0;
	m_iId = 0;

	m_strFileName.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CTexture::GetFileName()
///
/// \brief	Gets the file name. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The file name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CTexture::GetFileName()
{
	return m_strFileName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LPDIRECT3DTEXTURE9 CTexture::GetTexture()
///
/// \brief	Gets the texture. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The texture. 
////////////////////////////////////////////////////////////////////////////////////////////////////

LPDIRECT3DTEXTURE9 CTexture::GetTexture()
{
	return m_Texture;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CTexture::GetWidth()
///
/// \brief	Gets the width. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The width. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CTexture::GetWidth()
{
	return m_iWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CTexture::GetHeight()
///
/// \brief	Gets the height. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CTexture::GetHeight()
{
	return m_iHeight;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CTexture::GetId()
///
/// \brief	Gets the identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CTexture::GetId()
{
	return m_iId;
}