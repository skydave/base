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
#include "ObjIO.h"
#include <util/fs.h>




namespace base
{
	//< reads and creates a GeometryPtr from the given *.obj file. returns 0 if it fails
	GeometryPtr importObj( const Path &filename )
	{
		//GeometryPtr result = GeometryPtr(new Geometry(Geometry::QUAD));

		AttributePtr positions = Attribute::createVec3f();

		/*
		positions->appendElement( math::Vec3f(-.5f,-.5f,0.5f) );
		positions->appendElement( math::Vec3f(-.5f,0.5f,0.5f) );
		positions->appendElement( math::Vec3f(.5f,.5f,0.5f) );
		positions->appendElement( math::Vec3f(.5f,-.5f,0.5f) );

		positions->appendElement( math::Vec3f(-.5f,-.5f,-0.5f) );
		positions->appendElement( math::Vec3f(-.5f,0.5f,-0.5f) );
		positions->appendElement( math::Vec3f(.5f,.5f,-0.5f) );
		positions->appendElement( math::Vec3f(.5f,-.5f,-0.5f) );

		result->setAttr( "P", positions);

		result->addQuad( 0, 1, 2, 3 );
		result->addQuad( 7, 6, 5, 4 );
		result->addQuad( 3, 2, 6, 7 );
		result->addQuad( 0, 4, 5, 1 );
		result->addQuad( 2, 1, 5, 6 );
		result->addQuad( 0, 3, 7, 4 );
		*/

		//return result;
		// temporary vertex and index buffer
		std::vector<math::Vec3f> vertices;
		std::vector<int> indicees;


		File *file = fs::open( filename );

		// file open?
		if( !file )
			// error file could not be opened
			return GeometryPtr();

/*
		while(?)
		{
			std::string line = fs::getLine(file);
			std::cout << line << std::endl;
		}
*/
/*
		// now read the file
		while( file )
		{
		std::string line;
		std::getline( file, line, '\n' );

		// seperate commandlet
		std::string command = util::getWord( line, " " );

		if( command == "#" )
		{
		// comment
		}
		if( command == "mtllib" )
		{
		// material library location
		}
		if( command == "v" )
		{
		// vertex

		// read in a vertex which has format %f %f %f
		std::vector<std::string> components;
		util::splitString( line, components, " " );

		if( components.size() == 3 )
		{
		math::Vec3f position;
		std::istringstream( components[0] ) >> position.x;
		std::istringstream( components[1] ) >> position.y;
		std::istringstream( components[2] ) >> position.z;
		vertices.push_back( position );
		}
		}

		if( command == "f" )
		{
		// face

		// read in a face which has format %f %f %f
		std::vector<std::string> verts;
		util::splitString( line, verts, " " );

		// handle triangles
		if( verts.size() == 3 )
		{
		int v[3]; // vertex indicees
		int n[3]; // normal indicees
		int t[3]; // texture indicees

		// for each vertex
		for( unsigned int i=0; i<verts.size(); ++i )
		{
		// split the string again, since each component(vertexindex, normalindex, textureindex) is
		// seperated with a '/'
		std::vector<std::string> components;
		util::splitString( verts[i], components, "/" );

		// first component is the vertex index
		std::istringstream( components[0] ) >> v[i];

		// second component is the texture index

		// third component is the normal index

		}

		// we have to substract one since the obj file has one as first index not zero!
		indicees.push_back( v[0] - 1 );
		indicees.push_back( v[1] - 1 );
		indicees.push_back( v[2] - 1 );
		}else
		// handle quads
		if( verts.size() == 4 )
		{
		int v[4]; // vertex indicees
		int n[4]; // normal indicees
		int t[4]; // texture indicees

		// for each vertex
		for( unsigned int i=0; i<verts.size(); ++i )
		{
		// split the string again, since each component(vertexindex, normalindex, textureindex) is
		// seperated with a '/'
		std::vector<std::string> components;
		util::splitString( verts[i], components, "/" );

		// first component is the vertex index
		std::istringstream( components[0] ) >> v[i];

		// second component is the texture index

		// third component is the normal index

		}

		// we have to substract one since the obj file has one as first index not zero!
		// triangulate the quad on the fly
		indicees.push_back( v[0] - 1 );
		indicees.push_back( v[1] - 1 );
		indicees.push_back( v[2] - 1 );
		indicees.push_back( v[0] - 1 );
		indicees.push_back( v[2] - 1 );
		indicees.push_back( v[3] - 1 );
		}

		}

		//printf("%s\n", command.c_str() );
		}


		file.close();

		return new Mesh( vertices, indicees );
*/
		return GeometryPtr();
	}


}
