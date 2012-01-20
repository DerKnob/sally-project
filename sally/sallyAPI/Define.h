////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Define.h
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
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#pragma warning(disable: 4251)
#pragma warning(disable: 4229)
#pragma warning(disable: 4508)
#pragma warning(disable: 4995)

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "gdiplus.lib")

#pragma comment(lib, "sqlite3.lib")

#include <tchar.h>
#include <windows.h>
#include "_export.h"

#define SALLY_API_VERSION "0.8.1.1"

// ***************************************************************************** //

#define KEYDOWN(name, key) (name[key] & 0x80) 
#define KEYDOWNONCE(name, old, key) (KEYDOWN(name, key)) && (!KEYDOWN(old, key))

#define SafeRelease(pInterface) if(pInterface != NULL) {pInterface->Release(); pInterface=NULL;}
#define SafeDelete(pObject)	if(pObject != NULL) {delete pObject; pObject=NULL;}
#define CheckForNull(pObject) (pObject == NULL) ? "" : pObject;
#define ARRAY_SIZE( x ) ( (sizeof((x))/sizeof((x)[0])) )

#define DIRECTINPUT_VERSION 0x0800

// ** WINDOWMESSAGES *********************************************************** //

#define GUI_MESSAGE_UPDATE_ABSOLUTE_POSITION	100
#define GUI_MESSAGE_MOUSE_UP_INFO				101
#define GUI_CONTROL_MOVED						102
#define GUI_CONTROL_RESIZED						103
#define GUI_FORM_DOUBLECLICKED					104
#define GUI_FORM_CLICKED						105
#define GUI_CONTROL_BLENDED						106
#define GUI_CONTROL_ROTATED_X					107
#define GUI_CONTROL_ROTATED_Y					108
#define GUI_CONTROL_ROTATED_Z					109

#define MOUSEBUTTONDOWN(key)		(key & 0x80)

#define MOUSEBUTTON_LEFT			0
#define MOUSEBUTTON_RIGHT			1
#define MOUSEBUTTON_MIDDLE			2

#define ICON_SIZE					22

#define WINDOW_WIDTH				SallyAPI::Core::CGame::GetScreenWidth()
#define WINDOW_HEIGHT				SallyAPI::Core::CGame::GetScreenHeight()

#define MENU_HEIGHT					60
#define MENU_WIDTH					200
#define WINDOW_BORDER_H				24
#define WINDOW_BORDER_V				24
#define WIZARD_PANEL_WIDTH			780
#define WIZARD_PANEL_HEIGHT			540

#define APP_GRAPHIC_ID				40000
#define CONTROL_HEIGHT				30
#define GUI_APP_PLUGIN_ID			10000

#define MAX_WIDTH					1920
#define MAX_HEIGHT					1200

#define KEYBOARD_REQUEST_WORD_MAX	15

/************************************************************************/
/* GUI COMMANDS                                                         */
/************************************************************************/

#define GUI_PROCESSBAR_CLICKED							1000
#define GUI_PROCESSBAR_MOVED							1001
#define GUI_BUTTON_CLICKED								1002
#define GUI_EDIT_CHANGED								1004
#define GUI_LABEL_CLICKED								1005
#define GUI_CHECKBOX_CLICKED							1006
#define GUI_RADIOBUTTON_CLICKED							1007

#define GUI_LISTVIEW_ITEM_CLICKED						1008
#define GUI_LISTVIEW_ITEM_ACTION_CLICKED				1009

#define GUI_SLIDER_CLICKED								1010
#define GUI_SLIDER_MOVED								1011

#define GUI_IMAGEBOX_CLICKED							1012
#define GUI_IMAGEBOX_DOUBLECLICKED						1013

#define GUI_SCROLLLIST_CLICKED							1014

#define GUI_DROPDOWN_CLICKED							1016
#define GUI_DROPDOWN_SELECTED							1017
#define GUI_DROPDOWN_CANCELED							1018
#define GUI_NUMBER_SELECTOR_CHANGED						1019

#define GUI_LISTVIEW_ITEM_DOUBLECLICKED					1020
#define GUI_LISTVIEW_ITEM_START_DRAGGING				1021
#define GUI_LISTVIEW_ITEM_DRAGGED						1022

#define GUI_SCROLLBAR_CLICKED							1023
#define GUI_SCROLLBAR_MOVED								1024

#define GUI_BREADCRUMB_START_CLICKED					1025

#define GUI_LABEL_HOLDCLICKED							1027
#define GUI_BUTTON_HOLDCLICKED							1028
#define GUI_IMAGEBOX_HOLDCLICKED						1029
#define GUI_LISTVIEW_ITEM_HOLDCLICKED					1030

#define GUI_MOUSEMOVE_RIGHT								1030
#define GUI_MOUSEMOVE_LEFT								1031
#define GUI_MOUSEMOVE_UP								1033
#define GUI_MOUSEMOVE_DOWN								1034
#define GUI_MOUSEMOVE_RIGHT_UP							1035
#define GUI_MOUSEMOVE_RIGHT_DOWN						1036
#define GUI_MOUSEMOVE_LEFT_UP							1037
#define GUI_MOUSEMOVE_LEFT_DOWN							1038

#define GUI_MOUSEMOVE_RIGHT_SNAPED						1039
#define GUI_MOUSEMOVE_LEFT_SNAPED						1040
#define GUI_MOUSEMOVE_UP_SNAPED							1041
#define GUI_MOUSEMOVE_DOWN_SNAPED						1042
#define GUI_MOUSEMOVE_RIGHT_UP_SNAPED					1043
#define GUI_MOUSEMOVE_RIGHT_DOWN_SNAPED					1044
#define GUI_MOUSEMOVE_LEFT_UP_SNAPED					1045
#define GUI_MOUSEMOVE_LEFT_DOWN_SNAPED					1046

#define GUI_MOUSEMOVE_RIGHT_FAST						1047
#define GUI_MOUSEMOVE_LEFT_FAST							1048
#define GUI_MOUSEMOVE_UP_FAST							1049
#define GUI_MOUSEMOVE_DOWN_FAST							1050
#define GUI_MOUSEMOVE_RIGHT_UP_FAST						1051
#define GUI_MOUSEMOVE_RIGHT_DOWN_FAST					1052
#define GUI_MOUSEMOVE_LEFT_UP_FAST						1053
#define GUI_MOUSEMOVE_LEFT_DOWN_FAST					1054

#define GUI_MOUSEMOVE_RIGHT_FAST_SNAPED					1055
#define GUI_MOUSEMOVE_LEFT_FAST_SNAPED					1056
#define GUI_MOUSEMOVE_UP_FAST_SNAPED					1057
#define GUI_MOUSEMOVE_DOWN_FAST_SNAPED					1058
#define GUI_MOUSEMOVE_RIGHT_UP_FAST_SNAPED				1059
#define GUI_MOUSEMOVE_RIGHT_DOWN_FAST_SNAPED			1060
#define GUI_MOUSEMOVE_LEFT_UP_FAST_SNAPED				1061
#define GUI_MOUSEMOVE_LEFT_DOWN_FAST_SNAPED				1062

#define GUI_DROPDOWN_CHANGED							1063
#define GUI_RATING_CHANGED								1064

#define GUI_MESSAGE_CONTROL_SCROLLED					1065
#define GUI_LISTVIEW_ITEM_ADDED							1066

#define GUI_MOUSEMOVE_SMOOTH_RIGHT						1070
#define GUI_MOUSEMOVE_SMOOTH_LEFT						1071
#define GUI_MOUSEMOVE_SMOOTH_UP							1072
#define GUI_MOUSEMOVE_SMOOTH_DOWN						1073

#define GUI_SCREENSAVER_CONTROL_CLICKED					1012
#define GUI_SCREENSAVER_CONTROL_DOUBLECLICKED			1013

/************************************************************************/
/* THEME PICTURES                                                       */
/************************************************************************/

// ** Seperators ********************************************************************* //

#define GUI_THEME_SEPERATOR_H_LEFT						2000
#define GUI_THEME_SEPERATOR_H_CENTER					2001
#define GUI_THEME_SEPERATOR_H_RIGHT						2002

#define GUI_THEME_SEPERATOR_V_TOP						2003
#define GUI_THEME_SEPERATOR_V_CENTER					2004
#define GUI_THEME_SEPERATOR_V_BOTTOM					2005

// ** Edit ********************************************************************* //

#define GUI_THEME_EDIT_LEFT								2006
#define GUI_THEME_EDIT									2007
#define GUI_THEME_EDIT_RIGHT							2008

// ** Processbar *************************************************************** //

#define GUI_THEME_PROCESSBAR_LEFT						2009
#define GUI_THEME_PROCESSBAR							2010
#define GUI_THEME_PROCESSBAR_RIGHT						2011
#define GUI_THEME_PROCESSBAR_2LEFT						2012
#define GUI_THEME_PROCESSBAR_2							2013
#define GUI_THEME_PROCESSBAR_2RIGHT						2014

// ** Label ******************************************************************** //

#define GUI_THEME_LABEL_LEFT							2015
#define GUI_THEME_LABEL									2016
#define GUI_THEME_LABEL_RIGHT							2017

// ** Checkbox ****************************************************************** //

#define GUI_THEME_CHECKBOX_LEFT							2018
#define GUI_THEME_CHECKBOX								2019
#define GUI_THEME_CHECKBOX_RIGHT						2020
#define GUI_THEME_CHECKBOX_CHECKED						2021

// ** Radiobutton *************************************************************** //

#define GUI_THEME_RADIO_LEFT							2022
#define GUI_THEME_RADIO									2023
#define GUI_THEME_RADIO_RIGHT							2024
#define GUI_THEME_RADIO_CHECKED							2025

// ** Slieder ******************************************************************** //

#define GUI_THEME_SLIDER_LEFT							2026
#define GUI_THEME_SLIDER								2027
#define GUI_THEME_SLIDER_RIGHT							2028
#define GUI_THEME_SLIDER_2LEFT							2029
#define GUI_THEME_SLIDER_2								2030
#define GUI_THEME_SLIDER_2RIGHT							2031
#define GUI_THEME_SLIDER_KNOB							2032

// ** GroupBox ******************************************************************* //

#define GUI_THEME_GROUPBOX_LEFT_TOP						2033
#define GUI_THEME_GROUPBOX_TOP							2034
#define GUI_THEME_GROUPBOX_RIGHT_TOP					2035
#define GUI_THEME_GROUPBOX_LEFT							2036
#define GUI_THEME_GROUPBOX_CENTER						2037
#define GUI_THEME_GROUPBOX_RIGHT						2038
#define GUI_THEME_GROUPBOX_LEFT_BOTTOM					2039
#define GUI_THEME_GROUPBOX_BOTTOM						2040
#define GUI_THEME_GROUPBOX_RIGHT_BOTTOM					2041
#define GUI_THEME_GROUPBOX_ARROW_BOTTOM					2042
#define GUI_THEME_GROUPBOX_ARROW_TOP					2043

// ** EditBox ******************************************************************** //

#define GUI_THEME_EDITBOX_LEFT_TOP						2050
#define GUI_THEME_EDITBOX_TOP							2051
#define GUI_THEME_EDITBOX_RIGHT_TOP						2052
#define GUI_THEME_EDITBOX_LEFT							2053
#define GUI_THEME_EDITBOX_CENTER						2054
#define GUI_THEME_EDITBOX_RIGHT							2055
#define GUI_THEME_EDITBOX_LEFT_BOTTOM					2056
#define GUI_THEME_EDITBOX_BOTTOM						2057
#define GUI_THEME_EDITBOX_RIGHT_BOTTOM					2058

// ** LabelBox ******************************************************************* //

#define GUI_THEME_LABELBOX_LEFT_TOP						2060
#define GUI_THEME_LABELBOX_TOP							2061
#define GUI_THEME_LABELBOX_RIGHT_TOP					2062
#define GUI_THEME_LABELBOX_LEFT							2063
#define GUI_THEME_LABELBOX_CENTER						2064
#define GUI_THEME_LABELBOX_RIGHT						2065
#define GUI_THEME_LABELBOX_LEFT_BOTTOM					2066
#define GUI_THEME_LABELBOX_BOTTOM						2067
#define GUI_THEME_LABELBOX_RIGHT_BOTTOM					2068

// ** ListView Button ************************************************************ //

#define GUI_THEME_LISTVIEWBUTTON_NORMAL1_LEFT				2073
#define GUI_THEME_LISTVIEWBUTTON_NORMAL1					2074
#define GUI_THEME_LISTVIEWBUTTON_NORMAL1_RIGHT				2075
#define GUI_THEME_LISTVIEWBUTTON_NORMAL1_SEPERATOR_LEFT		2076
#define GUI_THEME_LISTVIEWBUTTON_NORMAL1_SEPERATOR_RIGHT	2077

#define GUI_THEME_LISTVIEWBUTTON_NORMAL2_LEFT				2083
#define GUI_THEME_LISTVIEWBUTTON_NORMAL2					2084
#define GUI_THEME_LISTVIEWBUTTON_NORMAL2_RIGHT				2085
#define GUI_THEME_LISTVIEWBUTTON_NORMAL2_SEPERATOR_LEFT		2086
#define GUI_THEME_LISTVIEWBUTTON_NORMAL2_SEPERATOR_RIGHT	2087

#define GUI_THEME_LISTVIEWBUTTON_SELECTED_LEFT				2078
#define GUI_THEME_LISTVIEWBUTTON_SELECTED					2079
#define GUI_THEME_LISTVIEWBUTTON_SELECTED_RIGHT				2080
#define GUI_THEME_LISTVIEWBUTTON_SELECTED_SEPERATOR_LEFT	2081
#define GUI_THEME_LISTVIEWBUTTON_SELECTED_SEPERATOR_RIGHT	2082

#define GUI_THEME_LISTVIEWBUTTON_SORTER						2088

// ** NotificationBox ************************************************************ //

#define GUI_THEME_NOTIFICATIONBOX_LEFT_TOP						2089
#define GUI_THEME_NOTIFICATIONBOX_TOP							2090
#define GUI_THEME_NOTIFICATIONBOX_RIGHT_TOP					2091
#define GUI_THEME_NOTIFICATIONBOX_LEFT							2092
#define GUI_THEME_NOTIFICATIONBOX_CENTER						2093
#define GUI_THEME_NOTIFICATIONBOX_RIGHT						2094
#define GUI_THEME_NOTIFICATIONBOX_LEFT_BOTTOM					2095
#define GUI_THEME_NOTIFICATIONBOX_BOTTOM						2096
#define GUI_THEME_NOTIFICATIONBOX_RIGHT_BOTTOM					2097
#define GUI_THEME_NOTIFICATIONBOX_ARROW_BOTTOM					2098
#define GUI_THEME_NOTIFICATIONBOX_ARROW_TOP					2099

// ** TabControl Button ************************************************************ //

#define GUI_THEME_TABCONTROL_LEFT_TOP				2100
#define GUI_THEME_TABCONTROL_TOP					2101
#define GUI_THEME_TABCONTROL_RIGHT_TOP				2102
#define GUI_THEME_TABCONTROL_LEFT					2103
#define GUI_THEME_TABCONTROL_CENTER					2104
#define GUI_THEME_TABCONTROL_RIGHT					2105
#define GUI_THEME_TABCONTROL_LEFT_BOTTOM			2106
#define GUI_THEME_TABCONTROL_BOTTOM					2107
#define GUI_THEME_TABCONTROL_RIGHT_BOTTOM			2108

#define GUI_THEME_TABCONTROL_BUTTON_NORMAL_LEFT		2110
#define GUI_THEME_TABCONTROL_BUTTON_NORMAL			2111
#define GUI_THEME_TABCONTROL_BUTTON_NORMAL_RIGHT	2112
#define GUI_THEME_TABCONTROL_BUTTON_SELECTED_LEFT	2113
#define GUI_THEME_TABCONTROL_BUTTON_SELECTED		2114
#define GUI_THEME_TABCONTROL_BUTTON_SELECTED_RIGHT	2115

// ** Label Big **************************************************************** //

#define GUI_THEME_LABEL_BIG_LEFT					2116
#define GUI_THEME_LABEL_BIG							2117
#define GUI_THEME_LABEL_BIG_RIGHT					2118

// ***************************************************************************** //

#define GUI_THEME_SALLY_MENU						2150
#define GUI_THEME_SALLY_CLOSE						2151
#define GUI_THEME_SALLY_POPUP_APPSELECTOR			2152
#define GUI_THEME_SALLY_LOADING						2153
#define GUI_THEME_SALLY_BACKGROUND					2154
#define GUI_THEME_SALLY_LOGOUT						2155
#define GUI_THEME_SALLY_SETTINGS					2156
#define GUI_THEME_SALLY_POPUP_NORMAL				2157
#define GUI_THEME_SALLY_FACEBOOK					2158

#define GUI_THEME_SALLY_SCREENSAVER					2159
#define GUI_THEME_SALLY_LOGO_SMALL					2160
#define GUI_THEME_SALLY_LOGO_BIG					2161
#define GUI_THEME_SALLY_WORKING_SMALL				2163
#define GUI_THEME_SALLY_MINIMIZE					2164
#define GUI_THEME_SALLY_WORKING_BIG					2165
#define GUI_THEME_SALLY_LOCK						2166
#define GUI_THEME_SALLY_SCREENSAVER_SMALL			2167

#define GUI_THEME_SALLY_POPUP_WORKING				2170
#define GUI_THEME_SALLY_BLACK_BACKGROUND			2172
#define GUI_THEME_SALLY_SETTINGS_TOP_BORDER			2173
#define GUI_THEME_SALLY_SETTINGS_BOTTOM_BORDER		2174

// ** ListView ***************************************************************** //

#define GUI_THEME_LISTVIEW_ITEM						2201
#define GUI_THEME_LISTVIEW_UP						2202
#define GUI_THEME_LISTVIEW_DOWN						2203
#define GUI_THEME_LISTVIEW_UP_FAST					2204
#define GUI_THEME_LISTVIEW_DOWN_FAST				2205

// ** Keyboard ***************************************************************** //

#define GUI_THEME_SALLY_KEYBOARD_CAPS				2204
#define GUI_THEME_SALLY_KEYBOARD_CAPSLOCK			2205
#define GUI_THEME_SALLY_KEYBOARD_CLEAR				2206
#define GUI_THEME_SALLY_KEYBOARD_BACKSPACE			2207

// ** Audio ******************************************************************** //

#define GUI_THEME_SALLY_AUDIO_HIGH					2208
#define GUI_THEME_SALLY_AUDIO_LOW					2209
#define GUI_THEME_SALLY_AUDIO_MEDIUM				2210
#define GUI_THEME_SALLY_AUDIO_MUTED					2211

// ** BreadCrumb ***************************************************************** //

#define GUI_THEME_BREADCRUMB_SEPERATOR				2214
#define GUI_THEME_BREADCRUMB						2215
#define GUI_THEME_BREADCRUMB_LEFT					2216
#define GUI_THEME_BREADCRUMB_RIGHT					2217

// ** DropDown ******************************************************************* //

#define GUI_THEME_DROPDOWN_NORMAL					2218
#define GUI_THEME_DROPDOWN_NORMAL_LEFT				2219
#define GUI_THEME_DROPDOWN_NORMAL_RIGHT				2220

#define GUI_THEME_DROPDOWN_SELECTED_LEFT			2224
#define GUI_THEME_DROPDOWN_SELECTED					2225
#define GUI_THEME_DROPDOWN_SELECTED_RIGHT			2226

#define GUI_THEME_DROPDOWN_DISABLED_LEFT			2227
#define GUI_THEME_DROPDOWN_DISABLED					2228
#define GUI_THEME_DROPDOWN_DISABLED_RIGHT			2229


// ** ButtonBar ****************************************************************** //

#define GUI_THEME_BUTTONBAR_NORMAL_LEFT					2230
#define GUI_THEME_BUTTONBAR_NORMAL						2231
#define GUI_THEME_BUTTONBAR_NORMAL_RIGHT				2232
#define GUI_THEME_BUTTONBAR_SEPERATOR_NORMAL_LEFT		2233
#define GUI_THEME_BUTTONBAR_SEPERATOR_NORMAL_RIGHT		2234

#define GUI_THEME_BUTTONBAR_SELECTED_LEFT				2235
#define GUI_THEME_BUTTONBAR_SELECTED					2236
#define GUI_THEME_BUTTONBAR_SELECTED_RIGHT				2237
#define GUI_THEME_BUTTONBAR_SEPERATOR_SELECTED_LEFT		2238
#define GUI_THEME_BUTTONBAR_SEPERATOR_SELECTED_RIGHT	2239

#define GUI_THEME_BUTTONBAR_DISABLED_LEFT				2240
#define GUI_THEME_BUTTONBAR_DISABLED					2241
#define GUI_THEME_BUTTONBAR_DISABLED_RIGHT				2242
#define GUI_THEME_BUTTONBAR_SEPERATOR_DISABLED_LEFT		2243
#define GUI_THEME_BUTTONBAR_SEPERATOR_DISABLED_RIGHT	2244

// ** RoundButton ************************************************************** //

#define GUI_THEME_ROUNDBUTTON_SMALL_DISABLED		2253
#define GUI_THEME_ROUNDBUTTON_SMALL_SELECTED		2254
#define GUI_THEME_ROUNDBUTTON_SMALL_NORMAL			2255

#define GUI_THEME_ROUNDBUTTON_BIG_DISABLED			2256
#define GUI_THEME_ROUNDBUTTON_BIG_SELECTED			2257
#define GUI_THEME_ROUNDBUTTON_BIG_NORMAL			2258

// ** Button ******************************************************************* //

#define GUI_THEME_BUTTON_NORMAL_LEFT				2261
#define GUI_THEME_BUTTON_NORMAL_CENTER				2262
#define GUI_THEME_BUTTON_NORMAL_RIGHT				2263
#define GUI_THEME_BUTTON_NORMAL_LEFT_TOP			2264
#define GUI_THEME_BUTTON_NORMAL_CENTER_TOP			2265
#define GUI_THEME_BUTTON_NORMAL_RIGHT_TOP			2266
#define GUI_THEME_BUTTON_NORMAL_LEFT_BOTTOM			2267
#define GUI_THEME_BUTTON_NORMAL_CENTER_BOTTOM		2268
#define GUI_THEME_BUTTON_NORMAL_RIGHT_BOTTOM		2269

#define GUI_THEME_BUTTON_SELECTED_LEFT				2271
#define GUI_THEME_BUTTON_SELECTED_CENTER			2272
#define GUI_THEME_BUTTON_SELECTED_RIGHT				2273
#define GUI_THEME_BUTTON_SELECTED_LEFT_TOP			2274
#define GUI_THEME_BUTTON_SELECTED_CENTER_TOP		2275
#define GUI_THEME_BUTTON_SELECTED_RIGHT_TOP			2276
#define GUI_THEME_BUTTON_SELECTED_LEFT_BOTTOM		2277
#define GUI_THEME_BUTTON_SELECTED_CENTER_BOTTOM		2278
#define GUI_THEME_BUTTON_SELECTED_RIGHT_BOTTOM		2279

#define GUI_THEME_BUTTON_DEFAULT_LEFT				2281
#define GUI_THEME_BUTTON_DEFAULT_CENTER				2282
#define GUI_THEME_BUTTON_DEFAULT_RIGHT				2283
#define GUI_THEME_BUTTON_DEFAULT_LEFT_TOP			2284
#define GUI_THEME_BUTTON_DEFAULT_CENTER_TOP			2285
#define GUI_THEME_BUTTON_DEFAULT_RIGHT_TOP			2286
#define GUI_THEME_BUTTON_DEFAULT_LEFT_BOTTOM		2287
#define GUI_THEME_BUTTON_DEFAULT_CENTER_BOTTOM		2288
#define GUI_THEME_BUTTON_DEFAULT_RIGHT_BOTTOM		2289

#define GUI_THEME_BUTTON_DISABLED_LEFT				2291
#define GUI_THEME_BUTTON_DISABLED_CENTER			2292
#define GUI_THEME_BUTTON_DISABLED_RIGHT				2293
#define GUI_THEME_BUTTON_DISABLED_LEFT_TOP			2294
#define GUI_THEME_BUTTON_DISABLED_CENTER_TOP		2295
#define GUI_THEME_BUTTON_DISABLED_RIGHT_TOP			2296
#define GUI_THEME_BUTTON_DISABLED_LEFT_BOTTOM		2297
#define GUI_THEME_BUTTON_DISABLED_CENTER_BOTTOM		2298
#define GUI_THEME_BUTTON_DISABLED_RIGHT_BOTTOM		2299

// ** Side Menu **************************************************************** //

#define GUI_THEME_SIDE_MENU_BACKGROUND				2300
#define GUI_THEME_SIDE_MENU_SEPERATOR				2301

#define GUI_THEME_SIDE_MENU_BUTTON_NORMAL_LEFT		2302
#define GUI_THEME_SIDE_MENU_BUTTON_NORMAL			2303
#define GUI_THEME_SIDE_MENU_BUTTON_NORMAL_RIGHT		2304

#define GUI_THEME_SIDE_MENU_BUTTON_SELECTED_LEFT	2305
#define GUI_THEME_SIDE_MENU_BUTTON_SELECTED			2306
#define GUI_THEME_SIDE_MENU_BUTTON_SELECTED_RIGHT	2307

// ** Scrollbar **************************************************************** //

#define GUI_THEME_SCROLLBAR_H_BAR_NORMAL_LEFT		2310
#define GUI_THEME_SCROLLBAR_H_BAR_NORMAL_CENTER		2311
#define GUI_THEME_SCROLLBAR_H_BAR_NORMAL_RIGHT		2312

#define GUI_THEME_SCROLLBAR_H_KNOB_NORMAL_LEFT		2313
#define GUI_THEME_SCROLLBAR_H_KNOB_NORMAL_CENTER	2314
#define GUI_THEME_SCROLLBAR_H_KNOB_NORMAL_RIGHT		2315

#define GUI_THEME_SCROLLBAR_V_BAR_NORMAL_TOP		2316
#define GUI_THEME_SCROLLBAR_V_BAR_NORMAL_CENTER		2317
#define GUI_THEME_SCROLLBAR_V_BAR_NORMAL_BOTTOM		2318

#define GUI_THEME_SCROLLBAR_V_KNOB_NORMAL_TOP		2319
#define GUI_THEME_SCROLLBAR_V_KNOB_NORMAL_CENTER	2320
#define GUI_THEME_SCROLLBAR_V_KNOB_NORMAL_BOTTOM	2321

#define GUI_THEME_SCROLLBAR_KNOB_SELECTED_TOP		2322
#define GUI_THEME_SCROLLBAR_KNOB_SELECTED_CENTER	2323
#define GUI_THEME_SCROLLBAR_KNOB_SELECTED_BOTTOM	2323

// ** Screensaver Controls ***************************************************** //

#define GUI_THEME_SCREENSAVER_CONTROL_ACTION		2330
#define GUI_THEME_SCREENSAVER_CONTROL_APP			2331
#define GUI_THEME_SCREENSAVER_CONTROL_CLOCK			2332

// ** ICONS ******************************************************************** //

#define GUI_THEME_SALLY_ICON_ADD					2500
#define GUI_THEME_SALLY_ICON_REMOVE					2501
#define GUI_THEME_SALLY_ICON_DELETE					2502
#define GUI_THEME_SALLY_ICON_NEXT					2503
#define GUI_THEME_SALLY_ICON_PREVIOUS				2504
#define GUI_THEME_SALLY_ICON_UP						2505
#define GUI_THEME_SALLY_ICON_DOWN					2506
#define GUI_THEME_SALLY_ICON_SEARCH					2507
#define GUI_THEME_SALLY_ICON_SAVE					2508
#define GUI_THEME_SALLY_ICON_EDIT					2509
#define GUI_THEME_SALLY_ICON_HOME					2510
#define GUI_THEME_SALLY_ICON_RELOAD					2511
#define GUI_THEME_SALLY_ICON_CANCEL					2512
#define GUI_THEME_SALLY_ICON_ACCEPT					2513
#define GUI_THEME_SALLY_ICON_FOLDER					2514
#define GUI_THEME_SALLY_ICON_HD						2515
#define GUI_THEME_SALLY_ICON_CD						2516
#define GUI_THEME_SALLY_ICON_DVD					2517
#define GUI_THEME_SALLY_ICON_NETWORK				2518
#define GUI_THEME_SALLY_ICON_USB					2519
#define GUI_THEME_SALLY_ICON_INFO					2520
#define GUI_THEME_SALLY_ICON_RATING_ON				2521
#define GUI_THEME_SALLY_ICON_RATING_OFF				2522
#define GUI_THEME_SALLY_ICON_SHUFFLE				2523
#define GUI_THEME_SALLY_ICON_TIMER					2524
#define GUI_THEME_SALLY_ICON_DATABASE				2525
#define GUI_THEME_SALLY_ICON_FONT					2526
#define GUI_THEME_SALLY_ICON_FULLSCREEN				2527
#define GUI_THEME_SALLY_ICON_SYSTEM_EXIT			2528
#define GUI_THEME_SALLY_ICON_SYSTEM_SHUTDOWN		2529
#define GUI_THEME_SALLY_ICON_PROPERTIES				2530
#define GUI_THEME_SALLY_ICON_ABOUT					2531
#define GUI_THEME_SALLY_ICON_FACEBOOK				2532
#define GUI_THEME_SALLY_ICON_DATE					2533
#define GUI_THEME_SALLY_ICON_SYSTEM_BATTERY			2534
#define GUI_THEME_SALLY_ICON_SYSTEM_LOCK			2535
#define GUI_THEME_SALLY_ICON_NOTIFY					2536
#define GUI_THEME_SALLY_ICON_SHUFFLE_OFF			2537

#define GUI_THEME_SALLY_ICON_BIG_FOLDER				2540
#define GUI_THEME_SALLY_ICON_BIG_HD					2541
#define GUI_THEME_SALLY_ICON_BIG_CD					2542
#define GUI_THEME_SALLY_ICON_BIG_DVD				2543
#define GUI_THEME_SALLY_ICON_BIG_NETWORK			2544
#define GUI_THEME_SALLY_ICON_BIG_USB				2545
#define GUI_THEME_SALLY_ICON_BIG_DESKTOP			2546
#define GUI_THEME_SALLY_ICON_BIG_HOME				2547


#define GUI_THEME_SALLY_ICON_MB_ERROR				2550
#define GUI_THEME_SALLY_ICON_MB_INFO				2551
#define GUI_THEME_SALLY_ICON_MB_WARNING				2552
#define GUI_THEME_SALLY_ICON_MB_QUESTION			2553

#define GUI_THEME_SALLY_ICON_FOLDER_UP				2560
#define GUI_THEME_SALLY_ICON_DESKTOP				2561
#define GUI_THEME_SALLY_ICON_KEYBOARD				2562
#define GUI_THEME_SALLY_ICON_MOUSE					2563

#define GUI_THEME_SALLY_ICON_MIMETYPE_MP3			2580
#define GUI_THEME_SALLY_ICON_MIMETYPE_IMAGE			2581
#define GUI_THEME_SALLY_ICON_MIMETYPE_HTML			2582
#define GUI_THEME_SALLY_ICON_MIMETYPE_VIDEO			2583
#define GUI_THEME_SALLY_ICON_MIMETYPE_TEXT			2584

#define GUI_THEME_SALLY_ICON_BIG_MIMETYPE_MP3		2590
#define GUI_THEME_SALLY_ICON_BIG_MIMETYPE_IMAGE		2591
#define GUI_THEME_SALLY_ICON_BIG_MIMETYPE_HTML		2592
#define GUI_THEME_SALLY_ICON_BIG_MIMETYPE_VIDEO		2593
#define GUI_THEME_SALLY_ICON_BIG_MIMETYPE_TEXT		2594

#define GUI_THEME_SALLY_ICON_MEDIA_PLAY				2601
#define GUI_THEME_SALLY_ICON_MEDIA_STOP				2602
#define GUI_THEME_SALLY_ICON_MEDIA_PAUSE			2603
#define GUI_THEME_SALLY_ICON_MEDIA_EJECT			2604
#define GUI_THEME_SALLY_ICON_MEDIA_SEEK_FORWARD		2605
#define GUI_THEME_SALLY_ICON_MEDIA_SEEK_BACKWARD	2606
#define GUI_THEME_SALLY_ICON_MEDIA_SKIP_FORWARD		2607
#define GUI_THEME_SALLY_ICON_MEDIA_SKIP_BACKWARD	2608

// ** OSM ********************************************************************** //

#define GUI_THEME_SALLY_OSM_PAUSE					2700
#define GUI_THEME_SALLY_OSM_PLAY					2701
#define GUI_THEME_SALLY_OSM_NEXT					2702
#define GUI_THEME_SALLY_OSM_PREVIOUS				2703
#define GUI_THEME_SALLY_OSM_SHUFFLEON				2704
#define GUI_THEME_SALLY_OSM_SHUFFLEOFF				2705
#define GUI_THEME_SALLY_OSM_STOP					2706
#define GUI_THEME_SALLY_OSM_ADD						2707
#define GUI_THEME_SALLY_OSM_SAVE					2708

// ** Audio BIG **************************************************************** //

#define GUI_THEME_SALLY_OSM_AUDIO_HIGH				2709
#define GUI_THEME_SALLY_OSM_AUDIO_LOW				2710
#define GUI_THEME_SALLY_OSM_AUDIO_MEDIUM			2711
#define GUI_THEME_SALLY_OSM_AUDIO_MUTED				2712

// ** Flags ******************************************************************** //

#define GUI_THEME_SALLY_FLAG_DE						2800
#define GUI_THEME_SALLY_FLAG_EN						2801
#define GUI_THEME_SALLY_FLAG_FR						2802
#define GUI_THEME_SALLY_FLAG_ES						2803
#define GUI_THEME_SALLY_FLAG_IT						2804
#define GUI_THEME_SALLY_FLAG_RO						2805
#define GUI_THEME_SALLY_FLAG_AT						2806
#define GUI_THEME_SALLY_FLAG_CA						2807
#define GUI_THEME_SALLY_FLAG_CH						2808

/************************************************************************/
/* Sally System Messages                                                */
/************************************************************************/

#define MS_SALLY_SHOW_KEYBOARD						3000
#define MS_SALLY_HIDE_KEYBOARD						3001
#define MS_SALLY_HIDE_KEYBOARD_CANCEL				3002
#define MS_SALLY_KEYBOARD_REQUEST_WORDS				3003
#define MS_SALLY_KEYBOARD_CLOSED					3004

#define MS_SALLY_SHOW_VOLUME						3006
#define MS_SALLY_HIDE_VOLUME						3007

#define MS_SALLY_POPUP_BLENDED						3010
#define MS_SALLY_SCHEDULER							3011
#define MS_SALLY_SHOW_POPUP_VIEW					3012
#define MS_SALLY_HIDE_POPUP_VIEW					3013
#define MS_SALLY_CHANGE_APP							3014
#define MS_SALLY_APP_FULLSCREEN_TRUE				3015
#define MS_SALLY_APP_FULLSCREEN_FALSE				3016
#define MS_SALLY_NEW_VOICE_COMMAND					3017
#define MS_SALLY_ADD_CHILD							3018
#define MS_SALLY_SCREENSAVER_START					3019
#define MS_SALLY_SCREENSAVER_STOP					3020
#define MS_SALLY_VOICE_RELEASE_FOCUS				3021
#define MS_SALLY_VOICE_SET_FOCUS					3022
#define MS_SALLY_NOTIFICATION_OSM					3023
#define MS_SALLY_SHOW_INPUTBOX						3024
#define MS_SALLY_SHOW_MESSAGEBOX					3025
#define MS_SALLY_SHOW_QUESTIONBOX					3026
#define MS_SALLY_APP_CONFIG_CHANGED					3027
#define MS_SALLY_SYSTEM_DEVICECHANGE				3028
#define MS_SALLY_SHOW_OPEN_FILE						3029
#define MS_SALLY_CANCEL_OPEN_FILE					3030
#define MS_SALLY_OK_OPEN_FILE						3031
#define MS_SALLY_SHOW_WORKING						3032
#define MS_SALLY_HIDE_WORKING						3033
#define MS_SALLY_APP_CONFIG_SAVED					3034
#define MS_SALLY_SHOW_SETTINGS						3035
#define MS_SALLY_HIDE_SETTINGS						3036
#define MS_SALLY_SALLY_CONFIG_CHANGED				3037
#define MS_SALLY_MUTE_SOUND							3038
#define MS_SALLY_UNMUTE_SOUND						3039
#define MS_SALLY_APP_SHOW_SHUTDOWN					3040
#define MS_SALLY_SHOW_FIRST_START_WIZARD			3041
#define MS_SALLY_NOTIFICATION_INFO_SHOW				3042
#define MS_SALLY_NOTIFICATION_INFO_HIDE				3043
#define MS_SALLY_APP_POPUP_INFO_NOTIFY				3044
#define MS_SALLY_APP_FACEBOOK_STATUS				3045
#define MS_SALLY_SHOW_FACEBOOK_CONFIG				3046
#define MS_SALLY_FACEBOOK_STATUS_UPDATE				3049
#define MS_SALLY_GET_APPLICATION_INFO				3050
#define MS_SALLY_FACEBOOK_NOTIFY					3051
#define MS_SALLY_APP_LOAD_COMPLETE					3052
#define MS_SALLY_SHOW_APPLICATION_SELECTOR			3053
#define MS_SALLY_SALLY_UPDATE_CHECKER				3054
#define MS_SALLY_SALLY_NEW_UPDATE_AVAIL				3055
#define MS_SALLY_SHOW_ALARM_WINDOW					3056
#define MS_SALLY_HIDE_ALARM_WINDOW					3057
#define MS_SALLY_FACEBOOK_CONFIG_CHANGED			3058
#define MS_SALLY_ADD_CONFIG_PANEL					3059
#define MS_SALLY_ADD_WIZARD_PANEL					3060
#define MS_SALLY_APP_SAVE_CONFIG					3061
#define MS_SALLY_GAME_START							3062
#define MS_SALLY_GAME_STOP							3063
#define MS_SALLY_GAME_START_THREAD					3064
#define MS_SALLY_APP_FACEBOOK_UPDATE_INFO			3065
#define MS_SALLY_VOLUME_CHANGED						3066
#define MS_SALLY_APP_MUTE_SOUND						3067
#define MS_SALLY_APP_UNMUTE_SOUND					3068
#define MS_SALLY_SALLY_THEME_CHANGED				3069
#define MS_SALLY_SYSTEM_APMSUSPEND					3070
#define MS_SALLY_SYSTEM_APMRESUMESUSPEND			3071
#define MS_SALLY_NOTIFICATION_TEXT					3072
#define MS_SALLY_SCREENSAVER_SHOW_MENU				3073
#define MS_SALLY_SCREENSAVER_HIDE_MENU				3074
#define MS_SALLY_ADD_SCREENSAVER_CONTROL			3075

#define MS_SALLY_LOCK_SCREEN						3100
#define MS_SALLY_UNLOCK_SCREEN						3101

#define MS_DIALOG_OK								3103
#define MS_DIALOG_CANCEL							3104
#define MS_DIALOG_YES								3105
#define MS_DIALOG_NO								3106

#define MS_INVALIDATE_CONTROL						3201

#define MS_SALLY_DEVICE_LOST						3301
#define MS_SALLY_DEVICE_RESTORE_START				3302
#define MS_SALLY_DEVICE_RESTORE_END					3303

#define FACEBOOK_USER_IMAGES						4000


#define COMMUNITY_FILESERVER	"fileserver.sally-project.org"
#define COMMUNITY_SERVER		"www.sally-project.org"
#define COMMUNITY_PORT			80
#define COMMUNITY_URL			"/"

#define GUI_NO_IMAGE		-1

#define SPECIAL_KEY_PLAY				179
#define SPECIAL_KEY_PREVIOUS			177
#define SPECIAL_KEY_NEXT				176
#define SPECIAL_KEY_STOP				178
#define SPECIAL_KEY_SEEK_FORWARD		70
#define SPECIAL_KEY_SEEK_BACKWARD		66
#define SPECIAL_KEY_RECORD				82
#define SPECIAL_KEY_ENTER				13
#define SPECIAL_KEY_SHUFFLE				84
#define SPECIAL_KEY_INFO				77
#define SPECIAL_KEY_ARROW_UP			38
#define SPECIAL_KEY_ARROW_DOWN			40
#define SPECIAL_KEY_ARROW_LEFT			37
#define SPECIAL_KEY_ARROW_RIGHT			39

// handled by sally
#define SPECIAL_KEY_PAGE_UP					33
#define SPECIAL_KEY_PAGE_DOWN				34
#define SPECIAL_KEY_SWITCH_SCREENSAVER_1	79
#define SPECIAL_KEY_SWITCH_SCREENSAVER_2	71
