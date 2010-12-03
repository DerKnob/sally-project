////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Control.cpp
///
/// \brief	Implements the control class. 
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

#include "control.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CControl::CControl(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int height, int controlId) :SallyAPI::GUI::CGUIBaseObject(parent), m_bVisible(true),
/// m_bEnabled(true), m_iControlId(controlId), m_rgbFontColour(0), m_iAlign(-1),
/// m_bLocalised(true), m_bMouseDown(false), m_iMouseDownY(-1), m_iMouseDownX(-1),
/// m_bPressed(false), m_fDeltaMoveX(0), m_bAnimationMoveX(false), m_fDeltaMoveY(0),
/// m_bAnimationMoveY(false), m_fDeltaResizeWidth(0), m_bAnimationResizeWidth(false),
/// m_fDeltaResizeHeight(0), m_bAnimationResizeHeight(false), m_fDeltaBlend(0),
/// m_bAnimationBlend(false), m_fDeltaRotateY(0), m_bAnimationRotateY(false), m_fDeltaRotateX(0),
/// m_bAnimationRotateX(false), m_fDeltaRotateZ(0), m_bAnimationRotateZ(false),
/// m_fSpeedRotateY(0), m_fSpeedRotateX(0), m_fSpeedRotateZ(0), m_fStopAtAngelX(360),
/// m_fStopAtAngelY(360), m_fStopAtAngelZ(360), m_iDestinationX(-1), m_iDestinationY(-1),
/// m_iDestinationHeight(-1), m_iDestinationWidth(-1), m_eRotationX(CONTROLROTATION_LEFT),
/// m_eRotationY(CONTROLROTATION_LEFT), m_eRotationZ(CONTROLROTATION_LEFT)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	height			The height. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CControl::CControl(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId)
	:SallyAPI::GUI::CGUIBaseObject(parent), m_bVisible(true), m_bEnabled(true), m_iControlId(controlId),
	m_rgbFontColour(0), m_iAlign(-1), m_bLocalised(true), m_bMouseDown(false), m_iMouseDownY(-1),
	m_iMouseDownX(-1),m_bPressed(false), 
	m_fDeltaMoveX(0), m_bAnimationMoveX(false), m_fDeltaMoveY(0), m_bAnimationMoveY(false),
	m_fDeltaResizeWidth(0), m_bAnimationResizeWidth(false), m_fDeltaResizeHeight(0), m_bAnimationResizeHeight(false),
	m_fDeltaBlend(0), m_bAnimationBlend(false),
	m_fDeltaRotateY(0), m_bAnimationRotateY(false),
	m_fDeltaRotateX(0), m_bAnimationRotateX(false),
	m_fDeltaRotateZ(0), m_bAnimationRotateZ(false),
	m_fSpeedRotateY(0), m_fSpeedRotateX(0), m_fSpeedRotateZ(0),
	m_fStopAtAngelX(360), m_fStopAtAngelY(360), m_fStopAtAngelZ(360),
	m_iDestinationX(-1), m_iDestinationY(-1), m_iDestinationHeight(-1), m_iDestinationWidth(-1),
	m_eRotationX(CONTROLROTATION_LEFT), m_eRotationY(CONTROLROTATION_LEFT), m_eRotationZ(CONTROLROTATION_LEFT)
{
	Move(x, y);
	Resize(width, height);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CControl::~CControl()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CControl::~CControl()
{
	CleanUp();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::CleanUp()
///
/// \brief	This method is called when the control is destroyed. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::CleanUp()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	This method implements the message bus to the parent control. Messages send to the
///         parent control will go through this method. You can hook in here, if you want to
///         intercept a message.
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case MS_INVALIDATE_CONTROL:
		m_bUpdateRequested = true;
		break;
	}
	m_pParent->SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	This method implements the message bus to the child controls. Messages send to all
///         child controls will go through this method. You can hook in here, if you want to
///         intercept a message.
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_MESSAGE_UPDATE_ABSOLUTE_POSITION:
		m_iXAbsolut = 0;
		m_iYAbsolut = 0;
		GetAbsolutPosition(&m_iXAbsolut, &m_iYAbsolut);
		break;
	case GUI_MESSAGE_MOUSE_UP_INFO:
		m_bPressed = false;
		break;
	case GUI_MESSAGE_CONTROL_SCROLLED:
		m_bPressed = false;
		m_bMouseDown = false;
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CControl::IsAnimationActivePositionX()
///
/// \brief	Query if this object is animation active position x coordinate. 
///
/// \author	Christian Knobloch
/// \date	15.11.2010
///
/// \return	true if animation active position x coordinate coordinate, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CControl::IsAnimationActivePositionX()
{
	return m_bAnimationMoveX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CControl::IsAnimationActivePositionY()
///
/// \brief	Query if this object is animation active position y coordinate. 
///
/// \author	Christian Knobloch
/// \date	15.11.2010
///
/// \return	true if animation active position y coordinate coordinate, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CControl::IsAnimationActivePositionY()
{
	return m_bAnimationMoveY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CControl::IsAnimationActiveWidth()
///
/// \brief	Query if this object is animation active width. 
///
/// \author	Christian Knobloch
/// \date	15.11.2010
///
/// \return	true if animation active width, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CControl::IsAnimationActiveWidth()
{
	return m_bAnimationResizeWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CControl::IsAnimationActiveHeight()
///
/// \brief	Query if this object is animation active height. 
///
/// \author	Christian Knobloch
/// \date	15.11.2010
///
/// \return	true if animation active height, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CControl::IsAnimationActiveHeight()
{
	return m_bAnimationResizeHeight;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CControl::GetDestinationPositionX()
///
/// \brief	Gets the destination position x coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	15.11.2010
///
/// \return	The destination position x coordinate coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CControl::GetDestinationPositionX()
{
	return m_iDestinationX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CControl::GetDestinationPositionY()
///
/// \brief	Gets the destination position y coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	15.11.2010
///
/// \return	The destination position y coordinate coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CControl::GetDestinationPositionY()
{
	return m_iDestinationX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CControl::GetDestinationHeight()
///
/// \brief	Gets the destination height. 
///
/// \author	Christian Knobloch
/// \date	15.11.2010
///
/// \return	The destination height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CControl::GetDestinationHeight()
{
	return m_iDestinationHeight;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CControl::GetDestinationWidth()
///
/// \brief	Gets the destination width. 
///
/// \author	Christian Knobloch
/// \date	15.11.2010
///
/// \return	The destination width. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CControl::GetDestinationWidth()
{
	return m_iDestinationWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::MoveInternal(int x, int y)
///
/// \brief	Move internal. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::MoveInternal(int x, int y)
{
	m_iMouseDownX += x - m_iX;
	m_iMouseDownY += y - m_iY;

	m_iX = x;
	m_iY = y;

	SendMessageToChilds(this, m_iControlId, GUI_MESSAGE_UPDATE_ABSOLUTE_POSITION);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::Move(int x, int y)
///
/// \brief	Moves. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::Move(int x, int y)
{
	m_bAnimationMoveY = false;
	m_bAnimationMoveX = false;
	m_fDeltaMoveX = 0;
	m_fDeltaMoveY = 0;

	MoveInternal(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::ResizeInternal(int width, int height)
///
/// \brief	Resize internal. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	width	The width. 
/// \param	height	The height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::ResizeInternal(int width, int height)
{
	m_iWidth = width;
	m_iHeight = height;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::Resize(int width, int height)
///
/// \brief	Resizes. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	width	The width. 
/// \param	height	The height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::Resize(int width, int height)
{
	m_bAnimationResizeHeight = false;
	m_bAnimationResizeWidth = false;
	m_fDeltaResizeHeight = 0;
	m_fDeltaResizeWidth = 0;

	ResizeInternal(width, height);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::SetStopAngelX(float angel)
///
/// \brief	Sets a stop angel x coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	angel	The angel. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::SetStopAngelX(float angel)
{
	m_fStopAtAngelX = angel;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::SetStopAngelY(float angel)
///
/// \brief	Sets a stop angel y coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	angel	The angel. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::SetStopAngelY(float angel)
{
	m_fStopAtAngelY = angel;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::SetStopAngelZ(float angel)
///
/// \brief	Sets a stop angel z coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	angel	The angel. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::SetStopAngelZ(float angel)
{
	m_fStopAtAngelZ = angel;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::Visible(bool visible)
///
/// \brief	Visibles. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	visible	true to show, false to hide. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::Visible(bool visible)
{
	m_bVisible = visible;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::Enable(bool enable)
///
/// \brief	Enables. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	enable	true to enable, false to disable. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::Enable(bool enable)
{
	m_bEnabled = enable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CControl::IsVisible()
///
/// \brief	Query if this object is visible. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if visible, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CControl::IsVisible()
{
	return m_bVisible;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CControl::IsEnabled()
///
/// \brief	Query if this object is enabled. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if enabled, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CControl::IsEnabled()
{
	return m_bEnabled;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CControl::IsMouseDown()
///
/// \brief	Query if this object is mouse down. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if mouse down, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CControl::IsMouseDown()
{
	return m_bMouseDown;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CControl::IsPressed()
///
/// \brief	Query if this object is pressed. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if pressed, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CControl::IsPressed()
{
	return m_bPressed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CControl::GetMouseDownX()
///
/// \brief	Gets the mouse down x coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The mouse down x coordinate coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CControl::GetMouseDownX()
{
	return m_iMouseDownX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CControl::GetMouseDownY()
///
/// \brief	Gets the mouse down y coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The mouse down y coordinate coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CControl::GetMouseDownY()
{
	return m_iMouseDownY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::Render()
///
/// \brief	Renders this object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::Render()
{
	if ((m_bVisible) && (m_iAlphaBlending > 0))
	{
		// is it on the screen?
		if ((m_iXAbsolut + m_iWidth > 0) && (m_iXAbsolut < WINDOW_WIDTH) &&
			(m_iYAbsolut + m_iHeight > 0) && (m_iYAbsolut < WINDOW_HEIGHT))
		{
			EnterRenderLock();

			RenderControl();

			LeaveRenderLock();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::Update()
///
/// \brief	Updates this object. 
///
/// \author	Christian Knobloch
/// \date	30.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::Update()
{
	if ((m_bVisible) && (m_iAlphaBlending > 0) && (m_bUpdateRequested))
	{
		// is it on the screen?
		if ((m_iXAbsolut + m_iWidth > 0) && (m_iXAbsolut < WINDOW_WIDTH) &&
			(m_iYAbsolut + m_iHeight > 0) && (m_iYAbsolut < WINDOW_HEIGHT))
		{
			EnterRenderLock();

			UpdateControl();

			LeaveRenderLock();

			m_bUpdateRequested = true;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::UpdateControl()
///
/// \brief	Updates a control. 
///
/// \author	Christian Knobloch
/// \date	30.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::UpdateControl()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::BlendAnimated(int alphaBlending, int speed, bool moveToEnd)
///
/// \brief	Blend animated. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	alphaBlending	The alpha blending. 
/// \param	speed			The speed. 
/// \param	moveToEnd		true to move to end. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::BlendAnimated(int alphaBlending, int speed, bool moveToEnd)
{
	if (alphaBlending > 255)
		alphaBlending = 255;
	else if (alphaBlending < 0)
		alphaBlending = 0;

	// are we alread blending???
	if ((m_bAnimationBlend) && (moveToEnd))
	{
		//SetAlphaBlending(m_iDestinationBlend);
		m_fDeltaBlend = 0;
		m_bAnimationBlend = false;

		SendMessageToParent(this, m_iControlId, GUI_CONTROL_BLENDED);
	}
	m_iAnimationSpeedBlend = speed;
	m_iStartBlend = m_iAlphaBlending;

	if (m_iAlphaBlending != alphaBlending)
	{
		// the destination width point has changed
		m_iDestinationBlend = alphaBlending;
		m_bAnimationBlend = true;
	}
	else
	{
		// the destination width point has NOT changed
		m_bAnimationBlend = false;
	}

	if (!m_bAnimationBlend)
	{
		SendMessageToParent(this, m_iControlId, GUI_CONTROL_BLENDED);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::ResizeAnimated(int width, int height, int speed, bool moveToEnd)
///
/// \brief	Resize animated. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	width		The width. 
/// \param	height		The height. 
/// \param	speed		The speed. 
/// \param	moveToEnd	true to move to end. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::ResizeAnimated(int width, int height, int speed, bool moveToEnd)
{
	ResizeAnimated(width, height, speed, speed, moveToEnd);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::ResizeAnimated(int width, int height, int speedWidth, int speedHeight,
/// bool moveToEnd)
///
/// \brief	Resize animated. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	width		The width. 
/// \param	height		The height. 
/// \param	speedWidth	Width of the speed. 
/// \param	speedHeight	Height of the speed. 
/// \param	moveToEnd	true to move to end. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::ResizeAnimated(int width, int height, int speedWidth, int speedHeight, bool moveToEnd)
{
	// are we already moving???
	if (((m_bAnimationResizeWidth) || (m_bAnimationResizeHeight)) && moveToEnd)
	{
		Resize(m_iDestinationWidth, m_iDestinationHeight);
		m_fDeltaResizeWidth = 0;
		m_fDeltaResizeHeight = 0;
		m_bAnimationResizeWidth = false;
		m_bAnimationResizeHeight = false;

		SendMessageToParent(this, m_iControlId, GUI_CONTROL_RESIZED);
	}
	if (!moveToEnd)
	{
		m_fDeltaResizeWidth = 0;
		m_fDeltaResizeHeight = 0;
		m_bAnimationResizeWidth = false;
		m_bAnimationResizeHeight = false;
	}
	m_iAnimationSpeedResizeWidth = speedWidth;
	m_iAnimationSpeedResizeHeight = speedHeight;
	m_iStartWidth = m_iWidth;
	m_iStartHeight = m_iHeight;

	if (m_iWidth != width)
	{
		// the destination width point has changed
		m_iDestinationWidth = width;
		m_bAnimationResizeWidth = true;
	}
	else
	{
		// the destination width point has NOT changed
		m_bAnimationResizeWidth = false;
	}

	if (m_iHeight != height)
	{
		// the destination width point has changed
		m_iDestinationHeight = height;
		m_bAnimationResizeHeight = true;
	}
	else
	{
		// the destination width point has NOT changed
		m_bAnimationResizeHeight = false;
	}

	if ((!m_bAnimationResizeWidth) && (!m_bAnimationResizeHeight))
	{
		SendMessageToParent(this, m_iControlId, GUI_CONTROL_RESIZED);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::SetAlphaBlending(int alphaBlending)
///
/// \brief	Sets an alpha blending. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	alphaBlending	The alpha blending. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::SetAlphaBlending(int alphaBlending)
{
// 	m_bAnimationResizeHeight = false;
// 	m_bAnimationResizeWidth = false;
// 	m_fDeltaResizeHeight = 0;
// 	m_fDeltaResizeWidth = 0;

	SallyAPI::GUI::CGUIBaseObject::SetAlphaBlending(alphaBlending);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::MoveAnimated(int x, int y, int speed, bool moveToEnd)
///
/// \brief	Move animated. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x			The x coordinate. 
/// \param	y			The y coordinate. 
/// \param	speed		The speed. 
/// \param	moveToEnd	true to move to end. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::MoveAnimated(int x, int y, int speed, bool moveToEnd)
{
	MoveAnimated(x, y, speed, speed, moveToEnd);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::MoveAnimated(int x, int y, int speedX, int speedY, bool moveToEnd)
///
/// \brief	Move animated. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x			The x coordinate. 
/// \param	y			The y coordinate. 
/// \param	speedX		The speed x coordinate. 
/// \param	speedY		The speed y coordinate. 
/// \param	moveToEnd	true to move to end. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::MoveAnimated(int x, int y, int speedX, int speedY, bool moveToEnd)
{
	// are we already moving???
	if (((m_bAnimationMoveX) || (m_bAnimationMoveY)) && moveToEnd)
	{
		Move(m_iDestinationX, m_iDestinationY);
		m_fDeltaMoveX = 0;
		m_fDeltaMoveY = 0;
		m_bAnimationMoveX = false;
		m_bAnimationMoveY = false;

		SendMessageToParent(this, m_iControlId, GUI_CONTROL_MOVED);
	}
	if (!moveToEnd)
	{
		m_fDeltaMoveX = 0;
		m_fDeltaMoveY = 0;
		m_bAnimationMoveX = false;
		m_bAnimationMoveY = false;
	}
	m_iAnimationSpeedMoveX = speedX;
	m_iAnimationSpeedMoveY = speedY;
	m_iStartX = m_iX;
	m_iStartY = m_iY;

	if (m_iX != x)
	{
		// the destination x point has changed
		m_iDestinationX = x;
		m_bAnimationMoveX = true;
	}
	else
	{
		// the destination x point has NOT changed
		m_bAnimationMoveX = false;
	}

	if (m_iY != y)
	{
		// the destination x point has changed
		m_iDestinationY = y;
		m_bAnimationMoveY = true;
	}
	else
	{
		// the destination x point has NOT changed
		m_bAnimationMoveY = false;
	}

	if ((!m_bAnimationMoveX) && (!m_bAnimationMoveY))
	{
		SendMessageToParent(this, m_iControlId, GUI_CONTROL_MOVED);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::RotateAnimatedY(float speed, bool bRun)
///
/// \brief	Rotate animated y coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	speed	The speed. 
/// \param	bRun	true to run. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::RotateAnimatedY(float speed, bool bRun)
{
	// are we already moving???
	m_fSpeedRotateY = speed;
	m_bAnimationRotateY = true;
	m_bAnimationRotateYRun = bRun;
	m_fDeltaRotateY = 0;
	
	m_fStartRotateY = GetRotationAngleY();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::RotateAnimatedX(float speed, bool bRun)
///
/// \brief	Rotate animated x coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	speed	The speed. 
/// \param	bRun	true to run. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::RotateAnimatedX(float speed, bool bRun)
{
	// are we already moving???
	m_fSpeedRotateX = speed;
	m_bAnimationRotateX = true;
	m_bAnimationRotateXRun = bRun;
	m_fDeltaRotateX = 0;

	m_fStartRotateX = GetRotationAngleX();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::RotateAnimatedZ(float speed, bool bRun)
///
/// \brief	Rotate animated z coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	speed	The speed. 
/// \param	bRun	true to run. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::RotateAnimatedZ(float speed, bool bRun)
{
	// are we already moving???
	m_fSpeedRotateZ = speed;
	m_bAnimationRotateZ = true;
	m_bAnimationRotateZRun = bRun;
	m_fDeltaRotateZ = 0;

	m_fStartRotateZ = GetRotationAngleZ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::Timer(float timeDelta)
///
/// \brief	Timers. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	timeDelta	The time delta. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::Timer(float timeDelta)
{
	SallyAPI::GUI::CGUIBaseObject::Timer(timeDelta);
	
	/************************************************************************/
	/* Resize                                                               */
	/************************************************************************/
	int newWidth = m_iStartWidth;
	int newHeight = m_iStartHeight;
	bool bResize = false;

	if (m_bAnimationResizeWidth)
	{
		bResize = true;
		// resize the width
		if (m_fDeltaResizeWidth == 0)
		{
			// first start of the move
			m_fDeltaResizeWidth = m_fTimeDelta;
		}
		float fDeltaDifferents = m_fTimeDelta - m_fDeltaResizeWidth;

		int change = (int) (fDeltaDifferents * m_iAnimationSpeedResizeWidth);
		if (m_iWidth > m_iDestinationWidth)
		{
			newWidth -= change;

			if (newWidth <= m_iDestinationWidth)
			{
				newWidth = m_iDestinationWidth;
				m_fDeltaResizeWidth = 0;
				m_bAnimationResizeWidth = false;
				m_iStartWidth = newWidth;
			}
		}
		if (m_iWidth < m_iDestinationWidth)
		{
			newWidth += change;

			if (newWidth >= m_iDestinationWidth)
			{
				newWidth = m_iDestinationWidth;
				m_fDeltaResizeWidth = 0;
				m_bAnimationResizeWidth = false;
				m_iStartWidth = newWidth;
			}
		}
	}

	if (m_bAnimationResizeHeight)
	{
		bResize = true;
		// resize the height
		if (m_fDeltaResizeHeight == 0)
		{
			// first start of the move
			m_fDeltaResizeHeight = m_fTimeDelta;
		}
		float fDeltaDifferents = m_fTimeDelta - m_fDeltaResizeHeight;

		int change = (int) (fDeltaDifferents * m_iAnimationSpeedResizeHeight);
		if (m_iHeight > m_iDestinationHeight)
		{
			newHeight -= change;

			if (newHeight <= m_iDestinationHeight)
			{
				newHeight = m_iDestinationHeight;
				m_fDeltaResizeHeight = 0;
				m_bAnimationResizeHeight = false;
				m_iStartHeight = newHeight;
			}
		}
		if (m_iHeight < m_iDestinationHeight)
		{
			newHeight += change;

			if (newHeight >= m_iDestinationHeight)
			{
				newHeight = m_iDestinationHeight;
				m_fDeltaResizeHeight = 0;
				m_bAnimationResizeHeight = false;
				m_iStartHeight = newHeight;
			}
		}
	}

	// has anything changed? than shake your ass... show me what you got!
	if (bResize)
	{
		ResizeInternal(newWidth, newHeight);

		if ((!m_bAnimationResizeWidth) && (!m_bAnimationResizeHeight))
		{
			SendMessageToParent(this, m_iControlId, GUI_CONTROL_RESIZED);
		}
	}

	/************************************************************************/
	/* Move                                                                 */
	/************************************************************************/
	int newX = m_iStartX;
	int newY = m_iStartY;
	bool bMove = false;

	if (m_bAnimationMoveX)
	{
		bMove = true;
		// Move the x position
		if (m_fDeltaMoveX == 0)
		{
			// first start of the move
			m_fDeltaMoveX = m_fTimeDelta;
		}
		float fDeltaDifferents = m_fTimeDelta - m_fDeltaMoveX;

		int change = (int) (fDeltaDifferents * m_iAnimationSpeedMoveX);
		if (m_iX > m_iDestinationX)
		{
			newX -= change;

			if (newX <= m_iDestinationX)
			{
				newX = m_iDestinationX;
				m_fDeltaMoveX = 0;
				m_bAnimationMoveX = false;
				m_iStartX = newX;
			}
		}
		if (m_iX < m_iDestinationX)
		{
			newX += change;

			if (newX >= m_iDestinationX)
			{
				newX = m_iDestinationX;
				m_fDeltaMoveX = 0;
				m_bAnimationMoveX = false;
				m_iStartX = newX;
			}
		}
	}

	if (m_bAnimationMoveY)
	{
		bMove = true;
		// Move the y position
		if (m_fDeltaMoveY == 0)
		{
			// first start of the move
			m_fDeltaMoveY = m_fTimeDelta;
		}
		float fDeltaDifferents = m_fTimeDelta - m_fDeltaMoveY;

		int change = (int) (fDeltaDifferents * m_iAnimationSpeedMoveY);
		if (m_iY > m_iDestinationY)
		{
			newY -= change;

			if (newY <= m_iDestinationY)
			{
				newY = m_iDestinationY;
				m_fDeltaMoveY = 0;
				m_bAnimationMoveY = false;
				m_iStartY = newY;

			}
		}
		if (m_iY < m_iDestinationY)
		{
			newY += change;

			if (newY >= m_iDestinationY)
			{
				newY = m_iDestinationY;
				m_fDeltaMoveY = 0;
				m_bAnimationMoveY = false;
				m_iStartY = newY;
			}
		}
	}

	// has anything changed? than shake your ass... show me what you got!
	if (bMove)
	{
		MoveInternal(newX, newY);

		if ((!m_bAnimationMoveX) && (!m_bAnimationMoveY))
		{
			SendMessageToParent(this, m_iControlId, GUI_CONTROL_MOVED);
		} 
	}

	/************************************************************************/
	/* Blend                                                                */
	/************************************************************************/
	int newAlphaBlending = m_iStartBlend;

	if (m_bAnimationBlend)
	{
		// belnd new
		if (m_fDeltaBlend == 0)
		{
			// first start of the blend
			m_fDeltaBlend = m_fTimeDelta;
		}
		float fDeltaDifferents = m_fTimeDelta - m_fDeltaBlend;

		int change = (int) (fDeltaDifferents * m_iAnimationSpeedBlend);
		if (newAlphaBlending > m_iDestinationBlend)
		{
			newAlphaBlending -= change;

			if (newAlphaBlending <= m_iDestinationBlend)
			{
				newAlphaBlending = m_iDestinationBlend;
				m_fDeltaBlend = 0;
				m_bAnimationBlend = false;
			}
		}
		if (newAlphaBlending < m_iDestinationBlend)
		{
			newAlphaBlending += change;

			if (newAlphaBlending >= m_iDestinationBlend)
			{
				newAlphaBlending = m_iDestinationBlend;
				m_fDeltaBlend = 0;
				m_bAnimationBlend = false;
			}
		}

		// has anything changed? than shake your ass... show me what you got!
		SetAlphaBlending(newAlphaBlending);

		if (!m_bAnimationBlend)
			SendMessageToParent(this, m_iControlId, GUI_CONTROL_BLENDED);
	}

	/************************************************************************/
	/* Rotate Y                                                             */
	/************************************************************************/
	float newRotateY = m_fStartRotateY;

	if (m_bAnimationRotateY)
	{
		if (m_fDeltaRotateY == 0)
		{
			// first start of rotate
			m_fDeltaRotateY = m_fTimeDelta;
		}
		float fDeltaDifferents = m_fTimeDelta - m_fDeltaRotateY;

		float change = fDeltaDifferents * m_fSpeedRotateY;
		if (m_eRotationY == CONTROLROTATION_LEFT)
			newRotateY += change;
		else
			newRotateY -= change;

		float rad360 = D3DXToRadian(360);
		if (m_bAnimationRotateYRun)
		{
			while (newRotateY > rad360)
			{
				newRotateY -= rad360;
				m_fStartRotateY = newRotateY;
				m_fDeltaRotateY = 0;
			}
		}
		else
		{
			float radToStop = D3DXToRadian(m_fStopAtAngelY);
			if (((newRotateY > radToStop) && (m_eRotationY == CONTROLROTATION_LEFT)) ||
				((newRotateY < radToStop) && (m_eRotationY == CONTROLROTATION_RIGHT)))
			{
				newRotateY = radToStop;
				m_bAnimationRotateY = false;
				SendMessageToParent(this, m_iControlId, GUI_CONTROL_ROTATED_Y);
			}
		}
		SetRotationAngleY(newRotateY);
	}
	

	/************************************************************************/
	/* Rotate X                                                             */
	/************************************************************************/
	float newRotateX = m_fStartRotateX;

	if (m_bAnimationRotateX)
	{
		if (m_fDeltaRotateX == 0)
		{
			// first start of rotate
			m_fDeltaRotateX = m_fTimeDelta;
		}
		float fDeltaDifferents = m_fTimeDelta - m_fDeltaRotateX;

		float change = fDeltaDifferents * m_fSpeedRotateX;
		if (m_eRotationX == CONTROLROTATION_LEFT)
			newRotateX += change;
		else
			newRotateX -= change;

		float rad360 = D3DXToRadian(360);
		if (m_bAnimationRotateXRun)
		{
			while (newRotateX > rad360)
			{
				newRotateX -= rad360;
				m_fStartRotateX = newRotateX;
				m_fDeltaRotateX = 0;
			}
		}
		else
		{
			float radToStop = D3DXToRadian(m_fStopAtAngelX);
			if (((newRotateX > radToStop) && (m_eRotationX == CONTROLROTATION_LEFT)) ||
				((newRotateX < radToStop) && (m_eRotationX == CONTROLROTATION_RIGHT)))
			{
				newRotateX = radToStop;
				m_bAnimationRotateX = false;
				SendMessageToParent(this, m_iControlId, GUI_CONTROL_ROTATED_X);
			}
		}
		SetRotationAngleX(newRotateX);
	}

	/************************************************************************/
	/* Rotate Z                                                             */
	/************************************************************************/
	float newRotateZ = m_fStartRotateZ;

	if (m_bAnimationRotateZ)
	{
		if (m_fDeltaRotateZ == 0)
		{
			// first start of rotate
			m_fDeltaRotateZ = m_fTimeDelta;
		}
		float fDeltaDifferents = m_fTimeDelta - m_fDeltaRotateZ;

		float change = fDeltaDifferents * m_fSpeedRotateZ;
		if (m_eRotationZ == CONTROLROTATION_LEFT)
			newRotateZ += change;
		else
			newRotateZ -= change;

		float rad360 = D3DXToRadian(360);
		if (m_bAnimationRotateZRun)
		{
			while (newRotateZ > rad360)
			{
				newRotateZ -= rad360;
				m_fStartRotateZ = newRotateZ;
				m_fDeltaRotateZ = 0;
			}
		}
		else
		{
			float radToStop = D3DXToRadian(m_fStopAtAngelZ);
			if (((newRotateZ > radToStop) && (m_eRotationZ == CONTROLROTATION_LEFT)) ||
				((newRotateZ < radToStop) && (m_eRotationZ == CONTROLROTATION_RIGHT)))
			{
				newRotateZ = radToStop;
				m_bAnimationRotateZ = false;
				SendMessageToParent(this, m_iControlId, GUI_CONTROL_ROTATED_Z);
			}
		}
		SetRotationAngleZ(newRotateZ);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::ProcessKeyboard(char KeyboardState[256], char KeyboardStatePressed[256])
///
/// \brief	Process a keyboard. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	KeyboardState			State of the keyboard. 
/// \param	KeyboardStatePressed	The keyboard state pressed. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::ProcessKeyboard(char KeyboardState[256], char KeyboardStatePressed[256])
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CControl::CheckProcessMouseDoubleClick(int x, int y)
///
/// \brief	Check process mouse double click. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CControl::CheckProcessMouseDoubleClick(int x, int y)
{
	if (!m_bVisible || !m_bEnabled)
		return false;

	if (x > m_iXAbsolut && x < m_iXAbsolut + m_iWidth)
	{
		if (y > m_iYAbsolut && y < m_iYAbsolut + m_iHeight)
		{
			return ProcessMouseDoubleClick(x, y);
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CControl::CheckProcessMouseUp(int x, int y)
///
/// \brief	Check process mouse up. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CControl::CheckProcessMouseUp(int x, int y)
{
	if (!m_bVisible || !m_bEnabled)
		return false;

	if (x > m_iXAbsolut && x < m_iXAbsolut + m_iWidth)
	{
		if (y > m_iYAbsolut && y < m_iYAbsolut + m_iHeight)
		{
			return ProcessMouseUp(x, y);
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CControl::CheckProcessMouseDown(int x, int y)
///
/// \brief	Check process mouse down. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CControl::CheckProcessMouseDown(int x, int y)
{
	if (!m_bVisible || !m_bEnabled)
		return false;

	if (x > m_iXAbsolut && x < m_iXAbsolut + m_iWidth)
	{
		if (y > m_iYAbsolut && y < m_iYAbsolut + m_iHeight)
		{
			return ProcessMouseDown(x, y);
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CControl::CheckProcessMouseMove(int x, int y)
///
/// \brief	Check process mouse move. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CControl::CheckProcessMouseMove(int x, int y)
{
	if (m_bEnabled && m_bVisible)
		return ProcessMouseMove(x, y);
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CControl::ProcessMouseDoubleClick(int x, int y)
///
/// \brief	Process a mouse double click. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CControl::ProcessMouseDoubleClick(int x, int y)
{
	m_bMouseDown = false;
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CControl::ProcessMouseUp(int x, int y)
///
/// \brief	Process a mouse up. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CControl::ProcessMouseUp(int x, int y)
{
	m_bPressed = false;
	m_bMouseDown = false;
	m_iMouseDownX = -1;
	m_iMouseDownY = -1;
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CControl::ProcessMouseDown(int x, int y)
///
/// \brief	Process a mouse down. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CControl::ProcessMouseDown(int x, int y)
{
	m_bPressed = true;
	m_bMouseDown = true;
	m_iMouseDownX = x;
	m_iMouseDownY = y;
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CControl::ProcessMouseMove(int x, int y)
///
/// \brief	Process a mouse move. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CControl::ProcessMouseMove(int x, int y)
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::DrawButtonBackground(int leftImage, int middleImage, int rightImage)
///
/// \brief	Draw button background. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	leftImage	The left image. 
/// \param	middleImage	The middle image. 
/// \param	rightImage	The right image. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::DrawButtonBackground(int leftImage, int middleImage, int rightImage)
{
	// Temp variables
	SallyAPI::GUI::CPicture* image = NULL;
	int imageWidthLeft = 0;
	int imageWidthRight = 0;
	int imageWidthMiddle = 1;

	image = GetPicture(leftImage);
	if (image != NULL) {
		imageWidthLeft = image->GetWidth();
	}
	image = GetPicture(rightImage);
	if (image != NULL) {
		imageWidthRight = image->GetWidth();
	}
	image = GetPicture(middleImage);
	if (image != NULL) {
		imageWidthMiddle = image->GetWidth();
	}

	// Draw Left
	DrawImage(leftImage, 0, 0);

	DrawImage(middleImage, imageWidthLeft, 0, m_iWidth - (imageWidthLeft + imageWidthRight), m_iHeight);

	// Draw Right
	DrawImage(rightImage, m_iWidth - imageWidthRight, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::DrawButtonBackground(int leftImageTop, int middleImageTop,
/// int rightImageTop, int leftImage, int middleImage, int rightImage, int leftImageBottom,
/// int middleImageBottom, int rightImageBottom)
///
/// \brief	Draw button background. 
///
/// \author	Christian Knobloch
/// \date	19.09.2010
///
/// \param	leftImageTop		The left image top. 
/// \param	middleImageTop		The middle image top. 
/// \param	rightImageTop		The right image top. 
/// \param	leftImage			The left image. 
/// \param	middleImage			The middle image. 
/// \param	rightImage			The right image. 
/// \param	leftImageBottom		The left image bottom. 
/// \param	middleImageBottom	The middle image bottom. 
/// \param	rightImageBottom	The right image bottom. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::DrawButtonBackground(int leftImageTop, int middleImageTop, int rightImageTop,
									int leftImage, int middleImage, int rightImage,
									int leftImageBottom, int middleImageBottom, int rightImageBottom)
{
	SallyAPI::GUI::CPicture* image = NULL;

	int imageWidthLeftTop = 0;
	int imageWidthRightTop = 0;
	int imageWidthTop = 1;
	int imageHeightTop = 0;

	int imageWidthLeft = 0;
	int imageWidthCenter = 0;
	int imageWidthRight = 1;
	int imageHeightCenter = 0;

	int imageWidthLeftBottom = 0;
	int imageWidthRightBottom = 0;
	int imageWidthBottom = 1;
	int imageHeightBottom = 0;

	// Top
	image = GetPicture(leftImageTop);
	if (image != NULL) {
		imageWidthLeftTop = image->GetWidth();
		if (imageHeightTop == 0) {
			imageHeightTop = image->GetHeight();
		}
	}
	image = GetPicture(rightImageTop);
	if (image != NULL) {
		imageWidthRightTop = image->GetWidth();
		if (imageHeightTop == 0) {
			imageHeightTop = image->GetHeight();
		}
	}
	image = GetPicture(middleImageTop);
	if (image != NULL) {
		imageWidthTop = image->GetWidth();
		if (imageHeightTop == 0) {
			imageHeightTop = image->GetHeight();
		}
	}

	// Center
	image = GetPicture(leftImage);
	if (image != NULL) {
		imageWidthLeft = image->GetWidth();
	}
	image = GetPicture(rightImage);
	if (image != NULL) {
		imageWidthRight = image->GetWidth();
	}
	image = GetPicture(middleImage);
	if (image != NULL) {
		imageWidthCenter = image->GetWidth();
	}

	// Bottom
	image = GetPicture(leftImageBottom);
	if (image != NULL) {
		imageWidthLeftBottom = image->GetWidth();
		if (imageHeightBottom == 0) {
			imageHeightBottom = image->GetHeight();
		}
	}
	image = GetPicture(rightImageBottom);
	if (image != NULL) {
		imageWidthRightBottom = image->GetWidth();
		if (imageHeightBottom == 0) {
			imageHeightBottom = image->GetHeight();
		}
	}
	image = GetPicture(middleImageBottom);
	if (image != NULL) {
		imageWidthBottom = image->GetWidth();
		if (imageHeightBottom == 0) {
			imageHeightBottom = image->GetHeight();
		}
	}

	imageHeightCenter = m_iHeight - (imageHeightBottom + imageHeightTop);

	// Top
	DrawImage(leftImageTop, 0, 0);
	DrawImage(middleImageTop, imageWidthLeftTop, 0, m_iWidth - (imageWidthLeftTop + imageWidthRightTop), imageHeightTop);
	DrawImage(rightImageTop, m_iWidth - imageWidthRightTop, 0);

	// Center
	DrawImage(leftImage, 0, imageHeightTop, imageWidthLeft, imageHeightCenter);
	DrawImage(middleImage, imageWidthLeft, imageHeightTop, m_iWidth - (imageWidthLeftBottom + imageWidthRightBottom), imageHeightCenter);
	DrawImage(rightImage, m_iWidth - imageWidthRight, imageHeightTop, imageWidthRight, imageHeightCenter);

	// Bottom
	DrawImage(leftImageBottom, 0, imageHeightCenter + imageHeightTop);
	DrawImage(middleImageBottom, imageWidthLeftBottom, imageHeightCenter + imageHeightTop, m_iWidth - (imageWidthLeftBottom + imageWidthRightBottom), imageHeightBottom);
	DrawImage(rightImageBottom, m_iWidth - imageWidthRightBottom, imageHeightCenter + imageHeightTop);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::DrawText(int leftImage, int rightImage, int borderLeft, int borderRight,
/// int topImage, int bottomImage, int borderTop, int borderBottom, const std::string& fontName,
/// const std::string& text)
///
/// \brief	Draw text. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// \param	leftImage		The left image. 
/// \param	rightImage		The right image. 
/// \param	borderLeft		The border left. 
/// \param	borderRight		The border right. 
/// \param	topImage		The top image. 
/// \param	bottomImage		The bottom image. 
/// \param	borderTop		The border top. 
/// \param	borderBottom	The border bottom. 
/// \param	fontName		Name of the font. 
/// \param	text			The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::DrawText(int leftImage, int rightImage, int borderLeft, int borderRight, int topImage, int bottomImage, int borderTop, int borderBottom, const std::string& fontName, const std::string& text)
{
	if (m_iAlphaBlending == 0)
		return;

	RECT r = GetTextRect(leftImage, rightImage, borderLeft, borderRight, topImage, topImage, borderTop, borderBottom);

	DrawText(r, fontName, text);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::DrawTextToRenderTarget(int width, int height, const std::string& fontName,
/// int offSetX, int offSetY, const std::string& text)
///
/// \brief	Draw text to render target. 
///
/// \author	Christian Knobloch
/// \date	04.05.2010
///
/// \param	width		The width. 
/// \param	height		The height. 
/// \param	fontName	Name of the font. 
/// \param	offSetX		The off set x coordinate. 
/// \param	offSetY		The off set y coordinate. 
/// \param	text		The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::DrawTextToRenderTarget(int width, int height, const std::string& fontName, int offSetX, int offSetY, const std::string& text)
{
	if (m_iAlphaBlending == 0)
		return;

	RECT r;
	r.left = offSetX;
	r.right = width + offSetX;
	r.top = offSetY;
	r.bottom = height + offSetY;

	DrawText(r, fontName, text);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::DrawText(RECT r, const std::string& fontName, const std::string& text)
///
/// \brief	Draw text. 
///
/// \author	Christian Knobloch
/// \date	04.05.2010
///
/// \param	r			The. 
/// \param	fontName	Name of the font. 
/// \param	text		The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::DrawText(RECT r, const std::string& fontName, const std::string& text)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CLanguageManager* lang = config->GetLanguageLocalization();
	SallyAPI::Config::CTheme* theme = config->GetTheme();
	SallyAPI::Core::CFont* font = GetCurrentFont(fontName);

	// set alpha blending
	font->SetAlphaBlending(m_iAlphaBlending);

	int align;
	if (m_iAlign == -1)
		align = DT_VCENTER | DT_SINGLELINE;
	else
		align = m_iAlign;

	if (text.length() == 0)
	{
		if (m_bLocalised)
			font->DrawText(lang->GetString(m_strText), &r, m_rgbFontColour, align);
		else
			font->DrawText(m_strText, &r, m_rgbFontColour, align);
	}
	else
	{
		if (m_bLocalised)
			font->DrawText(lang->GetString(text), &r, m_rgbFontColour, align);
		else
			font->DrawText(text, &r, m_rgbFontColour, align);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Core::CFont* CControl::GetCurrentFont(const std::string& fontName)
///
/// \brief	Gets a current font. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// \param	fontName	Name of the font. 
///
/// \return	null if it fails, else the current font. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Core::CFont* CControl::GetCurrentFont(const std::string& fontName)
{
	SallyAPI::Core::CFontManager* fontManager = SallyAPI::Core::CFontManager::GetInstance();

	SallyAPI::Core::CFont* font = NULL;
	if (m_bEnabled)
	{
		font = fontManager->GetFontByName(fontName);
	}
	else
	{
		std::string fontNameDisabled = fontName;
		fontNameDisabled.append(".disabled");
		font = fontManager->GetFontByName(fontNameDisabled);
	}
	return font;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	RECT CControl::GetTextRect(int leftImage, int rightImage, int borderLeft, int borderRight,
/// int topImage, int bottomImage, int borderTop, int borderBottom)
///
/// \brief	Gets a text rectangle. 
///
/// \author	Christian Knobloch
/// \date	21.09.2010
///
/// \param	leftImage		The left image. 
/// \param	rightImage		The right image. 
/// \param	borderLeft		The border left. 
/// \param	borderRight		The border right. 
/// \param	topImage		The top image. 
/// \param	bottomImage		The bottom image. 
/// \param	borderTop		The border top. 
/// \param	borderBottom	The border bottom. 
///
/// \return	The text rectangle. 
////////////////////////////////////////////////////////////////////////////////////////////////////

RECT CControl::GetTextRect(int leftImage, int rightImage, int borderLeft, int borderRight, int topImage, int bottomImage, int borderTop, int borderBottom)
{
	int imageWidthLeft = 0;
	int imageWidthRight = 0;
	int imageHeightTop = 0;
	int imageHeightBottom = 0;
	SallyAPI::GUI::CPicture* image = GetPicture(leftImage);
	if (image != NULL) {
		imageWidthLeft = image->GetWidth();
	}
	image = GetPicture(rightImage);
	if (image != NULL) {
		imageWidthRight = image->GetWidth();
	}
	image = GetPicture(topImage);
	if (image != NULL) {
		imageHeightTop = image->GetHeight();
	}
	image = GetPicture(bottomImage);
	if (image != NULL) {
		imageHeightBottom = image->GetHeight();
	}

	RECT r;
	r.left = m_iXAbsolut + imageWidthLeft + borderLeft;
	r.right = r.left + (m_iWidth - (imageWidthLeft + imageWidthRight + borderLeft + borderRight));
	r.top = m_iYAbsolut + imageHeightTop + borderTop;
	r.bottom = r.top + m_iHeight - (imageHeightTop + borderTop + imageHeightBottom + borderBottom);

	return r;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::SetText(const std::string& text)
///
/// \brief	Sets a text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	text	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::SetText(const std::string& text)
{
	SallyAPI::GUI::CGUIBaseObject::SetText(text);

	if (m_bLocalised)
	{
		// to load the option ini faster...
		SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
		SallyAPI::Config::CLanguageManager* lang = config->GetLanguageLocalization();

		lang->GetString(m_strText);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::DrawText(int leftImage, int rightImage, int borderLeft, int borderRight,
/// const std::string& fontName, const std::string& text)
///
/// \brief	Draw text. 
///
/// \author	Christian Knobloch
/// \date	04.05.2010
///
/// \param	leftImage	The left image. 
/// \param	rightImage	The right image. 
/// \param	borderLeft	The border left. 
/// \param	borderRight	The border right. 
/// \param	fontName	Name of the font. 
/// \param	text		The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::DrawText(int leftImage, int rightImage, int borderLeft, int borderRight, const std::string& fontName, const std::string& text)
{
	DrawText(leftImage, rightImage, borderLeft, borderRight, GUI_NO_IMAGE, GUI_NO_IMAGE, 0, 0, fontName, text);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::SetColor(D3DCOLOR rgbFontColour)
///
/// \brief	Sets a color. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	rgbFontColour	The rgb font colour. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::SetColor(D3DCOLOR rgbFontColour)
{
	m_rgbFontColour = rgbFontColour;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::SetAlign(int align)
///
/// \brief	Sets an align. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	align	The align. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::SetAlign(int align)
{
	m_iAlign = align;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::SetLocalised(bool localised)
///
/// \brief	Sets a localised. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	localised	true to localised. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::SetLocalised(bool localised)
{
	m_bLocalised = localised;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CControl::GetControlId()
///
/// \brief	Gets the control identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The control identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CControl::GetControlId()
{
	 return m_iControlId;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CControl::IsLocalised()
///
/// \brief	Query if this object is localised. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if localised, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CControl::IsLocalised()
{
	return m_bLocalised;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::SetRotationX(ControlRotation value)
///
/// \brief	Sets a rotation x coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::SetRotationX(ControlRotation value)
{
	m_eRotationX = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::SetRotationY(ControlRotation value)
///
/// \brief	Sets a rotation y coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::SetRotationY(ControlRotation value)
{
	m_eRotationY = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CControl::SetRotationZ(ControlRotation value)
///
/// \brief	Sets a rotation z coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CControl::SetRotationZ(ControlRotation value)
{
	m_eRotationZ = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	ControlRotation CControl::GetRotationX()
///
/// \brief	Gets the rotation x coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The rotation x coordinate coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

ControlRotation CControl::GetRotationX()
{
	return m_eRotationX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	ControlRotation CControl::GetRotationY()
///
/// \brief	Gets the rotation y coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The rotation y coordinate coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

ControlRotation CControl::GetRotationY()
{
	return m_eRotationY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	ControlRotation CControl::GetRotationZ()
///
/// \brief	Gets the rotation z coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The rotation z coordinate coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

ControlRotation CControl::GetRotationZ()
{
	return m_eRotationZ;
}

void CControl::InvalidateControl()
{
	SendMessageToParent(this, m_iControlId, MS_INVALIDATE_CONTROL);
}