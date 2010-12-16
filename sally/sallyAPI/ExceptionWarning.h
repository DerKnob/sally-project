////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ExceptionWarning.h
///
/// \brief	Declares the exception warning class. 
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
#include "exception.h"

namespace SallyAPI
{
	namespace Exception
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CExceptionWarning
		///
		/// \brief	Exception warning. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CExceptionWarning
			: public CException
		{
		public:
			CExceptionWarning(const std::string& message);
			virtual ~CExceptionWarning();
		};
	}
}