////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\Define.h
///
/// \brief	Declares the defines. 
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

#pragma warning(disable: 4995)

#define GUI_APP_PLAY						GUI_APP_PLUGIN_ID + 1
#define GUI_APP_STOP						GUI_APP_PLUGIN_ID + 2
#define GUI_APP_NEXT						GUI_APP_PLUGIN_ID + 4
#define GUI_APP_PREVIOUS					GUI_APP_PLUGIN_ID + 5
#define GUI_APP_MENU_CLEAR					GUI_APP_PLUGIN_ID + 6
#define GUI_APP_MENU_SHUFFLE				GUI_APP_PLUGIN_ID + 8
#define GUI_APP_UPDATE						GUI_APP_PLUGIN_ID + 9
#define GUI_APP_DEFAULT_CD					GUI_APP_PLUGIN_ID + 10
#define GUI_APP_REMOVE_FROM_LIST			GUI_APP_PLUGIN_ID + 11
#define GUI_APP_LV_TYPE_FOLDER				GUI_APP_PLUGIN_ID + 14
#define GUI_APP_SEARCHVIEW					GUI_APP_PLUGIN_ID + 17
#define GUI_APP_FILEWALKER					GUI_APP_PLUGIN_ID + 18
#define GUI_APP_ALBUMBROWSER				GUI_APP_PLUGIN_ID + 19
#define GUI_APP_ADD_ALL_RESULTS				GUI_APP_PLUGIN_ID + 20
#define GUI_APP_COMPLETE_FOLDER				GUI_APP_PLUGIN_ID + 21
#define GUI_APP_REFRESH_FOLDER				GUI_APP_PLUGIN_ID + 22
#define GUI_APP_PICTURE_PRESSED				GUI_APP_PLUGIN_ID + 23
#define GUI_APP_MOST_PLAYED					GUI_APP_PLUGIN_ID + 24
#define GUI_APP_ADD_ALL_RESULTS_MOST_PLAYED	GUI_APP_PLUGIN_ID + 25
#define GUI_APP_SAVE_PLAYLIST				GUI_APP_PLUGIN_ID + 28
#define GUI_APP_PICTURE_MAXIMIZED_PRESSED	GUI_APP_PLUGIN_ID + 29
#define GUI_APP_ADD_ALL_ALBUM				GUI_APP_PLUGIN_ID + 30
#define GUI_APP_ALBUM_BACK					GUI_APP_PLUGIN_ID + 31
#define GUI_APP_UPDATE_DB					GUI_APP_PLUGIN_ID + 32
#define GUI_APP_GET_COVERS					GUI_APP_PLUGIN_ID + 33
#define	GUI_APP_GET_COVERS_DONE				GUI_APP_PLUGIN_ID + 34
#define	GUI_APP_START_FULLSCREEN			GUI_APP_PLUGIN_ID + 35
#define	GUI_APP_STOP_FULLSCREEN				GUI_APP_PLUGIN_ID + 36
#define GUI_APP_UPDATE_DB_DONE				GUI_APP_PLUGIN_ID + 37
#define GUI_APP_TIMER_SEARCH				GUI_APP_PLUGIN_ID + 38
#define GUI_APP_SEARCH						GUI_APP_PLUGIN_ID + 39
#define GUI_APP_UPDATE_MP3_INFO				GUI_APP_PLUGIN_ID + 40
#define GUI_APP_ALWAYS_SHOW_HDS				GUI_APP_PLUGIN_ID + 41
#define GUI_APP_ALBUM_COVER_MP3HELPER_LOADED	GUI_APP_PLUGIN_ID + 42
#define GUI_APP_ADD_ALL_CURRENT_FOLDER		GUI_APP_PLUGIN_ID + 43
#define GUI_APP_SEARCH_EDIT					GUI_APP_PLUGIN_ID + 44
#define GUI_APP_FILTER_ARTIST_EDIT			GUI_APP_PLUGIN_ID + 45
#define GUI_APP_FILTER_ALBUM_EDIT			GUI_APP_PLUGIN_ID + 46
#define GUI_APP_FILTER_GENRE_EDIT			GUI_APP_PLUGIN_ID + 47
#define GUI_APP_PLAY_SNAP_BACK				GUI_APP_PLUGIN_ID + 48
#define GUI_APP_UPDATE_VIDEO_INFO			GUI_APP_PLUGIN_ID + 49
#define GUI_APP_DELETE_PLAYLIST				GUI_APP_PLUGIN_ID + 50
#define GUI_APP_DELETE_DATABASE				GUI_APP_PLUGIN_ID + 51
#define GUI_APP_AUTOSTART_PLAY				GUI_APP_PLUGIN_ID + 52
#define GUI_APP_UPDATE_COVERS_CANCEL		GUI_APP_PLUGIN_ID + 53
#define GUI_APP_UPDATE_DB_CANCEL			GUI_APP_PLUGIN_ID + 54
#define GUI_APP_MENU_SHUFFLE_BOTTOM			GUI_APP_PLUGIN_ID + 55
#define GUI_APP_SCREENSAVER_PLAY			GUI_APP_PLUGIN_ID + 56
#define GUI_APP_SCREENSAVER_NEXT			GUI_APP_PLUGIN_ID + 57
#define GUI_APP_SCREENSAVER_PREVIOUS		GUI_APP_PLUGIN_ID + 58
#define GUI_APP_SHOW_INFO					GUI_APP_PLUGIN_ID + 59
#define GUI_APP_REMOVE_RATING				GUI_APP_PLUGIN_ID + 60
#define GUI_APP_INFO_RATING_CHANGED			GUI_APP_PLUGIN_ID + 61
#define GUI_APP_MUSIC_BLEND_IN_OUT			GUI_APP_PLUGIN_ID + 62
#define GUI_APP_SCREENSAVER_RATING_CHANGED	GUI_APP_PLUGIN_ID + 63
#define GUI_APP_DEFAULT_RATING_CHANGED		GUI_APP_PLUGIN_ID + 64
#define GUI_APP_PLAYLIST					GUI_APP_PLUGIN_ID + 67
#define GUI_APP_SCHEDULER_CREATE_DB			GUI_APP_PLUGIN_ID + 68
#define GUI_APP_FOLDER_PIC_CHANGED			GUI_APP_PLUGIN_ID + 69
#define GUI_APP_RENDER_LOCK					GUI_APP_PLUGIN_ID + 70
#define GUI_APP_RENDER_RELEASE				GUI_APP_PLUGIN_ID + 71
#define GUI_APP_SCHEDULER_DOWNLOAD_COVERS	GUI_APP_PLUGIN_ID + 72
#define GUI_APP_FOLDER_PIC_LOADED			GUI_APP_PLUGIN_ID + 73
#define GUI_APP_SEARCH_TYPE					GUI_APP_PLUGIN_ID + 74
#define GUI_APP_FULL_AUTOMATIC_PLAYLIST_HISTORY		GUI_APP_PLUGIN_ID + 75
#define GUI_APP_FULL_PREVENT_DUPLICATES		GUI_APP_PLUGIN_ID + 76
#define GUI_APP_RESET_LAST_BUILD_DATABASE	GUI_APP_PLUGIN_ID + 77
#define GUI_APP_MENU_REMOVE_BEFORE			GUI_APP_PLUGIN_ID + 78
#define GUI_APP_MENU_REMOVE_AFTER			GUI_APP_PLUGIN_ID + 79
#define GUI_APP_LIKE_IT						GUI_APP_PLUGIN_ID + 80
#define GUI_APP_RELOAD_MP3_INFO				GUI_APP_PLUGIN_ID + 81
#define GUI_APP_RELOAD_MP3_STOP				GUI_APP_PLUGIN_ID + 82
#define GUI_APP_RELOAD_MP3_START			GUI_APP_PLUGIN_ID + 83

#define GUI_APP_MENU_NOW_PLAYING			GUI_APP_PLUGIN_ID + 90
#define GUI_APP_MENU_ADDMUSIC_SEARCH		GUI_APP_PLUGIN_ID + 91
#define GUI_APP_MENU_ADDMUSIC_ALBUM			GUI_APP_PLUGIN_ID + 92
#define GUI_APP_MENU_ADDMUSIC_FAVORITES		GUI_APP_PLUGIN_ID + 93
#define GUI_APP_MENU_ADDMUSIC_EXPLORER		GUI_APP_PLUGIN_ID + 94
#define GUI_APP_MENU_SAVE_PLAYLIST			GUI_APP_PLUGIN_ID + 95
#define GUI_APP_MENU_LOAD_PLAYLIST			GUI_APP_PLUGIN_ID + 96
#define GUI_APP_MENU_DELETE_PLAYLIST		GUI_APP_PLUGIN_ID + 97
#define GUI_APP_MENU_DBUPDATE_STARTED		GUI_APP_PLUGIN_ID + 98
#define GUI_APP_MENU_DBUPDATE_FINISHED		GUI_APP_PLUGIN_ID + 99
#define GUI_APP_THREAD_ON_COMMAND_PLAY		GUI_APP_PLUGIN_ID + 100
#define GUI_APP_PLAY_LAST_ADDED				GUI_APP_PLUGIN_ID + 101

#define GUI_APP_MY_PLAYLISTS				GUI_APP_PLUGIN_ID + 102
#define GUI_APP_AUTO_PLAYLISTS				GUI_APP_PLUGIN_ID + 103
#define GUI_APP_SHOW_ERROR_MESSAGE			GUI_APP_PLUGIN_ID + 104
#define GUI_APP_HIDE_SCREENSAVER_MENU		GUI_APP_PLUGIN_ID + 105
#define GUI_APP_SORT_BY_NAME				GUI_APP_PLUGIN_ID + 106
#define GUI_APP_SORT_BY_DATE				GUI_APP_PLUGIN_ID + 107
#define GUI_APP_CLEAR_TEXT_SEARCH			GUI_APP_PLUGIN_ID + 108
#define GUI_APP_TIMER_SEND_FACEBOOK			GUI_APP_PLUGIN_ID + 109

#define GUI_APP_SELECT_FOLDER				(GUI_APP_PLUGIN_ID + 200)
#define GUI_APP_EMPTEY_SELECTION			(GUI_APP_PLUGIN_ID + 250)
#define GUI_APP_SELECT_THIS_FOLDER			(GUI_APP_PLUGIN_ID + 251)

#define CHECK(pIn) (pIn == NULL) ? "-" : pIn

#define SMALL_VIDEO_WIDTH		m_pCoverForm->GetWidth()
#define SMALL_VIDEO_HEIGHT		m_pCoverForm->GetHeight() - 300

#define SMALL_VIDEO_X			WINDOW_BORDER_H
#define SMALL_VIDEO_Y			110

#define SMALL_PICTURE_X			((m_pCoverForm->GetWidth() - m_pAlbumImageContainer->GetWidth()) / 2) + WINDOW_BORDER_H
#define SMALL_PICTURE_Y			(((m_pCoverForm->GetHeight() - 320 - m_iCoverSize) / 2) + 130)

#define BIG_PICTURE_X			(WINDOW_WIDTH / 2) - m_pAlbumImageContainer->GetWidth() - 40
#define BIG_PICTURE_Y			(WINDOW_HEIGHT - m_iCoverSize) / 2

#define COVER_OUT_X				((-m_iCoverSize) - 200)

#define PICTURE_WIDTH_MAX_SMALL			170
#define PICTURE_HEIGHT_MAX_SMALL		170
#define PICTURE_HEIGHT_MAX_SMALL_SPACE	50
#define PICTURE_WIDTH_MAX_SMALL_SPACE	50

#define PICTURE_WIDTH_MAX_SMALL_COMPLETE (PICTURE_WIDTH_MAX_SMALL + PICTURE_WIDTH_MAX_SMALL_SPACE)