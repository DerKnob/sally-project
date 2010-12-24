////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\sallyAPI.h
///
/// \brief	The main header for the sallyAPI. 
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

#include "Define.h"

#include "ExceptionError.h"
#include "ExceptionWarning.h"
#include "WinAPIExceptionHandler.h"

#include "Ping.h"
#include "NetworkHelper.h"

#include "DateHelper.h"

#include "FileHelper.h"

#include "StringHelper.h"
#include "PathHelper.h"

#include "Thread.h"
#include "SmartThreadPool.h"
#include "SmartThreadPoolManager.h"
#include "Logger.h"
#include "Option.h"
#include "SystemHelper.h"
#include "NumberGenerator.h"
#include "CritSection.h"
#include "AutoLock.h"

#include "Form.h"
#include "Window.h"
#include "ParameterBase.h"
#include "ParameterInteger.h"
#include "ParameterPoint.h"
#include "ParameterString.h"
#include "ParameterKeyValue.h"

#include "GUIBaseObject.h"
#include "Game.h"

#include "Texture.h"
#include "TextureManager.h"
#include "Font.h"
#include "FontManager.h"
#include "Camera.h"
#include "Counter.h"
#include "Speech.h"

#include "Config.h"
#include "LanguageManager.h"
#include "Option.h"
#include "Speech.h"
#include "Theme.h"
#include "TextureAllocator.h"

#include "ConfigPanel.h"
#include "WizardPanel.h"
#include "Form.h"
#include "AppBase.h"
#include "ApplicationWindow.h"
#include "GameWindow.h"
#include "Button.h"
#include "RoundButton.h"
#include "Edit.h"
#include "EditBox.h"
#include "Processbar.h"
#include "PopUpWindow.h"
#include "ListViewItem.h"
#include "ListView.h"
#include "ListViewExt.h"
#include "Label.h"
#include "LabelBox.h"
#include "Checkbox.h"
#include "Slider.h"
#include "3DCircle.h"
#include "ImageBox.h"
#include "RadioButton.h"
#include "Timer.h"
#include "ThreadStarter.h"
#include "ButtonBar.h"
#include "ButtonBarButton.h"
#include "GroupBox.h"
#include "FileBrowser.h"
#include "Breadcrumb.h"
#include "DropDownItem.h"
#include "DropDown.h"
#include "Rating.h"
#include "SideMenu.h"
#include "TabcontrolButton.h"
#include "TabcontrolItem.h"
#include "Tabcontrol.h"
#include "BottonMenu.h"
#include "TopMenu.h"
#include "Working.h"
#include "NumberSelector.h"
#include "Scrollbar.h"
#include "HorizontalSeperator.h"
#include "VerticalSeperator.h"

#include "DatabaseConnection.h"
#include "DatabaseMetaData.h"
#include "DatabaseObject.h"
#include "ResultSet.h"
#include "ResultSetMetaData.h"
#include "SQLException.h"
#include "Statement.h"

#include "InputBox.h"
#include "MessageBox.h"
#include "QuestionBox.h"
#include "OpenDialog.h"
#include "AboutDialog.h"

#include "ParameterInputBox.h"
#include "ParameterMessageBox.h"
#include "ParameterQuestionBox.h"
#include "ParameterOpenDialog.h"
#include "ParameterOnScreenMenu.h"
#include "ParameterInfoPopup.h"
#include "ParameterApplicationInfo.h"
#include "ParameterScrollList.h"
#include "ParameterControl.h"
#include "ParameterListItem.h"

#include "KeyboardLayout.h"
#include "SmartKeyboard.h"

#include "GUIHelper.h"
#include "OptionManager.h"
#include "SpeechManager.h"
#include "SpeechThread.h"
#include "VolumeManager.h"
#include "VolumeControl.h"

#include "Scheduler.h"
#include "SchedulerManager.h"

#include "FacebookManager.h"
#include "FacebookDB.h"
#include "StatusMessage.h"
#include "SallyHelper.h"
#include "PlaySound.h"
#include "ScrollListItem.h"
#include "ScrollList.h"

#include "AmazonInfo.h"