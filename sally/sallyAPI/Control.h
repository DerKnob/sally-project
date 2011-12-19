////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Control.h
///
/// \brief	Declares the control class. 
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

#pragma once
#include "Define.h"
#include "GUIBaseObject.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \enum	ControlRotation
		///
		/// \brief	Values that represent ControlRotation. 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum ControlRotation {CONTROLROTATION_LEFT, CONTROLROTATION_RIGHT};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CControl
		///
		/// \brief	The base GUI control. Every GUI control will inherit from this class. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CControl
			: public SallyAPI::GUI::CGUIBaseObject
		{
		protected:
			bool		m_bVisible;
			bool		m_bEnabled;
			bool		m_bPressed;
			int			m_iControlId;
			D3DCOLOR	m_rgbFontColour;
			int			m_iAlign;
			bool		m_bLocalised;
			bool		m_bMouseDown;
			int			m_iMouseDownX;
			int			m_iMouseDownY;

			float		m_fDeltaMoveX;
			float		m_fDeltaMoveY;
			int			m_iDestinationX;
			int			m_iDestinationY;
			int			m_iStartX;
			int			m_iStartY;
			int			m_iAnimationSpeedMoveX;
			int			m_iAnimationSpeedMoveY;
			bool		m_bAnimationMoveY;
			bool		m_bAnimationMoveX;

			float		m_fDeltaResizeWidth;
			float		m_fDeltaResizeHeight;
			int			m_iDestinationWidth;
			int			m_iDestinationHeight;
			int			m_iStartWidth;
			int			m_iStartHeight;
			int			m_iAnimationSpeedResizeWidth;
			int			m_iAnimationSpeedResizeHeight;
			bool		m_bAnimationResizeWidth;
			bool		m_bAnimationResizeHeight;

			float		m_fDeltaBlend;
			int			m_iDestinationBlend;
			int			m_iStartBlend;
			int			m_iAnimationSpeedBlend;
			bool		m_bAnimationBlend;

			float		m_fDeltaRotateY;
			float		m_fStartRotateY;
			float		m_fSpeedRotateY;
			bool		m_bAnimationRotateY;
			bool		m_bAnimationRotateYRun;

			float		m_fDeltaRotateX;
			float		m_fStartRotateX;
			float		m_fSpeedRotateX;
			bool		m_bAnimationRotateX;
			bool		m_bAnimationRotateXRun;

			float		m_fDeltaRotateZ;
			float		m_fStartRotateZ;
			float		m_fSpeedRotateZ;
			bool		m_bAnimationRotateZ;
			bool		m_bAnimationRotateZRun;

			ControlRotation	m_eRotationX;
			ControlRotation	m_eRotationY;
			ControlRotation	m_eRotationZ;

			float		m_fStopAtAngelX;
			float		m_fStopAtAngelY;
			float		m_fStopAtAngelZ;

			float		m_fTimeMouseUp;
			float		m_fTimeMouseDown;

			virtual SallyAPI::Core::CFont*	GetCurrentFont(const std::string& fontName);
			virtual RECT					GetTextRect(int leftImage, int rightImage, int borderLeft, int borderRight, int topImage, int bottomImage, int borderTop, int borderBottom);

			virtual void	DrawButtonBackground(int leftImageTop, int middleImageTop, int rightImageTop,
								int leftImage, int middleImage, int rightImage,
								int leftImageBottom, int middleImageBottom, int rightImageBottom);
			virtual void	DrawButtonBackground(int leftImage, int middleImage, int rightImage);
			virtual void	DrawButtonBackground(int leftImage, int middleImage, int rightImage, int xCorrection, int yCorrection, int widthCorrection, int heightCorrection);
			virtual void	DrawText(RECT r, const std::string& fontName, const std::string& text);
			virtual void	DrawText(int leftImage, int rightImage, int borderLeft, int borderRight, const std::string& fontName, const std::string& text = "");
			virtual void	DrawText(int leftImage, int rightImage, int borderLeft, int borderRight, int topImage, int bottomImage, int borderTop, int borderBottom, const std::string& fontName, const std::string& text = "");
			virtual void	DrawTextToRenderTarget(int width, int height, const std::string& fontName, int offSetX, int offSetY, const std::string& text = "");

			virtual void	ResizeInternal(int width, int height);
			virtual void	MoveInternal(int x, int y);

			virtual void	UpdateControl();

			virtual void	ResetMouse();

			virtual bool	ProcessMouseDoubleClick(int x, int y);
			virtual bool	ProcessMouseDown(int x, int y);
			virtual bool	ProcessMouseUp(int x, int y);
			virtual bool	ProcessMouseMove(int x, int y);
		public:
			CControl(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId = 0);
			virtual ~CControl();

			virtual void	Move(int x, int y);
			virtual void	Resize(int width, int height);

			virtual void	MoveAnimated(int x, int y, int speed, bool moveToEnd = true);
			virtual void	ResizeAnimated(int width, int height, int speed, bool moveToEnd = true);

			virtual void	MoveAnimated(int x, int y, int speedX, int speedY, bool moveToEnd = true);
			virtual void	ResizeAnimated(int width, int height, int speedWidth, int speedHeight, bool moveToEnd = true);

			virtual void	BlendAnimated(int alphaBlending, int speed, bool moveToEnd = true);
			virtual void	RotateAnimatedY(float speed, bool bRun);
			virtual void	RotateAnimatedX(float speed, bool bRun);
			virtual void	RotateAnimatedZ(float speed, bool bRun);


			virtual void	SetRotationX(ControlRotation value);
			virtual void	SetRotationY(ControlRotation value);
			virtual void	SetRotationZ(ControlRotation value);

			virtual ControlRotation	GetRotationX();
			virtual ControlRotation	GetRotationY();
			virtual ControlRotation	GetRotationZ();

			virtual void	Visible(bool visible);
			virtual void	Enable(bool enable);
			virtual void	SetLocalised(bool localised);
			virtual void	InvalidateControl();

			virtual void	SetText(const std::string& text);

			virtual bool	IsVisible();
			virtual bool	IsEnabled();
			virtual bool	IsLocalised();
			virtual bool	IsMouseDown();
			virtual bool	IsPressed();

			virtual void	SetStopAngelX(float angel);
			virtual void	SetStopAngelY(float angel);
			virtual void	SetStopAngelZ(float angel);
			
			virtual int		GetMouseDownX();
			virtual int		GetMouseDownY();

			virtual void	SetColor(D3DCOLOR rgbFontColour);
			virtual void	SetAlign(int align);

			virtual int		GetControlId();

			virtual void	Timer(float timeDelta);
			virtual void	Render();
			virtual void	Update();
			virtual void	CleanUp();
			virtual void	SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			virtual void	SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			virtual void	ProcessKeyboard(char KeyboardState[256], char KeyboardStatePressed[256]);

			virtual bool	CheckProcessMouseDoubleClick(int x, int y);
			virtual bool	CheckProcessMouseDown(int x, int y);
			virtual bool	CheckProcessMouseUp(int x, int y);
			virtual bool	CheckProcessMouseMove(int x, int y);

			int				GetDestinationPositionX();
			int				GetDestinationPositionY();
			int				GetDestinationHeight();
			int				GetDestinationWidth();

			bool			IsAnimationActivePositionX();
			bool			IsAnimationActivePositionY();
			bool			IsAnimationActiveHeight();
			bool			IsAnimationActiveWidth();
			bool			IsControlHit(int x, int y);
		};
	}
}