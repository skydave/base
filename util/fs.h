#pragma once
#include "Path.h"





namespace base
{
	namespace fs
	{
		typedef struct File
		{
			void *opaque;
		} File;

		bool                                                       exists( const Path &path );
		File                                                        *open( const Path &path );
		void                                                              close( File *file );
		unsigned long                                                      size( File *file ); // returns size of file in byte
		unsigned long read( File *file, void *buffer, unsigned int size, unsigned int count );



		//
		// platform functionality - implemented in fs_<platform>.cpp
		//
		namespace platform
		{
			bool exists( const Path &path );
			void *open( const Path &path );
			void close( void *opaque );
			unsigned long size( void *opaque );
			unsigned long read( void *opaque, void *buffer, unsigned int size, unsigned int count );
		}
	}

}
