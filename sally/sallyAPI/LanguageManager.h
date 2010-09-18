////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\LanguageManager.h
///
/// \brief	Declares the language manager class. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2010 Sally Project
/// http://www.sally-project.de/
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
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "SystemHelper.h"
#include "PathHelper.h"
#include "StringHelper.h"
#include "Game.h"

namespace SallyAPI
{
	namespace Config
	{
		#define MAX_LANGSTRING	10000

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CLanguageManager
		///
		/// \brief	Manager for languages. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CLanguageManager
		{
		private:
			std::map<std::string, std::string>	m_mCache;
			std::vector<std::string>			m_vLangFileDirs;
			std::string							m_strBaseDir;
			std::string							m_strLang;
			std::string							m_strFileExtention;
		public:
			CLanguageManager(const std::string& lang, const std::string& fileExtention);
			~CLanguageManager();

			std::string GetString(const std::string& id);
			std::string GetString(const std::string& id, const char* first, ...);

			void						AddLangFileDir(const std::string& langFileDir);
			std::vector<std::string>	GetLangFileDirs();
		};
	}
}