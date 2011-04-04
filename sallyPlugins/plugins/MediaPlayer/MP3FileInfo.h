#pragma warning(disable: 4996)

#ifndef MP3FILEINFO_H
#define MP3FILEINFO_H
#define ID3LIB_LINKOPTION 3
#include <id3/tag.h>
#pragma comment(lib, "id3lib.lib")

  class MP3FileInfo
   {
    public:
      inline MP3FileInfo(void) :
        m_bOK(true),
        m_id3tag(0),
        m_mp3header(0),

          // Fileinfo
		  szFilename(""),
        bHasLyrics(false), bHasV1Tag(false), bHasV2Tag(false),
          // Headerinfo
		szCbrBitRate(""),szVbrBitRate(""), szBitRate(""), szMpegLayer(""), szMpegVersion(""), szChannelMode(""),

          // ID3V1 Tag elements
        szSampleRate(""),szLength(""), szArtist(""), szAlbum(""), szGenre(""), szTitle(""), szYear(""), szComment(""), szTrack(""),

          // ID3V2 Tag elements
        szComposer(""), szCopyright(""), szEncodedBy(""), szOriginalArtist(""), szURL(""), szBand("")
       {
        // nothing to do; use Init() instead
       }

      virtual ~MP3FileInfo(void) { Free(); }

      bool Init(const std::string& cszFilename);
      void Free(void);
      inline bool isOK(void) const { return m_bOK; }

	  // Member Getter and Setters

	  std::string GetSzBitRate() const { return szBitRate; }
	  void SetSzBitRate(const std::string& val) { szBitRate = val; }

	  std::string GetSzMpegLayer() const { return szMpegLayer; }
	  void SetSzMpegLayer(std::string& val) { szMpegLayer = val; }

	  std::string GetSzMpegVersion() const { return szMpegVersion; }
	  void SetSzMpegVersion(std::string& val) { szMpegVersion = val; }

	  std::string GetSzChannelMode() const { return szChannelMode; }
	  void SetSzChannelMode(std::string& val) { szChannelMode = val; }

	  std::string GetSzArtist() const { return szArtist; }
	  void SetSzArtist(std::string& val) { szArtist = val; }

	  std::string GetSzTitle() const { return szTitle; }
	  void SetSzTitle(std::string& val) { szTitle = val; }

	  std::string GetSzAlbum() const { return szAlbum; }
	  void SetSzAlbum(std::string& val) { szAlbum = val; }

	  std::string GetSzComment() const { return szComment; }
	  void SetSzComment(std::string& val) { szComment = val; }

	  std::string GetSzTrack() const { return szTrack; }
	  void SetSzTrack(std::string& val) { szTrack = val; }

	  std::string GetSzYear() const { return szYear; }
	  void SetSzYear(std::string& val) { szYear = val; }

	  std::string GetSzGenre() const { return szGenre; }
	  void SetSzGenre(std::string& val) { szGenre = val; }

	  std::string GetSzComposer() const { return szComposer; }
	  void SetSzComposer(std::string& val) { szComposer = val; }

	  std::string GetSzCopyright() const { return szCopyright; }
	  void SetSzCopyright(std::string& val) { szCopyright = val; }

	  std::string GetSzEncodedBy() const { return szEncodedBy; }
	  void SetSzEncodedBy(std::string& val) { szEncodedBy = val; }

	  std::string GetSzOriginalArtist() const { return szOriginalArtist; }
	  void SetSzOriginalArtist(std::string& val) { szOriginalArtist = val; }

	  std::string GetSzURL() const { return szURL; }
	  void SetSzURL(std::string& val) { szURL = val; }

	  std::string GetSzCbrBitRate() const { return szCbrBitRate; }
	  void SetSzCbrBitRate(std::string& val) { szCbrBitRate = val; }

	  std::string GetSzVbrBitRate() const { return szVbrBitRate; }
	  void SetSzVbrBitRate(std::string& val) { szVbrBitRate = val; }

	  std::string GetSzSampleRate() const { return szSampleRate; }
	  void SetSzSampleRate(std::string& val) { szSampleRate = val; }

	  std::string GetSzLength() const { return szLength; }
	  void SetSzLength(std::string& val) { szLength = val; }

	  std::string GetSzFilename() const { return szFilename; }
	  void SetSzFilename(std::string& val) { szFilename = val; }

	  std::string GetSzBand() const { return szBand; }
	  void SetSzBand(std::string& val) { szBand = val; }


      /* Fileinfo */
      
      bool  bHasLyrics, bHasV1Tag, bHasV2Tag;
      int   nFileSize;

   

      void  ShowAllFrames(void);

	  void	Update();

   private:
	      /* Headerinfo */
      //int nCbrBitRate, nVbrBitRate, nBitRate, nSampleRate, nLength;int nTrack;	  int nYear;
      std::string szBitRate;
	  std::string szMpegLayer; 
	  std::string szMpegVersion;	 
	  std::string szChannelMode;
	  std::string szCbrBitRate;
	  std::string szVbrBitRate;
	  std::string szSampleRate;
	  std::string szLength;


      /* V1 up */
      std::string szArtist;
	  std::string szTitle;
	  std::string szAlbum;
	  std::string szComment;
	  std::string szTrack; 
      std::string szYear; 
      std::string szGenre;

      /* V2 only */
      std::string szComposer;
	  std::string szCopyright;
	  std::string szEncodedBy;
	  std::string szOriginalArtist;
	  std::string szURL;
	  std::string szBand;

	  std::string szFilename;

    protected:
      bool m_bOK;

      ID3_Tag* m_id3tag;
      const Mp3_Headerinfo* m_mp3header;

      void m_parseFile(void);
      void m_parseHeader(void);
      void m_parseGenre(void);

      bool  m_getString(ID3_FrameID fid, char* szBuffer);
	  void	m_setNewString(ID3_FrameID fid, const std::string& value);
      std::string m_getNewString(ID3_FrameID fid);
   };



#endif // MP3FILEINFO_H