////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ListViewExt.cpp
///
/// \brief	Implements the list view ext class. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
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

#include "ListViewExt.h"

#define LISTVIEW_ITEM_COLUMN		1
#define LISTVIEW_ITEM_ROW			1000

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CListViewExt::CListViewExt(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int height, int cols, std::map<int, int> widthList,
/// int controlId) :SallyAPI::GUI::CForm(parent, x, y, width, height, controlId),
/// m_iStartItem(0), m_iCols(cols), m_iActive(-1), m_iOldPositionX(0), m_iOldPositionY(0)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	29.11.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	height			The height. 
/// \param	cols			The cols. 
/// \param	widthList		List of widthes. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CListViewExt::CListViewExt(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, 
					int cols, std::map<int, int> widthList, int controlId)
	:SallyAPI::GUI::CForm(parent, x, y, width, height, controlId), m_iStartItem(0),
	m_iCols(cols), m_iActive(-1), m_iOldPositionX(0), m_iOldPositionY(0), m_mWidthList(widthList)
{
	// sets the scroll type to smooth scrolling to get GUI_MOUSEMOVE_SMOOTH_UP, ... messages
	this->SetScrollType(SallyAPI::GUI::SCROLL_TYPE_SMOOTH);

	// call resize to calculate the cols new
	Resize(width, height);

	// calculate buttons total width
	int buttonWidthTotal = 0;
	for (int i = 0; i < ((int) m_mWidthList.size()); ++i)
	{
		buttonWidthTotal += m_mWidthList[i];
	}

	int buttonWidthMax = m_iWidth - CONTROL_HEIGHT;

	// now crate the rows we need
	for (int i = 0; i < m_iRows; ++i)
	{
		int buttonWidthUsed = 0;
		// the multilist of buttons
		std::map<int, SallyAPI::GUI::CListViewButton*> buttonList;

		for (int l = 0; l < m_iCols; ++l)
		{
			int ID = (LISTVIEW_ITEM_ROW * (i + 1)) + l;
			int buttonWidth = widthList[l];

			// calcuate max button width
			if (buttonWidth == 0)
				buttonWidth = buttonWidthMax - buttonWidthTotal;
			
			SallyAPI::GUI::CListViewButton* button = new SallyAPI::GUI::CListViewButton(this, buttonWidthUsed,
				(i * LISTVIEW_ITEM_HEIGHT) + LISTVIEW_ITEM_HEIGHT, buttonWidth, LISTVIEW_ITEM_HEIGHT, ID);
			button->Visible(false);
			button->SetNumber(i);
			this->AddChild(button);

			buttonWidthUsed += buttonWidth;

			// check if it is the first button
			if (l == 0)		
				button->SetFirst(true);
			// check if it is the last button
			if (l == m_iCols - 1)
				button->SetLast(true);

			buttonList[l] = button;
		}
		m_mButton[i] = buttonList;
	}

	m_pScrollbar = new SallyAPI::GUI::CScrollbar(this, m_iWidth - CONTROL_HEIGHT, 0, CONTROL_HEIGHT, m_iHeight,
		SallyAPI::GUI::SCROLLBAR_ALIGNMENT_VERTICAL);
	m_pScrollbar->ShowScrollbarIfNotScrollable(false);
	this->AddChild(m_pScrollbar);

	/************************************************************************/
	/* Setup Box2D                                                          */
	/************************************************************************/
	b2AABB worldAABB;

	float xBottom = (float) WINDOW_WIDTH * -1;
	float yBottom = (float) (WINDOW_HEIGHT + WINDOW_HEIGHT) * -1;

	float xTop = (float) WINDOW_WIDTH + WINDOW_WIDTH;
	float yTop = (float) WINDOW_HEIGHT;

	worldAABB.lowerBound.Set(xBottom, yBottom);
	worldAABB.upperBound.Set(xTop, yTop);

	// Define the gravity vector.
	b2Vec2 gravity(-100.0f, 0.0f);

	m_pb2World = new b2World(worldAABB, gravity, true);


	// Define the left body.
	b2BodyDef leftBodyDef;
	float leftX = (float) m_iXAbsolut - 20;
	float leftY = yBottom + m_iHeight;
	leftBodyDef.position.Set(leftX, leftY);

	// Call the body factory which allocates memory for the left body
	// from a pool and creates the left box shape (also from a pool).
	// The body is also added to the world.
	m_pb2LeftBody = m_pb2World->CreateBody(&leftBodyDef);

	// Define the left box shape.
	b2PolygonDef leftShapeDef;

	// The extents are the half-widths of the box.
	float leftHeight = (float) WINDOW_HEIGHT * 3;
	leftShapeDef.SetAsBox(15.0f, leftHeight);

	// Add the left shape to the left body.
	m_pb2LeftBody->CreateShape(&leftShapeDef);

	// Define the right body.
	b2BodyDef rightBodyDef;
	float rightX = (float) m_iXAbsolut + m_iWidth + 20;
	float rightY = (float) yBottom + m_iHeight;
	rightBodyDef.position.Set(rightX, rightY);

	// Call the body factory which allocates memory for the right body
	// from a pool and creates the right box shape (also from a pool).
	// The body is also added to the world.
	m_pb2RightBody = m_pb2World->CreateBody(&rightBodyDef);

	// Define the right box shape.
	b2PolygonDef rightShapeDef;

	// The extents are the half-widths of the box.
	float rightHeight = (float) WINDOW_HEIGHT * 3;
	rightShapeDef.SetAsBox(15.0f, rightHeight);

	// Add the right shape to the right body.
	m_pb2RightBody->CreateShape(&rightShapeDef);


	m_pb2Object = new CBox2DObject(this, 0, 0, m_iWidth - CONTROL_HEIGHT, m_iHeight);
// 	m_pb2Object->SetImage(GUI_THEME_SALLY_BLACK_BACKGROUND);
// 	m_pb2Object->SetAlphaBlending(100);
	this->AddChild(m_pb2Object);

	m_iOldPositionX = m_pb2Object->GetPositionX();
	m_iOldPositionY = m_pb2Object->GetPositionY();

	//m_pb2Object->GetAbsolutPosition(&m_iOldPositionX, &m_iOldPositionY);
	m_pb2Object->CreateBox2DObject(m_pb2World);

	ResetListView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CListViewExt::~CListViewExt()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CListViewExt::~CListViewExt()
{
	Clear();
	SafeDelete(m_pb2World);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewExt::Resize(int width, int height)
///
/// \brief	Resizes. 
///
/// \author	Christian Knobloch
/// \date	29.11.2010
///
/// \param	width	The width. 
/// \param	height	The height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewExt::Resize(int width, int height)
{
	// call the parent resize methode
	SallyAPI::GUI::CForm::Resize(width, height);

	// calculate the cols we need
	m_iRows = (m_iHeight / LISTVIEW_ITEM_HEIGHT) - 1;
	if (m_iRows < 0)
		m_iRows = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewExt::AddChild(SallyAPI::GUI::CControl* control)
///
/// \brief	Adds a child. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \param [in,out]	control	If non-null, the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewExt::AddChild(SallyAPI::GUI::CControl* control)
{
	SallyAPI::GUI::CForm::AddChild(control);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewExt::Timer(float timeDelta)
///
/// \brief	Timers. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \param	timeDelta	The time delta. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewExt::Timer(float timeDelta)
{
	SallyAPI::GUI::CForm::Timer(timeDelta);

	m_pb2World->Step(timeDelta * 3, 10);

	if (!m_bMouseDown)
	{
		int moveValueX = m_pb2Object->GetPositionX();
		int moveValueY = m_pb2Object->GetPositionY();

// 		int moveValueX = 0;
// 		int moveValueY = 0;
// 		m_pb2Object->GetAbsolutPosition(&moveValueX, &moveValueY);

		int moveValue = moveValueY - m_iOldPositionY;

		if (moveValue != 0)
		{
			SallyAPI::GUI::SendMessage::CParameterInteger integer(moveValue * 2);
			OnCommandMouseMove(&integer);
		}
	}
// 	m_iOldPositionX = 0;
// 	m_iOldPositionY = 0;
// 	m_pb2Object->GetAbsolutPosition(&m_iOldPositionX, &m_iOldPositionY);

	m_iOldPositionX = m_pb2Object->GetPositionX();
	m_iOldPositionY = m_pb2Object->GetPositionY();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CListViewExt::CheckProcessMouseDown(int x, int y)
///
/// \brief	Check process mouse down. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CListViewExt::CheckProcessMouseDown(int x, int y)
{
	if (ResetBox2Object())
	{
		m_pb2Object->CheckProcessMouseDown(x, y);
		return true;
	}

	return SallyAPI::GUI::CForm::CheckProcessMouseDown(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CListViewExt::CheckProcessMouseUp(int x, int y)
///
/// \brief	Check process mouse up. 
///
/// \author	Christian Knobloch
/// \date	29.11.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CListViewExt::CheckProcessMouseUp(int x, int y)
{
	bool isScrolling = IsScrolling();
	if (!isScrolling)
		ResetBox2Object();

	bool result = SallyAPI::GUI::CForm::CheckProcessMouseUp(x, y);

	if (!isScrolling)
		ResetBox2Object();

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CListViewExt::ResetBox2Object()
///
/// \brief	Resets a box 2 object. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CListViewExt::ResetBox2Object()
{
	bool move = m_pb2Object->MoveAndHold(0, 0);
// 	m_iOldPositionX = 0;
// 	m_iOldPositionY = 0;
// 	m_pb2Object->GetAbsolutPosition(&m_iOldPositionX, &m_iOldPositionY);

	m_iOldPositionX = m_pb2Object->GetPositionX();
	m_iOldPositionY = m_pb2Object->GetPositionY();

	return move;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewExt::SetStartItem(int startItem)
///
/// \brief	Sets a start item. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \param	startItem	The start item. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewExt::SetStartItem(int startItem)
{
	if (startItem >= (int) (m_vItems.size()))
		startItem = m_vItems.size() - 1;

	if (startItem + m_iRows > (int) m_vItems.size())
		startItem = m_vItems.size() - m_iRows + 1;
	if (startItem <= 0)
		startItem = 0;

	m_iStartItem = startItem;

	ResetBox2Object();

	ResetListView();
	UpdateView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CListViewExt::GetStartItem()
///
/// \brief	Gets the start item. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \return	The start item. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CListViewExt::GetStartItem()
{
	return m_iStartItem;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CListViewExt::GetMaxItemStart()
///
/// \brief	Gets the maximum item start. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \return	The maximum item start. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CListViewExt::GetMaxItemStart()
{
	int playlistSize = m_vItems.size();
	int maxItemStart = playlistSize - m_iRows;

	if (maxItemStart < 0)
		maxItemStart = 0;

	return maxItemStart;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CListViewExt::GetListSize()
///
/// \brief	Gets the list size. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \return	The list size. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CListViewExt::GetListSize()
{
	return m_vItems.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewExt::AddItem(SallyAPI::GUI::CListViewItem listItem)
///
/// \brief	Adds an item. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \param	listItem	The list item. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewExt::AddItem(SallyAPI::GUI::CListViewItem listItem)
{
	SallyAPI::GUI::CListViewItem* tempListItem = new SallyAPI::GUI::CListViewItem(listItem);

	m_vItems.push_back(tempListItem);

	if (m_iStartItem == -1)
		m_iStartItem = 0;

	UpdateView();

	SendMessageToParent(this, GetControlId(), GUI_LISTVIEW_ITEM_ADDED);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CListViewExt::RemoveItem(int index)
///
/// \brief	Removes the item described by index. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \param	index	Zero-based index of the. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CListViewExt::RemoveItem(int index)
{
	int i = 1;
	std::vector<SallyAPI::GUI::CListViewItem*>::iterator iter = m_vItems.begin();
	while (iter < m_vItems.end())
	{
		if (i == index)
		{
			SallyAPI::GUI::CListViewItem* temp = *iter;
			delete(temp);

			m_vItems.erase(iter);

			if (i == m_iActive + 1)
			{
				m_iActive = -1;
			}
			else if (i < m_iActive + 1)
			{
				--m_iActive;
			}

			// set new m_iStartPosition
			if (((int) m_vItems.size() < m_iStartItem + m_iRows) && (m_iStartItem > 0))
				--m_iStartItem;

			if ((int) m_vItems.size() <= m_iRows - 1)
				ResetListView();

			UpdateView();
			return true;
		}
		++i;
		++iter;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CListViewItem* CListViewExt::GetItem(int index)
///
/// \brief	Gets the requested item by index. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \param	index	Zero-based index of the. 
///
/// \return	null if it fails, else the item. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CListViewItem* CListViewExt::GetItem(int index)
{
	if ((index >= (int) m_vItems.size()) || (index < 0))
		return NULL;
	return m_vItems[index];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewExt::Clear()
///
/// \brief	Clears this object to its blank/initial state. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewExt::Clear()
{
	std::vector<SallyAPI::GUI::CListViewItem*>::iterator iter = m_vItems.begin();
	while (iter < m_vItems.end())
	{
		SallyAPI::GUI::CListViewItem* temp = *iter;
		SafeDelete(temp);

		m_vItems.erase(iter);
		iter = m_vItems.begin();
	}
	m_iStartItem = 0;
	ResetListView();
	UpdateView();
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewExt::ResetListView()
///
/// \brief	Resets a list view. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewExt::ResetListView()
{
	for (int k = 0; k < m_iRows; ++k)
	{
		std::map<int, SallyAPI::GUI::CListViewButton*> listViewButton = m_mButton[k];

		for (int l = 0; l < m_iCols; ++l)
		{
			SallyAPI::GUI::CListViewButton* button = listViewButton[l];

			button->Move(button->GetPositionX(), (k * LISTVIEW_ITEM_HEIGHT) + LISTVIEW_ITEM_HEIGHT);
		}
	}

	// reset alphablending first item
	std::map<int, SallyAPI::GUI::CListViewButton*> listViewButtonFirst = m_mButton[0];
	for (int l = 0; l < m_iCols; ++l)
	{
		SallyAPI::GUI::CListViewButton* button = listViewButtonFirst[l];

		button->SetAlphaBlending(255);
	}

	// reset alphablending last item
	std::map<int, SallyAPI::GUI::CListViewButton*> listViewButtonLast = m_mButton[m_iRows - 1];
	for (int l = 0; l < m_iCols; ++l)
	{
		SallyAPI::GUI::CListViewButton* button = listViewButtonLast[l];

		button->SetAlphaBlending(0);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::vector<SallyAPI::GUI::CListViewItem*>* CListViewExt::GetListItems()
///
/// \brief	Gets the list items. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \return	null if it fails, else the list items. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<SallyAPI::GUI::CListViewItem*>* CListViewExt::GetListItems()
{
	return &m_vItems;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewExt::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to childs. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewExt::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_MESSAGE_UPDATE_ABSOLUTE_POSITION:
		SallyAPI::GUI::CForm::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
		
// 		bool isScrolling = IsScrolling();
// 		if (!isScrolling)
// 			ResetBox2Object();

		if (m_pb2LeftBody != NULL)
		{
			float yBottom = (float) WINDOW_WIDTH * -1;

			float posX = (float) m_iXAbsolut - 20;
			float posY = (float) yBottom + m_iHeight;

			b2Vec2 vec(posX, posY);
			m_pb2LeftBody->SetXForm(vec, 0);
		}

		if (m_pb2RightBody != NULL)
		{
			float yBottom = (float) WINDOW_WIDTH * -1;

			float posX = (float) (m_iXAbsolut + m_iWidth + 20);
			float posY = (float) (yBottom + m_iHeight);

			b2Vec2 vec(posX, posY);
			m_pb2RightBody->SetXForm(vec, 0);
		}

		// 		if (!isScrolling)
		// 			ResetBox2Object();

		return;
	}
	SallyAPI::GUI::CForm::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewExt::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to parent. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewExt::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_SCROLLBAR_CLICKED:
	case GUI_SCROLLBAR_MOVED:
		OnCommandScrollbarMoved(reporter, messageParameter);
		return;
	case GUI_LISTVIEW_ITEM_DOUBLECLICKED:
		OnCommandDoubleclicked(reporterId);
		return;
	case GUI_MOUSEMOVE_SMOOTH_UP:
	case GUI_MOUSEMOVE_SMOOTH_DOWN:
		if (reporter == this)
		{
			OnCommandMouseMove(messageParameter);
		}
		return;
	}

	if ((reporterId >= LISTVIEW_ITEM_ROW && reporterId <= LISTVIEW_ITEM_ROW * (m_iRows + 1)) && (messageId == GUI_BUTTON_CLICKED))
	{
		int iRow = reporterId / LISTVIEW_ITEM_ROW;
		int iColumn = reporterId % LISTVIEW_ITEM_ROW;

		SallyAPI::GUI::SendMessage::CParameterListItem parameterListItem(iRow + m_iStartItem - 1, iColumn);
		m_pParent->SendMessageToParent(this, m_iControlId, GUI_LISTVIEW_ITEM_CLICKED, &parameterListItem);
		return;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewExt::OnCommandScrollbarMoved(SallyAPI::GUI::CGUIBaseObject* reporter,
/// SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Executes the command scrollbar moved action. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewExt::OnCommandScrollbarMoved(SallyAPI::GUI::CGUIBaseObject* reporter, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterInteger* parameterInteger = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);
	if (parameterInteger == NULL)
		return;

	SetStartItem(parameterInteger->GetInteger() / CONTROL_HEIGHT);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewExt::OnCommandDoubleclicked(int reporterId)
///
/// \brief	Executes the command doubleclicked action. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \param	reporterId	Identifier for the reporter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewExt::OnCommandDoubleclicked(int reporterId)
{
	int iRow = reporterId / LISTVIEW_ITEM_ROW;
	int iColumn = reporterId % LISTVIEW_ITEM_ROW;
		
	if (iRow >= GetListSize())
		return;

	SallyAPI::GUI::SendMessage::CParameterListItem parameterListItem(iRow - 1, iColumn);
	m_pParent->SendMessageToParent(this, m_iControlId, GUI_LISTVIEW_ITEM_DOUBLECLICKED, &parameterListItem);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewExt::OnCommandMouseMove(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Executes the command mouse move action. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewExt::OnCommandMouseMove(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	if (m_iMouseDownX >= m_iXAbsolut + m_iWidth - CONTROL_HEIGHT)
		return;

	if (((int) m_vItems.size()) < m_iRows)
		return;

	// remove the mouse down selection
	for (int k = 0; k < m_iRows; ++k)
	{
		std::map<int, SallyAPI::GUI::CListViewButton*> listViewButton = m_mButton[k];

		for (int l = 0; l < m_iCols; ++l)
		{
			SallyAPI::GUI::CListViewButton* button = listViewButton[l];

			button->SendMessageToChilds(0, 0, GUI_MESSAGE_CONTROL_SCROLLED, NULL);
		}
	}

	SallyAPI::GUI::SendMessage::CParameterInteger* interger = (SallyAPI::GUI::SendMessage::CParameterInteger*) messageParameter;
	int moveValue = interger->GetInteger();

// 	std::string ttt = SallyAPI::String::StringHelper::ConvertToString(moveValue);
// 	ttt.append("\n");
// 	OutputDebugStr(ttt.c_str());

	// move to the bottom
	std::map<int, SallyAPI::GUI::CListViewButton*> listViewButton = m_mButton[0];

	int yTemp = listViewButton[0]->GetPositionY();
	while ((yTemp + moveValue >= LISTVIEW_ITEM_HEIGHT) && (m_iStartItem > 0))
	{
		for (int k = 0; k < m_iRows; ++k)
		{
			std::map<int, SallyAPI::GUI::CListViewButton*> listViewButton = m_mButton[k];

			for (int l = 0; l < m_iCols; ++l)
			{
				SallyAPI::GUI::CListViewButton* button = listViewButton[l];

				button->SetNumber(button->GetNumber() - 1);
			}
		}
		moveValue -= LISTVIEW_ITEM_HEIGHT;
		m_iStartItem--;

// 		std::string ttt;
// 		ttt.append("--\n");
// 		OutputDebugStr(ttt.c_str());
	}

	// move to the top
	yTemp = listViewButton[0]->GetPositionY();
	while ((yTemp + moveValue <= 0) && (m_iStartItem + m_iRows < (int) m_vItems.size() + 1))
	{
		for (int k = 0; k < m_iRows; ++k)
		{
			std::map<int, SallyAPI::GUI::CListViewButton*> listViewButton = m_mButton[k];

			for (int l = 0; l < m_iCols; ++l)
			{
				SallyAPI::GUI::CListViewButton* button = listViewButton[l];

				button->SetNumber(button->GetNumber() + 1);
			}
		}

		moveValue += LISTVIEW_ITEM_HEIGHT;
		m_iStartItem++;

// 		std::string ttt;
// 		ttt.append("++\n");
// 		OutputDebugStr(ttt.c_str());
	}

	// can we scroll so far to the bottom?
	if (m_iStartItem <= 0)
	{
		int newY = listViewButton[0]->GetPositionY() + moveValue;
		if (newY > LISTVIEW_ITEM_HEIGHT)
		{
			moveValue = (listViewButton[0]->GetPositionY() - LISTVIEW_ITEM_HEIGHT) * -1;
			ResetBox2Object();
		}
	}

	// can we scroll so far to the top?
	if (m_iStartItem + m_iRows >= (int) m_vItems.size() + 1)
	{
		int newY = listViewButton[0]->GetPositionY() + moveValue;
		if (newY < LISTVIEW_ITEM_HEIGHT)
		{
			moveValue = LISTVIEW_ITEM_HEIGHT - listViewButton[0]->GetPositionY();
			ResetBox2Object();
		}
	}

	// rock'n'roll
	for (int k = 0; k < m_iRows; ++k)
	{
		std::map<int, SallyAPI::GUI::CListViewButton*> listViewButton = m_mButton[k];

		for (int l = 0; l < m_iCols; ++l)
		{
			SallyAPI::GUI::CListViewButton* button = listViewButton[l];

			button->Move(button->GetPositionX(), button->GetPositionY() + moveValue);
		}
	}
	UpdateView();

	// set image transparent
	std::map<int, SallyAPI::GUI::CListViewButton*> listViewFirstRow = m_mButton[0];
	for (int l = 0; l < m_iCols; ++l)
	{
		SallyAPI::GUI::CListViewButton* button = listViewFirstRow[l];

		button->SetAlphaBlending(255 / CONTROL_HEIGHT * button->GetPositionY());
	}
	std::map<int, SallyAPI::GUI::CListViewButton*> listViewLastRow = m_mButton[m_iRows - 1];
	for (int l = 0; l < m_iCols; ++l)
	{
		SallyAPI::GUI::CListViewButton* button = listViewLastRow[l];

		button->SetAlphaBlending(255 / CONTROL_HEIGHT * ((m_iRows * LISTVIEW_ITEM_HEIGHT) - button->GetPositionY()));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewExt::UpdateView()
///
/// \brief	Updates this object. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewExt::UpdateView()
{
	EnterRenderLock();

	if (m_iActive > GetListSize() - 1)
		m_iActive = -1;

	for (int i = 0; i < m_iRows; ++i)
	{
		SallyAPI::GUI::CListViewItem* listItem = NULL;

		int iRequestedElem = m_iStartItem + i;
		if ((iRequestedElem >= 0) && (iRequestedElem < (int) m_vItems.size()))
		{
			listItem = m_vItems[iRequestedElem];
		}

		if (listItem != NULL)
		{
			std::map<int, SallyAPI::GUI::CListViewButton*> listViewButton = m_mButton[i];

			for (int l = 0; l < m_iCols; ++l)
			{
				SallyAPI::GUI::CListViewButton* button = listViewButton[l];

				if (listItem->IsLocalised(l) == SallyAPI::GUI::LISTVIEW_LOCALISATION_FROM_PARENT)
					button->SetLocalised(m_bLocalised);
				else if (listItem->IsLocalised(l) == SallyAPI::GUI::LISTVIEW_LOCALISATION_FALSE)
					button->SetLocalised(false);
				else
					button->SetLocalised(true);
			
				button->SetText(listItem->GetText(l));
				button->SetImageId(listItem->GetImageId(l));
				button->SetSmallFont(listItem->GetSmallFont(l));
				
				if (iRequestedElem == m_iActive)
					button->SetActive(true);
				else
					button->SetActive(false);
				
				button->Visible(true);
			}
		}
		else
		{
			std::map<int, SallyAPI::GUI::CListViewButton*> listViewButton = m_mButton[i];

			for (int l = 0; l < m_iCols; ++l)
			{
				SallyAPI::GUI::CListViewButton* button = listViewButton[l];
				
				button->SetText("");
				button->SetImageId(0);
				button->SetActive(false);
				button->Visible(false);
			}
		}
	}

	std::map<int, SallyAPI::GUI::CListViewButton*> listViewButton = m_mButton[0];
	int maxPositon = (m_vItems.size() + 2 - m_iRows - 1) * CONTROL_HEIGHT;
	int postion = (m_iStartItem + 1) * CONTROL_HEIGHT - listViewButton[0]->GetPositionY();

	m_pScrollbar->SetMaxPosition(maxPositon);
	m_pScrollbar->SetPosition(postion);

	LeaveRenderLock();
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewExt::SetActive(int index)
///
/// \brief	Sets an active. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \param	index	Zero-based index of the. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewExt::SetActive(int index)
{
	m_iActive = index;
	UpdateView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CListViewExt::GetActive()
///
/// \brief	Gets the index of the active element. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \return	The active. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CListViewExt::GetActive()
{
	return m_iActive;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CListViewExt::GetMaxDisplayedElements()
///
/// \brief	Gets the maximum displayed elements. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \return	The maximum displayed elements. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CListViewExt::GetMaxDisplayedElements()
{
	return m_iRows;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewExt::Enable(bool enable)
///
/// \brief	Enables. 
///
/// \author	Christian Knobloch
/// \date	23.11.2010
///
/// \param	enable	true to enable, false to disable. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewExt::Enable(bool enable)
{
	SallyAPI::GUI::CForm::Enable(enable);

	// Send Timer Event to all Childs
	std::list<CControl*>::iterator iter = m_GUIControlList.begin();
	while (iter !=  m_GUIControlList.end())
	{
		CControl* control = *iter;
		control->Enable(enable);
		++iter;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListViewExt::ShowScrollbarIfNotScrollable(bool value)
///
/// \brief	Shows the scrollbar if not scrollable. 
///
/// \author	Christian Knobloch
/// \date	27.04.2010
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListViewExt::ShowScrollbarIfNotScrollable(bool value)
{
	m_pScrollbar->ShowScrollbarIfNotScrollable(value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CListViewExt::IsScrollbarVisibleIfNotScrollbable(void)
///
/// \brief	Query if this object is scrollbar visible if not scrollbable. 
///
/// \author	Christian Knobloch
/// \date	27.04.2010
///
/// \return	true if scrollbar visible if not scrollbable, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CListViewExt::IsScrollbarVisibleIfNotScrollbable()
{
	return m_pScrollbar->IsScrollbarVisibleIfNotScrollbable();;
}