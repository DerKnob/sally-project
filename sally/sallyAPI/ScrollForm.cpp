////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ScrollForm.cpp
///
/// \brief	Implements the scroll Windows Form. 
///
/// \author	Christian Knobloch
/// \date	07.07.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
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

#include "ScrollForm.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScrollForm::CScrollForm(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int height, int controlId) :SallyAPI::GUI::CForm(parent, x, y, width, height, controlId),
/// m_iScrollHeight(-1), m_iScrollWidth(-1)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	07.07.2011
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	height			The height. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CScrollForm::CScrollForm(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId)
	:SallyAPI::GUI::CForm(parent, x, y, width, height, controlId), m_iScrollHeight(-1), m_iScrollWidth(-1)
{
	m_pFormScrollbarVertical = new SallyAPI::GUI::CScrollbar(this, m_iWidth - CONTROL_HEIGHT, 0, CONTROL_HEIGHT, m_iHeight,
		SallyAPI::GUI::SCROLLBAR_ALIGNMENT_VERTICAL);
	m_pFormScrollbarVertical->Visible(false);
	this->AddChild(m_pFormScrollbarVertical);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScrollForm::~CScrollForm()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	07.07.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CScrollForm::~CScrollForm()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollForm::Resize(int width, int height)
///
/// \brief	Resizes. 
///
/// \author	Christian Knobloch
/// \date	07.07.2011
///
/// \param	width	The width. 
/// \param	height	The height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollForm::Resize(int width, int height)
{
	SallyAPI::GUI::CForm::Resize(width, height);

	// move the scrollbar
	m_pFormScrollbarVertical->Move(m_iWidth - CONTROL_HEIGHT, 0);
	m_pFormScrollbarVertical->Resize(m_pFormScrollbarVertical->GetWidth(), m_iHeight);

	ResizeScrollArea(m_iScrollWidth, m_iScrollHeight);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollForm::ResizeScrollArea(int width, int height)
///
/// \brief	Resize scroll area. Set to -1 to disable scrollbars again.
///
/// \author	Christian Knobloch
/// \date	07.07.2011
///
/// \param	width	The width. 
/// \param	height	The height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollForm::ResizeScrollArea(int width, int height)
{
	m_iScrollWidth = width;
	m_iScrollHeight = height;

	if (m_iScrollHeight > m_iHeight)
	{
		m_pFormScrollbarVertical->Visible(true);
		m_pFormScrollbarVertical->SetMaxPosition(m_iScrollHeight - m_iHeight);
		m_pFormScrollbarVertical->SetPosition(0);
	}
	else
	{
		m_pFormScrollbarVertical->Visible(false);
		m_pFormScrollbarVertical->SetMaxPosition(0);
		m_pFormScrollbarVertical->SetPosition(0);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollForm::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	08.07.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollForm::RenderControl()
{
	LPDIRECT3DDEVICE9 pD3DDevice = SallyAPI::Core::CGame::GetDevice();
	SallyAPI::Core::CCamera* camera = SallyAPI::Core::CGame::GetCamera();

	int x = 0;
	int y = 0;
	GetAbsolutPosition(&x, &y);

	// render the scrollbars
	m_pFormScrollbarVertical->Render();

	if (m_pFormScrollbarVertical->GetPosition() != 0)
	{
		int i = m_pFormScrollbarVertical->GetPosition();
		i++;
	}

	// set paint rect
	RECT rect;
	rect.left = x;
	rect.right = x + m_iWidth;
	rect.top = y;
	rect.bottom = y + m_iHeight;

	// correct Scissor Rect
	RECT scissorRect = camera->GetScissorRect();

	if (rect.left < scissorRect.left)
		rect.left = scissorRect.left;
	if (rect.top < scissorRect.top)
		rect.top = scissorRect.top;
	if (rect.bottom > scissorRect.bottom)
		rect.bottom = scissorRect.bottom;
	if (rect.right > scissorRect.right)
		rect.right= scissorRect.right;
	
	RECT rectOld = camera->SetupScissorRect(rect);

	// render now the controls
	std::list<CControl*>::iterator	iter;
	iter = m_GUIControlList.begin();
	while (iter != m_GUIControlList.end())
	{
		CControl* control = *iter;

		if (control != m_pFormScrollbarVertical)
		{
			int position = m_pFormScrollbarVertical->GetPosition();

			control->Move(control->GetPositionX(), control->GetPositionY() - position);
			control->Render();
			control->Move(control->GetPositionX(), control->GetPositionY() + position);
		}
		++iter;
	}

	camera->DisableScissorRect();

	// restore old SetupScissorRect
	camera->SetupScissorRect(rectOld);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CScrollForm::ProcessMouseDown(int x, int y)
///
/// \brief	Process a mouse down. 
///
/// \author	Christian Knobloch
/// \date	19.08.2011
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CScrollForm::ProcessMouseDown(int x, int y)
{
	if (x > m_iXAbsolut + m_iWidth - CONTROL_HEIGHT)
		return SallyAPI::GUI::CForm::ProcessMouseDown(x, y);

	{
		std::list<CControl*>::iterator iter = m_GUIControlList.begin();
		while (iter != m_GUIControlList.end())
		{
			CControl* control = *iter;
			if (control != m_pFormScrollbarVertical)
				control->Move(control->GetPositionX(), control->GetPositionY() - m_pFormScrollbarVertical->GetPosition());
			++iter;
		}
	}

	bool result = SallyAPI::GUI::CForm::ProcessMouseDown(x, y);

	{
		std::list<CControl*>::iterator iter = m_GUIControlList.begin();
		while (iter != m_GUIControlList.end())
		{
			CControl* control = *iter;
			if (control != m_pFormScrollbarVertical)
				control->Move(control->GetPositionX(), control->GetPositionY() + m_pFormScrollbarVertical->GetPosition());
			++iter;
		}
	}

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CScrollForm::ProcessMouseUp(int x, int y)
///
/// \brief	Process a mouse up. 
///
/// \author	Christian Knobloch
/// \date	19.08.2011
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CScrollForm::ProcessMouseUp(int x, int y)
{
	if (x > m_iXAbsolut + m_iWidth - CONTROL_HEIGHT)
		return SallyAPI::GUI::CForm::ProcessMouseUp(x, y);

	{
		std::list<CControl*>::iterator	iter;
		iter = m_GUIControlList.begin();
		while (iter != m_GUIControlList.end())
		{
			CControl* control = *iter;
			if (control != m_pFormScrollbarVertical)
				control->Move(control->GetPositionX(), control->GetPositionY() - m_pFormScrollbarVertical->GetPosition());
			++iter;
		}
	}

	bool result = SallyAPI::GUI::CForm::ProcessMouseUp(x, y);

	{
		std::list<CControl*>::iterator	iter;
		iter = m_GUIControlList.begin();
		while (iter != m_GUIControlList.end())
		{
			CControl* control = *iter;
			if (control != m_pFormScrollbarVertical)
				control->Move(control->GetPositionX(), control->GetPositionY() + m_pFormScrollbarVertical->GetPosition());
			++iter;
		}
	}

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CScrollForm::ProcessMouseDoubleClick(int x, int y)
///
/// \brief	Process a mouse double click. 
///
/// \author	Christian Knobloch
/// \date	19.08.2011
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CScrollForm::ProcessMouseDoubleClick(int x, int y)
{
	if (x > m_iXAbsolut + m_iWidth - CONTROL_HEIGHT)
		return SallyAPI::GUI::CForm::ProcessMouseDoubleClick(x, y);

	{
		std::list<CControl*>::iterator	iter;
		iter = m_GUIControlList.begin();
		while (iter != m_GUIControlList.end())
		{
			CControl* control = *iter;
			if (control != m_pFormScrollbarVertical)
				control->Move(control->GetPositionX(), control->GetPositionY() - m_pFormScrollbarVertical->GetPosition());
			++iter;
		}
	}

	bool result = SallyAPI::GUI::CForm::ProcessMouseDoubleClick(x, y);

	{
		std::list<CControl*>::iterator	iter;
		iter = m_GUIControlList.begin();
		while (iter != m_GUIControlList.end())
		{
			CControl* control = *iter;
			if (control != m_pFormScrollbarVertical)
				control->Move(control->GetPositionX(), control->GetPositionY() + m_pFormScrollbarVertical->GetPosition());
			++iter;
		}
	}

	return result;
}
