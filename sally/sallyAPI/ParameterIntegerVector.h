////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterIntegerVector.h
///
/// \brief	Declares the parameter integer vector class. 
///
/// \author	Christian Knobloch
/// \date	24.12.2011
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
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
#include "ParameterBase.h"
#include <vector>

namespace SallyAPI
{
	namespace GUI
	{
		namespace SendMessage
		{

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \class	CParameterIntegerVector
			///
			/// \brief	Parameter integer vector. 
			///
			/// \author	Christian Knobloch
			/// \date	24.12.2011
			////////////////////////////////////////////////////////////////////////////////////////////////////

			class DLL_API_SALLY CParameterIntegerVector
				: public SallyAPI::GUI::SendMessage::CParameterBase
			{
			private:
				std::vector<int>	m_vChangeList;
			public:
				CParameterIntegerVector(std::vector<int> changeList);
				virtual ~CParameterIntegerVector();

				std::vector<int>	GetChangeList();
			};
		}
	}
}