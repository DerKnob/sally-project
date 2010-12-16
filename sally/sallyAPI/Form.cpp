////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Form.cpp
///
/// \brief	Implements the form class. 
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

#include "Form.h"

using namespace SallyAPI::GUI;

#define LAST_VALUES 5

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CForm::CForm(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height,
/// int controlId) :SallyAPI::GUI::CControl(parent, x, y, width, height, controlId),
/// m_iSetMessage(0), m_fStartSendMessage(0), m_fStartMouseDown(0),
/// m_eScrollType(SallyAPI::GUI::SCROLL_TYPE_NONE), m_iScrollSmoothMouseOldX(-1),
/// m_iScrollSmoothMouseOldY(-1), m_bHandleInputIfItIsChildHandled(true),
/// m_bAlwaysHandleInput(false), m_bStopOnFirstSuccess(false), m_bScrolling(false)
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

CForm::CForm(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId)
	:SallyAPI::GUI::CControl(parent, x, y, width, height, controlId), m_iSetMessage(0), m_fStartSendMessage(0),
	m_fStartMouseDown(0), m_eScrollType(SallyAPI::GUI::SCROLL_TYPE_NONE), m_iScrollSmoothMouseOldX(-1),
	m_iScrollSmoothMouseOldY(-1), m_bHandleInputIfItIsChildHandled(true), m_bAlwaysHandleInput(false),
	m_bStopOnFirstSuccess(false), m_bScrolling(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CForm::~CForm()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CForm::~CForm()
{
	std::list<CControl*>::iterator	iter = m_GUIControlList.begin();

	while (iter != m_GUIControlList.end())
	{
		CControl* control = *iter;
		SafeDelete(control);
		m_GUIControlList.erase(iter);

		iter = m_GUIControlList.begin();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CForm::Render()
///
/// \brief	Renders this object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CForm::Render()
{
	if ((m_bVisible) && (m_iAlphaBlending > 0))
	{
		// is it on the screen?
		if ((m_iX + m_iWidth > 0) && (m_iX < WINDOW_WIDTH) &&
			(m_iY + m_iHeight > 0) && (m_iY < WINDOW_HEIGHT))
		{
			EnterRenderLock();

			RenderControl();

			LeaveRenderLock();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CForm::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CForm::RenderControl()
{
	std::list<CControl*>::iterator	iter;
	iter = m_GUIControlList.begin();
	while (iter != m_GUIControlList.end())
	{
		CControl* control = *iter;
		control->Render();
		++iter;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CForm::Update()
///
/// \brief	Updates this object. 
///
/// \author	Christian Knobloch
/// \date	30.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CForm::Update()
{
	if ((m_bVisible) && (m_iAlphaBlending > 0))
	{
		// is it on the screen?
		if ((m_iX + m_iWidth > 0) && (m_iX < WINDOW_WIDTH) &&
			(m_iY + m_iHeight > 0) && (m_iY < WINDOW_HEIGHT))
		{
			EnterRenderLock();

			UpdateControl();

			LeaveRenderLock();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CForm::UpdateControl()
///
/// \brief	Updates a control. 
///
/// \author	Christian Knobloch
/// \date	30.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CForm::UpdateControl()
{
	std::list<CControl*>::iterator	iter;
	iter = m_GUIControlList.begin();
	while (iter != m_GUIControlList.end())
	{
		CControl* control = *iter;
		control->Update();
		++iter;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CForm::AddChild(CControl* control)
///
/// \brief	Adds a child. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	control	If non-null, the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CForm::AddChild(CControl* control)
{
	m_GUIControlList.push_back(control);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CForm::RemoveChild(std::list<CControl*>::iterator iter)
///
/// \brief	Removes the child described by iter. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	iter	If non-null, the iterator. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CForm::RemoveChild(std::list<CControl*>::iterator iter)
{
	m_GUIControlList.erase(iter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CForm::SetScrollType(SallyAPI::GUI::ScrollType scrollType)
///
/// \brief	Sets a scroll type. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	scrollType	Type of the scroll. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CForm::SetScrollType(SallyAPI::GUI::ScrollType scrollType)
{
	m_eScrollType = scrollType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CForm::IsScrolling()
///
/// \brief	Query if this object is scrolling. Only works with Smooth Scroll.
///
/// \author	Christian Knobloch
/// \date	27.04.2010
///
/// \return	true if scrolling, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CForm::IsScrolling()
{
	return m_bScrolling;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CForm::SetAlphaBlending(int alphaBlending)
///
/// \brief	Sets an alpha blending. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	alphaBlending	The alpha blending. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CForm::SetAlphaBlending(int alphaBlending)
{
	CControl::SetAlphaBlending(alphaBlending);

	std::list<CControl*>::iterator iter = m_GUIControlList.begin();
	while (iter != m_GUIControlList.end())
	{
		CControl* control = *iter;
		control->SetAlphaBlending(alphaBlending);
		++iter;
	}
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CForm::SetHandleInputIfItIsChildHandled(bool handleInputIfItIsChildHandled)
///
/// \brief	Sets a handle input if iterator is child handled. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	handleInputIfItIsChildHandled	true if handle input if iterator is child handled. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CForm::SetHandleInputIfItIsChildHandled(bool handleInputIfItIsChildHandled)
{
	m_bHandleInputIfItIsChildHandled = handleInputIfItIsChildHandled;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CForm::SetAlwaysHandleInput(bool alwaysHandleInput)
///
/// \brief	Sets the always handle input. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	alwaysHandleInput	true to always handle input. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CForm::SetAlwaysHandleInput(bool alwaysHandleInput)
{
	m_bAlwaysHandleInput = alwaysHandleInput;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CForm::SetStopHandleInputOnFirstSuccess(bool stopOnFirstSuccess)
///
/// \brief	Sets a stop handle input on first success. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	stopOnFirstSuccess	true if the stop on first operation was a success, false if it
/// 							failed. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CForm::SetStopHandleInputOnFirstSuccess(bool stopOnFirstSuccess)
{
	m_bStopOnFirstSuccess = stopOnFirstSuccess;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CForm::ProcessMouseDown(int x, int y)
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

bool CForm::ProcessMouseDown(int x, int y)
{
	bool handled = false;
	
	CControl::ProcessMouseDown(x, y);

	m_vScrollSmoothLastX.clear();
	m_vScrollSmoothLastY.clear();
	m_vScrollSmoothLastTime.clear();

	m_fStartMouseDown = m_fTimeDelta;

	std::list<CControl*>::iterator iter = m_GUIControlList.begin();
	while (iter != m_GUIControlList.end())
	{
		CControl* control = *iter;
		if (control->CheckProcessMouseDown(x, y))
			handled = true;
		++iter;

		if ((handled) && (m_bStopOnFirstSuccess))
			return true;			
	}

	if ((!m_bHandleInputIfItIsChildHandled) && (handled))
		return true;

	if (m_bAlwaysHandleInput)
		return true;
	return handled;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CForm::ProcessMouseUp(int x, int y)
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

bool CForm::ProcessMouseUp(int x, int y)
{
	if (m_bMouseDown == false)
		return false;
	
	m_bScrolling = false;
	bool handled = false;

	CControl::ProcessMouseUp(x, y);

	std::list<SallyAPI::GUI::CControl*>::iterator iter = m_GUIControlList.begin();
	while (iter != m_GUIControlList.end())
	{
		CControl* control = *iter;
		if (control->CheckProcessMouseUp(x, y))
			handled = true;
		++iter;

		if ((handled) && (m_bStopOnFirstSuccess))
			return true;
	}

	if ((!m_bHandleInputIfItIsChildHandled) && (handled))
		return true;

	SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
	this->SendMessageToParent(this, m_iControlId, GUI_FORM_CLICKED, &message);

	if (m_bAlwaysHandleInput)
		return true;
	return handled;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CForm::ProcessMouseDoubleClick(int x, int y)
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

bool CForm::ProcessMouseDoubleClick(int x, int y)
{
	bool handled = false;

	CControl::ProcessMouseDoubleClick(x, y);

	std::list<SallyAPI::GUI::CControl*>::iterator iter = m_GUIControlList.begin();
	while (iter != m_GUIControlList.end())
	{
		CControl* control = *iter;
		if (control->CheckProcessMouseDoubleClick(x, y))
			handled = true;
		++iter;

		if ((handled) && (m_bStopOnFirstSuccess))
			return true;
	}

	if ((!m_bHandleInputIfItIsChildHandled) && (handled))
		return true;

	this->SendMessageToParent(this, m_iControlId, GUI_FORM_DOUBLECLICKED);

	if (m_bAlwaysHandleInput)
		return true;
	return handled;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CForm::Timer(float timeDelta)
///
/// \brief	Timers. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	timeDelta	The time delta. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CForm::Timer(float timeDelta)
{
	CControl::Timer(timeDelta);

	// Send Timer Event to all Childs
	std::list<CControl*>::iterator iter = m_GUIControlList.begin();
	while (iter !=  m_GUIControlList.end())
	{
		CControl* control = *iter;
		control->Timer(timeDelta);
		++iter;
	}

	if (m_eScrollType == SallyAPI::GUI::SCROLL_TYPE_NORMAL)
	{
		TimerScrollNormal();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CForm::TimerScrollNormal()
///
/// \brief	Timer scroll normal. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CForm::TimerScrollNormal()
{
	if ((m_fStartSendMessage != 0) && (m_iSetMessage != 0))
	{
		float check = ((float) 0.4);
		if (m_iSetMessage >= GUI_MOUSEMOVE_RIGHT_FAST_SNAPED)
			check = ((float) 0.2);

		if (m_fTimeDelta - m_fStartSendMessage > check)
		{
			m_fStartSendMessage = m_fTimeDelta;
			this->SendMessageToParent(this, m_iControlId, m_iSetMessage);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CForm::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject *reporter, int reporterId,
/// int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to parent. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CForm::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject *reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::CControl::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CForm::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId,
/// int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to childs. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CForm::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_MESSAGE_MOUSE_UP_INFO:
		if (m_iSetMessage != 0)
		{
			m_iSetMessage = 0;
			m_fStartSendMessage = 0;
		}
		m_iMouseDownX = -1;
		m_iMouseDownY = -1;
		m_iScrollSmoothMouseOldX = -1;
		m_iScrollSmoothMouseOldY = -1;
		m_bMouseDown = false;
		break;
	}

	std::list<SallyAPI::GUI::CControl*>::iterator iter = m_GUIControlList.begin();
	while (iter != m_GUIControlList.end())
	{
		SallyAPI::GUI::CControl* control = *iter;
		control->SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
		++iter;
	}
	SallyAPI::GUI::CControl::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CForm::ProcessMouseMove(int x, int y)
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

bool CForm::ProcessMouseMove(int x, int y)
{
	bool handled = false;

	std::list<CControl*>::iterator iter = m_GUIControlList.begin();
	while (iter != m_GUIControlList.end())
	{
		CControl* control = *iter;
		if (control->CheckProcessMouseMove(x, y))
			handled = true;
		++iter;
	}

	// we can't use this... :-(
// 	if ((!m_bHandleInputIfItIsChildHandled) && (handled))
// 		return true;

	if (m_eScrollType == SallyAPI::GUI::SCROLL_TYPE_NORMAL)
	{
		return ScrollTypeNormal(x, y);
	}
	
	if (m_eScrollType == SallyAPI::GUI::SCROLL_TYPE_SMOOTH)
	{
		return ScrollTypeSmooth(x, y);
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CForm::ScrollTypeSmooth(int x, int y)
///
/// \brief	Scroll type smooth. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CForm::ScrollTypeSmooth(int x, int y)
{
	if (m_bMouseDown)
	{
		if (MoveX(x))
			m_bScrolling = true;

		if (MoveY(y))
			m_bScrolling = true;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CForm::MoveX(int x)
///
/// \brief	Move x coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CForm::MoveX(int x)
{
	// check x

	// have we already started?
	if (m_iScrollSmoothMouseOldX == -1)
	{
		// if no than test if we have at last moved a little bit
		if (((x - m_iMouseDownX) < 15) && ((x - m_iMouseDownX) > -15))
		{
			// not enough moved
			return false;
		}
		m_iScrollSmoothMouseOldX += m_iMouseDownX;
	}

	// ok, than lets start...
	if (m_iScrollSmoothMouseOldX == -1)
	{
		m_iScrollSmoothMouseOldX = m_iMouseDownX;
	}
	int moveValue = (x - m_iScrollSmoothMouseOldX);

	SallyAPI::GUI::SendMessage::CParameterInteger intergerX(moveValue);

	if (moveValue > 0)
	{
		this->SendMessageToParent(this, GetControlId(), GUI_MOUSEMOVE_SMOOTH_RIGHT, &intergerX);
	}
	if (moveValue < 0)
	{
		this->SendMessageToParent(this, GetControlId(), GUI_MOUSEMOVE_SMOOTH_LEFT, &intergerX);
	}
	m_iScrollSmoothMouseOldX = x;

	m_vScrollSmoothLastX.push_back(x);
	if (m_vScrollSmoothLastX.size() > LAST_VALUES)
	{
		m_vScrollSmoothLastX.erase(m_vScrollSmoothLastX.begin());
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CForm::MoveY(int y)
///
/// \brief	Move y coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	y	The y coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CForm::MoveY(int y)
{
	// check y

	// have we already started?
	if (m_iScrollSmoothMouseOldY == -1)
	{
		// if no than test if we have at last moved a little bit
		if (((y - m_iMouseDownY) < 15) && ((y - m_iMouseDownY) > -15))
		{
			// not enough moved
			return false;
		}
		m_iScrollSmoothMouseOldY += m_iMouseDownY;
	}

	// ok, than lets start...
	if (m_iScrollSmoothMouseOldY == -1)
	{
		m_iScrollSmoothMouseOldY = m_iMouseDownY;
	}
	int moveValue = (y - m_iScrollSmoothMouseOldY);

	SallyAPI::GUI::SendMessage::CParameterInteger intergerY(moveValue);

	if (moveValue > 0)
	{
		SendMessageToParent(this, GetControlId(), GUI_MOUSEMOVE_SMOOTH_DOWN, &intergerY);
	}
	if (moveValue < 0)
	{
		SendMessageToParent(this, GetControlId(), GUI_MOUSEMOVE_SMOOTH_UP, &intergerY);
	}
	m_iScrollSmoothMouseOldY = y;

	m_vScrollSmoothLastY.push_back(y);
	if (m_vScrollSmoothLastY.size() > LAST_VALUES)
	{
		m_vScrollSmoothLastY.erase(m_vScrollSmoothLastY.begin());
	}

	// take the time
	m_vScrollSmoothLastTime.push_back(m_fTimeDelta);
	if (m_vScrollSmoothLastTime.size() > LAST_VALUES)
	{
		m_vScrollSmoothLastTime.erase(m_vScrollSmoothLastTime.begin());
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CForm::ScrollTypeNormal(int x, int y)
///
/// \brief	Scroll type normal. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CForm::ScrollTypeNormal(int x, int y)
{
	if ((!m_bMouseDown) || (m_iSetMessage != 0))
		return SallyAPI::GUI::CControl::ProcessMouseMove(x, y);

	if ((m_iMouseDownX == -1) && (m_iMouseDownY == -1))
	{
		SallyAPI::GUI::CControl::ProcessMouseMove(x, y);

		std::list<SallyAPI::GUI::CControl*>::iterator iter = m_GUIControlList.begin();
		while (iter != m_GUIControlList.end())
		{
			SallyAPI::GUI::CControl* control = *iter;
			control->CheckProcessMouseMove(x, y);
			++iter;
		}
		return false;
	}

	// Move right
	if ((x - m_iMouseDownX) > 100)
	{
		if (((y - m_iMouseDownY) < 50) && ((y - m_iMouseDownY) > -50))
		{
			m_bMouseDown = false;
			if (m_iSetMessage == 0)
			{
				if (m_fTimeDelta - m_fStartMouseDown < 0.25)
				{
					SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
					SendMessageToParent(this, m_iControlId, GUI_MOUSEMOVE_RIGHT_FAST, &message);
					m_iSetMessage = GUI_MOUSEMOVE_RIGHT_FAST_SNAPED;
				}
				else
				{
					SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
					SendMessageToParent(this, m_iControlId, GUI_MOUSEMOVE_RIGHT, &message);
					m_iSetMessage = GUI_MOUSEMOVE_RIGHT_SNAPED;
				}
				m_fStartSendMessage = m_fTimeDelta;
			}
			return true;
		}
	}

	// Move left
	if ((x - m_iMouseDownX) < -100)
	{
		if (((y - m_iMouseDownY) < 50) && ((y - m_iMouseDownY) > -50))
		{
			m_bMouseDown = false;
			if (m_iSetMessage == 0)
			{
				if (m_fTimeDelta - m_fStartMouseDown < 0.25)
				{
					SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
					SendMessageToParent(this, m_iControlId, GUI_MOUSEMOVE_LEFT_FAST, &message);
					m_iSetMessage = GUI_MOUSEMOVE_LEFT_FAST_SNAPED;
				}
				else
				{
					SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
					SendMessageToParent(this, m_iControlId, GUI_MOUSEMOVE_LEFT, &message);
					m_iSetMessage = GUI_MOUSEMOVE_LEFT_SNAPED;
				}
				m_fStartSendMessage = m_fTimeDelta;
			}
			return true;
		}
	}

	// Move down
	if ((y - m_iMouseDownY) > 100)
	{
		if (((x - m_iMouseDownX) < 50) && ((x - m_iMouseDownX) > -50))
		{
			m_bMouseDown = false;
			if (m_iSetMessage == 0)
			{
				if (m_fTimeDelta - m_fStartMouseDown < 0.25)
				{
					SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
					SendMessageToParent(this, m_iControlId, GUI_MOUSEMOVE_DOWN_FAST, &message);
					m_iSetMessage = GUI_MOUSEMOVE_DOWN_FAST_SNAPED;
				}
				else
				{
					SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
					SendMessageToParent(this, m_iControlId, GUI_MOUSEMOVE_DOWN, &message);
					m_iSetMessage = GUI_MOUSEMOVE_DOWN_SNAPED;
				}
				m_fStartSendMessage = m_fTimeDelta;
			}
			return true;
		}
	}

	// Move up
	if ((y - m_iMouseDownY) < -100)
	{
		if (((x - m_iMouseDownX) < 50) && ((x - m_iMouseDownX) > -50))
		{
			m_bMouseDown = false;
			if (m_iSetMessage == 0)
			{
				if (m_fTimeDelta - m_fStartMouseDown < 0.25)
				{
					SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
					SendMessageToParent(this, m_iControlId, GUI_MOUSEMOVE_UP_FAST, &message);
					m_iSetMessage = GUI_MOUSEMOVE_UP_FAST_SNAPED;
				}
				else
				{
					SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
					SendMessageToParent(this, m_iControlId, GUI_MOUSEMOVE_UP, &message);
					m_iSetMessage = GUI_MOUSEMOVE_UP_SNAPED;
				}
				m_fStartSendMessage = m_fTimeDelta;
			}
			return true;
		}
	}



	// Move right down
	if ((x - m_iMouseDownX) > 100)
	{
		if ((y - m_iMouseDownY) > 100)
		{
			m_bMouseDown = false;
			if (m_iSetMessage == 0)
			{
				if (m_fTimeDelta - m_fStartMouseDown < 0.25)
				{
					SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
					SendMessageToParent(this, m_iControlId, GUI_MOUSEMOVE_RIGHT_DOWN_FAST, &message);
					m_iSetMessage = GUI_MOUSEMOVE_RIGHT_DOWN_FAST_SNAPED;
				}
				else
				{
					SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
					SendMessageToParent(this, m_iControlId, GUI_MOUSEMOVE_RIGHT_DOWN, &message);
					m_iSetMessage = GUI_MOUSEMOVE_RIGHT_DOWN_SNAPED;
				}
				m_fStartSendMessage = m_fTimeDelta;
			}
			return true;
		}
	}

	// Move right up
	if ((x - m_iMouseDownX) > 100)
	{
		if ((y - m_iMouseDownY) < 100)
		{
			m_bMouseDown = false;
			if (m_iSetMessage == 0)
			{
				if (m_fTimeDelta - m_fStartMouseDown < 0.25)
				{
					SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
					SendMessageToParent(this, m_iControlId, GUI_MOUSEMOVE_RIGHT_UP_FAST, &message);
					m_iSetMessage = GUI_MOUSEMOVE_RIGHT_UP_FAST_SNAPED;
				}
				else
				{
					SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
					SendMessageToParent(this, m_iControlId, GUI_MOUSEMOVE_RIGHT_UP, &message);
					m_iSetMessage = GUI_MOUSEMOVE_RIGHT_UP_SNAPED;
				}
				m_fStartSendMessage = m_fTimeDelta;
			}
			return true;
		}
	}

	// Move left down
	if ((x - m_iMouseDownX) < -100)
	{
		if ((y - m_iMouseDownY) > 100)
		{
			m_bMouseDown = false;
			if (m_iSetMessage == 0)
			{
				if (m_fTimeDelta - m_fStartMouseDown < 0.25)
				{
					SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
					SendMessageToParent(this, m_iControlId, GUI_MOUSEMOVE_LEFT_DOWN_FAST, &message);
					m_iSetMessage = GUI_MOUSEMOVE_LEFT_DOWN_FAST_SNAPED;
				}
				else
				{
					SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
					SendMessageToParent(this, m_iControlId, GUI_MOUSEMOVE_LEFT_DOWN, &message);
					m_iSetMessage = GUI_MOUSEMOVE_LEFT_DOWN_SNAPED;
				}
				m_fStartSendMessage = m_fTimeDelta;
			}
			return true;
		}
	}

	// Move left up
	if ((x - m_iMouseDownX) < -100)
	{
		if ((y - m_iMouseDownY) < -100)
		{
			m_bMouseDown = false;
			if (m_iSetMessage == 0)
			{
				if (m_fTimeDelta - m_fStartMouseDown < 0.25)
				{
					SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
					SendMessageToParent(this, m_iControlId, GUI_MOUSEMOVE_LEFT_UP_FAST, &message);
					m_iSetMessage = GUI_MOUSEMOVE_LEFT_UP_FAST_SNAPED;
				}
				else
				{
					SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
					SendMessageToParent(this, m_iControlId, GUI_MOUSEMOVE_LEFT_UP, &message);
					m_iSetMessage = GUI_MOUSEMOVE_LEFT_UP_SNAPED;
				}
				m_fStartSendMessage = m_fTimeDelta;
			}
			return true;
		}
	}
	return false;
}