////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ListViewExt.h
///
/// \brief	Declares the list view ext class. 
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

#pragma once
#include "Define.h"
#include "Form.h"
#include <vector>
#include "ListViewButton.h"
#include "ListViewItem.h"
#include "Box2DObject.h"
#include "Scrollbar.h"
#include "ParameterListItem.h"
#include "ParameterKeyValue.h"
#include "ParameterListItemHoldClick.h"

#define LISTVIEW_ITEM_HEIGHT		CONTROL_HEIGHT

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CListViewExt
		///
		/// \brief	List view ext.
		///
		/// \author	Christian Knobloch
		/// \date	26.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CListViewExt :
			public SallyAPI::GUI::CForm
		{
		protected:
			std::vector<SallyAPI::GUI::CListViewItem*>		m_vItems;
			SallyAPI::GUI::CScrollbar*						m_pScrollbar;
			std::map<int, std::map<int, SallyAPI::GUI::CListViewButton*>>	m_mButton;

			int				m_iStartItem;
			int				m_iCols;
			int				m_iRows;
			int				m_iActive;
			b2World*		m_pb2World;
			CBox2DObject*	m_pb2Object;
			b2Body*			m_pb2LeftBody;
			b2Body*			m_pb2RightBody;
			int				m_iOldPositionX;
			int				m_iOldPositionY;
			std::map<int, int> m_mWidthList;
			bool			m_bSorting;
			int				m_iSortingMove;
			int				m_iSortingControl;
			
			void	ResetListView();
			int		GetMaxItemStart();

			bool	ResetBox2Object();

			void	OnCommandScrollbarMoved(SallyAPI::GUI::CGUIBaseObject* reporter, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
			void	OnCommandDoubleclicked(int reporterId);
			void	OnCommandMouseMove(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
			void	OnCommandSorting(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
			void	OnCommandStartDragging(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
			void	OnCommandItemHoldClicked(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

			virtual void AddChild(SallyAPI::GUI::CControl* control);
		public:
			CListViewExt(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, 
				int rows, std::map<int, int> widthList, int controlId = 0);
			virtual ~CListViewExt();

			virtual void	Resize(int width, int height);

			int			GetMaxDisplayedElements();
			int			GetListSize();

			void		AddItem(SallyAPI::GUI::CListViewItem listItem);
			bool		RemoveItem(int index);

			void		UpdateView();
			void		Clear();
			std::vector<SallyAPI::GUI::CListViewItem*>*	GetListItems();
			SallyAPI::GUI::CListViewItem*				GetItem(int index);

			void		SetActive(int index);
			int			GetActive();

			int			GetStartItem();
			void		SetStartItem(int startItem);

			void		SetPictureList(std::vector<int>& pictureList);
			void		RemovePictureList();
			void		SetActionImageID(int icon);

			void		ShowScrollbarIfNotScrollable(bool value);
			bool		IsScrollbarVisibleIfNotScrollbable();

			virtual void SetAlphaBlending(int alphaBlending);
			virtual bool CheckProcessMouseUp(int x, int y);
			virtual bool CheckProcessMouseDown(int x, int y);
			virtual void Enable(bool enable);
			virtual void Timer(float timeDelta);
			virtual void SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
		};
	}
}
