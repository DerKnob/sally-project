////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\FileHelper.h
///
/// \brief	Declares the file helper class. 
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
#include "Define.h"
#include "DateHelper.h"
#include "StringHelper.h"
#include <string>
#include <vector>
#include <map>
#include <sys/stat.h>
#include <sys/types.h>

namespace SallyAPI
{
	namespace File
	{
		enum DRIVE_TYPE {DRIVE_TYPE_UNKNOWN, DRIVE_TYPE_NO_ROOT_DIR, DRIVE_TYPE_REMOVABLE, DRIVE_TYPE_FIXED, DRIVE_TYPE_REMOTE, DRIVE_TYPE_CDROM, DRIVE_TYPE_RAMDISK, DRIVE_TYPE_DVDROM };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	FileHelper
		///
		/// \brief	File helper. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY FileHelper
		{
		private:
			FileHelper();
			~FileHelper();
		public:
			static bool FileExists(std::string fileName);
			static bool FileExistsAndNotEmpty(const std::string& fileName);
			static std::vector<std::string> ReadFileToVector(const std::string& file, std::vector<std::string>* addToVector = NULL);
			static bool AddLineToFile(const std::string& filePath, const std::string &line);
			static std::string GetFormatedFileSize(const std::string& fileName);
			static std::string GetFormatedFileCreateDate(const std::string& fileName);
			static std::string GetFormatedFileAccessDate(const std::string& fileName);
			static std::string GetFormatedFileWriteDate(const std::string& fileName);
			static std::map<std::string, SallyAPI::File::DRIVE_TYPE> GetDriveList();
			static bool IsDirectory(const std::string& filename);
			static std::string FormatFileTime(FILETIME filetime);
		};
	}
}
