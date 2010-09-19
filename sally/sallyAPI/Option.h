////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Option.h
///
/// \brief	Declares the option class. 
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

#include <map>
#include "SystemHelper.h"

namespace SallyAPI
{
	namespace System
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	COption
		///
		/// \brief	Option. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY COption
		{
		private:
			static std::string					m_strDefaultFile;
			std::string							m_strFile;
			std::map<std::string, std::string>	m_PropertyCache;
		public:
			COption(const std::string& iniFile = "");
			~COption();

			int				GetPropertyInt(const std::string& component, const std::string& id, int defaultValue = -1);
			std::string		GetPropertyString(const std::string& component, const std::string& id, const std::string& defaultValue = "");
			bool			GetPropertyBool(const std::string& component, const std::string& id, bool defaultValue = false);

			void			SetPropertyInt(const std::string& component, const std::string& id, int value);
			void			SetPropertyString(const std::string& component, const std::string& id, const std::string& value);
			void			SetPropertyBool(const std::string& component, const std::string& id, bool value);

			static int			GetPropertyIntStatic(const std::string& component, const std::string& id, int defaultValue = -1, const std::string& iniFile = "");
			static std::string	GetPropertyStringStatic(const std::string& component, const std::string& id, const std::string& defaultValue = "", const std::string& iniFile = "");
			static bool			GetPropertyBoolStatic(const std::string& component, const std::string& id, bool defaultValue = false, const std::string& iniFile = "");

			static void			SetPropertyIntStatic(const std::string& component, const std::string& id, int value, const std::string& iniFile);
			static void			SetPropertyStringStatic(const std::string& component, const std::string& id, const std::string& value, const std::string& iniFile);
			static void			SetPropertyBoolStatic(const std::string& component, const std::string& id, bool value, const std::string& iniFile);
			static void		SetDefaultFile(const std::string& defaultFile);
		};
	}
}