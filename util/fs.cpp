#include "fs.h"
#include <iostream>
#include <sstream>







namespace base
{
	namespace fs
	{
		bool exists( const Path &path )
		{
			return platform::exists(path);
		}

		File *open( const Path &path )
		{
			File *f = 0;
			void *opaque = platform::open(path);
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
			return platform::eof(file->opaque);
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
	}




}
