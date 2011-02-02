////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	SallyConfig\ConfigHelper.h
///
/// \brief	Declares the configuration helper functions. 
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

void EnableMutliMontorControls(HWND hDlg)
{
	if (IsDlgButtonChecked(hDlg, IDC_RADIO_MULTI_MONITOR))
	{
		ShowWindow(GetDlgItem(hDlg, IDC_COMBO_DEVICE), SW_HIDE);

		ShowWindow(GetDlgItem(hDlg, IDC_COMBO_MONITORS), SW_SHOW);
	}
	else
	{
		ShowWindow(GetDlgItem(hDlg, IDC_COMBO_DEVICE), SW_SHOW);

		ShowWindow(GetDlgItem(hDlg, IDC_COMBO_MONITORS), SW_HIDE);
	}

	if ((IsDlgButtonChecked(hDlg, IDC_RADIO_FULLSCREEN)) &&
		(IsDlgButtonChecked(hDlg, IDC_RADIO_MULTI_MONITOR)))
	{
		ShowWindow(GetDlgItem(hDlg, IDC_STATIC_RESOLUTION), SW_HIDE);

		ShowWindow(GetDlgItem(hDlg, IDC_COMBO_RESOLUTION), SW_HIDE);
	}
	else
	{
		ShowWindow(GetDlgItem(hDlg, IDC_STATIC_RESOLUTION), SW_SHOW);

		ShowWindow(GetDlgItem(hDlg, IDC_COMBO_RESOLUTION), SW_SHOW);
	}
}

void FillMonitors(HWND hMonitors, SallyAPI::System::COption* option)
{
	DWORD			iDevNum = 0;
	DISPLAY_DEVICE	lpDisplayDevice;
	DWORD			dwFlags= 0;
	int				select;
	int				lastAdd = 0;

	lpDisplayDevice.cb = sizeof(lpDisplayDevice);

	while(EnumDisplayDevices(NULL, iDevNum, &lpDisplayDevice, dwFlags))
	{
		if(!(lpDisplayDevice.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) &&
				lpDisplayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)
		{
			std::string temp;

			temp.append(lpDisplayDevice.DeviceName);

			lastAdd = SendMessage(hMonitors, CB_ADDSTRING, 0, (LPARAM) temp.c_str());
		}
		iDevNum++;
	}
	if (option->GetPropertyString("GraphicDevice", "MonitorId").length() == 0)
	{
		// first start than select the highest?
		SendMessage(hMonitors, CB_SETCURSEL, lastAdd, 0);
	}
	else
	{
		// load old values?
		std::string monitorId = option->GetPropertyString("GraphicDevice", "MonitorId");

		select = (int) SendMessage(hMonitors, CB_FINDSTRING, 0, (LPARAM) monitorId.c_str());
		if (select != CB_ERR)
		{
			SendMessage(hMonitors, CB_SETCURSEL, select, 0);
		}
	}	
}

void FillResulutionCombo(HWND hGraphicResolution, HWND hGraphicDevice, SallyAPI::System::COption* option)
{
	UINT			AllDisplayModes;
	D3DDISPLAYMODE	temp_mode;
	int				intAdapterNumber;
	char			tmpString[20];
	int				select;
	int				lastAdd = 0;

	// empty dropdown
	SendMessage(hGraphicResolution, CB_RESETCONTENT, 0, 0);

	intAdapterNumber = (int) SendMessage(hGraphicDevice, CB_GETCURSEL, 0, 0);

	AllDisplayModes = m_lpD3D->GetAdapterModeCount(intAdapterNumber, (D3DFORMAT)D3DFMT_R5G6B5);
	// go threw all resolutions and insert them in the combobox
	// Find all resolutions guilty for the given Graphicadapter and BackBuffer Formats
	for(UINT cur_mode = 0; cur_mode < AllDisplayModes; cur_mode++)
	{
		if(SUCCEEDED(m_lpD3D->EnumAdapterModes(intAdapterNumber, (D3DFORMAT)D3DFMT_R5G6B5, cur_mode, &temp_mode))){
			sprintf_s(tmpString,"%i X %i",temp_mode.Width,temp_mode.Height);

			if ((temp_mode.Width >= 1024) && (temp_mode.Width <= 1920) &&
				(temp_mode.Height >= 600) && (temp_mode.Height <= 1200)) 
			{
				if (SendMessage(hGraphicResolution, CB_FINDSTRING, 0, (LPARAM) tmpString) == CB_ERR)
				{
					lastAdd = SendMessage(hGraphicResolution, CB_ADDSTRING, 0, (LPARAM) tmpString);
				}
			}
		}
	}
	if (option->GetPropertyInt("GraphicDevice", "Width", 0) == 0)
	{
		// first start than select the highest?
		SendMessage(hGraphicResolution, CB_SETCURSEL, lastAdd, 0);
	}
	else
	{
		// load old values?
		sprintf_s(tmpString, "%i X %i", option->GetPropertyInt("GraphicDevice", "Width", 1024),
			option->GetPropertyInt("GraphicDevice", "Height", 768));

		select = (int) SendMessage(hGraphicResolution, CB_FINDSTRING, 0, (LPARAM) tmpString);
		if (select != CB_ERR)
		{
			SendMessage(hGraphicResolution, CB_SETCURSEL, select, 0);
		}
	}	
}

bool FillGraphicDeviceCombo(HWND hGraphicDevice, SallyAPI::System::COption* option)
{
	UINT tmp_adapter;
	for(tmp_adapter = 0; tmp_adapter < m_lpD3D->GetAdapterCount(); tmp_adapter++ ){

		if(FAILED(m_lpD3D->GetAdapterIdentifier(tmp_adapter,0,&(Adapterinfo[tmp_adapter].adapter)))){
			return false;
		}
		std::string temp = Adapterinfo[tmp_adapter].adapter.Description;
		temp.append(" (");
		temp.append(Adapterinfo[tmp_adapter].adapter.DeviceName);
		temp.append(")");

		deviceList[Adapterinfo[tmp_adapter].adapter.DeviceName] = tmp_adapter;

		SendMessage(hGraphicDevice, CB_ADDSTRING, 0, (LPARAM) temp.c_str());
	}
	SendMessage(hGraphicDevice, CB_SETCURSEL, option->GetPropertyInt("GraphicDevice", "DeviceId", 0), 0);
	return true;
}

void Save(HWND hDlg)
{

	if (IsDlgButtonChecked(hDlg, IDC_RADIO_FULLSCREEN))
	{
		option->SetPropertyInt("GraphicDevice", "Fullscreen", 1);
	}
	else
	{
		option->SetPropertyInt("GraphicDevice", "Fullscreen", 0);
	}

	if (IsDlgButtonChecked(hDlg, IDC_CHECK_SHOW_MOUSE))
	{
		option->SetPropertyInt("sally", "showcursor", 1);
	}
	else
	{
		option->SetPropertyInt("sally", "showcursor", 0);
	}

	if (IsDlgButtonChecked(hDlg, IDC_RADIO_MULTI_MONITOR))
	{
		option->SetPropertyInt("GraphicDevice", "MultiMonitor", 1);
	}
	else
	{
		option->SetPropertyInt("GraphicDevice", "MultiMonitor", 0);
	}

	int deviceId = (int) SendMessage(GetDlgItem(hDlg, IDC_COMBO_DEVICE), CB_GETCURSEL, 0, 0);
	option->SetPropertyInt("GraphicDevice", "DeviceId", deviceId);

	// Get Resolution
	int resolutionId = (int) SendMessage(GetDlgItem(hDlg, IDC_COMBO_RESOLUTION), CB_GETCURSEL, 0, 0);
	if (resolutionId != CB_ERR)
	{
		char temp[20];

		SendMessage(GetDlgItem(hDlg, IDC_COMBO_RESOLUTION), CB_GETLBTEXT, resolutionId, (LPARAM) temp);
		char width[11];
		char height[11];
		int iwidth, iheight;

		int i;
		int k;
		for (i = 0; temp[i] != ' '; i++)
		{
			width[i] = temp[i];
		}
		width[i] = '\0';
		i += 3;
		for (k = 0; temp[i + k] != '\0'; k++)
		{
			height[k] = temp[i + k];
		}
		height[k] = '\0';

		iwidth = atoi(width);
		iheight = atoi(height);

		option->SetPropertyInt("GraphicDevice", "Width", iwidth);
		option->SetPropertyInt("GraphicDevice", "Height", iheight);
	}

	int monitorId = (int) SendMessage(GetDlgItem(hDlg, IDC_COMBO_MONITORS), CB_GETCURSEL, 0, 0);
	if (monitorId != CB_ERR)
	{
		char temp[250];

		SendMessage(GetDlgItem(hDlg, IDC_COMBO_MONITORS), CB_GETLBTEXT, monitorId, (LPARAM) temp);

		option->SetPropertyString("GraphicDevice", "MonitorId", temp);

		// do this only if we have enabled multimonitor
		if (IsDlgButtonChecked(hDlg, IDC_RADIO_MULTI_MONITOR))
		{
			int deviceId = deviceList[temp];
			option->SetPropertyInt("GraphicDevice", "DeviceId", deviceId);
		}
	}
}

void Load(HWND hDlg)
{
	// Check if the option.ini exists
	bool optionExists = SallyAPI::File::FileHelper::FileExists(iniFile);

	if (!optionExists)
	{
		std::string modulePath = SallyAPI::System::SystemHelper::GetModulePath();
		modulePath.append("option.ini");

		CopyFile(modulePath.c_str(), iniFile.c_str(), false);
	}

	// open the ini file
	option = new SallyAPI::System::COption(iniFile);

	if( NULL == (m_lpD3D = Direct3DCreate9( D3D_SDK_VERSION ))) {
		MessageBox(NULL,"Error initialization DirectX",0,0);
	}
	if (false == FillGraphicDeviceCombo(GetDlgItem(hDlg, IDC_COMBO_DEVICE), option)) {
		MessageBox(NULL,"Error receiving adapter identifier",0,0);
	}

	FillResulutionCombo(GetDlgItem(hDlg, IDC_COMBO_RESOLUTION), GetDlgItem(hDlg, IDC_COMBO_DEVICE), option);
	FillMonitors(GetDlgItem(hDlg, IDC_COMBO_MONITORS), option);

	// multimonitor?
	int multiMonitor = option->GetPropertyInt("GraphicDevice", "MultiMonitor", 0);
	if (multiMonitor == 1)
	{
		CheckDlgButton(hDlg, IDC_RADIO_MULTI_MONITOR, BST_CHECKED);
	}
	else
	{
		CheckDlgButton(hDlg, IDC_RADIO_SINGEL_MONITOR, BST_CHECKED);
	}

	// fullscreen?
	int fullscreen = option->GetPropertyInt("GraphicDevice", "Fullscreen", 1);
	if (fullscreen == 1)
	{
		CheckDlgButton(hDlg, IDC_RADIO_FULLSCREEN, BST_CHECKED);
	}
	else
	{
		CheckDlgButton(hDlg, IDC_RADIO_WINDOW, BST_CHECKED);
	}

	// check monitor count
	int monitorCount = SendMessage(GetDlgItem(hDlg, IDC_COMBO_MONITORS), CB_GETCOUNT, 0, 0);
	if (monitorCount <= 1)
	{
		ShowWindow(GetDlgItem(hDlg, IDC_RADIO_MULTI_MONITOR), SW_HIDE);
		CheckDlgButton(hDlg, IDC_RADIO_SINGEL_MONITOR, BST_CHECKED);
	}	

	EnableMutliMontorControls(hDlg);

	// mouse cursour
	int showMouse = option->GetPropertyInt("sally", "showcursor", 1);
	if (showMouse == 1)
	{
		CheckDlgButton(hDlg, IDC_CHECK_SHOW_MOUSE, BST_CHECKED);
	}
}