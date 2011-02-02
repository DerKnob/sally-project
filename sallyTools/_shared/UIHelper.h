////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	_shared\UIHelper.h
///
/// \brief	Declares the user interface helper functions. 
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

#include <windowsx.h>
#include <dwmapi.h>
#include <Uxtheme.h>

#pragma comment(lib, "delayimp")

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "UxTheme.lib")

HMODULE libraryDWmapi = ::LoadLibrary("dwmapi.dll");
HMODULE libraryUxTheme = ::LoadLibrary("UxTheme.dll");

//HMODULE libraryDWmapi = NULL;
//HMODULE libraryUxTheme = NULL;

BOOL glassOn = false;
HBRUSH hBrush = NULL;

std::string GetLocalisation(int id, HINSTANCE hInstance)
{
	char szStringBuffer[512];
	LoadString (hInstance, id, szStringBuffer, sizeof(szStringBuffer));

	std::string buffer;
	buffer.append(szStringBuffer);

	return buffer;
}

void ActivateGlass(HWND hDlg)
{
	if (hBrush != NULL)
	{
		DeleteObject(hBrush);
		hBrush = NULL;
	}

	if (0 != libraryDWmapi)
	{
		::DwmIsCompositionEnabled(&glassOn);

		if (glassOn)
		{
			hBrush = CreateSolidBrush(RGB (0,0,0));

			// Extend the frame into the client area.
			MARGINS margins;

			margins.cxLeftWidth = 0;
			margins.cxRightWidth = 0;
			margins.cyBottomHeight = 0;
			margins.cyTopHeight = TOP_BORDER_HEIGHT;

			::DwmExtendFrameIntoClientArea(hDlg, &margins);
			return;
		}
	}
	hBrush = (HBRUSH) (COLOR_WINDOWFRAME);
}

void DrawTextXP(HDC hdc, RECT &rcClient, RECT &rect, int LANG_ID) 
{
	HFONT hFont, oldFont;
	hFont = CreateFont(14,5,0,70,0,FALSE,FALSE,FALSE,
		DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
		DEFAULT_PITCH,"Arial");

	RECT rcPaint;
	rcPaint.right = rcClient.right - 60 - 20;
	rcPaint.bottom = TOP_BORDER_HEIGHT - 20;
	rcPaint.top = 10;
	rcPaint.left = 48 + 10;

	oldFont = (HFONT)SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(0,0,0));
	SetBkMode(hdc, TRANSPARENT);

	std::string temp = GetLocalisation(LANG_ID, hInstance);
	DrawText (hdc, temp.c_str(), strlen(temp.c_str()), &rcPaint, DT_LEFT | DT_WORD_ELLIPSIS | DT_WORDBREAK);

	DeleteObject (SelectObject (hdc, oldFont)) ;

}

void DrawTextVista(HDC hdc, RECT &rcClient, RECT &rect, int LANG_ID) 
{
	HTHEME hTheme = OpenThemeData(NULL, L"CompositedWindow::Window");
	if (hTheme)
	{
		HDC hdcPaint = CreateCompatibleDC(hdc);
		if (hdcPaint)
		{
			// Define the BITMAPINFO structure used to draw text.
			// Note that biHeight is negative. This is done because
			// DrawThemeTextEx() needs the bitmap to be in top-to-bottom
			// order.
			BITMAPINFO dib = { 0 };
			dib.bmiHeader.biSize            = sizeof(BITMAPINFOHEADER);
			dib.bmiHeader.biWidth           = rcClient.right - 60;
			dib.bmiHeader.biHeight          = -TOP_BORDER_HEIGHT;
			dib.bmiHeader.biPlanes          = 1;
			dib.bmiHeader.biBitCount        = 32;
			dib.bmiHeader.biCompression     = BI_RGB;

			HBITMAP hbm = CreateDIBSection(hdc, &dib, DIB_RGB_COLORS, NULL, NULL, 0);

			if (hbm)
			{
				HBITMAP hbmOld = (HBITMAP)SelectObject(hdcPaint, hbm);

				rect.top		= 0;
				rect.bottom		= TOP_BORDER_HEIGHT;
				rect.left		= 0;
				rect.right		= rcClient.right - 60;
				FillRect (hdcPaint, &rect, hBrush) ;

				// Setup the theme drawing options.
				DTTOPTS DttOpts = {sizeof(DTTOPTS)};
				DttOpts.dwFlags = DTT_COMPOSITED | DTT_GLOWSIZE;
				DttOpts.iGlowSize = 15;

				// Select a font.
				LOGFONTW lgFont;
				HFONT hFontOld = NULL;
				if (SUCCEEDED(GetThemeSysFont(hTheme, 801, &lgFont)))
				{
					HFONT hFont = CreateFontIndirectW(&lgFont);
					hFontOld = (HFONT) SelectObject(hdcPaint, hFont);
				}

				// Draw the title.
				RECT rcPaint;
				rcPaint.right = rcClient.right - 60 - 20;
				rcPaint.bottom = TOP_BORDER_HEIGHT - 20;
				rcPaint.top = 10;
				rcPaint.left = 10;

				std::string temp = GetLocalisation(LANG_ID, hInstance);
				int			filenameSize;
				LPWSTR		filenameWSTR;

				// File
				filenameSize = MultiByteToWideChar(CP_THREAD_ACP, MB_PRECOMPOSED, temp.c_str(), -1, 0, 0);
				filenameWSTR = (LPWSTR) malloc(filenameSize * sizeof(LPWSTR));
				MultiByteToWideChar(CP_THREAD_ACP, MB_PRECOMPOSED, temp.c_str(), -1, filenameWSTR, filenameSize);

				DrawThemeTextEx(hTheme, 
					hdcPaint, 
					0, 0, 
					filenameWSTR, 
					-1,
					DT_LEFT | DT_WORD_ELLIPSIS | DT_WORDBREAK, 
					&rcPaint, 
					&DttOpts);

				free (filenameWSTR);

				// Blit text to the frame.
				BitBlt(hdc, 48, 0, rcPaint.right, TOP_BORDER_HEIGHT, hdcPaint, 0, 0, SRCCOPY);

				SelectObject(hdcPaint, hbmOld);
				if (hFontOld)
				{
					SelectObject(hdcPaint, hFontOld);
				}
				DeleteObject(hbm);
			}
			DeleteDC(hdcPaint);
		}
		CloseThemeData(hTheme);
	}
	else
	{
		DrawTextXP(hdc, rcClient, rect, LANG_ID);
	}
}

HDC OnPaint(HWND hDlg, RECT &windowRect, HDC hdc, PAINTSTRUCT ps, int LANG_ID) 
{
	RECT	rect;

	GetClientRect(hDlg, &windowRect);

	hdc = BeginPaint (hDlg, &ps);

	rect.top		= 0;
	rect.bottom		= TOP_BORDER_HEIGHT;
	rect.left		= 0;
	rect.right		= windowRect.right;
	FillRect (hdc, &rect, hBrush) ;

	DrawIcon(hdc, 10, 10, hIcon);

	if (0 != libraryUxTheme)
	{
		DrawTextVista(hdc, windowRect, rect, LANG_ID);
	}
	else
	{
		DrawTextXP(hdc, windowRect, rect, LANG_ID);
	}

	EndPaint (hDlg, &ps);
	return hdc;
}