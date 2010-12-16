////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\Mesh.cpp
///
/// \brief	Implements the mesh class. 
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

#include "Mesh.h"

CMesh::CMesh(const std::string& filename)
{
	LPD3DXBUFFER pMaterialsBuffer = NULL;
	LPD3DXMESH pMesh = NULL;	
	
	LPDIRECT3DDEVICE9 direct3DDevice = SallyAPI::Core::CGame::GetDevice();

	if(FAILED(D3DXLoadMeshFromX(filename.c_str(), D3DXMESH_SYSTEMMEM, direct3DDevice, NULL, 
                                &pMaterialsBuffer, 0, &m_dwNumMaterials, &pMesh)))
	{
		m_pMesh = NULL;
		m_pMeshMaterials = NULL;
		m_pMeshTextures = NULL;

		return;
	}

    D3DXMATERIAL* matMaterials = (D3DXMATERIAL*)pMaterialsBuffer->GetBufferPointer();
    
	//Create two arrays. One to hold the materials and only to hold the textures
	m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
    m_pMeshTextures  = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

    for(DWORD i = 0; i < m_dwNumMaterials; i++)
    {
        // Copy the material
        m_pMeshMaterials[i] = matMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
		m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;

		// Create the texture
        if(FAILED(D3DXCreateTextureFromFile(direct3DDevice, 
                                            matMaterials[i].pTextureFilename, 
                                            &m_pMeshTextures[i])))
        {
            m_pMeshTextures[i] = NULL;
        }
    }

    //We've finished with the material buffer, so release it
    SafeRelease(pMaterialsBuffer);

	
	//Make sure that the normals are setup for our mesh
	pMesh->CloneMeshFVF(D3DXMESH_MANAGED, MESH_D3DFVF_CUSTOMVERTEX, direct3DDevice, &m_pMesh);
	SafeRelease(pMesh);

	D3DXComputeNormals(m_pMesh, NULL);
}

CMesh::~CMesh()
{
	SafeDelete(m_pMeshMaterials);

	if(m_pMeshTextures != NULL)
	{
		for(DWORD i = 0; i < m_dwNumMaterials; i++)
		{
			if(m_pMeshTextures[i])
			{
				SafeRelease(m_pMeshTextures[i]);
			}
		}
	}

	SafeDelete(m_pMeshTextures);
	SafeRelease(m_pMesh);
}

void CMesh::Render()
{
	if(m_pMesh != NULL)
	{
		LPDIRECT3DDEVICE9 direct3DDevice = SallyAPI::Core::CGame::GetDevice();
		
		for(DWORD i = 0; i < m_dwNumMaterials; i++)
		{
			direct3DDevice->SetMaterial(&m_pMeshMaterials[i]);
			direct3DDevice->SetTexture(0, m_pMeshTextures[i]);
        
			m_pMesh->DrawSubset(i);
		}
	}
}
