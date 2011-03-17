#pragma once
#include <string>
#include <map>

#include "Attribute.h"



/*
// these definitions are used to map Attributeids to unique identifiers which will be used as index
#define ATTR_P            0
#define ATTR_N            1
#define ATTR_Cd           2
#define ATTR_W            3
#define ATTR_CATMULLT     4
#define ATTR_BONEWEIGHTS  5
#define ATTR_BONEINDICES  6
#define ATTR_UV           7

struct Shader;
*/

#define PointGeometry() Geometry(Geometry::POINT)
#define LineGeometry() Geometry(Geometry::LINE)
#define QuadGeometry() Geometry(Geometry::QUAD)

namespace base
{

	BASE_DECL_SMARTPTR(Geometry);

	struct Geometry
	{
		enum PrimitiveType
		{
			POINT = 0,
			LINE = 1,
			TRIANGLE = 4,
			QUAD = 7
		};

		Geometry( PrimitiveType pt = TRIANGLE );


		//virtual void render( Shader *shader = 0 );
		//void clear(); // removes all attributes and primitives
		//Geometry *copy();




		//
		// manipulation
		//
		//void splitVertex( int vertexIndex, int numCopies );
		//void removeVertex( int vertexIndex );
		//void autoWeld( float distance );



		//
		// Attribute management
		//
		AttributePtr getAttr( const std::string &name );
		void setAttr( const std::string &name, AttributePtr attr );
		bool hasAttr( const std::string &name );

		std::map< std::string, AttributePtr > m_attributes;

		//
		// uniform management
		//
		/*
		void setUniform( const char *name, Attribute *uniform );
		Attribute *getUniform( int uniformIndex );
		vector<void *>     m_uniforms; // list of uniforms
		vector<const char *> m_uniformNames; // list of uniform names
		*/

		//
		// primitive management
		//
/*
		void resetPrimitiveType( int newPrimitiveType );
		int addPoint( int vId0 );
		int addLine( int vId0, int vId1 );
		int addTriangle( int vId0, int vId1, int vId2 );
*/
		PrimitiveType primitiveType();
		unsigned int numPrimitives();
		unsigned int addTriangle( unsigned int vId0, unsigned int vId1, unsigned int vId2 );
		unsigned int addQuad( unsigned int vId0, unsigned int vId1, unsigned int vId2, unsigned int vId3 );



/*
		int                            m_numComponents;
*/
		PrimitiveType                  m_primitiveType; // determines the primitive type indexBuffer is pointing to...
		std::vector<unsigned int>        m_indexBuffer;
		unsigned int                   m_numPrimitives;

		//
		// OpenGL specific
		//
		//unsigned int                   m_indexBufferId;
	};





	//
	// geometry generation functions
	//
	GeometryPtr geo_pointCloud();
	GeometryPtr geo_quad();
	GeometryPtr geo_grid( int xres, int zres );
	GeometryPtr geo_cube();

}
