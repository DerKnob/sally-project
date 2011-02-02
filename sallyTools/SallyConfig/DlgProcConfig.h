////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	SallyConfig\DlgProcConfig.h
///
/// \brief	Declares the dialog proc configuration. 
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

BOOL CALLBACK DlgProcConfig(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT			WindowRectSelf;
	static HWND			hOk;

	static PAINTSTRUCT			ps;
	static HDC					hdc;
	static HICON				hWindow;
	static HICON				hFullscreen;
	static HICON				hMulti;
	static HICON				hSingel;
	
	switch (message)
	{
	case WM_INITDIALOG:
		hWindow = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_WINDOW), IMAGE_ICON, 32, 32, NULL);
		hFullscreen = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_FULLSCREEN), IMAGE_ICON, 32, 32, NULL);
		hMulti = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_MULTI), IMAGE_ICON, 64, 64, NULL);
		hSingel = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_SINGEL), IMAGE_ICON, 32, 32, NULL);

		hOk = GetDlgItem(hDlg, IDOK);
		
		Load(hDlg);
		break;
	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDC_RADIO_SINGEL_MONITOR:
			CheckDlgButton(hDlg, IDC_RADIO_SINGEL_MONITOR, BST_CHECKED);
			CheckDlgButton(hDlg, IDC_RADIO_MULTI_MONITOR, BST_UNCHECKED);
			EnableMutliMontorControls(hDlg);
			break;
		case IDC_RADIO_MULTI_MONITOR:
			CheckDlgButton(hDlg, IDC_RADIO_MULTI_MONITOR, BST_CHECKED);
			CheckDlgButton(hDlg, IDC_RADIO_SINGEL_MONITOR, BST_UNCHECKED);
			EnableMutliMontorControls(hDlg);
			break;
		case IDC_RADIO_FULLSCREEN:
			CheckDlgButton(hDlg, IDC_RADIO_FULLSCREEN, BST_CHECKED);
			CheckDlgButton(hDlg, IDC_RADIO_WINDOW, BST_UNCHECKED);
			EnableMutliMontorControls(hDlg);
			break;
		case IDC_RADIO_WINDOW:
			CheckDlgButton(hDlg, IDC_RADIO_WINDOW, BST_CHECKED);
			CheckDlgButton(hDlg, IDC_RADIO_FULLSCREEN, BST_UNCHECKED);
			EnableMutliMontorControls(hDlg);
			break;
		case IDC_COMBO_DEVICE:
			switch (HIWORD (wParam))
			{
			case CBN_SELCHANGE:
				FillResulutionCombo(GetDlgItem(hDlg, IDC_COMBO_RESOLUTION), GetDlgItem(hDlg, IDC_COMBO_DEVICE), option);
				break;
			}
			break;
		case IDOK:
			Save(hDlg);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint (hDlg, &ps);

		DrawIconEx(hdc, 10, 10, hSingel, 32, 32, 0, NULL, DI_NORMAL | DI_COMPAT);
		DrawIconEx(hdc, 150, -6, hMulti, 64, 64, 0, NULL, DI_NORMAL | DI_COMPAT);

		DrawIconEx(hdc, 10, 85, hFullscreen, 32, 32, 0, NULL, DI_NORMAL | DI_COMPAT);
		DrawIconEx(hdc, 150, 85, hWindow, 32, 32, 0, NULL, DI_NORMAL | DI_COMPAT);

		EndPaint (hDlg, &ps);
		break;
	}
	return FALSE;
}