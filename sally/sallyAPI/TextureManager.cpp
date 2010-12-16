////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\TextureManager.cpp
///
/// \brief	Implements the texture manager class. 
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

#include "TextureManager.h"

using namespace SallyAPI::Core;

SallyAPI::Core::CTextureManager*		CTextureManager::m_pObject = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CTextureManager::CTextureManager()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CTextureManager::CTextureManager()
{
	m_iCounter = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CTextureManager::~CTextureManager()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CTextureManager::~CTextureManager()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	std::vector<SallyAPI::Core::CTexture*>::iterator iter;

	iter = m_vTextures.begin();

	while (iter != m_vTextures.end())
	{
		SallyAPI::Core::CTexture* temp = *iter;
		SafeDelete(temp);

		m_vTextures.erase(iter);
		iter = m_vTextures.begin();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CTextureManager::GetNextId()
///
/// \brief	Gets the next identifier. 
///
/// \author	Christian Knobloch
/// \date	27.04.2010
///
/// \return	The next identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CTextureManager::GetNextId()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);
	++m_iCounter;

	return m_iCounter;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CTextureManager* CTextureManager::GetInstance()
///
/// \brief	Gets the instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the instance. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CTextureManager* CTextureManager::GetInstance()
{
	if (m_pObject == NULL) {
		m_pObject = new SallyAPI::Core::CTextureManager();
	}
	return m_pObject;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Core::CTexture* CTextureManager::LoadTexture(const std::string& fileName,
/// bool createMipMaps)
///
/// \brief	Loads a texture. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fileName		Filename of the file. 
/// \param	createMipMaps	true to create mip maps. 
///
/// \return	null if it fails, else the texture. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Core::CTexture* CTextureManager::LoadTexture(const std::string& fileName, bool createMipMaps)
{
	SallyAPI::Core::CTexture* temp;

	temp = new SallyAPI::Core::CTexture();

	int id = GetNextId();

	if (temp->LoadTexture(fileName, id, createMipMaps))
	{
		SallyAPI::System::CAutoLock lock(&m_Lock);
		m_vTextures.push_back(temp);
		return temp;
	}

	SafeDelete(temp);
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Core::CTexture* CTextureManager::CreateEmptyTexture(int iWidth, int iHeight,
/// D3DFORMAT d3dFormat)
///
/// \brief	Creates an emptey texture. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	iWidth		The width. 
/// \param	iHeight		The height. 
/// \param	d3dFormat	The 3d format. 
///
/// \return	null if it fails, else. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Core::CTexture* CTextureManager::CreateEmptyTexture(int iWidth, int iHeight, D3DFORMAT d3dFormat)
{
	SallyAPI::Core::CTexture* temp = new SallyAPI::Core::CTexture();

	int id = GetNextId();

	if (temp->CreateEmpty(iWidth, iHeight, m_iCounter, d3dFormat))
	{
		SallyAPI::System::CAutoLock lock(&m_Lock);
		m_vTextures.push_back(temp);

		return temp;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Core::CTexture* CTextureManager::CreateEmptyTextureAsRenderTarget(int iWidth,
/// int iHeight, D3DFORMAT d3dFormat)
///
/// \brief	Creates an empty texture as a render target. 
///
/// \author	Christian Knobloch
/// \date	27.04.2010
///
/// \param	iWidth		The width. 
/// \param	iHeight		The height. 
/// \param	d3dFormat	The 3d format. 
///
/// \return	null if it fails, else. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Core::CTexture* CTextureManager::CreateEmptyTextureAsRenderTarget(int iWidth, int iHeight, D3DFORMAT d3dFormat)
{
	SallyAPI::Core::CTexture* temp = new SallyAPI::Core::CTexture();

	int id = GetNextId();

	if (temp->CreateEmptyRenderTarget(iWidth, iHeight, m_iCounter, d3dFormat))
	{
		SallyAPI::System::CAutoLock lock(&m_Lock);

		m_vTextures.push_back(temp);
		m_mTexturesRenderTarget[id] = temp;

		return temp;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Core::CTexture* CTextureManager::GetTextureById(int id)
///
/// \brief	Gets a texture by identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	id	The identifier. 
///
/// \return	null if it fails, else the texture by identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Core::CTexture* CTextureManager::GetTextureById(int id)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	std::vector<SallyAPI::Core::CTexture*>::iterator iter;

	SallyAPI::Core::CTexture* temp;
	iter = m_vTextures.begin();

	while (iter != m_vTextures.end())
	{
		temp = *iter;

		if (temp->GetId() == id)
		{
			return temp;
		}
		++iter;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Core::CTexture* CTextureManager::GetTextureByName(const std::string& fileName)
///
/// \brief	Gets a texture by name. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fileName	Filename of the file. 
///
/// \return	null if it fails, else the texture by name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Core::CTexture* CTextureManager::GetTextureByName(const std::string& fileName)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	std::vector<SallyAPI::Core::CTexture*>::iterator iter;

	SallyAPI::Core::CTexture* temp;
	iter = m_vTextures.begin();

	while (iter != m_vTextures.end())
	{
		temp = *iter;

		if (temp->GetFileName().compare(fileName) == 0)
		{
			return temp;
		}
		++iter;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CTextureManager::RemoveTextureById(int id)
///
/// \brief	Removes the texture by identifier described by id. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	id	The identifier. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CTextureManager::RemoveTextureById(int id)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	std::vector<SallyAPI::Core::CTexture*>::iterator iter;

	SallyAPI::Core::CTexture* temp;
	iter = m_vTextures.begin();

	while (iter != m_vTextures.end())
	{
		temp = *iter;

		if (temp->GetId() == id)
		{
			SallyAPI::Core::CTexture* texture = *iter;
			m_mTexturesRenderTarget.erase(texture->GetId());

			m_vTextures.erase(iter);
			delete(texture);

			return true;
		}
		++iter;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CTextureManager::RemoveTextureByName(const std::string& fileName)
///
/// \brief	Removes the texture by name described by fileName. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fileName	Filename of the file. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CTextureManager::RemoveTextureByName(const std::string& fileName)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	std::vector<SallyAPI::Core::CTexture*>::iterator iter;

	SallyAPI::Core::CTexture* temp;
	iter = m_vTextures.begin();

	while (iter != m_vTextures.end())
	{
		temp = *iter;

		if (temp->GetFileName().compare(fileName) == 0)
		{
			SallyAPI::Core::CTexture* texture = *iter;
			m_mTexturesRenderTarget.erase(texture->GetId());

			m_vTextures.erase(iter);
			delete(texture);
			return true;
		}
		++iter;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CTextureManager::RemoveTextureByTexture(SallyAPI::Core::CTexture* texture)
///
/// \brief	Removes the texture by texture described by texture. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	texture	If non-null, the texture. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CTextureManager::RemoveTextureByTexture(SallyAPI::Core::CTexture* texture)
{
	if (texture == NULL)
		return false;

	SallyAPI::System::CAutoLock lock(&m_Lock);

	std::vector<SallyAPI::Core::CTexture*>::iterator iter;

	SallyAPI::Core::CTexture* temp;
	iter = m_vTextures.begin();

	while (iter != m_vTextures.end())
	{
		temp = *iter;

		if (temp == texture)
		{
			m_mTexturesRenderTarget.erase(temp->GetId());

			m_vTextures.erase(iter);
			delete(temp);

			return true;
		}
		++iter;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SallyAPI::Core::CTextureManager::DeleteInstance()
///
/// \brief	Deletes an instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void SallyAPI::Core::CTextureManager::DeleteInstance()
{
	SafeDelete(m_pObject);
}