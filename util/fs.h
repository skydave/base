#pragma once
#include "Path.h"
#include "types.h"
#include <vector>
#include <set>
#include <map>




namespace base
{
	namespace fs
	{
		struct Options
		{
			static                                         void setFileLogging( bool enabled );
			static                                                          bool fileLogging();
			static                                            void logFile( std::string path );
			static void                            getFileLog( std::vector<std::string> &out );

			static void                                     setFileRedirection( bool enabled );
			static void          redirectFile( const std::string &in, const std::string &out );
			static std::string                               realPath( const std::string &in );
		private:
			static bool                                                             m_logFiles; // keep track of which files have been accessed
			static bool                                                  m_useRedirectionTable; // use hashmap to look up real file position
			static std::set<std::string>                                             m_fileLog;
			static std::map<std::string, std::string>                   m_fileRedirectionTable;
		};
		typedef struct File
		{
			void *opaque;
		} File;

		std::string                                                                  getCWD();
		bool                                                       exists( const Path &path );
		std::string                                                  read( const Path &path ); // returns contenst of path specified as string (convinience)
		void                                             read( const Path &path, char **ptr ); // allocates memory and reads file content into given ptr
		File                                *open( const Path &path, std::string mode = "r" );
		void                                                              close( File *file );
		uint64                                                             size( File *file ); // returns size of file in byte
		uint64        read( File *file, void *buffer, unsigned int size, unsigned int count );
		uint64 write( File *file, const void *buffer, unsigned int size, unsigned int count );
		bool                                                                eof( File *file ); // returns true if end of file is reached
		sint64                                                             tell( File *file ); // returns current position
		std::string                                                     getLine( File *file ); // convenience
		void                     getLines( const Path &path, std::vector<std::string> &lines); // convenience
		void                                      write( File *file, const std::string line ); // convinience



		//
		// platform functionality - implemented in fs_<platform>.cpp
		//
		namespace platform
		{
			bool exists( const Path &path );
			void *open( const Path &path, std::string mode = "r" );
			void close( void *opaque );
			uint64 size( void *opaque );
			uint64 read( void *opaque, void *buffer, unsigned int size, unsigned int count );
			uint64 write( void *opaque, const void *buffer, unsigned int size, unsigned int count );
			sint64 tell( void *opaque );
			//std::string getLine( void *opaque );
		}
	}

}
