#include "fs.h"




#ifdef linux

#include <sys/stat.h>
#include <fcntl.h>

namespace base
{
	namespace fs
	{
		namespace platform
		{
			bool exists( const Path &path )
			{
				struct stat info;
				return (stat( path.c_str(), &info) == 0);
			}
			void *open( const Path &path, std::string mode )
			{
				int fd;
				fd = ::open(path.c_str(), O_RDONLY, S_IRUSR | S_IWUSR);

				int *op = (int*) malloc( sizeof(int) );
				*op = fd;

				return op;
			}
			void close( void *opaque )
			{
				int fd = *((int *)opaque);
				::close(fd);
				free(opaque);
			}

			uint64 size( void *opaque )
			{
				int fd = *((int *) opaque);
				struct stat statbuf;
				fstat(fd, &statbuf);
				return (unsigned long) statbuf.st_size;
			}

			uint64 read( void *opaque, void *buffer, unsigned int size, unsigned int count )
			{
				int fd = *((int *) opaque);
				int max = size * count;
				int rc = ::read(fd, buffer, max);

				if ((rc < max) && (size > 1))
					lseek(fd, -(rc % size), SEEK_CUR); // rollback to object boundary.

				return(rc / size);
			}


			sint64 tell( void *opaque )
			{
				int fd = *((int *) opaque);
				sint64 retval;
				retval = (sint64) lseek(fd, 0, SEEK_CUR);
				return retval;
			}

		}







		 /*
		  bool isDirectory( const std::string &path )
		  {
			  struct stat info;
			  if(stat( path.c_str(), &info) != 0)
				  return false;
			  return S_ISDIR(info.st_mode);
		  }
		  */
	}
}




#endif






