/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

                        MP3FileInfo 1.0
                    -----------------------

  MP3FileInfo is a wrapper class for id3lib, a library capable
  of reading and writing ID3 Tags of MP3 Files. It has a
  simple interface and provides all data as public members.

  Written by Christian Richardt (cr@whizer.net).

  Release history:
    Mai 31, 2003: Version 1.0. First release.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/

/**
  \file    MP3FileInfo.cpp
  \brief   Reads Header and ID3 Tags of MP3 Files
  \date    2003-05-31
  \author  Christian Richardt (cr@whizer.net)
**/


  #include "MP3FileInfo.h"
  #include <assert.h>
  #include <stdio.h>



/* ----------------------- Init and Free ------------------------ */


    /**
      \brief   Initialize and parse all data

      Always Initalize this class by calling Init(). It returns
      false if an error occured.

      You can call Init() as often as you want. All data is being
      freed before extracting the data of the new file.
    **/
  bool MP3FileInfo::Init(const std::string& cszFilename)
   {
    Free();
    m_bOK = true;

    m_id3tag = new ID3_Tag;
    m_id3tag->Link(cszFilename.c_str());

    /* Fileinfo */
    bHasLyrics = m_id3tag->HasLyrics();
    bHasV1Tag  = m_id3tag->HasV1Tag();
    bHasV2Tag  = m_id3tag->HasV2Tag();
    nFileSize  = (int)m_id3tag->GetFileSize();

    /* Headerinfo */
    m_parseHeader();
    if(!m_bOK) { Free(); return m_bOK; }

    /* V1 up */
    szAlbum = m_getNewString(ID3FID_ALBUM);
    szArtist  = m_getNewString(ID3FID_LEADARTIST);
    szComment = m_getNewString(ID3FID_COMMENT);
    szTitle   = m_getNewString(ID3FID_TITLE);
    szTrack   = m_getNewString(ID3FID_TRACKNUM);
    szYear    = m_getNewString(ID3FID_YEAR);

    /* V2 */
    szComposer        = m_getNewString(ID3FID_COMPOSER);
    szCopyright       = m_getNewString(ID3FID_COPYRIGHT);
    szEncodedBy       = m_getNewString(ID3FID_ENCODEDBY);
    szOriginalArtist  = m_getNewString(ID3FID_ORIGARTIST);
    szURL             = m_getNewString(ID3FID_WWWUSER);

    m_parseGenre();

    return m_bOK;
   }


    /**
      \brief   Free all allocated ressources

      You can call Free() as often as you want.
      No error is raised.
    **/
  void MP3FileInfo::Free(void)
   {
    if(m_bOK) // only free if OK
     {
      m_bOK = false;

      if(m_id3tag!=0)
       {
          // in rare cases (e.g. if file is corrupt)
          // "delete m_id3tag" fails ...
        try { delete m_id3tag; } catch(...) {}
        m_id3tag = 0;
       }

      /* fileinfo */
	  szFilename="";
      bHasLyrics = false; bHasV1Tag = false; bHasV2Tag = false;

      /* headerinfo */
      m_mp3header = 0;
      szBitRate ="";
      szMpegLayer = "";
      szMpegVersion = "";
      szChannelMode = "";

      /* ID3V1 Tag elements */
      szArtist="";
      szTitle="";
      szAlbum="";
	  szComment="";
      szTrack="";
      szYear="";
      szGenre="";

      /* V2 up */
      szComposer="";
      szCopyright="";
      szEncodedBy="";
      szOriginalArtist="";
      szURL="";
     }
   }


/* ---------------- public member functions ---------------- */


    /// prints all frames the file contains
  void MP3FileInfo::ShowAllFrames(void)
   {
    assert(m_bOK);

      // all FrameIDs
    ID3_FrameID fids[] = { ID3FID_NOFRAME,          ID3FID_AUDIOCRYPTO,     ID3FID_PICTURE,             ID3FID_AUDIOSEEKPOINT,  ID3FID_COMMENT,
                           ID3FID_COMMERCIAL,       ID3FID_CRYPTOREG,       ID3FID_EQUALIZATION2,       ID3FID_EQUALIZATION,    ID3FID_EVENTTIMING,
                           ID3FID_GENERALOBJECT,    ID3FID_GROUPINGREG,     ID3FID_INVOLVEDPEOPLE,      ID3FID_LINKEDINFO,      ID3FID_CDID,
                           ID3FID_MPEGLOOKUP,       ID3FID_OWNERSHIP,       ID3FID_PRIVATE,             ID3FID_PLAYCOUNTER,     ID3FID_POPULARIMETER,
                           ID3FID_POSITIONSYNC,     ID3FID_BUFFERSIZE,      ID3FID_VOLUMEADJ2,          ID3FID_VOLUMEADJ,       ID3FID_REVERB,
                           ID3FID_SEEKFRAME,        ID3FID_SIGNATURE,       ID3FID_SYNCEDLYRICS,        ID3FID_SYNCEDTEMPO,     ID3FID_ALBUM,
                           ID3FID_BPM,              ID3FID_COMPOSER,        ID3FID_CONTENTTYPE,         ID3FID_COPYRIGHT,       ID3FID_DATE,
                           ID3FID_ENCODINGTIME,     ID3FID_PLAYLISTDELAY,   ID3FID_ORIGRELEASETIME,     ID3FID_RECORDINGTIME,   ID3FID_RELEASETIME,
                           ID3FID_TAGGINGTIME,      ID3FID_INVOLVEDPEOPLE2, ID3FID_ENCODEDBY,           ID3FID_LYRICIST,        ID3FID_FILETYPE,
                           ID3FID_TIME,             ID3FID_CONTENTGROUP,    ID3FID_TITLE,               ID3FID_SUBTITLE,        ID3FID_INITIALKEY,
                           ID3FID_LANGUAGE,         ID3FID_SONGLEN,         ID3FID_MUSICIANCREDITLIST,  ID3FID_MEDIATYPE,       ID3FID_MOOD,
                           ID3FID_ORIGALBUM,        ID3FID_ORIGFILENAME,    ID3FID_ORIGLYRICIST,        ID3FID_ORIGARTIST,      ID3FID_ORIGYEAR,
                           ID3FID_FILEOWNER,        ID3FID_LEADARTIST,      ID3FID_BAND,                ID3FID_CONDUCTOR,       ID3FID_MIXARTIST,
                           ID3FID_PARTINSET,        ID3FID_PRODUCEDNOTICE,  ID3FID_PUBLISHER,           ID3FID_TRACKNUM,        ID3FID_RECORDINGDATES,
                           ID3FID_NETRADIOSTATION,  ID3FID_NETRADIOOWNER,   ID3FID_SIZE,                ID3FID_ALBUMSORTORDER,  ID3FID_PERFORMERSORTORDER,
                           ID3FID_TITLESORTORDER,   ID3FID_ISRC,            ID3FID_ENCODERSETTINGS,     ID3FID_SETSUBTITLE,     ID3FID_USERTEXT,
                           ID3FID_YEAR,             ID3FID_UNIQUEFILEID,    ID3FID_TERMSOFUSE,          ID3FID_UNSYNCEDLYRICS,  ID3FID_WWWCOMMERCIALINFO,
                           ID3FID_WWWCOPYRIGHT,     ID3FID_WWWAUDIOFILE,    ID3FID_WWWARTIST,           ID3FID_WWWAUDIOSOURCE,  ID3FID_WWWRADIOPAGE,
                           ID3FID_WWWPAYMENT,       ID3FID_WWWPUBLISHER,    ID3FID_WWWUSER,             ID3FID_METACRYPTO,      ID3FID_METACOMPRESSION,
                           ID3FID_LASTFRAMEID
                         };

      // a frame usually contains one field of the following fields:
    ID3_FieldID fiids[] = { ID3FN_NOFIELD, ID3FN_TEXTENC, ID3FN_TEXT, ID3FN_URL, ID3FN_DATA, ID3FN_DESCRIPTION, ID3FN_OWNER, ID3FN_EMAIL, ID3FN_RATING, ID3FN_FILENAME, ID3FN_LANGUAGE, ID3FN_PICTURETYPE, ID3FN_IMAGEFORMAT, ID3FN_MIMETYPE, ID3FN_COUNTER, ID3FN_ID, ID3FN_VOLUMEADJ, ID3FN_NUMBITS, ID3FN_VOLCHGRIGHT, ID3FN_VOLCHGLEFT, ID3FN_PEAKVOLRIGHT, ID3FN_PEAKVOLLEFT, ID3FN_TIMESTAMPFORMAT, ID3FN_CONTENTTYPE, ID3FN_LASTFIELDID};
    char*       idesc[] = { "No field.", "Text encoding (unicode or ASCII).", "Text field.", "A URL.", "Data field.", "Description field.", "Owner field.", "Email field.", "Rating field.", "Filename field.", "Language field.", "Picture type field.", "Image format field.", "Mimetype field.", "Counter field.", "Identifier/Symbol field.", "Volume adjustment field.", "Number of bits field.", "Volume chage on the right channel.", "Volume chage on the left channel.", "Peak volume on the right channel.", "Peak volume on the left channel.", "SYLT Timestamp Format.", "SYLT content type.", "Last field placeholder."};


    int i=0;
	std::string buf = "";
    ID3_Frame* frame = 0;
    ID3_Field* field = 0;
    while(i<96)
     {
      if(i==2 || i==10) // picture and encapsulated object often fail
       {
        printf("%2i. Skipping\n", i);
        i++; continue;
       }

      try { frame = m_id3tag->Find(fids[i]); }
      catch(...)
       {
        printf("ERROR: SKIPPING FILE (i=%i)\n", i);
        Free();
        return;
       }

      if(frame==0) { i++; continue; } // Frame not found
      printf("%2i. %-35s : ", i, frame->GetDescription());

      buf = m_getNewString(fids[i]);
      if(buf == "")
       {
        printf("\"%s\"\n", buf);
		buf = "";
        frame = 0;
        i++; continue;
       }

      printf("No Text found, but:\n", buf);

      for(int k=1; k<24; k++)
       {
        try
         {
          if(frame->Contains(fiids[k]))
           { printf("%-43s- %s\n", "", idesc[k]); }
         }
        catch(...)
         {
          printf("%-42sERROR: SKIPPING FILE (i=%i; k=%i)\n", "", i, k);
          Free();
          return;
         }
       }
      printf("\n");
      frame = 0;
      i++; continue;
     }
    printf("\n");
   }



/* ---------------- protected member functions ---------------- */

    /**
      \brief   parses the genre of the file

      In ID3V1 Tags, the genre is a simple byte and needs to be
      converted to a string first, but ID3V2 allows arbitrary
      genre descriptions.
    **/
  void MP3FileInfo::m_parseGenre(void)
   {
    if(bHasV2Tag) // Use V2 if present
     {
      szGenre = m_getNewString(ID3FID_CONTENTTYPE);
      return;
     }

    char buf[18];
    if(!m_getString(ID3FID_CONTENTTYPE, buf))
     {
      return;
     }

    int gid = -1;
    sscanf(buf, "(%i)", &gid); // filter Genre ID

	if (gid > -1)
	{
		if(gid >= ID3_NR_OF_V1_GENRES) // genre ID too large
		{
		szGenre= buf;
		return;
		}

		
		szGenre =  ID3_V1GENRE2DESCRIPTION(gid);
	}
   }

    /// parses header infomation
  void MP3FileInfo::m_parseHeader(void)
   {
    assert(m_bOK);

    m_mp3header = m_id3tag->GetMp3HeaderInfo();
    if(!m_mp3header)
     {
      //printf("\n[Error]:\nGetMp3HeaderInfo() failed!\n(file: %s)\n\n", szFilename);
      Free();
      return;
     }
	char buff[100];

    szBitRate = m_mp3header->bitrate;

	if(m_mp3header->vbr_bitrate>0)
	{
		sprintf(buff, "%i kbit/s (VBR)", m_mp3header->vbr_bitrate/1000);
		szBitRate = buff;
	} else
	{
		sprintf(buff, "%i kbit/s", m_mp3header->bitrate/1000);
		szBitRate = buff;
	}
	
	sprintf(buff, "%i",m_mp3header->frequency);
    szSampleRate = buff;
	sprintf(buff, "%i",m_mp3header->time);
    szLength     = buff;

      // MPEG Layer
    switch(m_mp3header->layer)
     {
      case MPEGLAYER_UNDEFINED: szMpegLayer="UNDEFINED";break;
      case MPEGLAYER_III:       szMpegLayer="Layer III"; break;
      case MPEGLAYER_II:        szMpegLayer="Layer II";  break;
      case MPEGLAYER_I:         szMpegLayer="Layer I";   break;
      default:                  szMpegLayer = "";
     }

      // MPEG Version
    switch(m_mp3header->version)
     {
      case MPEGVERSION_2_5: szMpegVersion="MPEG 2.5"; break;
      case MPEGVERSION_2:   szMpegVersion="MPEG 2";   break;
      case MPEGVERSION_1:   szMpegVersion="MPEG 1";   break;
      default:              szMpegVersion = "";
     }

      // Channel Mode
    switch(m_mp3header->channelmode)
     {
      case MP3CHANNELMODE_STEREO:         szChannelMode="Stereo";         break;
      case MP3CHANNELMODE_JOINT_STEREO:   szChannelMode="Joint Stereo";   break;
      case MP3CHANNELMODE_DUAL_CHANNEL:   szChannelMode="Dual Channel";   break;
      case MP3CHANNELMODE_SINGLE_CHANNEL: szChannelMode="Single Channel"; break;
      default:                            szChannelMode = "";
     }
   }


    /// extracts a string
  bool MP3FileInfo::m_getString(ID3_FrameID fid, char* szBuffer)
   {
    assert(m_bOK);

    ID3_Frame* frame = m_id3tag->Find(fid); // frame
    if(frame==0)
     {
      sprintf(szBuffer, "[Not Found]");
      return false;
     }

    if(!frame->Contains(ID3FN_TEXTENC))
     {
      sprintf(szBuffer, "[Contains No Text]");
      return false;
     }

    ID3_Field* field = 0;
    if(frame->Contains(ID3FN_TEXT))
     { field = frame->GetField(ID3FN_TEXT); }

      // String is an URL
    if(frame->Contains(ID3FN_URL) && (field==0))
     { field = frame->GetField(ID3FN_URL); }

    if(field==0)
     {
      delete frame;
      sprintf(szBuffer, "[Error]");
      return false;
     }

    field->SetEncoding(ID3TE_ISO8859_1);
    const char* res = field->GetRawText(); // GetRawUnicodeText
    if(res==0)
     {
      delete frame;
      sprintf(szBuffer, "[Empty]");
      return false;
     }

    sprintf(szBuffer, res);
    return true;
   }


    /// extracts a string
  std::string MP3FileInfo::m_getNewString(ID3_FrameID fid)
   {
    assert(m_bOK);

    ID3_Frame* frame = m_id3tag->Find(fid); // find frame
    if(frame==0) { return ""; }

    if(!frame->Contains(ID3FN_TEXTENC))
     { delete frame; return ""; } // frame contains no text

      // look for field type
    ID3_Field* field = 0;
    if(frame->Contains(ID3FN_TEXT)) // Text field
     { field = frame->GetField(ID3FN_TEXT); }

    if(frame->Contains(ID3FN_URL) && (field==0)) // URL field
     { field = frame->GetField(ID3FN_URL); }

    if(field==0) { delete frame; return ""; } // no field found

    field->SetEncoding(ID3TE_ISO8859_1); // use Latin-1 charset

    const char* res = field->GetRawText(); // TODO: GetRawUnicodeText
	std::string buf = ""; // new string
    buf=res; // copy

    return buf;
   }
