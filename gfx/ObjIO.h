//
//
// ObjIO offers import and export support for obj files.
//
// Obj files are very simple ascii based mesh description files which store
// vertices, normals and texturecoordinates of n-polygonal meshes.
// The format was originally developed by AliasWavefront a long time ago.
// Despite being a little out of age it is very well known and supported
// throught the industry.
//
//
#pragma once
#include <util/Path.h>
#include "Geometry.h"

namespace base
{
	GeometryPtr importObj( const Path &filename ); ///< reads and creates a GeometryPtr from the given *.obj file. returns 0 if it fails
	//void exportToObjFile( Mesh *mesh, std::string filename ); ///< creates and writes a Obj file from the given mesh
}
