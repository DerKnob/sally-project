////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Rating.cpp
///
/// \brief	Implements the rating class. 
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

#include "Rating.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CRating::CRating(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y,
/// SallyAPI::GUI::RatingType ratingType, int controlId) :SallyAPI::GUI::CForm(parent, x, y,
/// CONTROL_HEIGHT * 5, CONTROL_HEIGHT, controlId), m_iRatingTemp(0), m_iRating(0),
/// m_eRatingType(ratingType)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	20.01.2012
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	ratingType		Type of the rating. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CRating::CRating(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, SallyAPI::GUI::RatingType ratingType,
				 int controlId)
	:SallyAPI::GUI::CForm(parent, x, y, CONTROL_HEIGHT * 5, CONTROL_HEIGHT, controlId), m_iRatingTemp(0), m_iRating(0),
	m_eRatingType(ratingType)
{
	if (m_eRatingType == RATING_TYPE_SELECT)
	{
		for (int i = 0; i < 5; ++i)
		{
			m_pImages[i] = new SallyAPI::GUI::CImageBox(this, ((i + 1) * 4) + (i * 22), 4, 22, 22, i);
			m_pImages[i]->SetImageId(GUI_THEME_SALLY_ICON_RATING_OFF);
			this->AddChild(m_pImages[i]);
		}
	}
	else
	{
		this->Enable(false);
		this->Resize((CONTROL_HEIGHT / 2) * 5, CONTROL_HEIGHT / 2);

		for (int i = 0; i < 5; ++i)
		{
			m_pImages[i] = new SallyAPI::GUI::CImageBox(this, ((i + 1) * 2) + (i * 11), 2, 11, 11, i);
			m_pImages[i]->SetImageId(GUI_THEME_SALLY_ICON_RATING_OFF);
			this->AddChild(m_pImages[i]);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CRating::~CRating()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CRating::~CRating()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CRating::GetRating()
///
/// \brief	Gets the rating. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The rating. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CRating::GetRating()
{
	return m_iRating - 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CRating::SetRating(int rating)
///
/// \brief	Sets a rating. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	rating	The rating. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CRating::SetRating(int rating)
{
	m_iRating = rating + 1;
	UpdateRating();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CRating::ProcessMouseUp(int x, int y)
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

bool CRating::ProcessMouseUp(int x, int y)
{
	if (m_bMouseDown)
	{
		m_iRatingTemp = 0;

		int xClicked = x - m_iXAbsolut;
		m_iRating = (xClicked / 26) + 2;

		UpdateRating();

		SallyAPI::GUI::SendMessage::CParameterInteger parameterInterger(m_iRating);
		m_pParent->SendMessageToParent(this, m_iControlId, GUI_RATING_CHANGED, &parameterInterger);
	}

	SallyAPI::GUI::CForm::ProcessMouseUp(x, y);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CRating::ProcessMouseDown(int x, int y)
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

bool CRating::ProcessMouseDown(int x, int y)
{
	int xClicked = x - m_iXAbsolut;
	m_iRatingTemp = (xClicked / 26) + 2;

	UpdateRating();

	SallyAPI::GUI::CForm::ProcessMouseDown(x, y);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CRating::ProcessMouseMove(int x, int y)
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

bool CRating::ProcessMouseMove(int x, int y)
{
	if (!m_bMouseDown)
		return false;

	int xClicked = x - m_iXAbsolut;

	if (xClicked < 0)
		xClicked = 0;
	else if (xClicked > m_iWidth)
		xClicked = m_iWidth;

	m_iRatingTemp = (xClicked / 26) + 2;

	UpdateRating();
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CRating::UpdateRating()
///
/// \brief	Updates a rating. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CRating::UpdateRating()
{
	int rating;

	if (m_iRatingTemp > 0)
		rating = m_iRatingTemp;
	else
		rating = m_iRating;

	for (int i = 0; i < 5; ++i)
	{
		if (rating <= i + 1)
			m_pImages[i]->SetImageId(GUI_THEME_SALLY_ICON_RATING_OFF);
		else 
			m_pImages[i]->SetImageId(GUI_THEME_SALLY_ICON_RATING_ON);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CRating::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId,
/// int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to childs. 
///
/// \author	Christian Knobloch
/// \date	08.06.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CRating::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_MESSAGE_MOUSE_UP_INFO:
		if (m_bMouseDown)
		{
			m_bMouseDown = false;
			m_iRatingTemp = 0;
			UpdateRating();
		}
		return;
	}

	SallyAPI::GUI::CForm::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
}