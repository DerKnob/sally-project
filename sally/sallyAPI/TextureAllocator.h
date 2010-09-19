////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\TextureAllocator.h
///
/// \brief	Declares the texture allocator class. 
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
#include <dshow.h>
#include <atlbase.h>
#include <comdef.h>
#include <vector>
#include <D3d9.h>
#include <Vmr9.h>
#include <d3dx9tex.h>
#include "AutoLock.h"
#include "CritSection.h"
#include "Picture.h"
#include "Game.h"

#pragma comment( lib, "strmiids.lib" )
#pragma comment( lib, "Quartz.lib" )
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

namespace SallyAPI
{
	namespace Core
	{
		class DLL_API_SALLY CTextureAllocator : public IVMRSurfaceAllocator9, 
			IVMRImagePresenter9
		{
		public:
			CTextureAllocator(SallyAPI::GUI::CPicture *picture);
			virtual ~CTextureAllocator();

			// IVMRSurfaceAllocator9
			virtual HRESULT STDMETHODCALLTYPE InitializeDevice( 
				/* [in] */ DWORD_PTR dwUserID,
				/* [in] */ VMR9AllocationInfo *lpAllocInfo,
				/* [out][in] */ DWORD *lpNumBuffers);

			virtual HRESULT STDMETHODCALLTYPE TerminateDevice( 
				/* [in] */ DWORD_PTR dwID);

			virtual HRESULT STDMETHODCALLTYPE GetSurface( 
				/* [in] */ DWORD_PTR dwUserID,
				/* [in] */ DWORD SurfaceIndex,
				/* [in] */ DWORD SurfaceFlags,
				/* [out] */ IDirect3DSurface9 **lplpSurface);

			virtual HRESULT STDMETHODCALLTYPE AdviseNotify( 
				/* [in] */ IVMRSurfaceAllocatorNotify9 *lpIVMRSurfAllocNotify);

			// IVMRImagePresenter9
			virtual HRESULT STDMETHODCALLTYPE StartPresenting( 
				/* [in] */ DWORD_PTR dwUserID);

			virtual HRESULT STDMETHODCALLTYPE StopPresenting( 
				/* [in] */ DWORD_PTR dwUserID);

			virtual HRESULT STDMETHODCALLTYPE PresentImage( 
				/* [in] */ DWORD_PTR dwUserID,
				/* [in] */ VMR9PresentationInfo *lpPresInfo);

			// IUnknown
			virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
				REFIID riid,
				void** ppvObject);

			virtual ULONG STDMETHODCALLTYPE AddRef();
			virtual ULONG STDMETHODCALLTYPE Release();

			void LockRender();
			void UnlockRender();

			int GetVideoWidth();
			int GetVideoHeight();
		protected:
			HRESULT CreateDevice();

			// a helper function to erase every surface in the vector
			void DeleteSurfaces();

			bool NeedToHandleDisplayChange();

			// This function is here so we can catch the loss of surfaces.
			// All the functions are using the FAIL_RET macro so that they exit
			// with the last error code.  When this returns with the surface lost
			// error code we can restore the surfaces.
			HRESULT PresentHelper(VMR9PresentationInfo *lpPresInfo);

		private:
			// needed to make this a thread safe object
			SallyAPI::System::CCritSection	m_ObjectLock;
			long							m_refCount;

			CComPtr<IDirect3D9>                     m_pD3D;
			CComPtr<IDirect3DDevice9>               m_pD3DDev;
			CComPtr<IVMRSurfaceAllocatorNotify9>    m_lpIVMRSurfAllocNotify;
			std::vector<CComPtr<IDirect3DSurface9>>	m_pSurfaces;
			CComPtr<IDirect3DSurface9>              m_pRenderTarget;
			CComPtr<IDirect3DTexture9>              m_pPrivateTexture;

			SallyAPI::GUI::CPicture*	m_pPicture;
			int							m_iVideoX;
			int							m_iVideoY;
			int							m_iVideoWidth;
			int							m_iVideoHeight;
			int							m_iOrgVideoWidth;
			int							m_iOrgVideoHeight;	
		};
	}
}