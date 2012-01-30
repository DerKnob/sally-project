////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\MediaHelper.cpp
///
/// \brief	Implements the media helper class. 
///
/// \author	Christian Knobloch
/// \date	07.07.2010
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

#include "MediaHelper.h"

using namespace SallyAPI::Media;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CMediaHelper::CMediaHelper()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	07.07.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CMediaHelper::CMediaHelper()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CMediaHelper::~CMediaHelper()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	07.07.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CMediaHelper::~CMediaHelper()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CMediaHelper::GetVLCPluginDirectory()
///
/// \brief	Gets the vlc plugin directory. 
///
/// \author	Christian Knobloch
/// \date	07.07.2011
///
/// \return	The vlc plugin directory. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string	CMediaHelper::GetVLCPluginDirectory()
{
	std::string vlcPlugins = SallyAPI::System::SystemHelper::GetModulePath();
	vlcPlugins.append("plugins\\");

	return vlcPlugins;
}