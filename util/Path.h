//
//
//
//
#pragma once
#include <string>


namespace base
{
// generic typedefs
typedef unsigned long UInt32;

//#if !defined(_WINDOWS)
	// typedefs and #defines required on Unix platform since not compiling on Windows platform, 
	typedef const char * 	LPCSTR;
	typedef char * 			LPSTR;
	typedef char			TCHAR;
	
	//#if !defined(TEXT)
	//	#define TEXT(txt)			txt
	//#endif
	
	#if !defined(_T)
		#define _T(txt)				TEXT(txt)
	#endif
	
	#define StrCmp(a, b)		strcmp(a, b)
//#endif



	//
	//
	//
	class Path
	{
	private:
		std::string m_sPath; /// the string version of the represented BSD path
    
	#if defined(_WINDOWS)
		std::string m_sDrive; /// the drive the path resides on.  (will always be empty on UNIX systems)
		static std::string m_sTmpBuff; /// internal temp buffer so we can return a LPCSTR via the c_str() method.
	#endif

	public:
		// constructor / destructor
		Path();
    
		/**
		Construct a Path object with an existing BSD path string
		*/
		Path(/// fully resolved BSD path
			LPCSTR sPath);
    
		/**
		Construct a Path object with an existing BSD path string
		*/
		Path(/// fully resolved BSD path
			const std::string & sPath);
    
		/**
		Is the path valid?\n
		Returns true if so, false otherwise
		*/
		bool IsValid() const;
    
		/**
		Return a copy of the internal stl string
		*/
		std::string str() const;
    
		/**
		Return the path as a LPCSTR
		*/
		LPCSTR c_str() const;
    
		/**
			* Return length of path string
			*/
		UInt32 length( void );

		/**
		create a new path that is the direct parent of this one
		*/
		Path GetParent();
    
		/**
		return a new path object which is a parent path of the current path up to the specified depth.
		If the specifed depth is >= to the current path's depth, then a copy of the current path object is returned.\n
		\n
		ie. Path("/lev1/lev2/lev3/lev4/lev5").GetParentAtDepthN(3).c_str()\n
		returns\n
		"/lev1/lev2/lev3/"
		*/
		Path GetParentAtDepthN(UInt32 nDepth);
    
		/**
		Return then name of the bottom most level path entry (includes any extension)
		*/
		std::string GetBaseFileName();
    
		/**
		retrieve the paths bottom level filename with out the extension
		*/
		std::string GetBaseFileTitle();
    
		/**
		retrieve the paths extension (if it has one) does not include the dot
		*/
		std::string GetBaseFileExtension();
    
		/**
		function returns true if the current path is the root directory, otherwise it returns false.
		*/
		bool IsRoot();
    
		/**
		Return the current paths' depth\n\n
		ie. "/usr/some directory with spaces in it/hello"\n
		returns a depth value of 3
		*/
		UInt32 Depth();
    
		/**
		Return the root path
		*/
		Path Root();

		/**
		operator == to compare file path objects
		*/
		bool operator == (const Path & rhs);
    
		/**
		operator == to compare this path object to a LPSTR path
		*/
		bool operator == (LPCSTR rhs);
    
		/**
		operator == to compare this path object to a std::string path
		*/
		bool operator == (const std::string & rhs);


        
		/**
		operator != to compare file path objects
		*/
		bool operator != (const Path & rhs);
    
		/**
		operator != to compare this path object to a LPSTR path
		*/
		bool operator != (LPCSTR rhs);
    
		/**
		operator != to compare this path object to a std::string path
		*/
		bool operator != (const std::string & rhs);
    
    
    
		/**
		operator + to add two path objects together to produce a new path object
		*/
		Path operator + (const Path & rhs);
    
		/**
		operator + to add a path object to a LPSTR path to produce a new path object
		*/
		Path operator + (LPCSTR rhs);
    
		/**
		operator + to add a path object to a std::string path to produce a new path object
		*/
		Path operator + (const std::string & rhs);
        
	    
    
		/**
		operator += to append another path to this path object
		*/
		Path & operator += (const Path & rhs);
    
		/**
		operator += to append a LPSTR path to this path object
		*/
		Path & operator += (LPCSTR rhs);
    
		/**
		operator += to append a std::string path to this path object
		*/
		Path & operator += (const std::string & rhs);

		// implicit conversion operator - take care
		operator const char* ( ) { return this->c_str(); }
        
	private:
    
		/**
		Special private constructor to create a path object with a BSD path string
		*/
		Path(/// fully resolved BSD path
			const std::string & sPath,
			/// clean up the path?
			bool bCleanup);
	
		/**
		Static function that can be used to retrieve the current users HOME path
		*/
		static Path RetrieveHomePath();
    
		/**
		This function cleans up the path string used to construct the Path object:\n\n
		1. Get rid of multiple / 's one after the other...\n
		\n
		ie. "///usr/bin///hello/////there//\n
		becomes\n
		/usr/bin/hello/there/\n
		\n
		2. Resolve any ~ directory found to the current users HOME path!
		*/
		void CleanupPath();

	#if defined(_WINDOWS)
		/**
		This function extract the drive letter in the path string used to construct the Path object
		*/
		void ExtractDriveLetter();
	#endif
	};



} // namespace base
