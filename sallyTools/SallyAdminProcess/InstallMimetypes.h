////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	SallyAdminProcess\InstallMimetypes.h
///
/// \brief	Declares the install mimetypes functions. 
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

bool SaveKey(char _c_Unterordner[MAX_PATH], char _c_write[MAX_PATH], HKEY globalkey)
{
	if(ERROR_SUCCESS != RegCreateKey(HKEY_CLASSES_ROOT,_c_Unterordner,&globalkey))
		return FALSE;
	if(ERROR_SUCCESS != RegSetValueEx(globalkey,"",NULL,REG_SZ,(byte*)_c_write,strlen(_c_write)))
	{
		RegCloseKey(globalkey);
		return FALSE;
	}
	else
	{
		RegCloseKey(globalkey);
		return TRUE;
	}
}

void AddFileExtensionToRegistry( std::string fileExtension, std::string fileName, std::string moduelPath , std::string iconId )
{
	HKEY globalkey;

	char _c_Temp[255];
	char Path[MAX_PATH], Icon[MAX_PATH], Parameter[MAX_PATH];

	RegDeleteKey(HKEY_CLASSES_ROOT, fileExtension.c_str()); //Key Löschen ".endung"
	RegCreateKeyEx(HKEY_CLASSES_ROOT, fileExtension.c_str(),0,0,0,KEY_ALL_ACCESS,0,&globalkey,0); //Key neu anlegen ".endung" und öffnen

	if(ERROR_SUCCESS != RegSetValueEx(globalkey,"",0,REG_SZ,(BYTE *)fileName.c_str() ,strlen(fileName.c_str()))) //Standart Setzen auf Name
	{
	}
	RegCloseKey(globalkey);

	RegDeleteKey(HKEY_CLASSES_ROOT,fileName.c_str()); //Name löschen

	strcpy_s(_c_Temp, fileName.c_str());
	strcat_s(_c_Temp, "\\shell\\open\\command");
	RegCreateKeyEx(HKEY_CLASSES_ROOT,_c_Temp,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,&globalkey,0);
	//Name neu erstellen (Unterverzeichniss)

	strcpy_s(_c_Temp, fileName.c_str());
	strcat_s(_c_Temp, "\\DefaultIcon");
	RegCreateKeyEx(HKEY_CLASSES_ROOT,_c_Temp,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,&globalkey,0);
	//Name neu erstellen (Unterverzeichniss)

	RegCloseKey(globalkey); //Schliessen

	//**************************************************************************

	strcpy_s(Path,fileName.c_str());
	strcat_s(Path,"\\shell\\Open\\command");

	strcpy_s(Parameter, "\"");
	strcat_s(Parameter, moduelPath.c_str());
	strcat_s(Parameter,"\" %1");

	if (SaveKey(Path, Parameter, globalkey) == FALSE)
	{
	}

	strcpy_s(Path,fileName.c_str());
	strcat_s(Path,"\\DefaultIcon");

	strcpy_s(Icon, moduelPath.c_str());
	strcat_s(Icon, ",");
	strcat_s(Icon, iconId.c_str());

	if (SaveKey(Path, Icon, globalkey) == FALSE)
	{
	}
}

void InstallMimetypes()
{
	std::string moduelPath = SallyAPI::System::SystemHelper::GetModulePath();
	moduelPath.append("SallyConfig.exe");

	AddFileExtensionToRegistry(".sallyplugin", "Sally-Plugin", moduelPath, "3");

	AddFileExtensionToRegistry(".sallyapplication", "Sally-Application", moduelPath, "3");

	AddFileExtensionToRegistry(".sallytheme", "Sally-Theme", moduelPath, "5");

	AddFileExtensionToRegistry(".sallylanguage", "Sally-Language", moduelPath, "4");
	
	AddFileExtensionToRegistry(".sallykeyboard", "Sally-Keyboard", moduelPath, "6");
}