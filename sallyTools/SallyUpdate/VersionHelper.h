////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	SallyUpdate\VersionHelper.h
///
/// \brief	Declares the version helper functions. 
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
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU General Public License for more details.
/// 
/// You should have received a copy of the GNU General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool newVersion(std::string localVersion, std::string onlineVersion)
{
	std::vector<std::string> localVector = SallyAPI::String::StringHelper::TokenizeString(localVersion, ".");
	std::vector<std::string> onlineVector = SallyAPI::String::StringHelper::TokenizeString(onlineVersion, ".");

	for (int i = 0; i < ((int) localVector.size()) && i < ((int) onlineVector.size()); ++i)
	{
		int localInt = SallyAPI::String::StringHelper::ConvertToInt(localVector[i]);
		int onlineInt = SallyAPI::String::StringHelper::ConvertToInt(onlineVector[i]);

		if (onlineInt > localInt)
			return true;
		else if (onlineInt < localInt)
			return false;
	}
	if (localVector.size() < onlineVector.size())
		return true;
	
	return false;
}