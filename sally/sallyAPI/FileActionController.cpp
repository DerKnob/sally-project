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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	DWORD CALLBACK ProgrRoutine(LARGE_INTEGER TotalFileSize,
/// LARGE_INTEGER TotalBytesTransferred, LARGE_INTEGER StreamSize,
/// LARGE_INTEGER StreamBytesTransferred, DWORD dwStreamNumber, DWORD dwCallbackReason,
/// HANDLE hSourceFile, HANDLE hDestinationFile, LPVOID lpData)
///
/// \brief	Progr routine. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \param	TotalFileSize			Size of the total file. 
/// \param	TotalBytesTransferred	The total bytes transferred. 
/// \param	StreamSize				Size of the stream. 
/// \param	StreamBytesTransferred	The stream bytes transferred. 
/// \param	dwStreamNumber			The dw stream number. 
/// \param	dwCallbackReason		The dw callback reason. 
/// \param	hSourceFile				Handle of the source file. 
/// \param	hDestinationFile		Handle of the destination file. 
/// \param	lpData					Information describing the pointer to a. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD CALLBACK ProgrRoutine(LARGE_INTEGER TotalFileSize, LARGE_INTEGER TotalBytesTransferred, LARGE_INTEGER StreamSize, LARGE_INTEGER StreamBytesTransferred,
							DWORD dwStreamNumber, DWORD dwCallbackReason, HANDLE hSourceFile, HANDLE hDestinationFile, LPVOID lpData)
{
	CFileActionController* fileActionController = (CFileActionController*) lpData;

	if (fileActionController->ShouldCancel())
		return PROGRESS_CANCEL;

	LONGLONG erg = TotalFileSize.QuadPart / 1000;

	int i = 0;
	
	if ((TotalBytesTransferred.QuadPart > 0) && (erg > 0))
		i =	(int) (TotalBytesTransferred.QuadPart / erg);

	fileActionController->SetCurrentFileSize(1000);
	fileActionController->SetCurrentFileSizeTransferred(i);

	return PROGRESS_CONTINUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CFileActionController::CFileActionController(FILE_ACTION fileAction)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \param	fileAction	The file action. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CFileActionController::CFileActionController(FILE_ACTION fileAction)
	: m_iProcessed(0), m_bCancel(false), m_eFileAction(fileAction),
	m_iCurrentFileSizeTransferred(0), m_iCurrentFileSize(0), m_iRequestAnswer(-1)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CFileActionController::~CFileActionController()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CFileActionController::~CFileActionController()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CFileActionController::GetProcessedCount()
///
/// \brief	Gets the processed count. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \return	The processed count. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CFileActionController::GetProcessedCount()
{
	return m_iProcessed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileActionController::SetProcessedCount(int processed)
///
/// \brief	Sets a processed count. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \param	processed	The processed. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileActionController::SetProcessedCount(int processed)
{
	m_iProcessed = processed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CFileActionController::GetCurrentFile()
///
/// \brief	Gets the current file. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \return	The current file. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CFileActionController::GetCurrentFile()
{
	return m_strCurrentFile;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileActionController::SetCurrentFile(const std::string& currentFile)
///
/// \brief	Sets a current file. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \param	currentFile	The current file. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileActionController::SetCurrentFile(const std::string& currentFile)
{
	m_strCurrentFile = currentFile;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileActionController::SetCurrentFileSizeTransferred(int fileSize)
///
/// \brief	Sets a current file size transferred. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \param	fileSize	Size of the file. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileActionController::SetCurrentFileSizeTransferred(int fileSize)
{
	m_iCurrentFileSizeTransferred = fileSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileActionController::SetCurrentFileSize(int fileSize)
///
/// \brief	Sets a current file size. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \param	fileSize	Size of the file. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileActionController::SetCurrentFileSize(int fileSize)
{
	m_iCurrentFileSize = fileSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CFileActionController::GetCurrentFileSizeTransferred()
///
/// \brief	Gets the current file size transferred. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \return	The current file size transferred. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CFileActionController::GetCurrentFileSizeTransferred()
{
	return m_iCurrentFileSizeTransferred;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CFileActionController::GetCurrentFileSize()
///
/// \brief	Gets the current file size. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \return	The current file size. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CFileActionController::GetCurrentFileSize()
{
	return m_iCurrentFileSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileActionController::Cancel()
///
/// \brief	Cancels this object. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileActionController::Cancel()
{
	m_bCancel = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CFileActionController::ShouldCancel()
///
/// \brief	Queries if we should cancel. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CFileActionController::ShouldCancel()
{
	return m_bCancel;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	FILE_ACTION CFileActionController::GetFileAction()
///
/// \brief	Gets the file action. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \return	The file action. 
////////////////////////////////////////////////////////////////////////////////////////////////////

FILE_ACTION CFileActionController::GetFileAction()
{
	return m_eFileAction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CFileActionController::CopyFile(const std::string& sourceFile,
/// const std::string& destinationFile)
///
/// \brief	Copies a file. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \param	sourceFile		Source file. 
/// \param	destinationFile	Destination file. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CFileActionController::CopyFile(const std::string& sourceFile, const std::string& destinationFile)
{
	BOOL result = ::CopyFileEx(sourceFile.c_str(), destinationFile.c_str(), &ProgrRoutine, (LPVOID)this, FALSE,
		COPY_FILE_FAIL_IF_EXISTS | COPY_FILE_OPEN_SOURCE_FOR_WRITE);
	if (result == FALSE)
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CFileActionController::MoveFile(const std::string& sourceFile,
/// const std::string& destinationFile)
///
/// \brief	Move file. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \param	sourceFile		Source file. 
/// \param	destinationFile	Destination file. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CFileActionController::MoveFile(const std::string& sourceFile, const std::string& destinationFile)
{
	BOOL result = ::MoveFileWithProgress(sourceFile.c_str(), destinationFile.c_str(), &ProgrRoutine, (LPVOID)this, NULL);
	if (result == FALSE)
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileActionController::RequestUserAction(const std::string& message,
/// const std::string& filename)
///
/// \brief	Request user action. Called by FileHelper::FileAction
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \param	message		The message. 
/// \param	filename	Filename of the file. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileActionController::RequestUserAction(const std::string& message, const std::string& filename)
{
	m_strRequestMessage = message;
	m_strRequestFilename = filename;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CFileActionController::IsRequestWaiting()
///
/// \brief	Query if this object is request waiting. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \return	true if request waiting, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CFileActionController::IsRequestWaiting()
{
	if (m_strRequestFilename.size() > 0)
		return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CFileActionController::GetRequestMessage()
///
/// \brief	Gets the request message. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \return	The request message. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CFileActionController::GetRequestMessage()
{
	return m_strRequestMessage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CFileActionController::GetRequestFilename()
///
/// \brief	Gets the request filename. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \return	The request filename. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CFileActionController::GetRequestFilename()
{
	return m_strRequestFilename;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileActionController::SetRequestAnswer(bool value)
///
/// \brief	Sets a request answer. 
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileActionController::SetRequestAnswer(bool value)
{
	if (value)
		m_iRequestAnswer = 1;
	else
		m_iRequestAnswer = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CFileActionController::GetRequestAnswer()
///
/// \brief	Gets the request answer. Called by FileHelper::FileAction
///
/// \author	Christian Knobloch
/// \date	23.12.2011
///
/// \return	The request answer. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CFileActionController::GetRequestAnswer()
{
	return m_iRequestAnswer;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileActionController::ResetRequestAnswer()
///
/// \brief	Resets a request answer. Called by FileHelper::FileAction
///
/// \author	Christian Knobloch
/// \date	23.12.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileActionController::ResetRequestAnswer()
{
	m_iRequestAnswer = -1;
	m_strRequestFilename = "";
}