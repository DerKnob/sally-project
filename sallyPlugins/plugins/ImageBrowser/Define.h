////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\Define.h
///
/// \brief	Declares the defines. 
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
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU General Public License for more details.
/// 
/// You should have received a copy of the GNU General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#define PICTURE_WIDTH_MAX_SMALL			128
#define PICTURE_HEIGHT_MAX_SMALL		128
#define PICTURE_HEIGHT_MAX_SMALL_SPACE	20
#define PICTURE_WIDTH_MAX_SMALL_SPACE	20

#define PICTURE_WIDTH_MAX_SMALL_COMPLETE (PICTURE_WIDTH_MAX_SMALL + PICTURE_WIDTH_MAX_SMALL_SPACE)

#define INFO_VIEW_HEIGHT				440
#define INFO_VIEW_HEIGHT_HIDE			-440
#define INFO_VIEW_HEIGHT_SHOW_BUTTON	-370
#define INFO_VIEW_HEIGHT_SHOW_INFO		-20

#define PICTURE_WIDTH_MAX_BIG			(WINDOW_WIDTH - 50)
#define PICTURE_HEIGHT_MAX_BIG			(WINDOW_HEIGHT - 50 - 70)

#define GUI_APP_PREVIOUS_SCREENSAVER		GUI_APP_PLUGIN_ID + 1
#define GUI_APP_NEXT_SCREENSAVER			GUI_APP_PLUGIN_ID + 2
#define GUI_APP_PREVIOUS					GUI_APP_PLUGIN_ID + 3
#define GUI_APP_NEXT						GUI_APP_PLUGIN_ID + 4
#define GUI_APP_PLAY						GUI_APP_PLUGIN_ID + 5
#define GUI_APP_SCHEDULER_CREATE_DB			GUI_APP_PLUGIN_ID + 6
#define GUI_APP_CHANGE_DIRECTORY			GUI_APP_PLUGIN_ID + 7
#define GUI_APP_DIASHOW_INCLUDE_SUBDIRS		GUI_APP_PLUGIN_ID + 8
#define GUI_APP_START_DIASHOW				GUI_APP_PLUGIN_ID + 9
#define GUI_APP_FOLDER_UP					GUI_APP_PLUGIN_ID + 10
#define GUI_APP_INCLUDE_SUBFOLDERS			GUI_APP_PLUGIN_ID + 11
#define GUI_APP_ZOOM_BACKGROUND				GUI_APP_PLUGIN_ID + 12
#define GUI_APP_BIG_PIC_LOADED_SWITCH		GUI_APP_PLUGIN_ID + 13
#define GUI_APP_SHUFFLE						GUI_APP_PLUGIN_ID + 14
#define	GUI_APP_STOP_FULLSCREEN				GUI_APP_PLUGIN_ID + 15
#define GUI_APP_FOLDER_PIC_CHANGED			GUI_APP_PLUGIN_ID + 16
#define GUI_APP_SCREENSAVER_PIC_LOADED		GUI_APP_PLUGIN_ID + 17
#define GUI_APP_BIG_PIC_LOADED_ZOOM			GUI_APP_PLUGIN_ID + 18
#define GUI_APP_PAUSE						GUI_APP_PLUGIN_ID + 19
#define GUI_APP_UPDATE_DB					GUI_APP_PLUGIN_ID + 20
#define GUI_APP_DELETE_DATABASE				GUI_APP_PLUGIN_ID + 21
#define GUI_APP_UPDATE_DB_DONE				GUI_APP_PLUGIN_ID + 22
#define GUI_APP_SHOW_FOLDERS				GUI_APP_PLUGIN_ID + 23
#define GUI_APP_SHOW_DATE					GUI_APP_PLUGIN_ID + 24
#define GUI_APP_DATABASE_LOAD_DONE			GUI_APP_PLUGIN_ID + 25
#define GUI_APP_UPDATE_DB_CANCEL			GUI_APP_PLUGIN_ID + 26
#define GUI_APP_RENDER_LOCK					GUI_APP_PLUGIN_ID + 27
#define GUI_APP_RENDER_RELEASE				GUI_APP_PLUGIN_ID + 28
#define GUI_APP_ALWAYS_SHOW_HDS				GUI_APP_PLUGIN_ID + 29
#define GUI_APP_REFRESH_FOLDER				GUI_APP_PLUGIN_ID + 30
#define GUI_APP_FOLDER_PIC_LOADED			GUI_APP_PLUGIN_ID + 40
#define GUI_APP_SHOW_INFO_VIEW				GUI_APP_PLUGIN_ID + 41
#define GUI_APP_SHOW_INFO					GUI_APP_PLUGIN_ID + 42
#define GUI_APP_PREVIOUS_SCREENSAVER_TIMER	GUI_APP_PLUGIN_ID + 43
#define GUI_APP_NEXT_SCREENSAVER_TIMER		GUI_APP_PLUGIN_ID + 44
#define GUI_APP_RESET_LAST_BUILD_DATABASE	GUI_APP_PLUGIN_ID + 45


#define GUI_APP_SELECT_FOLDER				(GUI_APP_PLUGIN_ID + 100)
#define GUI_APP_EMPTEY_SELECTION			(GUI_APP_PLUGIN_ID + 200)
#define GUI_APP_SELECT_THIS_FOLDER			(GUI_APP_PLUGIN_ID + 300)