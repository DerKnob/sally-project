////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\AudioFile.cpp
///
/// \brief	Implements the audio file class. 
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

#include "AudioFile.h"

CAudioFile::CAudioFile(const std::string& fileName) : CMediaFile(fileName)
{
	m_eType = MEDIAFILE_AUDIO;
	m_bTagInit = false;

	// Read Mp3Tag
	m_bTagInit = m_Mp3Tag.Init(fileName);
}

CAudioFile::~CAudioFile(void)
{
	m_Mp3Tag.Free();
}

void CAudioFile::ReloadMp3Tags()
{
	m_Mp3Tag.Free();

	m_bTagInit = false;

	m_bTagInit = m_Mp3Tag.Init(m_strFilename);
}

MP3FileInfo* CAudioFile::GetMp3Tag()
{
	if (m_bTagInit != false)
		return &m_Mp3Tag;
	else
		return NULL;
}

SallyAPI::GUI::CPicture* CAudioFile::GetPicture()
{
	std::string	pictureFile, folder;
	SallyAPI::GUI::CPicture* pictureTemp;

	folder = SallyAPI::String::PathHelper::GetDirectoryFromPath(m_strFilename);

	pictureFile.append(folder);
	pictureFile.append(GetCoverName());

	pictureTemp = new SallyAPI::GUI::CPicture();
	if (pictureTemp->LoadTexture(pictureFile))
		return pictureTemp;
	
	delete pictureTemp;
	return NULL;
}

std::string CAudioFile::GetFormatedText()
{
	std::string result;
	if ((m_bTagInit != false) && (m_Mp3Tag.GetSzArtist().length() > 0 || m_Mp3Tag.GetSzTitle().length() > 0))
	{
		result.append(m_Mp3Tag.GetSzArtist());
		result.append(" - ");
		result.append(m_Mp3Tag.GetSzTitle());
	}
	else
	{
		result = SallyAPI::String::PathHelper::GetFileFromPath(m_strFilename);
	}
	return result;
}

std::string CAudioFile::GetCoverName()
{
	if (m_bTagInit != false)
	{
		if ((m_Mp3Tag.GetSzAlbum().length() > 0) &&
			((m_Mp3Tag.GetSzBand().length() > 0) || (m_Mp3Tag.GetSzArtist().length() > 0)))
		{
			if (m_Mp3Tag.GetSzBand().length() > 0)
				return GetCoverName(m_Mp3Tag.GetSzBand(), m_Mp3Tag.GetSzAlbum());

			// fallback (but should be the normal case
			return GetCoverName(m_Mp3Tag.GetSzArtist(), m_Mp3Tag.GetSzAlbum());
		}
	}
	return "";
}

std::string CAudioFile::GetCoverName(const std::string& artist, const std::string& album)
{
	std::string outfile;

	std::string outArtist = SallyAPI::String::PathHelper::CorrectFileName(artist);
	std::string outAlbum = SallyAPI::String::PathHelper::CorrectFileName(album);

	outfile.append(outArtist);
	outfile.append(" - ");
	outfile.append(outAlbum);
	outfile.append(".jpg");
	return outfile;
}

std::string CAudioFile::GetMp3Genre(const std::string& number)
{
	if (number.length() <= 0)
		return "";
	std::string temp = number.substr(1, number.length() - 2);

	int i = atoi(temp.c_str());
	if ((i == 0) && (temp[0] != '0'))
		return number;

	switch(i)
	{
		case 0: return "Blues";
		case 1: return "Classic Rock";
		case 2: return "Country";
		case 3: return "Dance";
		case 4: return "Disco";
		case 5: return "Funk";
		case 6: return "Grunge";
		case 7: return "Hip-Hop";
		case 8: return "Jazz";
		case 9: return "Metal";
		case 10: return "New Age";
		case 11: return "Oldies";
		case 12: return "Other";
		case 13: return "Pop";
		case 14: return "R&B";
		case 15: return "Rap";
		case 16: return "Reggae";
		case 17: return "Rock";
		case 18: return "Techno";
		case 19: return "Industrial";
		case 20: return "Alternative";
		case 21: return "Ska";
		case 22: return "Death Metal";
		case 23: return "Pranks";
		case 24: return "Soundtrack";
		case 25: return "Euro-Techno";
		case 26: return "Ambient";
		case 27: return "Trip-Hop";
		case 28: return "Vocal";
		case 29: return "Jazz&Funk";
		case 30: return "Fusion";
		case 31: return "Trance";
		case 32: return "Classical";
		case 33: return "Instrumental";
		case 34: return "Acid";
		case 35: return "House";
		case 36: return "Game";
		case 37: return "Sound Clip";
		case 38: return "Gospel";
		case 39: return "Noise";
		case 40: return "Alternative Rock";
		case 41: return "Bass";
		case 42: return "Soul";
		case 43: return "Punk";
		case 44: return "Space";
		case 45: return "Meditative";
		case 46: return "Instrumental Pop";
		case 47: return "Instrumental Rock";
		case 48: return "Ethnic";
		case 49: return "Gothic";
		case 50: return "Darkwave";
		case 51: return "Techno-Industrial";
		case 52: return "Electronic";
		case 53: return "Pop-Folk";
		case 54: return "Eurodance";
		case 55: return "Dream";
		case 56: return "Southern Rock";
		case 57: return "Comedy";
		case 58: return "Cult";
		case 59: return "Gangsta";
		case 60: return "Top 40";
		case 61: return "Christian Rap";
		case 62: return "Pop/Funk";
		case 63: return "Jungle";
		case 64: return "Native US";
		case 65: return "Cabaret";
		case 66: return "New Wave";
		case 67: return "Psychedelic";
		case 68: return "Rave";
		case 69: return "Showtunes";
		case 70: return "Trailer";
		case 71: return "Lo-Fi";
		case 72: return "Tribal";
		case 73: return "Acid Punk";
		case 74: return "Acid Jazz";
		case 75: return "Polka";
		case 76: return "Retro";
		case 77: return "Musical";
		case 78: return "Rock & Roll";
		case 79: return "Hard Rock";
		case 80: return "Folk";
		case 81: return "Folk-Rock";
		case 82: return "National Folk";
		case 83: return "Swing";
		case 84: return "Fast Fusion";
		case 85: return "Bebop";
		case 86: return "Latin";
		case 87: return "Revival";
		case 88: return "Celtic";
		case 89: return "Bluegrass";
		case 90: return "Avantgarde";
		case 91: return "Gothic Rock";
		case 92: return "Progressive Rock";
		case 93: return "Psychedelic Rock";
		case 94: return "Symphonic Rock";
		case 95: return "Slow Rock";
		case 96: return "Big Band";
		case 97: return "Chorus";
		case 98: return "Easy Listening";
		case 99: return "Acoustic";
		case 100: return "Humour";
		case 101: return "Speech";
		case 102: return "Chanson";
		case 103: return "Opera";
		case 104: return "Chamber Music";
		case 105: return "Sonata";
		case 106: return "Symphony";
		case 107: return "Booty Bass";
		case 108: return "Primus";
		case 109: return "Porn Groove";
		case 110: return "Satire";
		case 111: return "Slow Jam";
		case 112: return "Club";
		case 113: return "Tango";
		case 114: return "Samba (Musik)";
		case 115: return "Folklore";
		case 116: return "Ballad";
		case 117: return "Power Ballad";
		case 118: return "Rhytmic Soul";
		case 119: return "Freestyle";
		case 120: return "Duet";
		case 121: return "Punk Rock";
		case 122: return "Drum Solo";
		case 123: return "Acapella";
		case 124: return "Euro-House";
		case 125: return "Dance Hall";
		case 126: return "Goa";
		case 127: return "Drum’n’Bass";
		case 128: return "Club-House";
		case 129: return "Hardcore";
		case 130: return "Terror";
		case 131: return "Indie";
		case 132: return "BritPop";
		case 133: return "Negerpunk";
		case 134: return "Polsk Punk";
		case 135: return "Beat";
		case 136: return "Christian Gangsta";
		case 137: return "Heavy Metal";
		case 138: return "Black Metal";
		case 139: return "Crossover";
		case 140: return "Contemporary Christian";
		case 141: return "Christian Rock";
		case 142: return "Merengue";
		case 143: return "Salsa";
		case 144: return "Thrash Metal";
		case 145: return "Anime";
		case 146: return "JPop";
		case 147: return "SynthPop";
		default: return number;
	}
}

bool CAudioFile::IsAudioFile(const std::string& filename)
{
	std::vector<std::string> fileExtensions = GetFileExtensions();
	std::vector<std::string>::iterator iter = fileExtensions.begin();

	while (iter != fileExtensions.end())
	{
		std::string end = *iter;
		if (SallyAPI::String::StringHelper::StringEndsWith(filename, end))
			return true;
		++iter;
	}
	return false;
}

std::vector<std::string> CAudioFile::GetFileExtensions()
{
	static std::vector<std::string> m_FileExtensions;
	
	// init on first call
	if (m_FileExtensions.size() == 0)
	{
		m_FileExtensions.push_back(".a52");
		m_FileExtensions.push_back(".aac");
		m_FileExtensions.push_back(".ac3");
		m_FileExtensions.push_back(".dts");
		m_FileExtensions.push_back(".flac");
		m_FileExtensions.push_back(".m4a");
		m_FileExtensions.push_back(".m4p");
		m_FileExtensions.push_back(".mka");
		m_FileExtensions.push_back(".mod");
		m_FileExtensions.push_back(".mp1");
		m_FileExtensions.push_back(".mp2");
		m_FileExtensions.push_back(".mp3");
		m_FileExtensions.push_back(".oma");
		m_FileExtensions.push_back(".oga");
		m_FileExtensions.push_back(".spx");
		m_FileExtensions.push_back(".wav");
		m_FileExtensions.push_back(".wv");
		m_FileExtensions.push_back(".xm");
		m_FileExtensions.push_back(".wma");
		m_FileExtensions.push_back(".ogg");
	}

	return m_FileExtensions;
}