#include "fs.h"




#ifdef _WINDOWS

#include <windows.h>

namespace base
{
	namespace fs
	{
		namespace platform
		{
			struct WinApiFile
			{
				HANDLE handle;
				int readonly;
			};

			bool exists( const Path &path )
			{
				if(GetFileAttributes(path.c_str()) == 0xFFFFFFFF  )
					return false;

				return true;
			}
			void *open( const Path &path )
			{
				HANDLE fileHandle;
				WinApiFile *retval;

				fileHandle = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

				retval = new WinApiFile;
				//retval->readonly = rdonly;
				retval->handle = fileHandle;
				return(retval);
			}
			void close( void *opaque )
			{
				WinApiFile *fd = (WinApiFile *)opaque;
				CloseHandle(fd->handle);
				delete fd;
			}

			uint64 size( void *opaque )
			{
				HANDLE Handle = ((WinApiFile *) opaque)->handle;
				DWORD SizeHigh;
				DWORD SizeLow;
				uint64 retval;

				SizeLow = GetFileSize(Handle, &SizeHigh);
				if ( (SizeLow == 0xFFFFFFFF ) && (GetLastError() != NO_ERROR) )
				{
					// error
				}else
				{
					// Combine the high/low order to create the 64-bit position value
					retval = (((uint64) SizeHigh) << 32) | SizeLow;
				}

				return(retval);
			}

			uint64 read( void *opaque, void *buffer, unsigned int size, unsigned int count )
			{
				HANDLE Handle = ((WinApiFile *) opaque)->handle;
				DWORD CountOfBytesRead;
				uint64 retval;

				// Read data from the file
				// !!! FIXME: uint32 might be a greater # than DWORD
				if(!ReadFile(Handle, buffer, count * size, &CountOfBytesRead, NULL))
				{
					// error
				}else
				{
					// Return the number of "objects" read.
					// !!! FIXME: What if not the right amount of bytes was read to make an object?
					retval = CountOfBytesRead / size;
				}

				return(retval);
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






