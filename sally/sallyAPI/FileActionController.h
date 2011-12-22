////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\FileActionController.h
///
/// \brief	Declares the file action controller class. 
///
/// \author	Christian Knobloch
/// \date	22.12.2011
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
#include <string>

namespace SallyAPI
{
	namespace File
	{
		enum FILE_ACTION { FILE_ACTION_MOVE, FILE_ACTION_COPY, FILE_ACTION_DELETE };

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CFileActionController
		///
		/// \brief	File action controller.
		///
		/// \author	Christian Knobloch
		/// \date	22.12.2011
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CFileActionController
		{
		protected:
			FILE_ACTION		m_eFileAction;
			int				m_iProcessed;
			std::string		m_strCurrentFile;
			bool			m_bCancel;
			int				m_iCurrentFileSize;
			int				m_iCurrentFileSizeTransferred;
		public:
			CFileActionController(FILE_ACTION fileAction);
			virtual ~CFileActionController();

			int				GetCurrentFileSize();
			int				GetCurrentFileSizeTransferred();

			void			SetCurrentFileSize(int fileSize);
			void			SetCurrentFileSizeTransferred(int fileSize);

			int				GetProcessedCount();
			void			SetProcessedCount(int processed);

			std::string		GetCurrentFile();
			void			SetCurrentFile(const std::string& currentFile);

			FILE_ACTION		GetFileAction();

			void			Cancel();
			bool			ShouldCancel();

			bool			CopyFile(const std::string& sourceFile, const std::string& destinationFile);
			bool			MoveFile(const std::string& sourceFile, const std::string& destinationFile);
		};
	}
}