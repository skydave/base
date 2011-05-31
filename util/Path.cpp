//
//
//
#include "Path.h"
#include <vector>
    
#if !defined(_WINDOWS)
    #include <unistd.h>
    #include <stdio.h>
    #include <pwd.h>
    
    #define PATH_SEPARATOR_STR  "/"
    #define PATH_SEPARATOR_CHAR  '/'

#else
    #include <direct.h>
    #include <string.h>
    
    #define PATH_SEPARATOR_STR  "\\"
    #define PATH_SEPARATOR_CHAR  '\\'

#endif

#define TRACE(a, b)						// #define as required to do tracing
#define TRACE_WITH_LEVEL_MAX(a)		TRACE(a, 10 /*maximum tracing level? */)

namespace base
{
	#if defined(_WINDOWS)
	std::string Path::m_sTmpBuff;
	#endif

	Path::Path() : m_sPath("")
	{
	#if defined(WIN32) 
		ExtractDriveLetter();
	#endif
		CleanupPath();
	}

	Path::Path(LPCSTR sPath) :
		m_sPath(sPath)
	{
	#if defined(WIN32) 
		ExtractDriveLetter();
	#endif
		CleanupPath();
	}
	
	Path::Path(const std::string & sPath) :
		m_sPath(sPath)
	{
	#if defined(WIN32)
		ExtractDriveLetter();
	#endif
		CleanupPath();
	}
	
	Path::Path(const std::string & sPath,
				bool bCleanup) :
		m_sPath(sPath)
	{
	#if defined(_WINDOWS)
		ExtractDriveLetter();
	#endif
	
		if(bCleanup)
		{
			CleanupPath();
		}
	}
	
	Path Path::RetrieveHomePath()
	{			
		// use getenv() function to retrieve HOME path
		const char* pfx = getenv("HOME");
		
		#if !defined(_WINDOWS)
		if (!pfx)
		{
			// We're trying to expand ~/, but HOME isn't set!
			// lets get access to the password database and 
			// try to retrieve the HOME path that way...
			struct passwd * pw = getpwuid(getuid());
			
			if (pw)
				pfx = pw -> pw_dir;
		}
		#endif
	
		// create path object from HOME, but don't compress it!
		return Path(pfx ? pfx : "", false);		
	}
	
	#if defined(_WINDOWS)
	void Path::ExtractDriveLetter()
	{
		// extract the drive the path resides on...
		if(m_sPath.length() > 2 && m_sPath[1] == ':')
		{
			TCHAR cDriveLetter = '\0';
	
			if(sscanf(m_sPath.substr(0,2).c_str(), "%c:", &cDriveLetter) == 1)
			{
				m_sDrive = m_sPath.substr(0, 2);
				m_sPath = m_sPath.substr(2, m_sPath.length() - 1);
			}
		}
	}
	#endif
	
	void Path::CleanupPath()
	{
		// compress the path, and resolve ~
		bool bHasFSOnEnd = (m_sPath.length() > 1 && m_sPath[m_sPath.length() - 1] == PATH_SEPARATOR_CHAR);
		bool bHasFSOnStart = (m_sPath.length() > 0 && m_sPath[0] == PATH_SEPARATOR_CHAR);
		
		// create a copy of m_sPath.
		std::vector<TCHAR> pPath(m_sPath.length() + 1);
		strcpy(&pPath[0], m_sPath.c_str());
		
		// reset m_sPath as required.
		m_sPath = bHasFSOnStart ? PATH_SEPARATOR_STR : "";
		
		// split path into it tokens, using strtok which is NOT reentrant
		// so be careful!
		LPSTR p = strtok(&pPath[0], PATH_SEPARATOR_STR);
			
		while(p)
		{
			if(StrCmp(p, "~") == 0 &&		// check for ~
			p == &pPath[0] &&			// check that the ~ is the first character in the path
			RetrieveHomePath().IsValid())// check that we have been able to resolve the home path
			{
				// create new path with resolved home path
				Path homePath = RetrieveHomePath();
					
				m_sPath = homePath.m_sPath;
	
	#if defined(_WINDOWS)
				// drive letter must be updated
				m_sDrive = homePath.m_sDrive;
	#endif
			}
			else if(StrCmp(p, ".") == 0 &&	// check for .
					p == &pPath[0])			// check that the . is the first character in the path
			{                        
				// get current working directory
				TCHAR * pBuff = getcwd(NULL, 0);
				
				// create new path with resolved working directory
				Path workingPath = Path(pBuff != NULL ? pBuff : ".", false);
				
				if(!pBuff)
				{
					TRACE(("Unable to retrieve current working directory!  getwd() failed with error %s (%d)",
							strerror(errno),
							errno), 
							DEBUG_LEVEL_MIN);
				}
				else
				{
					free(pBuff);
				}
				
				m_sPath = workingPath.m_sPath;
				
	
	#if defined(_WINDOWS)
				// drive letter must be updated
				m_sDrive = workingPath.m_sDrive;
	#endif
			}
			else if(StrCmp(p, "..") == 0 && GetParent().IsValid())
			{
				// update path to parent path.
				m_sPath = GetParent().m_sPath;
			}
			else
			{
				if(m_sPath.length() > 0 && m_sPath[m_sPath.length() - 1] != PATH_SEPARATOR_CHAR)
				{
					m_sPath += PATH_SEPARATOR_CHAR;
				}
				
				m_sPath += p;
			}
			
			p = strtok(NULL, PATH_SEPARATOR_STR);
		}
		
		// put / on end as required
		if(bHasFSOnEnd && (m_sPath.length() > 0 ? (m_sPath[m_sPath.length() - 1] != PATH_SEPARATOR_CHAR) : true))
		{
			m_sPath += PATH_SEPARATOR_CHAR;
		}
	}
	
	bool Path::IsValid() const
	{
		return m_sPath.length() > 0 ? true : false;
	}
	
	std::string Path::GetSTLStr()
	{
	#if defined(_WINDOWS)
		return m_sDrive + m_sPath;
	#else
		return m_sPath;
	#endif
	}
	
	LPCSTR Path::c_str() const
	{
	#if defined(_WINDOWS)
		m_sTmpBuff = m_sDrive + m_sPath;
		return m_sTmpBuff.c_str();
	#else
		return m_sPath.c_str();
	#endif
	}
	
	UInt32 Path::length( void )
	{
	#if defined( _WINDOWS )
		return (UInt32) std::string( m_sDrive + m_sPath).length( );
	#else
		return (UInt32) m_sPath.length( );
	#endif
	}
		
	Path Path::GetParent()
	{
		if(m_sPath.length() == 0 || IsRoot())
		{
			// return empty path.
			return Path("");
		}
		
		// reverse find a / ignoring the end / if it exists.
		std::string::size_type nLength(m_sPath.length());
		std::string::size_type nOffset ((m_sPath[nLength - 1] == PATH_SEPARATOR_CHAR && nLength - 2 > 0) ? nLength - 2 : std::string::npos);
		std::string::size_type nPos (m_sPath.rfind(PATH_SEPARATOR_STR, nOffset));
		
		// create new path object given position of find / and return it.
		if(nPos != std::string::npos)
		{
	#if defined(_WINDOWS)
			return Path(m_sDrive + m_sPath.substr(0, nPos + 1), false);
	#else
			return Path(m_sPath.substr(0, nPos + 1), false);
	#endif
		}
		else
		{
			// not parent path avaliable, return an empty path.
			return Path("");
		}    
	}
	
	Path Path::GetParentAtDepthN(UInt32 nDepth)
	{    
		if(!IsValid() ||
		nDepth >= Depth())
		{
			return *this;
		}
			
		// create Path object to parent object at depth N relative to this
		// path object.
		bool bHasFSOnStart = (m_sPath.length() > 0 && m_sPath[0] == PATH_SEPARATOR_CHAR);
		
		// create a copy of m_sPath.
		std::vector<TCHAR> pPath(m_sPath.length() + 1);
		strcpy(&pPath[0], m_sPath.c_str());
		
		// reset m_sPath as required.
		std::string sTmpPath = bHasFSOnStart ? PATH_SEPARATOR_STR : "";
		
		// split path into it tokens.
		LPSTR p = strtok(&pPath[0], PATH_SEPARATOR_STR);
			
		while(p && nDepth > 0)
		{
			if(sTmpPath.length() > 0 && sTmpPath[sTmpPath.length() - 1] != PATH_SEPARATOR_CHAR)
			{
				sTmpPath += PATH_SEPARATOR_CHAR;
			}
			
			sTmpPath += p;
			--nDepth;
			
			p = strtok(NULL, PATH_SEPARATOR_STR);
		}
		
		// put / on end as required
		if(sTmpPath.length() > 0 ? (sTmpPath[sTmpPath.length() - 1] != PATH_SEPARATOR_CHAR) : true)
		{
			sTmpPath += PATH_SEPARATOR_CHAR;
		}
	
	#if defined(_WINDOWS)
		return Path(m_sDrive + sTmpPath, false);
	#else
		return Path(sTmpPath, false);
	#endif
	}
	
	std::string Path::GetBaseFileName()
	{
		if(m_sPath.length() == 0)
		{
			// return empty name.
			return "";
		}
		
		// reverse find a / ignoring the end / if it exists.
		std::string::size_type nLength(m_sPath.length());
		std::string::size_type nOffset((m_sPath[nLength - 1] == PATH_SEPARATOR_CHAR && nLength - 2 > 0) ? nLength - 2 : std::string::npos);
		std::string::size_type nPos (m_sPath.rfind(PATH_SEPARATOR_STR, nOffset));
		
		// extract filename given position of find / and return it.
		if(nPos != std::string::npos)
		{
			return m_sPath.substr(nPos + 1, nLength - (nPos + 1) - (nOffset != std::string::npos ? 1 : 0));
		}
		else
		{
			return m_sPath;
		} 
	}
	
	std::string Path::GetBaseFileTitle()
	{
		if(!IsValid())
		{
			return "";
		}
		
		std::string sTmpStr = GetBaseFileName();
		std::string::size_type pos = sTmpStr.rfind('.');
		
		if(pos != std::string::npos)
		{
			// remove extension.
			sTmpStr = sTmpStr.substr(0, pos);
		}
		
		return sTmpStr;
	}
	
	std::string Path::GetBaseFileExtension()
	{
		if(!IsValid())
		{
			return "";
		}
		
		std::string sTmpStr = GetBaseFileName();
		
		// make sure there is no / on the end.
		if(sTmpStr[sTmpStr.length() - 1] == PATH_SEPARATOR_CHAR)
		{
			sTmpStr = sTmpStr.substr(0, sTmpStr.length() - 1);
		}
				
		std::string::size_type pos = sTmpStr.rfind('.');
		
		if(pos != std::string::npos)
		{
			// extract extension.
			return sTmpStr.substr(pos + 1, sTmpStr.length() - (pos + 1));
		}
		else
		{
			// no extension!
			return "";
		}
	}
	
	bool Path::IsRoot()
	{
		if(!IsValid())
		{
			return false;
		}
		
		return m_sPath == PATH_SEPARATOR_STR ? true : false;
	}
	
	UInt32 Path::Depth()
	{
		std::string::size_type nLength(m_sPath.length());
		UInt32 nDepth(0);
		
		for(std::string::size_type i = 0; i < nLength; i++)
		{
			if(m_sPath[i] == PATH_SEPARATOR_CHAR)
			{
				++nDepth;
			}
		}
		
		if(nDepth > 0 &&
		nLength > 0 &&
		m_sPath[nLength - 1] == PATH_SEPARATOR_CHAR)
		{
			// PATH_SEPARATOR_CHAR on the end, reduce count by 1
			--nDepth;
		}
		
		TRACE_WITH_LEVEL_MAX(("Depth of path %s is %lu", m_sPath.c_str(), nDepth));
		
		return nDepth;
	}
	
	Path Path::Root()
	{
	#if defined(_WINDOWS)
		return Path((m_sDrive.length() ? (m_sDrive + PATH_SEPARATOR_STR) : ""));
	#else
		return Path(PATH_SEPARATOR_STR);
	#endif
	}
	
	bool Path::operator == (const Path & rhs)
	{
		if(!IsValid() && !rhs.IsValid())
		{
			// both paths are invalid!
			return true;
		}
	
		if(IsValid() != rhs.IsValid())
		{
			// one of the paths is valid, whilst the other is not!
			return false;
		}
		
		std::string sTmpStr[2] =
		{
	#if defined(WIN32)
			m_sDrive + m_sPath,
			rhs.m_sDrive + rhs.m_sPath
	#else
			m_sPath,
			rhs.m_sPath
	#endif	    
		};
		
		// make sure both paths have a / on the end.
		if(sTmpStr[0][sTmpStr[0].length() - 1] != PATH_SEPARATOR_CHAR)
		{
			sTmpStr[0] += PATH_SEPARATOR_CHAR;
		}
		
		if(sTmpStr[1][sTmpStr[1].length() - 1] != PATH_SEPARATOR_CHAR)
		{
			sTmpStr[1] += PATH_SEPARATOR_CHAR;
		}
		
		return sTmpStr[0] == sTmpStr[1];
	}
	
	bool Path::operator == (LPCSTR rhs)
	{
		return (*this == Path(rhs));
	}
	
	bool Path::operator == (const std::string & rhs)
	{
		return (*this == Path(rhs));
	}
	
	bool Path::operator != (const Path & rhs)
	{
		bool bRetValue = (*this == rhs);
		return !bRetValue;
	}
	
	bool Path::operator != (LPCSTR rhs)
	{
		bool bRetValue = (*this == Path(rhs));
		return !bRetValue;
	}
	
	bool Path::operator != (const std::string & rhs)
	{
		bool bRetValue = (*this == Path(rhs));
		return !bRetValue;
	}
	
	Path Path::operator + (const Path & rhs)
	{
		if(!IsValid() && !rhs.IsValid())
		{
			// return this invalid path.
			return *this;
		}
		
		if(IsValid() && !rhs.IsValid())
		{
			// return this valid path
			return *this;
		}
		
		if(!IsValid() && rhs.IsValid())
		{
			// return the rhs valid path.
			return rhs;
		}
		
		// both paths are valid...
		std::string sTmpStr[2] =
		{
	#if defined(_WINDOWS)
			m_sDrive + m_sPath,
			rhs.m_sPath,
	#else
			m_sPath,
			rhs.m_sPath
	#endif
		};
	
		// make sure sTmpStr has a / on the end.
		if(sTmpStr[0][sTmpStr[0].length() - 1] != PATH_SEPARATOR_CHAR)
		{
			sTmpStr[0] += PATH_SEPARATOR_CHAR;
		}
			
		// make sure rhs path has NOT got a / at the start.
		if(sTmpStr[1][0] == PATH_SEPARATOR_CHAR)
		{
			sTmpStr[1] = sTmpStr[1].substr(1, sTmpStr[1].length() - 1);
		}
		
		// create a new path object with the two path strings appended together.
		return Path(sTmpStr[0] + sTmpStr[1], false);
	}
	
	Path Path::operator + (LPCSTR rhs)
	{
		return (*this + Path(rhs));
	}
	
	Path Path::operator + (const std::string & rhs)
	{
		return (*this + Path(rhs));
	}
	
	Path & Path::operator += (const Path & rhs)
	{
		if(!IsValid() && !rhs.IsValid())
		{
			// return this invalid object
			return *this;
		}
		
		if(IsValid() && !rhs.IsValid())
		{
			// return this valid object
			return *this;
		} 
	
		if(!IsValid() && rhs.IsValid())
		{
			// set this object to be the same as the rhs
			m_sPath = rhs.m_sPath;
			
			// return this now valid object
			return *this;
		}
		
		// both paths are valid...
		std::string sTmpStr[2] =
		{
			m_sPath,
			rhs.m_sPath
		};
	
		// make sure sTmpStr has a / on the end.
		if(sTmpStr[0][sTmpStr[0].length() - 1] != PATH_SEPARATOR_CHAR)
		{
			sTmpStr[0] += PATH_SEPARATOR_CHAR;
		}
			
		// make sure rhs path has NOT got a / at the start.
		if(sTmpStr[1][0] == PATH_SEPARATOR_CHAR)
		{
			sTmpStr[1] = sTmpStr[1].substr(1, sTmpStr[1].length() - 1);
		}
		
		// create new path string.
		m_sPath = sTmpStr[0] + sTmpStr[1];
		
		// return this object.
		return *this;
	}
	
	Path & Path::operator += (LPCSTR rhs)
	{
		return (*this += Path(rhs));
	}
	
	Path & Path::operator += (const std::string & rhs)
	{
		return (*this += Path(rhs));
	}
} // namespace base

