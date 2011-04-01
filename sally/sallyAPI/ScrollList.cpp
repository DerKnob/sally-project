////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ScrollList.cpp
///
/// \brief	Implements the scroll list class. 
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

#include "ScrollList.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScrollList::CScrollList(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int height, int iconSize, int iconSizeSpace, int controlId) :SallyAPI::GUI::CForm(parent, x,
/// y, width, height, controlId), m_iIconSize(iconSize), m_iIconSizeSpace(iconSizeSpace),
/// m_iStartPicture(0)
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
/// \param	iconSize		Size of the icon. 
/// \param	iconSizeSpace	The icon size space. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CScrollList::CScrollList(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int iconSize, int iconSizeSpace, int controlId)
	:SallyAPI::GUI::CForm(parent, x, y, width, height, controlId),
	m_iIconSize(iconSize), m_iIconSizeSpace(iconSizeSpace), m_iStartPicture(0)
{
	this->SetScrollType(SallyAPI::GUI::SCROLL_TYPE_SMOOTH);

	// cols / rows
	m_iCols = (width / (iconSize + m_iIconSizeSpace));
	if ((width % (iconSize + m_iIconSizeSpace)))
		m_iCols++;

	m_iRows = height / (m_iIconSize + m_iIconSizeSpace + CONTROL_HEIGHT);
	m_iBorderTop = (height % (m_iIconSize + m_iIconSizeSpace + CONTROL_HEIGHT)) / 2;

	for (int k = 0 ; k < m_iCols; ++k)
	{
		std::vector<SallyAPI::GUI::CButton*> imageBoxVector;
		std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector;

		for (int i = 0; i < m_iRows; ++i)
		{
			int id = (k * 100) + i + 1;

			SallyAPI::GUI::CButton* imageBox = new SallyAPI::GUI::CButton(this, 0, 0, 0, 0, id, SallyAPI::GUI::BUTTON_TYPE_ONLY_IMAGE);
			imageBox->SetLocalised(false);
			this->AddChild(imageBox);

			imageBoxVector.push_back(imageBox);

			SallyAPI::GUI::CLabelBox* imageName = new SallyAPI::GUI::CLabelBox(this, 0, 0, iconSize, 30, id);
			imageName->SetAlign(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
			imageName->SetFont("scrolllist.font");
			this->AddChild(imageName);

			imageNameVector.push_back(imageName);
		}
		m_mImageBox[k] = imageBoxVector;
		m_mImageName[k] = imageNameVector;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScrollList::~CScrollList()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CScrollList::~CScrollList()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollList::AddChild(SallyAPI::GUI::CControl* control)
///
/// \brief	Adds a child. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	control	If non-null, the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollList::AddChild(SallyAPI::GUI::CControl* control)
{
	SallyAPI::GUI::CForm::AddChild(control);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollList::Clear()
///
/// \brief	Clears this object to its blank/initial state. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollList::Clear()
{
	m_vItems.clear();

	ResetImages();
	UpdateImages();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollList::AddItem(const SallyAPI::GUI::CScrollListItem& item)
///
/// \brief	Adds an item. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	item	The item. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollList::AddItem(const SallyAPI::GUI::CScrollListItem& item)
{
	m_vItems.push_back(item);

	ResetImages();
	UpdateImages();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollList::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
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

void CScrollList::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		OnCommandProcessClicked(reporterId);
		break;
	case GUI_MOUSEMOVE_SMOOTH_LEFT:
	case GUI_MOUSEMOVE_SMOOTH_RIGHT:
		if (reporter == this)
		{
			OnCommandMouseMove(messageParameter);
		}
		return;
	}

	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollList::OnCommandMouseMove(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Executes the command mouse move action. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollList::OnCommandMouseMove(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	// remove the mouse down selection
	for (int k = 0; k < m_iCols; ++k)
	{
		std::vector<SallyAPI::GUI::CButton*> imageBoxVector = m_mImageBox[k];
		std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector = m_mImageName[k];
		for (int i = 0; i < m_iRows; ++i)
		{
			imageBoxVector[i]->SendMessageToChilds(0, 0, GUI_MESSAGE_CONTROL_SCROLLED, NULL);
			imageNameVector[i]->SendMessageToChilds(0, 0, GUI_MESSAGE_CONTROL_SCROLLED, NULL);
		}
	}

	// are there enough icons to move?

	std::vector<SallyAPI::GUI::CLabelBox*> imageNameVectorTest = m_mImageName[0];
	int maxRowsNeeded = ((m_vItems.size() - 1) / m_iRows) + 1;

	int calculatedSize = (maxRowsNeeded * (m_iIconSize + m_iIconSizeSpace)) - m_iIconSizeSpace + (m_iIconSize * 2);

	if (m_iWidth >= calculatedSize)
		return;

	SallyAPI::GUI::SendMessage::CParameterInteger* interger = (SallyAPI::GUI::SendMessage::CParameterInteger*) messageParameter;
	if (interger == NULL)
		return;

	int moveValue = interger->GetInteger();

	// move to the left
	int xTemp = imageNameVectorTest[0]->GetPositionX();

	while ((m_iStartPicture > 0) && (xTemp + moveValue >= m_iIconSize))
	{
		moveValue -= m_iIconSize + m_iIconSizeSpace;
		m_iStartPicture--;
	}

	// move to the right
	xTemp = imageNameVectorTest[0]->GetPositionX();

	while ((m_iStartPicture + m_iCols < maxRowsNeeded) && (xTemp + moveValue <= -m_iIconSize))
	{
		moveValue += m_iIconSize + m_iIconSizeSpace;
		m_iStartPicture++;
	}

	// can we scroll so far to the left?
	if (m_iStartPicture <= 0)
	{
		std::vector<SallyAPI::GUI::CLabelBox*> imageNameVectorStart = m_mImageName[0];

		int newX = imageNameVectorStart[0]->GetPositionX() + moveValue;
		if (newX > m_iIconSize)
		{
			moveValue = (imageNameVectorStart[0]->GetPositionX() - m_iIconSize) * -1;
		}
	}

	// can we scroll so far to the right?
	if (m_iStartPicture + m_iCols >= maxRowsNeeded)
	{
		std::vector<SallyAPI::GUI::CLabelBox*> imageNameVectorEnd = m_mImageName[m_iCols - 1];
		if (maxRowsNeeded < m_iCols)
			imageNameVectorEnd = m_mImageName[maxRowsNeeded];

		int newX = imageNameVectorEnd[0]->GetPositionX() + moveValue;
		if (newX < m_iWidth - (m_iIconSize * 2))
		{
			moveValue = imageNameVectorEnd[0]->GetPositionX() - (m_iWidth - (m_iIconSize * 2));
		}
	}

	// rock'n'roll
	for (int k = 0; k < m_iCols; ++k)
	{
		std::vector<SallyAPI::GUI::CButton*> imageBoxVector = m_mImageBox[k];
		std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector = m_mImageName[k];
		for (int i = 0; i < m_iRows; ++i)
		{
			imageBoxVector[i]->Move(imageBoxVector[i]->GetPositionX() + moveValue, imageBoxVector[i]->GetPositionY());
			imageNameVector[i]->Move(imageNameVector[i]->GetPositionX() + moveValue, imageNameVector[i]->GetPositionY());
		}
	}

	UpdateImages();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollList::UpdateImages()
///
/// \brief	Updates the images. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollList::UpdateImages()
{
	EnterRenderLock();

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
	SallyAPI::Config::CTheme* theme = config->GetTheme();

	std::vector<SallyAPI::GUI::CScrollListItem>::iterator iter = m_vItems.begin();

	int k = 0;
	int i = 0;

	for (int l = 0; (l < m_iStartPicture * m_iRows) && (iter != m_vItems.end()); ++l)
	{
		++iter;
	}

	while ((k < m_iCols) && (iter != m_vItems.end()))
	{
		std::vector<SallyAPI::GUI::CButton*> imageBoxVector = m_mImageBox[k];
		std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector = m_mImageName[k];

		for (; (i < m_iRows) && (iter != m_vItems.end()); i++)
		{
			SallyAPI::GUI::CScrollListItem item = *iter;

			imageBoxVector[i]->SetImageId(item.GetGraphicId());
			imageBoxVector[i]->Visible(true);

			imageNameVector[i]->SetText(item.GetName());
			imageNameVector[i]->Visible(true);
			++iter;
		}
		if (iter != m_vItems.end())
		{
			i = 0;
			k++;
		}
	}

	for (; k < m_iCols; ++k)
	{
		std::vector<SallyAPI::GUI::CButton*> imageBoxVector = m_mImageBox[k];
		std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector = m_mImageName[k];

		for (; i < m_iRows; ++i)
		{
			imageBoxVector[i]->Visible(false);
			imageNameVector[i]->Visible(false);
			imageBoxVector[i]->SetImageId(GUI_NO_IMAGE);
			imageNameVector[i]->SetText("");
		}
		i = 0;
	}
	LeaveRenderLock();

	UpdateAlpha();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollList::ResetImages()
///
/// \brief	Resets the images. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollList::ResetImages()
{
	for (int k = 0; k < m_iCols; ++k)
	{
		std::vector<SallyAPI::GUI::CButton*> imageBoxVector = m_mImageBox[k];
		std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector = m_mImageName[k];

		for (int i = 0; i < m_iRows; i++)
		{
			int x = 0;
			int y = 0;

			x = m_iIconSize;
			x += (m_iIconSize + m_iIconSizeSpace) * k;

			y = m_iBorderTop;
			y += (m_iIconSize + m_iIconSizeSpace + CONTROL_HEIGHT) * i;

			imageBoxVector[i]->SetImageId(GUI_NO_IMAGE);
			imageBoxVector[i]->Resize(m_iIconSize, m_iIconSize);
			imageBoxVector[i]->Move(x, y);

			y = m_iBorderTop;
			y += (m_iIconSize + m_iIconSizeSpace + CONTROL_HEIGHT) * i;
			y += m_iIconSize;
			imageNameVector[i]->Move(x, y + 5);
		}
	}
	m_iStartPicture = 0;

	UpdateAlpha();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollList::OnCommandProcessClicked(int reporterId)
///
/// \brief	Executes the command process clicked action. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	reporterId	Identifier for the reporter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollList::OnCommandProcessClicked(int reporterId)
{
	if (reporterId == 0)
		return;

	reporterId = reporterId - 1;

	int col = reporterId / 100;
	int row = reporterId % 100;

	std::vector<SallyAPI::GUI::CButton*> imageBoxVector = m_mImageBox[col];
	std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector = m_mImageName[col];

	int fileID = (m_iStartPicture * m_iRows) + (col * m_iRows) + row;
	SallyAPI::GUI::CScrollListItem item = m_vItems[fileID];

	SallyAPI::GUI::SendMessage::CParameterScrollList sendMessageParameterScrollList(item);
	this->SendMessageToParent(this, GetControlId(), GUI_SCROLLLIST_CLICKED, &sendMessageParameterScrollList);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollList::UpdateAlpha()
///
/// \brief	Updates an alpha. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollList::UpdateAlpha()
{
	float alphaStep = (float) m_iAlphaBlending / m_iIconSize;

	std::vector<SallyAPI::GUI::CButton*> imageBoxVector = m_mImageBox[0];
	std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector = m_mImageName[0];

	// first one
	for (int i = 0; i < m_iRows; ++i)
	{
		int alpha = imageNameVector[i]->GetPositionX();
		alpha = (int) (alpha * alphaStep);

		if (alpha > m_iAlphaBlending)
			alpha = m_iAlphaBlending;
		if (alpha < 0)
			alpha = 0;

		imageBoxVector[i]->SetAlphaBlending(alpha);
		imageNameVector[i]->SetAlphaBlending(alpha);
	}

	// center

	for (int k = 1; k < m_iCols - 2; ++k)
	{
		imageBoxVector = m_mImageBox[k];
		imageNameVector = m_mImageName[k];

		for (int i = 0; i < m_iRows; ++i)
		{
			int alpha = imageNameVector[i]->GetPositionX();
			alpha = (int) (alpha * alphaStep);

			if (alpha > m_iAlphaBlending)
				alpha = m_iAlphaBlending;
			if (alpha < 0)
				alpha = 0;

			imageBoxVector[i]->SetAlphaBlending(alpha);
			imageNameVector[i]->SetAlphaBlending(alpha);
		}
	}

	// last one
	imageBoxVector = m_mImageBox[m_iCols - 2];
	imageNameVector = m_mImageName[m_iCols - 2];

	for (int i = 0; i < m_iRows; ++i)
	{
		int alpha = m_iWidth - (imageNameVector[i]->GetPositionX() + m_iIconSize);
		alpha = (int) (alpha * alphaStep);

		if (alpha > m_iAlphaBlending)
			alpha = m_iAlphaBlending;
		if (alpha < 0)
			alpha = 0;

		imageBoxVector[i]->SetAlphaBlending(alpha);
		imageNameVector[i]->SetAlphaBlending(alpha);
	}

	// last one
	imageBoxVector = m_mImageBox[m_iCols - 1];
	imageNameVector = m_mImageName[m_iCols - 1];

	for (int i = 0; i < m_iRows; ++i)
	{
		int alpha = m_iWidth - (imageNameVector[i]->GetPositionX() + m_iIconSize);
		alpha = (int) (alpha * alphaStep);

		if (alpha > m_iAlphaBlending)
			alpha = m_iAlphaBlending;
		if (alpha < 0)
			alpha = 0;

		imageBoxVector[i]->SetAlphaBlending(alpha);
		imageNameVector[i]->SetAlphaBlending(alpha);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollList::SetAlphaBlending(int alphaBlending)
///
/// \brief	Sets an alpha blending. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	alphaBlending	The alpha blending. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollList::SetAlphaBlending(int alphaBlending)
{
	// don't call the CForm SetAlphaBlending
	SallyAPI::GUI::CControl::SetAlphaBlending(alphaBlending);

	UpdateAlpha();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollList::SetFont(const std::string& font)
///
/// \brief	Sets a font. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	font	The font. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollList::SetFont(const std::string& font)
{
	for (int k = 0; k < m_iCols; ++k)
	{
		std::vector<SallyAPI::GUI::CLabelBox*> imageNameVector = m_mImageName[k];
		for (int i = 0; i < m_iRows; ++i)
		{
			imageNameVector[i]->SetFont(font);
		}
	}
}