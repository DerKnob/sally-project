////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\GUIBaseObject.h
///
/// \brief	Declares the graphical user interface base object class. 
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
#include <string>
#include "Config.h"
#include "BaseObject.h"
#include "ParameterBase.h"
#include "ParameterInteger.h"
#include "ParameterPoint.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CGUIBaseObject
		///
		/// \brief	Cgui base object. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CGUIBaseObject
			: public SallyAPI::Core::CBaseObject
		{
		protected:
			CRITICAL_SECTION	m_critSectLockRender;
			std::string			m_strText;
			float				m_fTimeDelta;

			SallyAPI::GUI::CGUIBaseObject*	m_pParent;
		
			bool			m_bUpdateRequested;
			int				m_iX;
			int				m_iY;
			int				m_iXAbsolut;
			int				m_iYAbsolut;
			int				m_iWidth;
			int				m_iHeight;
			int				m_iAlphaBlending;
			float           m_fAngleY;
			float           m_fAngleX;
			float           m_fAngleZ;

			void DrawImage(int gui, int x, int y);
			void DrawImagePart(int gui, int x, int y, int srcX, int srcY, int srcW, int srcH);
			void DrawImage(int gui, int x, int y, int width, int height);
			void EnterRenderLock();
			void LeaveRenderLock();

			virtual void	RenderControl() = 0;
			virtual void	UpdateControl() = 0;

			SallyAPI::GUI::CPicture*	GetPicture(int gui);
			RECT						GetImageRect(int gui);
		public:
			CGUIBaseObject(CGUIBaseObject* parent);
			virtual ~CGUIBaseObject();

			int		GetWidth();
			int		GetHeight();
			int		GetPositionX();
			int		GetPositionY();
			SallyAPI::GUI::CGUIBaseObject*	GetParent();

			virtual void		SetAlphaBlending(int alphaBlending);
			virtual int			GetAlphaBlending();

			virtual void		SetRotationAngleY(float angle);
			virtual float		GetRotationAngleY();

			virtual void		SetRotationAngleX(float angle);
			virtual float		GetRotationAngleX();

			virtual void		SetRotationAngleZ(float angle);
			virtual float		GetRotationAngleZ();

			virtual void		SetText(const std::string& text);
			virtual std::string	GetText();

			virtual void		GetAbsolutPosition(int* x, int* y);

			virtual void		SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int iReporterID, int iMessageID, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL) = 0;
			virtual void		SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int iReporterID, int iMessageID, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL) = 0;
			virtual void		Timer(float timeDelta);
			
			virtual void		Render();
			virtual void		Update();
		};
	}
}