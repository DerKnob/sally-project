////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ListView.cpp
///
/// \brief	Implements the list view class. 
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

#include "ListView.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CListView::CListView(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int height, int cols, int actionGraphic, std::vector<int>& pictureList,
/// int controlId) :SallyAPI::GUI::CForm(parent, x, y, width, height, controlId),
/// m_iStartPicture(0), m_iCols(cols), m_iActionGraphic(actionGraphic),
/// m_vPictureList(pictureList), m_iActive(-1), m_bUseImageList(true), m_iOldPositionX(0),
/// m_iOldPositionY(0)
///
/// \brief	Constructor. Depricated!!!
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param [in,out]	parent		If non-null, the parent. 
/// \param	x					The x coordinate. 
/// \param	y					The y coordinate. 
/// \param	width				The width. 
/// \param	height				The height. 
/// \param	cols				The cols. 
/// \param	actionGraphic		The action graphic. 
/// \param [in,out]	pictureList	List of pictures. 
/// \param	controlId			Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CListView::CListView(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, 
					 int cols, int actionGraphic, std::vector<int>& pictureList, int controlId)
	:SallyAPI::GUI::CForm(parent, x, y, width, height, controlId), m_iStartPicture(0),
	m_iCols(cols), m_iActionGraphic(actionGraphic), m_vPictureList(pictureList), m_iActive(-1),
	m_bUseImageList(true), m_iOldPositionX(0), m_iOldPositionY(0)
{
	CreateListView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CListView::CListView(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int height, int cols, int actionGraphic, int controlId) :SallyAPI::GUI::CForm(parent, x, y,
/// width, height, controlId), m_iStartPicture(0), m_iCols(cols), m_iActionGraphic(actionGraphic),
/// m_iActive(-1), m_bUseImageList(false), m_iOldPositionX(0), m_iOldPositionY(0)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	height			The height. 
/// \param	cols			The cols. 
/// \param	actionGraphic	The action graphic. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CListView::CListView(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, 
					int cols, int actionGraphic, int controlId)
	:SallyAPI::GUI::CForm(parent, x, y, width, height, controlId), m_iStartPicture(0),
	m_iCols(cols), m_iActionGraphic(actionGraphic), m_iActive(-1), m_bUseImageList(false),
	m_iOldPositionX(0), m_iOldPositionY(0)
{
	CreateListView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListView::CreateListView()
///
/// \brief	Creates the list view. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::CreateListView()
{
	this->SetScrollType(SallyAPI::GUI::SCROLL_TYPE_SMOOTH);

	m_iRows = (m_iHeight / LISTVIEW_ITEM_HEIGHT) - 1;
	if (m_iRows < 0)
		m_iRows = 0;

	if (m_iCols > 0)
	{
		for (int i = 0; i < m_iRows; ++i)
		{
			int ID = LISTVIEW_ITEM_ACTION_NO + i;
			m_mButtonAction[i] = new SallyAPI::GUI::CListViewButton(this, 0,
				(i * LISTVIEW_ITEM_HEIGHT) + LISTVIEW_ITEM_HEIGHT, LISTVIEW_ITEM_HEIGHT, LISTVIEW_ITEM_HEIGHT, ID);
			m_mButtonAction[i]->SetImageId(m_iActionGraphic);
			m_mButtonAction[i]->Visible(false);
			m_mButtonAction[i]->SetFirst(true);
			m_mButtonAction[i]->SetNumber(i);
			this->AddChild(m_mButtonAction[i]);
		}
	}

	for (int i = 0; i < m_iRows; ++i)
	{
		int id = LISTVIEW_ITEM_NO + i;
		m_mButtonItem[i] = new SallyAPI::GUI::CListViewButton(this, (m_iCols * LISTVIEW_ITEM_HEIGHT),
			(i * LISTVIEW_ITEM_HEIGHT) + LISTVIEW_ITEM_HEIGHT, m_iWidth - (m_iCols * LISTVIEW_ITEM_HEIGHT) - CONTROL_HEIGHT,
			LISTVIEW_ITEM_HEIGHT, id);
		m_mButtonItem[i]->SetLocalised(false);
		m_mButtonItem[i]->Visible(false);
		m_mButtonItem[i]->SetLast(true);
		m_mButtonItem[i]->SetNumber(i);
		if (m_iCols == 0)
			m_mButtonItem[i]->SetFirst(true);
		this->AddChild(m_mButtonItem[i]);
	}

	m_pScrollbar = new SallyAPI::GUI::CScrollbar(this, m_iWidth - CONTROL_HEIGHT, 0, CONTROL_HEIGHT, m_iHeight, SallyAPI::GUI::SCROLLBAR_ALIGNMENT_VERTICAL);
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
	this->AddChild(m_pb2Object);

	m_iOldPositionX = m_pb2Object->GetPositionX();
	m_iOldPositionY = m_pb2Object->GetPositionY();

	m_pb2Object->CreateBox2DObject(m_pb2World);

	ResetListView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CListView::~CListView()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CListView::~CListView()
{
	Clear();
	SafeDelete(m_pb2World);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListView::AddChild(SallyAPI::GUI::CControl* control)
///
/// \brief	Adds a child. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param [in,out]	control	If non-null, the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::AddChild(SallyAPI::GUI::CControl* control)
{
	SallyAPI::GUI::CForm::AddChild(control);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListView::Timer(float timeDelta)
///
/// \brief	Timers. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param	timeDelta	The time delta. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::Timer(float timeDelta)
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
/// \fn	bool CListView::CheckProcessMouseDown(int x, int y)
///
/// \brief	Check process mouse down. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CListView::CheckProcessMouseDown(int x, int y)
{
	if (ResetBox2Object())
	{
		m_pb2Object->CheckProcessMouseDown(x, y);
		return true;
	}

	return SallyAPI::GUI::CForm::CheckProcessMouseDown(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CListView::CheckProcessMouseUp(int x, int y)
///
/// \brief	Check process mouse up. 
///
/// \author	Christian Knobloch
/// \date	27.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CListView::CheckProcessMouseUp(int x, int y)
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
/// \fn	bool CListView::ResetBox2Object()
///
/// \brief	Resets a box 2 object. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CListView::ResetBox2Object()
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
/// \fn	void CListView::SetPictureList(std::vector<int>& pictureList)
///
/// \brief	Sets a picture list. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param [in,out]	pictureList	List of pictures. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::SetPictureList(std::vector<int>& pictureList)
{
	m_vPictureList = pictureList;
	m_bUseImageList = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListView::RemovePictureList()
///
/// \brief	Removes a picture list. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::RemovePictureList()
{
	m_bUseImageList = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListView::SetStartItem(int startItem)
///
/// \brief	Sets a start item. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param	startItem	The start item. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::SetStartItem(int startItem)
{
	if (startItem >= (int) (m_vItems.size()))
		startItem = m_vItems.size() - 1;

	if (startItem + m_iRows > (int) m_vItems.size())
		startItem = m_vItems.size() - m_iRows + 1;
	if (startItem <= 0)
		startItem = 0;

	m_iStartPicture = startItem;

	ResetBox2Object();

	ResetListView();
	UpdateView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CListView::GetStartItem()
///
/// \brief	Gets the start item. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \return	The start item. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CListView::GetStartItem()
{
	return m_iStartPicture;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListView::SetActionImageID(int icon)
///
/// \brief	Sets an action image identifier. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param	icon	The icon. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::SetActionImageID(int icon)
{
	if (m_iCols > 0)
	{
		for (int i = 0; i < m_iRows; ++i)
		{
			m_mButtonAction[i]->SetImageId(icon);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CListView::GetMaxItemStart()
///
/// \brief	Gets the maximum item start. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \return	The maximum item start. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CListView::GetMaxItemStart()
{
	int playlistSize = m_vItems.size();
	int maxItemStart = playlistSize - m_iRows;

	if (maxItemStart < 0)
		maxItemStart = 0;

	return maxItemStart;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CListView::GetListSize()
///
/// \brief	Gets the list size. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \return	The list size. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CListView::GetListSize()
{
	return m_vItems.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListView::AddItem(SallyAPI::GUI::CListViewItem listItem)
///
/// \brief	Adds an item. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param	listItem	The list item. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::AddItem(SallyAPI::GUI::CListViewItem listItem)
{
	SallyAPI::GUI::CListViewItem* tempListItem = new SallyAPI::GUI::CListViewItem(listItem);

	if (m_bUseImageList)
	{
		if (listItem.GetImageId() >= ((int) m_vPictureList.size()))
			tempListItem->SetImageId(-1);
	}
	m_vItems.push_back(tempListItem);

	if (m_iStartPicture == -1)
		m_iStartPicture = 0;

	UpdateView();

	SendMessageToParent(this, GetControlId(), GUI_LISTVIEW_ITEM_ADDED);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CListView::RemoveItem(int index)
///
/// \brief	Removes the item described by index. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param	index	Zero-based index of the. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CListView::RemoveItem(int index)
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
			if (((int) m_vItems.size() < m_iStartPicture + m_iRows) && (m_iStartPicture > 0))
				--m_iStartPicture;

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
/// \fn	SallyAPI::GUI::CListViewItem* CListView::GetItem(int index)
///
/// \brief	Gets the requested item by index. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param	index	Zero-based index of the. 
///
/// \return	null if it fails, else the item. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CListViewItem* CListView::GetItem(int index)
{
	if ((index >= (int) m_vItems.size()) || (index < 0))
		return NULL;
	return m_vItems[index];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListView::Clear()
///
/// \brief	Clears this object to its blank/initial state. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::Clear()
{
	std::vector<SallyAPI::GUI::CListViewItem*>::iterator iter = m_vItems.begin();
	while (iter < m_vItems.end())
	{
		SallyAPI::GUI::CListViewItem* temp = *iter;
		SafeDelete(temp);

		m_vItems.erase(iter);
		iter = m_vItems.begin();
	}

	// remove the flicker
	for (int k = 0; k < m_iRows; ++k)
	{
		m_mButtonItem[k]->ResetFlicker();
		if (m_iCols > 0)
		{
			m_mButtonAction[k]->ResetFlicker();
		}
	}

	m_iStartPicture = 0;
	ResetListView();
	UpdateView();
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListView::ResetListView()
///
/// \brief	Resets a list view. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::ResetListView()
{
	for (int k = 0; k < m_iRows; ++k)
	{
		m_mButtonItem[k]->Move(m_mButtonItem[k]->GetPositionX(), (k * LISTVIEW_ITEM_HEIGHT) + LISTVIEW_ITEM_HEIGHT);
		if (m_iCols > 0)
			m_mButtonAction[k]->Move(m_mButtonAction[k]->GetPositionX(), (k * LISTVIEW_ITEM_HEIGHT) + LISTVIEW_ITEM_HEIGHT);
	}

	m_mButtonItem[0]->SetAlphaBlending(m_iAlphaBlending);
	m_mButtonItem[m_iRows -1]->SetAlphaBlending(0);
	if (m_iCols > 0)
	{
		m_mButtonAction[0]->SetAlphaBlending(m_iAlphaBlending);
		m_mButtonAction[m_iRows -1]->SetAlphaBlending(0);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::vector<SallyAPI::GUI::CListViewItem*>* CListView::GetListItems()
///
/// \brief	Gets the list items. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \return	null if it fails, else the list items. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<SallyAPI::GUI::CListViewItem*>* CListView::GetListItems()
{
	return &m_vItems;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListView::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to childs. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
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
/// \fn	void CListView::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to parent. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
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

	if (reporterId >= LISTVIEW_ITEM_NO && reporterId <= LISTVIEW_ITEM_NO + 90)
	{
		int iGoTo;
		iGoTo = (m_iStartPicture + reporterId - LISTVIEW_ITEM_NO);
		if (iGoTo >= GetListSize())
			return;

		SallyAPI::GUI::SendMessage::CParameterInteger parameterGoTo(iGoTo);
		m_pParent->SendMessageToParent(this, m_iControlId, GUI_LISTVIEW_ITEM_CLICKED, &parameterGoTo);
		return;
	}

	if (reporterId >= LISTVIEW_ITEM_ACTION_NO && reporterId <= LISTVIEW_ITEM_ACTION_NO + 90)
	{
		int iAction;
		iAction = (m_iStartPicture + reporterId - LISTVIEW_ITEM_ACTION_NO);
		if (iAction >= GetListSize())
			return;

		SallyAPI::GUI::SendMessage::CParameterInteger parameterAction(iAction);
		m_pParent->SendMessageToParent(this, m_iControlId, GUI_LISTVIEW_ITEM_ACTION_CLICKED, &parameterAction);
		return;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListView::OnCommandScrollbarMoved(SallyAPI::GUI::CGUIBaseObject* reporter,
/// SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Executes the command scrollbar moved action. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::OnCommandScrollbarMoved(SallyAPI::GUI::CGUIBaseObject* reporter, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterInteger* interger = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);
	if (interger == NULL)
		return;

	SetStartItem(interger->GetInteger() / CONTROL_HEIGHT);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListView::OnCommandDoubleclicked(int reporterId)
///
/// \brief	Executes the command doubleclicked action. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param	reporterId	Identifier for the reporter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::OnCommandDoubleclicked(int reporterId)
{
	int iItem;
	iItem = (m_iStartPicture + reporterId - LISTVIEW_ITEM_NO);
	if (iItem >= GetListSize())
		return;

	SallyAPI::GUI::SendMessage::CParameterInteger parameterItem(iItem);
	m_pParent->SendMessageToParent(this, GetControlId(), GUI_LISTVIEW_ITEM_DOUBLECLICKED, &parameterItem);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListView::OnCommandMouseMove(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Executes the command mouse move action. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::OnCommandMouseMove(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	if (m_iMouseDownX >= m_iXAbsolut + m_iWidth - CONTROL_HEIGHT)
		return;

	if (((int) m_vItems.size()) < m_iRows)
		return;

	// remove the mouse down selection and the flicker
	for (int k = 0; k < m_iRows; ++k)
	{
		m_mButtonItem[k]->SendMessageToChilds(0, 0, GUI_MESSAGE_CONTROL_SCROLLED, NULL);
		m_mButtonItem[k]->ResetFlicker();
		if (m_iCols > 0)
		{
			m_mButtonAction[k]->SendMessageToChilds(0, 0, GUI_MESSAGE_CONTROL_SCROLLED, NULL);
			m_mButtonAction[k]->ResetFlicker();
		}
	}

	SallyAPI::GUI::SendMessage::CParameterInteger* interger = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);
	if (interger == NULL)
		return;

	int moveValue = interger->GetInteger();

// 	std::string ttt = SallyAPI::String::StringHelper::ConvertToString(moveValue);
// 	ttt.append("\n");
// 	OutputDebugStr(ttt.c_str());

	// move to the bottom
	int yTemp = m_mButtonItem[0]->GetPositionY();
	while ((yTemp + moveValue >= LISTVIEW_ITEM_HEIGHT) && (m_iStartPicture > 0))
	{
		for (int k = 0; k < m_iRows; ++k)
		{
			m_mButtonItem[k]->SetNumber(m_mButtonItem[k]->GetNumber() - 1);
			if (m_iCols > 0)
				m_mButtonAction[k]->SetNumber(m_mButtonAction[k]->GetNumber() - 1);
		}

		moveValue -= LISTVIEW_ITEM_HEIGHT;
		m_iStartPicture--;

// 		std::string ttt;
// 		ttt.append("--\n");
// 		OutputDebugStr(ttt.c_str());
	}

	// move to the top
	yTemp = m_mButtonItem[0]->GetPositionY();
	while ((yTemp + moveValue <= 0) && (m_iStartPicture + m_iRows < (int) m_vItems.size() + 1))
	{
		for (int k = 0; k < m_iRows; ++k)
		{
			m_mButtonItem[k]->SetNumber(m_mButtonItem[k]->GetNumber() + 1);
			if (m_iCols > 0)
				m_mButtonAction[k]->SetNumber(m_mButtonAction[k]->GetNumber() + 1);
		}

		moveValue += LISTVIEW_ITEM_HEIGHT;
		m_iStartPicture++;

// 		std::string ttt;
// 		ttt.append("++\n");
// 		OutputDebugStr(ttt.c_str());
	}

	// can we scroll so far to the bottom?
	if (m_iStartPicture <= 0)
	{
		int newY = m_mButtonItem[0]->GetPositionY() + moveValue;
		if (newY > LISTVIEW_ITEM_HEIGHT)
		{
			moveValue = (m_mButtonItem[0]->GetPositionY() - LISTVIEW_ITEM_HEIGHT) * -1;
			ResetBox2Object();
		}
	}

	// can we scroll so far to the top?
	if (m_iStartPicture + m_iRows >= (int) m_vItems.size() + 1)
	{
		int newY = m_mButtonItem[0]->GetPositionY() + moveValue;
		if (newY < LISTVIEW_ITEM_HEIGHT)
		{
			moveValue = LISTVIEW_ITEM_HEIGHT - m_mButtonItem[0]->GetPositionY();
			ResetBox2Object();
		}
	}

	// rock'n'roll
	for (int k = 0; k < m_iRows; ++k)
	{
		m_mButtonItem[k]->Move(m_mButtonItem[k]->GetPositionX(), m_mButtonItem[k]->GetPositionY() + moveValue);
		if (m_iCols > 0)
		{
			m_mButtonAction[k]->Move(m_mButtonAction[k]->GetPositionX(), m_mButtonAction[k]->GetPositionY() + moveValue);
		}
	}
	UpdateView();

	// set image transparent
	int blend = m_iAlphaBlending / CONTROL_HEIGHT * m_mButtonItem[0]->GetPositionY();
	m_mButtonItem[0]->SetAlphaBlending(blend);
	if (m_iCols > 0)
		m_mButtonAction[0]->SetAlphaBlending(blend);

	blend = m_iAlphaBlending / CONTROL_HEIGHT * ((m_iRows * LISTVIEW_ITEM_HEIGHT) - m_mButtonItem[m_iRows - 1]->GetPositionY());
	m_mButtonItem[m_iRows - 1]->SetAlphaBlending(blend);
	if (m_iCols > 0)
		m_mButtonAction[m_iRows - 1]->SetAlphaBlending(blend);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListView::UpdateView()
///
/// \brief	Updates this object. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::UpdateView()
{
	EnterRenderLock();

	if (m_iActive > GetListSize() - 1)
		m_iActive = -1;

	for (int i = 0; i < m_iRows; ++i)
	{
		SallyAPI::GUI::CListViewItem* listItem = NULL;

		int iRequestedElem = m_iStartPicture + i;
		if ((iRequestedElem >= 0) && (iRequestedElem < (int) m_vItems.size()))
		{
			listItem = m_vItems[iRequestedElem];
		}

		if (listItem != NULL)
		{
			if (listItem->IsLocalised() == SallyAPI::GUI::LISTVIEW_LOCALISATION_FROM_PARENT)
				m_mButtonItem[i]->SetLocalised(m_bLocalised);
			else if (listItem->IsLocalised() == SallyAPI::GUI::LISTVIEW_LOCALISATION_FALSE)
				m_mButtonItem[i]->SetLocalised(false);
			else
				m_mButtonItem[i]->SetLocalised(true);

			m_mButtonItem[i]->SetText(listItem->GetText());
			
			// do we use the image list?
			if (m_bUseImageList)
			{
				if ((listItem->GetImageId() < ((int) m_vPictureList.size())) && (listItem->GetImageId() >= 0))
					m_mButtonItem[i]->SetImageId(m_vPictureList[listItem->GetImageId()]);
				else
					m_mButtonItem[i]->SetImageId(-1);
			}
			else
			{
				m_mButtonItem[i]->SetImageId(listItem->GetImageId());
			}

			if (iRequestedElem == m_iActive)
			{
				m_mButtonItem[i]->SetActive(true);
				if (m_iCols > 0)
					m_mButtonAction[i]->SetActive(true);
			}
			else
			{
				m_mButtonItem[i]->SetActive(false);
				if (m_iCols > 0)
					m_mButtonAction[i]->SetActive(false);
			}

			m_mButtonItem[i]->Visible(true);
			if (m_iCols > 0)
				m_mButtonAction[i]->Visible(true);
		}
		else
		{
			m_mButtonItem[i]->SetText("");
			m_mButtonItem[i]->SetImageId(0);
			m_mButtonItem[i]->SetActive(false);
			m_mButtonItem[i]->Visible(false);
			if (m_iCols > 0)
			{
				m_mButtonAction[i]->SetActive(false);
				m_mButtonAction[i]->Visible(false);
			}
		}
	}

	int maxPositon = (m_vItems.size() + 2 - m_iRows - 1) * CONTROL_HEIGHT;
	int postion = (m_iStartPicture + 1) * CONTROL_HEIGHT - m_mButtonItem[0]->GetPositionY();

	m_pScrollbar->SetMaxPosition(maxPositon);
	m_pScrollbar->SetPosition(postion);

	LeaveRenderLock();
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListView::SetActive(int index)
///
/// \brief	Sets an active. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param	index	Zero-based index of the. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::SetActive(int index)
{
	m_iActive = index;
	UpdateView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CListView::GetActive()
///
/// \brief	Gets the index of the active element. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \return	The active. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CListView::GetActive()
{
	return m_iActive;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CListView::GetMaxDisplayedElements()
///
/// \brief	Gets the maximum displayed elements. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \return	The maximum displayed elements. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CListView::GetMaxDisplayedElements()
{
	return m_iRows;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListView::Enable(bool enable)
///
/// \brief	Enables. 
///
/// \author	Christian Knobloch
/// \date	26.04.2010
///
/// \param	enable	true to enable, false to disable. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::Enable(bool enable)
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
/// \fn	void CListView::ShowScrollbarIfNotScrollable(bool value)
///
/// \brief	Shows the scrollbar if not scrollable. 
///
/// \author	Christian Knobloch
/// \date	27.04.2010
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::ShowScrollbarIfNotScrollable(bool value)
{
	m_pScrollbar->ShowScrollbarIfNotScrollable(value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CListView::IsScrollbarVisibleIfNotScrollbable(void)
///
/// \brief	Query if this object is scrollbar visible if not scrollbable. 
///
/// \author	Christian Knobloch
/// \date	27.04.2010
///
/// \return	true if scrollbar visible if not scrollbable, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CListView::IsScrollbarVisibleIfNotScrollbable()
{
	return m_pScrollbar->IsScrollbarVisibleIfNotScrollbable();;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CListView::SetAlphaBlending(int alphaBlending)
///
/// \brief	Sets an alpha blending. 
///
/// \author	Christian Knobloch
/// \date	03.04.2011
///
/// \param	alphaBlending	The alpha blending. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CListView::SetAlphaBlending(int alphaBlending)
{
	SallyAPI::GUI::CForm::SetAlphaBlending(alphaBlending);

	int blend = m_iAlphaBlending / CONTROL_HEIGHT * m_mButtonItem[0]->GetPositionY();
	m_mButtonItem[0]->SetAlphaBlending(blend);
	if (m_iCols > 0)
		m_mButtonAction[0]->SetAlphaBlending(blend);

	blend = m_iAlphaBlending / CONTROL_HEIGHT * ((m_iRows * LISTVIEW_ITEM_HEIGHT) - m_mButtonItem[m_iRows - 1]->GetPositionY());
	m_mButtonItem[m_iRows - 1]->SetAlphaBlending(blend);
	if (m_iCols > 0)
		m_mButtonAction[m_iRows - 1]->SetAlphaBlending(blend);
}