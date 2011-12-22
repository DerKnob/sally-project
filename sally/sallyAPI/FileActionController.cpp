////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\FileActionController.cpp
///
/// \brief	Implements the file action controller class. 
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

#include "FileActionController.h"

using namespace SallyAPI::File;

DWORD CALLBACK ProgrRoutine(LARGE_INTEGER TotalFileSize, LARGE_INTEGER TotalBytesTransferred, LARGE_INTEGER StreamSize, LARGE_INTEGER StreamBytesTransferred,
							DWORD dwStreamNumber, DWORD dwCallbackReason, HANDLE hSourceFile, HANDLE hDestinationFile, LPVOID lpData)
{
	CFileActionController* fileActionController = (CFileActionController*) lpData;

	if (fileActionController->ShouldCancel())
		return PROGRESS_CANCEL;

	LONGLONG erg = TotalFileSize.QuadPart / 1000;

	int i = TotalBytesTransferred.QuadPart / erg;

	fileActionController->SetCurrentFileSize(1000);
	fileActionController->SetCurrentFileSizeTransferred(i);

	return PROGRESS_CONTINUE;
}

CFileActionController::CFileActionController(FILE_ACTION fileAction)
	: m_iProcessed(0), m_bCancel(false), m_eFileAction(fileAction),
	m_iCurrentFileSizeTransferred(0), m_iCurrentFileSize(0)
{
}

CFileActionController::~CFileActionController()
{
}

int CFileActionController::GetProcessedCount()
{
	return m_iProcessed;
}

void CFileActionController::SetProcessedCount(int processed)
{
	m_iProcessed = processed;
}

std::string CFileActionController::GetCurrentFile()
{
	return m_strCurrentFile;
}

void CFileActionController::SetCurrentFile(const std::string& currentFile)
{
	m_strCurrentFile = currentFile;
}

void CFileActionController::SetCurrentFileSizeTransferred(int fileSize)
{
	m_iCurrentFileSizeTransferred = fileSize;
}

void CFileActionController::SetCurrentFileSize(int fileSize)
{
	m_iCurrentFileSize = fileSize;
}

int CFileActionController::GetCurrentFileSizeTransferred()
{
	return m_iCurrentFileSizeTransferred;
}

int CFileActionController::GetCurrentFileSize()
{
	return m_iCurrentFileSize;
}

void CFileActionController::Cancel()
{
	m_bCancel = true;
}

bool CFileActionController::ShouldCancel()
{
	return m_bCancel;
}

FILE_ACTION CFileActionController::GetFileAction()
{
	return m_eFileAction;
}

bool CFileActionController::CopyFile(const std::string& sourceFile, const std::string& destinationFile)
{
	BOOL result =  CopyFileEx(sourceFile.c_str(), destinationFile.c_str(), &ProgrRoutine, (LPVOID)this, NULL, COPY_FILE_OPEN_SOURCE_FOR_WRITE);
	if (result == FALSE)
		return false;
	return true;
}

bool CFileActionController::MoveFile(const std::string& sourceFile, const std::string& destinationFile)
{
	BOOL result = MoveFileWithProgress(sourceFile.c_str(), destinationFile.c_str(), &ProgrRoutine, (LPVOID)this, NULL);
	if (result == FALSE)
		return false;
	return true;
}