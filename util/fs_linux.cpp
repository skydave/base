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
			void *open( const Path &path )
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

			unsigned long size( void *opaque )
			{
				int fd = *((int *) opaque);
				struct stat statbuf;
				fstat(fd, &statbuf);
				return (unsigned long) statbuf.st_size;
			}

			unsigned long read( void *opaque, void *buffer, unsigned int size, unsigned int count )
			{
				int fd = *((int *) opaque);
				int max = size * count;
				int rc = ::read(fd, buffer, max);

				if ((rc < max) && (size > 1))
					lseek(fd, -(rc % size), SEEK_CUR); // rollback to object boundary.

				return(rc / size);
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






