#pragma once
#include <string>
#include <map>

#include "Attribute.h"
#include <math/Math.h>



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


namespace base
{

	BASE_DECL_SMARTPTR_STRUCT(Geometry);

	struct Geometry
	{
		enum PrimitiveType
		{
			POINT = 0, // GL_POINTS
			LINE = 1,
			TRIANGLE = 4,
			QUAD = 7,
			POLYGON = 9
		};

		Geometry( PrimitiveType pt = TRIANGLE );


		//virtual void render( Shader *shader = 0 );
		void clear(); // removes all attributes and primitives
		//Geometry *copy();




		//
		// manipulation
		//
		//void splitVertex( int vertexIndex, int numCopies );
		//void removeVertex( int vertexIndex );
		//void autoWeld( float distance );
		void reverse(); // reverses the order of vertices for each primitive (CW polys become CCW)



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
		void bindIndexBuffer();

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
		unsigned int numPrimitiveVertices(); // Point=1; Line=2; Triangle=3; Quad=4
		unsigned int addPoint( unsigned int vId );
		unsigned int addLine( unsigned int vId0, unsigned int vId1 );
		unsigned int addTriangle( unsigned int vId0, unsigned int vId1, unsigned int vId2 );
		unsigned int addQuad( unsigned int vId0, unsigned int vId1, unsigned int vId2, unsigned int vId3 );
		unsigned int addPolygonVertex( unsigned int v );



/*
		int                            m_numComponents;
*/
		PrimitiveType                  m_primitiveType; // determines the primitive type indexBuffer is pointing to...
		std::vector<unsigned int>        m_indexBuffer;
		bool                      m_indexBufferIsDirty;
		unsigned int                   m_numPrimitives;
		unsigned int            m_numPrimitiveVertices; // Point=1; Line=2; Triangle=3; Quad=4; polygon:*

		//
		// OpenGL specific
		//
		unsigned int                   m_indexBufferId;

		//
		// static creators
		//
		static GeometryPtr       createPointGeometry();
		static GeometryPtr        createLineGeometry();
		static GeometryPtr    createTriangleGeometry();
		static GeometryPtr        createQuadGeometry();
		static GeometryPtr        createPolyGeometry();
		static GeometryPtr       createReferenceMesh();
	};





	//
	// geometry generation functions
	//
	GeometryPtr geo_pointCloud();
	GeometryPtr geo_quad();
	GeometryPtr geo_grid( int xres, int zres, Geometry::PrimitiveType primType = Geometry::TRIANGLE );
	GeometryPtr geo_cube();
	GeometryPtr geo_sphere( int uSubdivisions, int vSubdivisions, float radius, math::Vec3f center = math::Vec3f(0.0f,0.0f,0.0f), Geometry::PrimitiveType primType = Geometry::TRIANGLE );
	GeometryPtr geo_circle( int uSubdivisions, float radius, math::Vec3f center = math::Vec3f(0.0f,0.0f,0.0f), Geometry::PrimitiveType primType = Geometry::LINE );

	//
	// geometry manipulation functions
	//
	void apply_transform( GeometryPtr geo, math::Matrix44f tm );
	void apply_normals( GeometryPtr geo );

	// geometry utility functions
	math::BoundingBox3d compute_bound( GeometryPtr geo );

}
