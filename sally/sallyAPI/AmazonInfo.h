////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\AmazonInfo.h
///
/// \brief	Declares the amazon information class. 
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
#include "StringHelper.h"
#include "NetworkHelper.h"
#include "SallyHelper.h"

namespace SallyAPI
{
	namespace Media
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \enum	COVER_SIZE
		///
		/// \brief	Values that represent COVER_SIZE. 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum COVER_SIZE {COVER_SIZE_BIG, COVER_SIZE_MEDIUM, COVER_SIZE_SMALL};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CAmazonInfo
		///
		/// \brief	Information about the amazon. 
		///
		/// \author	Christian Knobloch
		/// \date	20.05.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CAmazonInfo
		{
		private:
			std::string	NormalizeString(const std::string& in);
			void		CleanupStrings(std::string& artist, std::string& album);
			bool		DownloadImage(std::string& imageURL, std::string& outputFile);
			void		GetXML(std::string& artist, std::string& album, std::string* response);

			bool		GetCoverInternal(std::string& artist, std::string& album, std::string& outputFile, std::string& tempFile, COVER_SIZE size);
		public:
			CAmazonInfo();
			~CAmazonInfo();

			bool	GetCover(std::string artist, std::string album, std::string outputFile, COVER_SIZE size = COVER_SIZE_BIG);
		};
	}
}