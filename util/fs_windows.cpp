#include "fs.h"




#ifdef _WINDOWS

#include <windows.h>
#include <iostream>

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
			void *open( const Path &path, std::string mode )
			{
				HANDLE fileHandle;
				WinApiFile *retval;
				DWORD dwDesiredAccess = GENERIC_READ;
				DWORD creationDisposition = OPEN_EXISTING;

				if( mode == "w" )
				{
					dwDesiredAccess |= GENERIC_WRITE;
					creationDisposition = CREATE_ALWAYS;
				}

				fileHandle = CreateFile(path.c_str(), dwDesiredAccess, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, creationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);

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

			uint64 write( void *opaque, const void *buffer, unsigned int size, unsigned int count )
			{
				HANDLE Handle = ((WinApiFile *) opaque)->handle;
				DWORD CountOfBytesWritten;
				uint64 retval = 0;

				// Read data from the file
				// !!! FIXME: uint32 might be a greater # than DWORD
				if(!WriteFile(Handle, buffer, count * size, &CountOfBytesWritten, NULL))
				{
					// error
					DWORD ec = GetLastError();
					std::cout << ec << std::endl;
				}else
				{
					// Return the number of "objects" read.
					// !!! FIXME: What if not the right amount of bytes was read to make an object?
					retval = CountOfBytesWritten / size;
				}

				return(retval);
			}

			sint64 tell( void *opaque )
			{
				HANDLE handle = ((WinApiFile *) opaque)->handle;
				LARGE_INTEGER filepos;
				LARGE_INTEGER move;
				move.QuadPart = 0;

				// get current position
				if( !SetFilePointerEx( handle, move, &filepos, FILE_CURRENT) )
				{
					// error
				}

				return filepos.QuadPart;
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






