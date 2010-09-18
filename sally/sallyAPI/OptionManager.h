////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\OptionManager.h
///
/// \brief	Declares the option manager class. 
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
#include "define.h"
#include "StringHelper.h"
#include "PathHelper.h"

#include "Option.h"
#include "SystemHelper.h"
#include "SallyHelper.h"

namespace SallyAPI
{
	namespace Core
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	COptionManager
		///
		/// \brief	Manager for options. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY COptionManager
		{
		private:
			static std::map<std::string, SallyAPI::System::COption*>	m_OptionMap;
			static SallyAPI::Core::COptionManager*						m_pObject;

			COptionManager();
			~COptionManager();
		public:
			static SallyAPI::Core::COptionManager*		GetInstance();
			static void	DeleteInstance();

			SallyAPI::System::COption* GetOption(const std::string& explicidAppName);
		};
	}
}