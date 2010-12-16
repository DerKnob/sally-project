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
	
	switch (message)
	{
	case WM_INITDIALOG:
		hOk = GetDlgItem(hDlg, IDOK);
		
		Load(hDlg);
		break;
	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDC_CHECK_MULTIMONITOR:
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
     }
     return FALSE;
}