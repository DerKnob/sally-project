////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ListView.h
///
/// \brief	Declares the list view class. 
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

#pragma once
#include "Define.h"
#include "Form.h"
#include <vector>
#include "ListViewButton.h"
#include "ListViewItem.h"
#include "Button.h"
#include "Groupbox.h"
#include "Box2DObject.h"
#include "Scrollbar.h"

#define LISTVIEW_ITEM_HEIGHT		CONTROL_HEIGHT
#define LISTVIEW_ITEM_NO			(WM_USER + 100)
#define LISTVIEW_ITEM_ACTION_NO		(WM_USER + 200)

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CListView
		///
		/// \brief	List view. 
		///
		/// \author	Christian Knobloch
		/// \date	26.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CListView :
			public SallyAPI::GUI::CForm
		{
		protected:
			std::map<int, SallyAPI::GUI::CListViewButton*>	m_mButtonItem;
			std::map<int, SallyAPI::GUI::CListViewButton*>	m_mButtonAction;
			std::vector<SallyAPI::GUI::CListViewItem*>		m_vItems;
			int												m_iActionGraphic;
			std::vector<int>								m_vPictureList;
			SallyAPI::GUI::CScrollbar*						m_pScrollbar;

			int				m_iStartPicture;
			int				m_iCols;
			int				m_iRows;
			int				m_iActive;
			bool			m_bUseImageList;
			b2World*		m_pb2World;
			CBox2DObject*	m_pb2Object;
			b2Body*			m_pb2LeftBody;
			b2Body*			m_pb2RightBody;
			int				m_iOldPositionX;
			int				m_iOldPositionY;
			
			void	CreateListView();
			void	ResetListView();
			int		GetMaxItemStart();

			bool	ResetBox2Object();

			void	OnCommandScrollbarMoved(SallyAPI::GUI::CGUIBaseObject* reporter, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
			void	OnCommandDoubleclicked(int reporterId);
			void	OnCommandMouseMove(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);

			virtual void AddChild(SallyAPI::GUI::CControl* control);
		public:
			CListView(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, 
				int rows, int actionGraphic, std::vector<int>& pictureList, int controlId = 0);
			CListView(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, 
				int rows, int actionGraphic, int controlId = 0);
			virtual ~CListView();

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

			virtual bool CheckProcessMouseUp(int x, int y);
			virtual bool CheckProcessMouseDown(int x, int y);
			virtual void Enable(bool enable);
			virtual void Timer(float timeDelta);
			virtual void SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
		};
	}
}
