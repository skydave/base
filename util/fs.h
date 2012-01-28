#pragma once
#include "Path.h"
#include "types.h"




namespace base
{
	namespace fs
	{
		typedef struct File
		{
			void *opaque;
		} File;

		bool                                                       exists( const Path &path );
		std::string                                                  read( const Path &path ); // returns contenst of path specified as string
		File                                                        *open( const Path &path );
		void                                                              close( File *file );
		uint64                                                             size( File *file ); // returns size of file in byte
		uint64        read( File *file, void *buffer, unsigned int size, unsigned int count );
		bool                                                                eof( File *file ); // returns true if end of file is reached
		sint64                                                             tell( File *file ); // returns current position
		std::string                                                     getLine( File *file ); // convenience



		//
		// platform functionality - implemented in fs_<platform>.cpp
		//
		namespace platform
		{
			bool exists( const Path &path );
			void *open( const Path &path );
			void close( void *opaque );
			uint64 size( void *opaque );
			uint64 read( void *opaque, void *buffer, unsigned int size, unsigned int count );
			sint64 tell( void *opaque );
			//std::string getLine( void *opaque );
		}
	}

}
