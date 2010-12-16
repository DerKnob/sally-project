////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	_shared\FunctionHelper.h
///
/// \brief	Declares the helper functions. 
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

std::string GetIniFile();

std::string iniFile = GetIniFile();

bool MoveDirectory(LPCTSTR lpszDirFrom, LPCTSTR lpszDirTo)
{
	int len = _tcslen(lpszDirFrom);
	TCHAR *pszFrom = new TCHAR[len+2];
	_tcscpy(pszFrom, lpszDirFrom);
	pszFrom[len] = 0;
	pszFrom[len+1] = 0;

	len = _tcslen(lpszDirTo);
	TCHAR *pszTo = new TCHAR[len+2];
	_tcscpy(pszTo, lpszDirTo);
	pszTo[len] = 0;
	pszTo[len+1] = 0;

	SHFILEOPSTRUCT fileop;
	fileop.hwnd   = NULL;    // no status display
	fileop.wFunc  = FO_MOVE;  // delete operation
	fileop.pFrom  = pszFrom;  // source file name as double null terminated string
	fileop.pTo    = pszTo;    // no destination needed
	fileop.fFlags = FOF_NOCONFIRMATION|FOF_SILENT|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI;  // do not prompt the user

	fileop.fAnyOperationsAborted = FALSE;
	fileop.lpszProgressTitle     = NULL;
	fileop.hNameMappings         = NULL;

	int ret = SHFileOperation(&fileop);
	delete [] pszFrom;
	delete [] pszTo;
	return (ret == 0);
}

bool DeleteDirectory(LPCTSTR lpszDir, bool noRecycleBin = true)
{
	int len = _tcslen(lpszDir);
	TCHAR *pszFrom = new TCHAR[len+2];
	_tcscpy(pszFrom, lpszDir);
	pszFrom[len] = 0;
	pszFrom[len+1] = 0;

	SHFILEOPSTRUCT fileop;
	fileop.hwnd   = NULL;    // no status display
	fileop.wFunc  = FO_DELETE;  // delete operation
	fileop.pFrom  = pszFrom;  // source file name as double null terminated string
	fileop.pTo    = NULL;    // no destination needed
	fileop.fFlags = FOF_NOCONFIRMATION|FOF_SILENT|FOF_NOERRORUI;  // do not prompt the user

	if(!noRecycleBin)
		fileop.fFlags |= FOF_ALLOWUNDO;

	fileop.fAnyOperationsAborted = FALSE;
	fileop.lpszProgressTitle     = NULL;
	fileop.hNameMappings         = NULL;

	int ret = SHFileOperation(&fileop);
	delete [] pszFrom;  
	return (ret == 0);
}

bool ExecProcessAndWait(std::string program, std::string parameter)
{
	SHELLEXECUTEINFO ExecInfo;
	memset(&ExecInfo, 0, sizeof(ExecInfo)); 
	ExecInfo.cbSize = sizeof(SHELLEXECUTEINFO );
	ExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ExecInfo.hwnd = GetActiveWindow();
	ExecInfo.lpVerb = "open";
	ExecInfo.lpFile = program.c_str();
	ExecInfo.lpParameters = parameter.c_str();
	ExecInfo.nShow = SW_SHOW;

	if(!ShellExecuteEx(&ExecInfo))
	{
		return false;
	}

	WaitForInputIdle(ExecInfo.hProcess, INFINITE); 
	WaitForSingleObject(ExecInfo.hProcess, INFINITE); 

	DWORD exitCode = 0;
	GetExitCodeProcess(ExecInfo.hProcess, &exitCode);

	CloseHandle(ExecInfo.hProcess);

	if (exitCode == 0)
		return true;
	return false;
}

bool UninstallExtensionAsAdmin(const std::string& folder)
{
	std::string exe = SallyAPI::System::SystemHelper::GetModulePath();
	exe.append("SallyAdminProcess.exe");

	std::string processCmd = "delete::";
	processCmd.append(folder);

	return ExecProcessAndWait(exe, processCmd);
}

bool InstallExtensionAsAdmin(const std::string& folder)
{
	std::string exe = SallyAPI::System::SystemHelper::GetModulePath();
	exe.append("SallyAdminProcess.exe");

	std::string cmdline = "";

	if (SallyAPI::System::COption::GetPropertyBoolStatic("sally", "beta", false, iniFile))
	{
		cmdline.append("installbeta::");
	}
	else
	{
		cmdline.append("install::");
	}

	cmdline.append(folder);

	return ExecProcessAndWait(exe, cmdline);
}

std::string GetUpdateFolder()
{
	char szPath[MAX_PATH];
	LPITEMIDLIST lpStartFolder = NULL;
	SHGetSpecialFolderLocation(0, CSIDL_COMMON_DOCUMENTS, &lpStartFolder);
	SHGetPathFromIDList(lpStartFolder, szPath);

	std::string temp = szPath;
	temp.append("\\Sally Project\\");
	CreateDirectory(temp.c_str(), NULL);
	temp.append("updates\\");
	CreateDirectory(temp.c_str(), NULL);

	CoTaskMemFree(lpStartFolder);
	lpStartFolder = NULL;

	return temp;
}

std::string GetUpdateFolderTemp()
{
	char szPath[MAX_PATH];
	LPITEMIDLIST lpStartFolder = NULL;
	SHGetSpecialFolderLocation(0, CSIDL_COMMON_DOCUMENTS, &lpStartFolder);
	SHGetPathFromIDList(lpStartFolder, szPath);

	std::string temp = szPath;
	temp.append("\\Sally Project\\");
	CreateDirectory(temp.c_str(), NULL);
	temp.append("updatesTemp\\");
	CreateDirectory(temp.c_str(), NULL);

	CoTaskMemFree(lpStartFolder);
	lpStartFolder = NULL;

	return temp;
}

std::string GetIniFile()
{
	char szPath[MAX_PATH];
	LPITEMIDLIST lpStartFolder = NULL;
	SHGetSpecialFolderLocation(0, CSIDL_MYDOCUMENTS, &lpStartFolder);
	SHGetPathFromIDList(lpStartFolder, szPath);

	std::string temp = szPath;
	temp.append("\\Sally Project\\");
	CreateDirectory(temp.c_str(), NULL);
	temp.append("option.ini");

	CoTaskMemFree(lpStartFolder);
	lpStartFolder = NULL;

	return temp;
}

std::string GetPluginFolder(const std::string& plugin)
{
	char szPath[MAX_PATH];
	LPITEMIDLIST lpStartFolder = NULL;
	SHGetSpecialFolderLocation(0, CSIDL_MYDOCUMENTS, &lpStartFolder);
	SHGetPathFromIDList(lpStartFolder, szPath);

	std::string temp = szPath;
	temp.append("\\Sally Project\\");
	CreateDirectory(temp.c_str(), NULL);
	temp.append("applications\\");
	CreateDirectory(temp.c_str(), NULL);
	temp.append(plugin);
	temp.append("\\");
	CreateDirectory(temp.c_str(), NULL);

	CoTaskMemFree(lpStartFolder);
	lpStartFolder = NULL;

	return temp;
}

void DownloadOnline()
{
	ShellExecute(NULL, "open", "http://www.sally-project.org/index.php?menu=pluginOverview", NULL, NULL, SW_SHOWNORMAL);
}