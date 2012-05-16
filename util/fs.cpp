#include "fs.h"
#include <direct.h>
#include <iostream>
#include <sstream>

#include "StringManip.h"







namespace base
{
	namespace fs
	{
		// ==============================
		// Options
		// ==============================

		static Options g_options;

		bool Options::m_logFiles = false;
		std::set<std::string> Options::m_fileLog;
		bool Options::m_useRedirectionTable = false;
		std::map<std::string, std::string> Options::m_fileRedirectionTable;


		void Options::setFileLogging( bool enabled )
		{
			g_options.m_logFiles = enabled;
		}

		bool Options::fileLogging()
		{
			return g_options.m_logFiles;
		}

		void Options::logFile( std::string path )
		{
			g_options.m_fileLog.insert(path);
		}

		void Options::getFileLog( std::vector<std::string> &out )
		{
			out.clear();
			for( std::set<std::string>::iterator it = g_options.m_fileLog.begin(); it != g_options.m_fileLog.end(); ++it )
				out.push_back(*it);
		}



		void Options::setFileRedirection( bool enabled )
		{
			g_options.m_useRedirectionTable = enabled;
		}

		void Options::redirectFile( const std::string &in, const std::string &out )
		{
			g_options.m_fileRedirectionTable[in] = out;
		}

		std::string Options::realPath( const std::string &in )
		{
			if( g_options.m_useRedirectionTable )
			{
				std::map<std::string, std::string>::iterator it = g_options.m_fileRedirectionTable.find(in);
				if( it != g_options.m_fileRedirectionTable.end() )
					return it->second;
			}
			return in;
		}


		// ==============================
		//
		// ==============================

		std::string getCWD()
		{
			std::string result = "";
			char* buffer;

			// Get the current working directory: 
			if( (buffer = _getcwd( NULL, 0 )) == NULL )
				perror( "_getcwd error" );
			else
			{
				result = std::string(buffer, strnlen(buffer, 2000));
				free(buffer);
			}

			return result;
		}


		bool exists( const Path &path )
		{
			return platform::exists(Options::realPath(path.str()));
		}

		// returns contenst of file at path specified as string
		std::string read( const Path &path )
		{
			if( !exists( path ) )
				return "";
			File *f = open( path );
			if(!f)
				return "";
			uint64 l = size(f);
			char *buffer = (char*)malloc( l*sizeof(char) );
			read( f, buffer, sizeof(char), (unsigned int)l );
			close(f);
			std::string result( buffer, l );
			free( buffer );
			return result;
		}

		// allocates memory and reads file content into given ptr
		void read( const Path &path, char **ptr )
		{
			*ptr = 0;
			if( !exists( path ) )
				return;
			File *f = open( path );
			if(!f)
				return;
			uint64 l = size(f);
			*ptr = (char*)malloc( l*sizeof(char) );
			read( f, *ptr, sizeof(char), (unsigned int)l );
			close(f);
		}

		File *open( const Path &path )
		{
			if( Options::fileLogging() )
				Options::logFile(path.str());
			File *f = 0;
			void *opaque = platform::open(Options::realPath(path.str()));
			if(opaque)
			{
				f = new File();
				f->opaque = opaque;
			}
			return f;
		}

		void close( File *file )
		{
			if( file )
			{
				platform::close(file->opaque);
				delete file;
			}
		}

		uint64 size( File *file )
		{
			if( file )
			{
				return platform::size(file->opaque);
			}
			return 0;
		}

		uint64 read( File *file, void *buffer, unsigned int size, unsigned int count )
		{
			if( file )
			{
				return platform::read(file->opaque, buffer, size, count);
			}
			return 0;
		}

		// returns true if end of file is reached
		bool eof( File *file )
		{
			sint64 filepos;
			bool retval = false;

			// get the current position in the file
			if ((filepos = platform::tell(file->opaque)) != 0)
			{
				// non-zero if eof is equal to the file length
				retval = filepos == platform::size(file->opaque);
			}

			return(retval);
		}

		std::string getLine( File *f )
		{
			std::stringstream os;
			char c;
			while( (platform::read( f->opaque, &c, 1, 1 ) == 1) && (c != '\n') )
			{
				os << c;
			}
			return os.str();
		}

		// convenience
		void getLines( const Path &path, std::vector<std::string> &lines)
		{
			lines.clear();
			std::string content = read( path );
			splitString( content, lines, "\n", false);
		}
	}




}
