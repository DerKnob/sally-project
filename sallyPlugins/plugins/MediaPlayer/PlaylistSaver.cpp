////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\PlaylistSaver.cpp
///
/// \brief	Implements the playlist saver class. 
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

#include "PlaylistSaver.h"

typedef std::pair<std::string, int> MyPair;


struct MyTestCompare
{

	bool operator()(const MyPair& firstPair, const MyPair& secondPair) const
	{
		return firstPair.second > secondPair.second;
	}
};

CPlaylistSaver::CPlaylistSaver(std::vector<SallyAPI::GUI::CListViewItem>& itemList,
							   std::map<std::string, int>& bandList, const std::string& playlistName)
{
	m_pItemList = itemList;
	m_mBandList = bandList;
	m_strPlaylistName = playlistName;
}

CPlaylistSaver::~CPlaylistSaver()
{
}

void CPlaylistSaver::RunEx()
{
	std::string fileName = m_strPlaylistName;

	// now sort the map
	std::multiset<MyPair,MyTestCompare> sortedBandList;

	std::map<std::string, int>::iterator iterBandList = m_mBandList.begin();

	while (iterBandList != m_mBandList.end())
	{
		sortedBandList.insert(*iterBandList);
		++iterBandList;
	}

	// build the file name
	std::string bandFilename;

	std::multiset<MyPair,MyTestCompare>::iterator iterSortedList = sortedBandList.begin();
	
	for (int i = 0; (i < 5) && (iterSortedList != sortedBandList.end()); ++i)
	{
		if (i > 0)
			bandFilename.append(" - ");

		std::pair<std::string, int> pair = *iterSortedList;
		bandFilename.append(pair.first);

		++iterSortedList;
	}
	bandFilename = SallyAPI::String::PathHelper::CorrectFileName(bandFilename);
	
	fileName.append(bandFilename);

	if (fileName.length() > MAX_PATH - 5)
		fileName = fileName.substr(0, MAX_PATH - 5);

	fileName.append(".m3u");

	CPlaylistHelper::SavePlaylist(m_pItemList, fileName);
}