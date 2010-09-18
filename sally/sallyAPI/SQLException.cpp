////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SQLException.cpp
///
/// \brief	Implements the sql exception class. 
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

#include "SQLException.h"

using namespace SallyAPI::Database;

CSQLException::CSQLException(int code)
: m_nCode(code)
{
	setMessageFromCode();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSQLException::CSQLException(const std::string& msg)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	msg	The message. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CSQLException::CSQLException(const std::string& msg) 
: m_nCode(0), m_strMsg(msg)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSQLException::~CSQLException() throw ()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSQLException::~CSQLException() throw ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	const char* CSQLException::what() const throw()
///
/// \brief	Whats this object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else. 
////////////////////////////////////////////////////////////////////////////////////////////////////

const char* CSQLException::what() const throw()
{
	std::ostringstream oss;
	oss << m_nCode << ": " << m_strMsg;
	return oss.str().c_str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CSQLException::GetCode()
///
/// \brief	Gets the code. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The code. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CSQLException::GetCode() 
{
	return m_nCode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CSQLException::GetMessage()
///
/// \brief	Gets the message. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The message. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CSQLException::GetMessage() 
{
	return m_strMsg;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSQLException::setMessageFromCode()
///
/// \brief	Sets the message from code. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSQLException::setMessageFromCode() 
{
	if (m_nCode == SQLITE_ERROR) m_strMsg = "SQL error or missing database";
	else if(m_nCode == SQLITE_INTERNAL) m_strMsg = "An internal logic error in the database";
	else if(m_nCode == SQLITE_PERM) m_strMsg = "Access permission denied";
	else if(m_nCode == SQLITE_ABORT) m_strMsg = "Callback routine requested an abort";
	else if(m_nCode == SQLITE_BUSY) m_strMsg = "The database file is locked";
	else if(m_nCode == SQLITE_LOCKED) m_strMsg = "A table in the database is locked";
	else if(m_nCode == SQLITE_NOMEM) m_strMsg = "A memory allocation failed in the database";
	else if(m_nCode == SQLITE_READONLY) m_strMsg = "Attempt to write a readonly database";
	else if(m_nCode == SQLITE_INTERRUPT) m_strMsg = "Operation terminated by a database interrupt";
	else if(m_nCode == SQLITE_IOERR) m_strMsg = "Some kind of disk I/O error occurred";
	else if(m_nCode == SQLITE_CORRUPT) m_strMsg = "The database disk image is malformed";
	else if(m_nCode == SQLITE_NOTFOUND) m_strMsg = "Table or record not found";
	else if(m_nCode == SQLITE_FULL) m_strMsg = "Insertion failed because database is full";
	else if(m_nCode == SQLITE_CANTOPEN) m_strMsg = "Unable to open the database file";
	else if(m_nCode == SQLITE_PROTOCOL) m_strMsg = "Database lock protocol error";
	else if(m_nCode == SQLITE_EMPTY) m_strMsg = "Database table is empty";
	else if(m_nCode == SQLITE_SCHEMA) m_strMsg = "The database schema changed";
	else if(m_nCode == SQLITE_TOOBIG) m_strMsg = "Too much data for one row of a table";
	else if(m_nCode == SQLITE_CONSTRAINT) m_strMsg = "Abort due to contraint violation";
	else if(m_nCode == SQLITE_MISMATCH) m_strMsg = "Data type mismatch";
	else if(m_nCode == SQLITE_MISUSE) m_strMsg = "Library used incorrectly";
	else if(m_nCode == SQLITE_NOLFS) m_strMsg = "Uses OS features not supported on host";
	else if(m_nCode == SQLITE_AUTH) m_strMsg = "Authorization denied";
	else m_strMsg = "Unknown database error";
}
