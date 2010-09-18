////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\3DCircle.cpp
///
/// \brief	Implements the 3 d circle class. 
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

#include "3DCircle.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	C3DCircle::C3DCircle(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int iconSize) :SallyAPI::GUI::CForm(parent, x, y, width, (int) (width * 0.5)),
/// m_iIconSize(iconSize), m_fWinkelCorrection(0), m_bMoveAnimation(false),
/// m_iMoveAnimationSpeed(30), m_fDeltaStart(0)
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
/// \param	iconSize		Size of the icon. 
////////////////////////////////////////////////////////////////////////////////////////////////////

C3DCircle::C3DCircle(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int iconSize)
	:SallyAPI::GUI::CForm(parent, x, y, width, (int) (width * 0.5)), m_iIconSize(iconSize),
	m_fWinkelCorrection(0), m_bMoveAnimation(false), m_iMoveAnimationSpeed(30), m_fDeltaStart(0)
{
	this->SetScrollType(SallyAPI::GUI::SCROLL_TYPE_SMOOTH);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	C3DCircle::~C3DCircle()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

C3DCircle::~C3DCircle()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void C3DCircle::AddChild(SallyAPI::GUI::CControl* control)
///
/// \brief	Adds a child. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	control	If non-null, the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void C3DCircle::AddChild(SallyAPI::GUI::CControl* control)
{
	SallyAPI::GUI::CForm::AddChild(control);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void C3DCircle::AddItem(int image, const std::string& text, int controlId)
///
/// \brief	Adds an item. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	image		The image. 
/// \param	text		The text. 
/// \param	controlId	Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void C3DCircle::AddItem(int image, const std::string& text, int controlId)
{
	SallyAPI::GUI::CButton* temp = new SallyAPI::GUI::CButton(this, 0 , 0, 128, 128,
		controlId, SallyAPI::GUI::BUTTON_TYPE_ONLY_IMAGE);
	temp->SetImageId(image);
	this->AddChild(temp);
	m_ApplicationButtons.push_back(temp);

	SallyAPI::GUI::CLabel* tempName = new SallyAPI::GUI::CLabel(this, 0 , 0, 200);
	tempName->SetLocalised(false);
	tempName->SetAlign(DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	tempName->SetText(text);
	tempName->SetFont("3dcircel.font");
	this->AddChild(tempName);
	m_ApplicationLabels.push_back(tempName);

	m_iWinkelSize = (float) 360 / m_ApplicationButtons.size();
	UpdateRotation();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void C3DCircle::UpdateRotation()
///
/// \brief	Updates a rotation. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void C3DCircle::UpdateRotation()
{
	if (m_ApplicationButtons.size() == 0)
		return;

	float winkelCurrent = m_fWinkelCorrection;
	float radius = (float) (m_iWidth / 2) - m_iIconSize;

	std::vector<SallyAPI::GUI::CButton*>::iterator iter = m_ApplicationButtons.begin();

	int i = 0;
	while (iter != m_ApplicationButtons.end())
	{
		SallyAPI::GUI::CButton* temp = *iter;

		float winkel2 = (float) (M_PI / 180 * winkelCurrent);
		int b = (int) (radius * sin(winkel2));
		int c = (int) (radius * cos(winkel2));

		float k = (float) (c + (radius * 3));
		float l = (float) (k / (radius * 4));
		int size = (int) (m_iIconSize * l);
		temp->Resize(size, size);

		int moveXIcon = (int) (b + (m_iWidth / 2) - (size / 2));
		int moveYIcon = (int) ((c / 2.2) + ((m_iHeight - 100) / 2.2));
		temp->Move(moveXIcon, moveYIcon);

		// set the Label
		SallyAPI::GUI::CLabel* tempName = m_ApplicationLabels.at(i);
		int moveXLabel = (int) (b + (m_iWidth / 2) - 100);
		int moveYLabel = (int) ((c / 2.2) + ((m_iHeight - 100) / 2.2) + size + 10);
		tempName->Move(moveXLabel, moveYLabel);

		winkelCurrent += m_iWinkelSize;
		++iter;
		++i;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool C3DCircle::ProcessMouseMove(int x, int y)
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

bool C3DCircle::ProcessMouseMove(int x, int y)
{
	if (y < m_iYAbsolut + (m_iHeight / 2))
		m_bMovedAtTopX = true;
	else
		m_bMovedAtTopX = false;

	if (x < m_iXAbsolut + (m_iWidth / 2))
		m_bMovedAtTopY = true;
	else
		m_bMovedAtTopY = false;

	return SallyAPI::GUI::CForm::ProcessMouseMove(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void C3DCircle::MoveMenuX(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Move menu x coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void C3DCircle::MoveMenuX(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterInteger* integer = (SallyAPI::GUI::SendMessage::CParameterInteger*) messageParameter;
	int moveX = integer->GetInteger();

	if (m_bMovedAtTopX)
		moveX = moveX * -1;

	m_fWinkelCorrection += moveX / 4;

	UpdateRotation();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void C3DCircle::MoveMenuY(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Move menu y coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void C3DCircle::MoveMenuY(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterInteger* integer = (SallyAPI::GUI::SendMessage::CParameterInteger*) messageParameter;
	int moveY = integer->GetInteger();

	if (!m_bMovedAtTopY)
		moveY = moveY * -1;

	m_fWinkelCorrection += moveY / 4;

	UpdateRotation();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void C3DCircle::MoveMenuFastXLeft(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Move menu fast x coordinate left. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void C3DCircle::MoveMenuFastXLeft(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	m_bMoveAnimation = true;
	m_fDeltaStart = 0;

	SallyAPI::GUI::SendMessage::CParameterInteger* integer = (SallyAPI::GUI::SendMessage::CParameterInteger*) messageParameter;
	m_iMoveAnimationSpeed = -integer->GetInteger();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void C3DCircle::MoveMenuFastXRight(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Move menu fast x coordinate right. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void C3DCircle::MoveMenuFastXRight(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	m_bMoveAnimation = true;
	m_fDeltaStart = 0;

	SallyAPI::GUI::SendMessage::CParameterInteger* integer = (SallyAPI::GUI::SendMessage::CParameterInteger*) messageParameter;
	m_iMoveAnimationSpeed = integer->GetInteger();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void C3DCircle::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
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

void C3DCircle::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId,
									SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch(messageId)
	{
	case GUI_MOUSEMOVE_SMOOTH_LEFT:
	case GUI_MOUSEMOVE_SMOOTH_RIGHT:
		MoveMenuX(messageParameter);
		return;
	case GUI_MOUSEMOVE_SMOOTH_UP:
	case GUI_MOUSEMOVE_SMOOTH_DOWN:
		MoveMenuY(messageParameter);
		return;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool C3DCircle::ProcessMouseDown(int x, int y)
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

bool C3DCircle::ProcessMouseDown(int x, int y)
{
	m_bMoveAnimation = false;
	return CForm::ProcessMouseDown(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void C3DCircle::Timer(float timeDelta)
///
/// \brief	Timers. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	timeDelta	The time delta. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void C3DCircle::Timer(float timeDelta)
{
	SallyAPI::GUI::CForm::Timer(timeDelta);

	if (m_bMoveAnimation)
	{
		if (m_fDeltaStart == 0)
		{
			m_fDeltaStart = m_fTimeDelta;
			m_fDeltaLastDifferents = 0;
			m_fDeltaRemainder = 0;
		}
		float fDeltaDifferents = m_fTimeDelta - m_fDeltaStart;
		float fDeltaCalculate = fDeltaDifferents - m_fDeltaLastDifferents + m_fDeltaRemainder;
		int useForCalc = (int) fDeltaCalculate * 100;

		m_fWinkelCorrection = m_fWinkelCorrection 
			+ (float) (m_iMoveAnimationSpeed * useForCalc);

		m_iMoveAnimationSpeed = m_iMoveAnimationSpeed - (useForCalc);
		
		if (m_iMoveAnimationSpeed <= 0)
		{
			m_bMoveAnimation = false;
		}

		UpdateRotation();

		m_fDeltaLastDifferents = fDeltaDifferents;
		m_fDeltaRemainder = (float) ((fDeltaCalculate * 100) - (fDeltaCalculate * 100));
	}
}