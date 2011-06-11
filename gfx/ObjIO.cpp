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
#include <util/StringManip.h>
#include <iostream>



namespace base
{
	// contains all duplicates
	struct Vertex
	{
		Vertex( int v, int vt, int n ) : vIndex(v), uvIndex(vt), nIndex(n){}
		Vertex() : vIndex(-1), uvIndex(-1), nIndex(-1){}
		bool matches( int v, int vt, int vn )
		{
			bool result = true;
			if( (vIndex != -1)&&(v != -1) )
				result = result && (vIndex == v);
			if( (uvIndex != -1)&&(vt != -1) )
				result = result && (uvIndex == vt);
			if( (nIndex != -1)&&(vn != -1) )
				result = result && (nIndex == vn);
			return result;
		}
		// indices into data
		int vIndex;
		int uvIndex;
		int nIndex;
		// record of duplicates (vertices at same position but with different uv or n indices)
		// dupicated contains list of indices into vertex list
		std::vector<int> duplicates;
	};

	struct Polygon
	{
		Polygon()
		{
		}
		Polygon( int numVerts )
		{
			v.resize(numVerts);
		}
		std::vector<int> v; // vertex indices
	};



	//< reads and creates a GeometryPtr from the given *.obj file. returns 0 if it fails
	GeometryPtr importObj( const Path &filename )
	{
		std::vector<Polygon> polys;
		std::vector<math::Vec3f> P_data;
		std::vector<math::Vec2f> UV_data;
		std::vector<math::Vec3f> N_data;
		std::vector<Vertex> vertices;

		bool hasUV = false;
		bool hasN = false;

		fs::File *file = fs::open( filename );

		// file open?
		if( !file )
			// error file could not be opened
			return GeometryPtr();


		while(!fs::eof(file))
		{
			std::string line = fs::getLine(file);

			// seperate commandlet
			std::string command = base::getWord( line, " " );

			if( command == "#" )
			{
				// comment
			}else
			if( command == "mtllib" )
			{
				// material library location
			}else
			if( command == "v" )
			{
				// vertex

				// read in a vertex which has format %f %f %f
				std::vector<std::string> components;
				base::splitString( line, components, " " );

				if( components.size() == 3 )
				{
					math::Vec3f position;
					std::istringstream( components[0] ) >> position.x;
					std::istringstream( components[1] ) >> position.y;
					std::istringstream( components[2] ) >> position.z;
					P_data.push_back( position );
				}
			}else
			if( command == "vt" )
			{
				// texture vertex

				// read in a vertex which has format %f %f
				std::vector<std::string> components;
				base::splitString( line, components, " " );

				if( components.size() >= 2 )
				{
					math::Vec2f uv;
					std::istringstream( components[0] ) >> uv.x;
					std::istringstream( components[1] ) >> uv.y;
					uv.y = 1.0f - uv.y; // objs from houdini: needs flipping
					UV_data.push_back( uv );
				}

				hasUV = true;
			}else
			if( command == "vn" )
			{
				// normal

				// read in a vertex which has format %f %f %f
				std::vector<std::string> components;
				base::splitString( line, components, " " );

				if( components.size() >= 3 )
				{
					math::Vec3f normal;
					std::istringstream( components[0] ) >> normal.x;
					std::istringstream( components[1] ) >> normal.y;
					std::istringstream( components[2] ) >> normal.z;
					N_data.push_back( normal );
				}

				hasN = true;
			}else
			if( command == "f" )
			{
				// face
				std::vector<std::string> verts;
				base::splitString( line, verts, " " );

				if( verts.size() > 2 )
				{
					polys.push_back(Polygon((int)verts.size()));
					Polygon &p = polys.back();

					// for each vertex
					for( unsigned int i=0; i<verts.size(); ++i )
					{
						int v = -1;
						int vt = -1;
						int vn = -1;

						// split the string again, since each component(vertexindex, normalindex, textureindex) is
						// seperated with a '/'
						std::vector<std::string> components;
						base::splitString( verts[i], components, "/" );

						// first component is the vertex index
						std::istringstream( components[0] ) >> v; v -= 1; // we have to substract one since the obj file has one as first index not zero!

						// second component is the texture index
						if( (components.size() >= 2)&&(!components[1].empty()) )
							std::istringstream( components[1] ) >> vt; vt -= 1; // we have to substract one since the obj file has one as first index not zero!

						// third component is the normal index
						if( (components.size() >= 3)&&(!components[2].empty()) )
							std::istringstream( components[2] ) >> vn; vn -= 1; // we have to substract one since the obj file has one as first index not zero!

						//
						// find or create a vertex from given index combination
						//

						// if we havent got any vertices yet, reserve for efficiency
						if( vertices.empty() )
							vertices.resize( P_data.size() );

						// if vertex hasnt been initialised
						if( vertices[v].vIndex == -1 )
						{
							// initialise
							vertices[v].vIndex = v;
							vertices[v].uvIndex = vt;
							vertices[v].nIndex = vn;
							p.v[i] = v;
						}else
						{
							Vertex &vert = vertices[v];
							int duplicate = -1;

							if( vert.matches( v, vt, vn ) )
								duplicate = v;
							else
								// look for duplicate
								for( std::vector<int>::iterator it = vert.duplicates.begin(); it != vert.duplicates.end(); ++it )
								{
									int &dupIndex = *it;
									Vertex &dup = vertices[dupIndex];
									if( dup.matches( v, vt, vn ) )
									{
										duplicate = dupIndex;
										break;
									}
								}

							// not found/compatible?
							if( duplicate == -1 )
							{
								// create new vertex
								vertices.push_back( Vertex( v, vt, vn ) );
								duplicate = (int)vertices.size() - 1;
							}
							
							// store in duplicate list of origin vertex
							p.v[i] = duplicate;
						}
					}
				}else
				{
					// error: malformed face
				}
			}
		}

		fs::close( file );

		// create geometry
		GeometryPtr result = GeometryPtr(new Geometry(Geometry::TRIANGLE));

		// create vertex arrays
		AttributePtr P, UV, N;
		P = Attribute::createVec3f((int)vertices.size());
		result->setAttr( "P", P );
		if( hasUV )
		{
			UV = Attribute::createVec2f((int)vertices.size());
			result->setAttr( "UV", UV );
		}
		if( hasN )
		{
			N = Attribute::createVec3f((int)vertices.size());
			result->setAttr( "N", N );
		}

		int current = 0;
		for( std::vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); ++it, ++current )
		{
			Vertex &vertex = *it;
			P->set<math::Vec3f>( current, P_data[vertex.vIndex] );
			if(hasUV)
				UV->set<math::Vec2f>( current, UV_data[vertex.uvIndex] );
			if(hasN)
				N->set<math::Vec3f>( current, N_data[vertex.nIndex] );
		}
		
		

		// now iterate all polygons, turn them into triangles if necessary and add to geometry
		for( std::vector<Polygon>::iterator it = polys.begin(); it != polys.end(); ++it )
		{
			Polygon &p = *it;

			// basic triangluation TODO: employ more advanced triangluation
			int numVerts = (int) p.v.size();
			int numTris = numVerts - 2;
			for( int i=0;i<numTris;++i )
				result->addTriangle( p.v[0], p.v[i+1], p.v[i+2] );
		}

		return result;
	}


}
