////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\TextureAllocator.cpp
///
/// \brief	Implements the texture allocator class. 
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

#include "TextureAllocator.h"

#define FAIL_RET(x) do { if( FAILED( hr = ( x  ) ) ) return hr; } while(0)

using namespace SallyAPI::Core;

CTextureAllocator::CTextureAllocator(SallyAPI::GUI::CPicture* picture)
	: m_refCount(1), m_pPicture(picture), m_pRenderTarget(NULL), m_pPrivateTexture(NULL),
	m_iVideoWidth(0), m_iVideoHeight(0)
{
	SallyAPI::System::CAutoLock Lock(&m_ObjectLock);
	m_pD3D = SallyAPI::Core::CGame::GetD3D();
	m_pD3DDev = SallyAPI::Core::CGame::GetDevice();

	m_pPicture->GetTexture()->SetTexture(NULL);
}

CTextureAllocator::~CTextureAllocator()
{
    DeleteSurfaces();
}

HRESULT CTextureAllocator::CreateDevice()
{
	m_pRenderTarget = NULL;

    return m_pD3DDev->GetRenderTarget(0, &m_pRenderTarget.p);
}


void CTextureAllocator::DeleteSurfaces()
{
	SallyAPI::System::CAutoLock Lock(&m_ObjectLock);

	m_pPrivateTexture = NULL;

    for(size_t i = 0; i < m_pSurfaces.size(); ++i) 
    {
		m_pSurfaces[i] = NULL;
    }
}


//IVMRSurfaceAllocator9
HRESULT CTextureAllocator::InitializeDevice( 
            /* [in] */ DWORD_PTR dwUserID,
            /* [in] */ VMR9AllocationInfo *lpAllocInfo,
            /* [out][in] */ DWORD *lpNumBuffers)
{
    if( lpNumBuffers == NULL )
    {
        return E_POINTER;
    }
    if(m_lpIVMRSurfAllocNotify == NULL)
    {
        return E_FAIL;
    }

    HRESULT hr = S_OK;

	m_iVideoHeight = lpAllocInfo->dwHeight;
	m_iVideoWidth = lpAllocInfo->dwWidth;

    lpAllocInfo->dwFlags |= VMR9AllocFlag_TextureSurface;
	
    DeleteSurfaces();
    m_pSurfaces.resize(*lpNumBuffers);
	hr = m_lpIVMRSurfAllocNotify->AllocateSurfaceHelper(lpAllocInfo, lpNumBuffers, & m_pSurfaces.at(0) );

	if(FAILED(hr) && !(lpAllocInfo->dwFlags & VMR9AllocFlag_3DRenderTarget))
	{
		DeleteSurfaces();            

		// is surface YUV ?
		if (lpAllocInfo->Format > '0000') 
		{           
			D3DDISPLAYMODE dm; 
			FAIL_RET(m_pD3DDev->GetDisplayMode(NULL,  & dm ));

			FAIL_RET(m_pD3DDev->CreateTexture(lpAllocInfo->dwWidth, lpAllocInfo->dwHeight,
					1,  D3DUSAGE_RENDERTARGET,  dm.Format, 
					D3DPOOL_DEFAULT /* default pool - usually video memory */, 
					&m_pPrivateTexture.p, NULL));
		}

		lpAllocInfo->dwFlags &= ~VMR9AllocFlag_TextureSurface;
		lpAllocInfo->dwFlags |= VMR9AllocFlag_OffscreenSurface;

		FAIL_RET(m_lpIVMRSurfAllocNotify->AllocateSurfaceHelper(lpAllocInfo, lpNumBuffers, & m_pSurfaces.at(0)));
		m_pPicture->GetTexture()->SetTexture(m_pPrivateTexture);
	}
	return hr;
}
            
HRESULT CTextureAllocator::TerminateDevice( 
        /* [in] */ DWORD_PTR dwID)
{
    DeleteSurfaces();
    return S_OK;
}
    
HRESULT CTextureAllocator::GetSurface( 
        /* [in] */ DWORD_PTR dwUserID,
        /* [in] */ DWORD SurfaceIndex,
        /* [in] */ DWORD SurfaceFlags,
        /* [out] */ IDirect3DSurface9 **lplpSurface)
{
    if( lplpSurface == NULL )
    {
        return E_POINTER;
    }
	if (SurfaceIndex >= m_pSurfaces.size() ) 
	{
		return E_FAIL;
	}	

	SallyAPI::System::CAutoLock Lock(&m_ObjectLock);

	return m_pSurfaces[SurfaceIndex].CopyTo(lplpSurface);
}
    
HRESULT CTextureAllocator::AdviseNotify( 
        /* [in] */ IVMRSurfaceAllocatorNotify9 *lpIVMRSurfAllocNotify)
{
	SallyAPI::System::CAutoLock Lock(&m_ObjectLock);

	HRESULT hr;
 
    m_lpIVMRSurfAllocNotify = lpIVMRSurfAllocNotify;

 	HMONITOR hMonitor = m_pD3D->GetAdapterMonitor(D3DADAPTER_DEFAULT);
	FAIL_RET(m_lpIVMRSurfAllocNotify->SetD3DDevice(m_pD3DDev, hMonitor));

	return hr;
}

HRESULT CTextureAllocator::StartPresenting( 
    /* [in] */ DWORD_PTR dwUserID)
{
	SallyAPI::System::CAutoLock Lock(&m_ObjectLock);

    if( m_pD3DDev == NULL )
    {
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CTextureAllocator::StopPresenting( 
    /* [in] */ DWORD_PTR dwUserID)
{
	return S_OK;
}

HRESULT CTextureAllocator::PresentImage( 
    /* [in] */ DWORD_PTR dwUserID,
    /* [in] */ VMR9PresentationInfo *lpPresInfo)
{
	SallyAPI::System::CAutoLock Lock(&m_ObjectLock);

	HRESULT hr;

	// if we are in the middle of the display change
// 	if( NeedToHandleDisplayChange() )
// 	{
// 		// TODO
// 	}

	hr = PresentHelper(lpPresInfo);

	if( hr == D3DERR_DEVICELOST)
	{
// 		if (m_pD3DDev->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) 
// 		{
// 			DeleteSurfaces();
// 			FAIL_RET(CreateDevice());
// 
// 			HMONITOR hMonitor = m_pD3D->GetAdapterMonitor(D3DADAPTER_DEFAULT);
// 
// 			FAIL_RET(m_lpIVMRSurfAllocNotify->ChangeD3DDevice(m_pD3DDev, hMonitor));
// 
// 		}
// 
// 		hr = S_OK;
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error("CTextureAllocator->PresentImage failed");
		logger->Error(hr);
		hr = S_FALSE;
	}
	return hr;
}

HRESULT CTextureAllocator::PresentHelper(VMR9PresentationInfo *lpPresInfo)
{
	// parameter validation
	if( lpPresInfo == NULL )
	{
		return E_POINTER;
	}
	else if( lpPresInfo->lpSurf == NULL )
	{
		return E_POINTER;
	}

	SallyAPI::System::CAutoLock Lock(&m_ObjectLock);
	HRESULT hr;

	m_pD3DDev->SetRenderTarget(0, m_pRenderTarget);

	if(m_pPrivateTexture != NULL)
	{   
		CComPtr<IDirect3DSurface9> surface;
		FAIL_RET( m_pPrivateTexture->GetSurfaceLevel(0, &surface.p));

		// copy the full surface onto the texture's surface
		FAIL_RET( m_pD3DDev->StretchRect( lpPresInfo->lpSurf, NULL,
			surface, NULL,
			D3DTEXF_NONE ) );

		m_pPicture->GetTexture()->SetTexture(m_pPrivateTexture);
	}
	else
	{
		CComPtr<IDirect3DTexture9> texture;
		lpPresInfo->lpSurf->GetContainer( IID_IDirect3DTexture9, (LPVOID*) & texture.p ) ;  

		m_pPicture->GetTexture()->SetTexture(texture);
	}
	return NULL;
}

bool CTextureAllocator::NeedToHandleDisplayChange()
{
    if(!m_lpIVMRSurfAllocNotify)
    {
        return false;
    }

    D3DDEVICE_CREATION_PARAMETERS Parameters;
    if(FAILED(m_pD3DDev->GetCreationParameters(&Parameters)))
    {
        return false;
    }

    HMONITOR currentMonitor = m_pD3D->GetAdapterMonitor(Parameters.AdapterOrdinal);
    HMONITOR hMonitor = m_pD3D->GetAdapterMonitor(D3DADAPTER_DEFAULT);

    return hMonitor != currentMonitor;
}


// IUnknown
HRESULT CTextureAllocator::QueryInterface( 
        REFIID riid,
        void** ppvObject)
{
    HRESULT hr = E_NOINTERFACE;

    if( ppvObject == NULL ) {
        hr = E_POINTER;
    } 
    else if( riid == IID_IVMRSurfaceAllocator9 ) {
        *ppvObject = dynamic_cast<IVMRSurfaceAllocator9*>( this );
        AddRef();
        hr = S_OK;
    } 
    else if( riid == IID_IVMRImagePresenter9 ) {
        *ppvObject = dynamic_cast<IVMRImagePresenter9*>( this );
        AddRef();
        hr = S_OK;
    } 
    else if( riid == IID_IUnknown ) {
        *ppvObject = 
            dynamic_cast<IUnknown*>( 
            dynamic_cast<IVMRSurfaceAllocator9*>( this ) );
        AddRef();
        hr = S_OK;    
    }

    return hr;
}

ULONG CTextureAllocator::AddRef()
{
    return InterlockedIncrement(& m_refCount);
}

ULONG CTextureAllocator::Release()
{
    ULONG ret = InterlockedDecrement(& m_refCount);
    if( ret == 0 )
    {
        delete this;
    }

    return ret;
}

int CTextureAllocator::GetVideoWidth()
{
	return m_iVideoWidth;
}

int CTextureAllocator::GetVideoHeight()
{
	return m_iVideoHeight;
}

void CTextureAllocator::LockRender()
{
	m_ObjectLock.Lock();
}

void CTextureAllocator::UnlockRender()
{
	m_ObjectLock.Unlock();
}