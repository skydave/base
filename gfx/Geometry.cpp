#include "Geometry.h"
#include <iostream>

#include <math/Math.h>
#include <gltools/gl.h>
#include <util/tuple.h>
#include "ObjIO.h"




namespace base
{
	Geometry::Geometry( PrimitiveType pt ) : m_primitiveType(pt), m_numPrimitives(0), m_indexBufferIsDirty(true)
	{
		// initialize indexbuffer
		glGenBuffers(1, &m_indexBufferId);

		switch( pt )
		{
		case Geometry::POINT:m_numPrimitiveVertices = 1;break;
		case Geometry::LINE:m_numPrimitiveVertices = 2;break;
		default:
		case Geometry::TRIANGLE:m_numPrimitiveVertices = 3;break;
		case Geometry::QUAD:m_numPrimitiveVertices = 4;break;
		case Geometry::POLYGON:m_numPrimitiveVertices = 0;break;
		}
	}

	AttributePtr Geometry::getAttr( const std::string &name )
	{
		std::map<std::string, AttributePtr>::iterator it = m_attributes.find(name);
		if(it != m_attributes.end())
			return it->second;
		return AttributePtr();
	}

	void Geometry::setAttr( const std::string &name, AttributePtr attr )
	{
		m_attributes[name] = attr;
	}

	bool Geometry::hasAttr( const std::string &name )
	{
		std::map<std::string, AttributePtr>::iterator it = m_attributes.find(name);
		return(it != m_attributes.end());
	}

	void Geometry::bindIndexBuffer()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
		if( m_indexBufferIsDirty && !m_indexBuffer.empty() )
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer.size()*sizeof(unsigned int), &m_indexBuffer[0], GL_STATIC_DRAW);
			m_indexBufferIsDirty = false;
		}
	}

	Geometry::PrimitiveType Geometry::primitiveType()
	{
		return m_primitiveType;
	}

	unsigned int Geometry::numPrimitives()
	{
		return m_numPrimitives;
	}

	// Point=1; Line=2; Triangle=3; Quad=4; polygon=*
	unsigned int Geometry::numPrimitiveVertices()
	{
		return m_numPrimitiveVertices;
	}

	unsigned int Geometry::addPoint( unsigned int vId )
	{
		m_indexBuffer.push_back(vId);
		m_indexBufferIsDirty = true;
		return m_numPrimitives++;
	}

	unsigned int Geometry::addLine( unsigned int vId0, unsigned int vId1 )
	{
		m_indexBuffer.push_back(vId0);
		m_indexBuffer.push_back(vId1);
		m_indexBufferIsDirty = true;
		return m_numPrimitives++;
	}

	unsigned int Geometry::addTriangle( unsigned int vId0, unsigned int vId1, unsigned int vId2 )
	{
		m_indexBuffer.push_back(vId0);
		m_indexBuffer.push_back(vId1);
		m_indexBuffer.push_back(vId2);
		m_indexBufferIsDirty = true;
		return m_numPrimitives++;
	}

	unsigned int Geometry::addQuad( unsigned int vId0, unsigned int vId1, unsigned int vId2, unsigned int vId3 )
	{
		m_indexBuffer.push_back(vId0);
		m_indexBuffer.push_back(vId1);
		m_indexBuffer.push_back(vId2);
		m_indexBuffer.push_back(vId3);
		m_indexBufferIsDirty = true;
		return m_numPrimitives++;
	}

	unsigned int Geometry::addPolygonVertex( unsigned int v )
	{
		m_indexBuffer.push_back(v);
		++m_numPrimitiveVertices;
		m_indexBufferIsDirty = true;
		if( !m_numPrimitives )
			// only 0 or 1 number of primitives allowed with polygons
			m_numPrimitives = 1;
		return m_numPrimitives;
	}



	// reverses the order of vertices for each primitive (CW polys become CCW)
	void Geometry::reverse()
	{
		for( unsigned int i=0;i<m_numPrimitives;++i )
		{
			int offset = i*m_numPrimitiveVertices;
			std::reverse(m_indexBuffer.begin()+offset, m_indexBuffer.begin()+offset+m_numPrimitiveVertices );
		}
		m_indexBufferIsDirty = true;
	}


	//
	// removes all attributes and primitives
	//
	void Geometry::clear()
	{
		for( std::map< std::string, AttributePtr >::iterator it = m_attributes.begin(); it != m_attributes.end(); ++it )
			it->second->clear();

		m_indexBuffer.clear();
		m_numPrimitives = 0;
		m_indexBufferIsDirty = true;
	}

	GeometryPtr Geometry::createPointGeometry()
	{
		GeometryPtr result = GeometryPtr( new Geometry(POINT) );
		AttributePtr positions = AttributePtr( Attribute::createVec3f() );
		result->setAttr( "P", positions);
		return result;
	}

	GeometryPtr Geometry::createLineGeometry()
	{
		GeometryPtr result = GeometryPtr( new Geometry(LINE) );
		AttributePtr positions = AttributePtr( Attribute::createVec3f() );
		result->setAttr( "P", positions);
		return result;
	}


	GeometryPtr Geometry::createTriangleGeometry()
	{
		GeometryPtr result = GeometryPtr( new Geometry(TRIANGLE) );
		AttributePtr positions = AttributePtr( Attribute::createVec3f() );
		result->setAttr( "P", positions);
		return result;
	}

	GeometryPtr Geometry::createQuadGeometry()
	{
		GeometryPtr result = GeometryPtr( new Geometry(QUAD) );
		AttributePtr positions = AttributePtr( Attribute::createVec3f() );
		result->setAttr( "P", positions);
		return result;
	}

	GeometryPtr Geometry::createPolyGeometry()
	{
		GeometryPtr result = GeometryPtr( new Geometry(POLYGON) );
		AttributePtr positions = AttributePtr( Attribute::createVec3f() );
		result->setAttr( "P", positions);
		return result;
	}

	GeometryPtr Geometry::createReferenceMesh()
	{
		return importObj( Path( BASE_PATH ) + "/data/meshref.obj" );
	}

	//
	// useful geometry generation functions
	//
	GeometryPtr geo_pointCloud()
	{
		GeometryPtr result = GeometryPtr( new Geometry(Geometry::POINT) );

		AttributePtr positions = Attribute::createVec3f();

		positions->appendElement( math::Vec3f(0.0f,0.0f,0.0f) );
		positions->appendElement( math::Vec3f(.5f,0.0f,0.0f) );
		positions->appendElement( math::Vec3f(-.5f,0.0f,0.0f) );

		result->setAttr( "P", positions);

		//result->addPoint( 0 );
		//result->addPoint( 1 );
		//result->addPoint( 2 );

		return result;
	}

	GeometryPtr geo_quad()
	{
		GeometryPtr result = GeometryPtr(new Geometry(Geometry::QUAD));

		AttributePtr positions = Attribute::createVec3f();
		AttributePtr uvs = Attribute::createVec2f();

		positions->appendElement( math::Vec3f(-1.0f,-1.0f,0.0f) );
		uvs->appendElement( .0f, .0f );
		positions->appendElement( math::Vec3f(-1.0f,1.0f,0.0f) );
		uvs->appendElement( .0f, 1.0f );
		positions->appendElement( math::Vec3f(1.0f,1.0f,0.0f) );
		uvs->appendElement( 1.0f, 1.0f );
		positions->appendElement( math::Vec3f(1.0f,-1.0f,0.0f) );
		uvs->appendElement( 1.0f, .0f );

		result->setAttr( "P", positions);
		result->setAttr( "UV", uvs );

		result->addQuad( 3, 2, 1, 0 );


		return result;
	}

	GeometryPtr geo_grid( int xres, int zres, Geometry::PrimitiveType primType )
	{
		GeometryPtr result = GeometryPtr(new Geometry(primType));

		AttributePtr positions = Attribute::createVec3f();
		result->setAttr( "P", positions);

		AttributePtr uvs = Attribute::createVec2f();
		result->setAttr( "UV", uvs );

		for( int j=0; j<zres; ++j )
			for( int i=0; i<xres; ++i )
			{
				float u = i/(float)(xres-1);
				float v = j/(float)(zres-1);
				positions->appendElement( math::Vec3f(u-0.5f,0.0f,v-0.5f) );
				uvs->appendElement( u, 1.0f - v ); // 1.0 - v because opengl texture space
			}

		if( primType == Geometry::POINT )
		{
			int numPoints = xres*zres;
			for( int i=0;i<numPoints;++i )
				result->addPoint( i );
		}else
		if( primType == Geometry::LINE )
		{
			for( int j=0; j<zres; ++j )
			{
				for( int i=0; i<xres-1; ++i )
				{
					int vo = (j*xres);
					result->addLine( vo+i, vo+i+1 );
				}
			}
			for( int j=0; j<xres; ++j )
			{
				for( int i=0; i<zres-1; ++i )
				{
					int v0 = (i*xres) + j;
					int v1 = ((i+1)*xres) + j;
					result->addLine( v0, v1 );
				}
			}
		}else
		if( primType == Geometry::TRIANGLE )
			for( int j=0; j<zres-1; ++j )
				for( int i=0; i<xres-1; ++i )
				{
					int vo = (j*xres);
					result->addTriangle( vo+xres+i+1, vo+i+1, vo+i );
					result->addTriangle( vo+xres+i, vo+xres+i+1, vo+i );

				}


		return result;
	}

	GeometryPtr geo_cube()
	{
		GeometryPtr result = GeometryPtr(new Geometry(Geometry::QUAD));

		// unique points
		std::vector<math::Vec3f> pos;
		pos.push_back( math::Vec3f(-.5f,-.5f,0.5f) );
		pos.push_back( math::Vec3f(-.5f,0.5f,0.5f) );
		pos.push_back( math::Vec3f(.5f,.5f,0.5f) );
		pos.push_back( math::Vec3f(.5f,-.5f,0.5f) );

		pos.push_back( math::Vec3f(-.5f,-.5f,-0.5f) );
		pos.push_back( math::Vec3f(-.5f,0.5f,-0.5f) );
		pos.push_back( math::Vec3f(.5f,.5f,-0.5f) );
		pos.push_back( math::Vec3f(.5f,-.5f,-0.5f) );

		// quads
		std::vector< std::tuple<int, int, int, int> > quads;
		quads.push_back( std::make_tuple(3, 2, 1, 0) );
		quads.push_back( std::make_tuple(4, 5, 6, 7) );
		quads.push_back( std::make_tuple(7, 6, 2, 3) );
		quads.push_back( std::make_tuple(1, 5, 4, 0) );
		quads.push_back( std::make_tuple(6, 5, 1, 2) );
		quads.push_back( std::make_tuple(4, 7, 3, 0) );

		// split per face (because we have uv shells)
		AttributePtr positions = Attribute::createVec3f();
		AttributePtr uv = Attribute::createVec2f();


		for( std::vector< std::tuple<int, int, int, int> >::iterator it = quads.begin(); it != quads.end(); ++it )
		{
			std::tuple<int, int, int, int> &quad = *it;
			int i0, i1, i2, i3;

			i0 = positions->appendElement( pos[std::get<0>(quad)] );
			uv->appendElement( math::Vec2f(0.0f, 0.0f) );
			i1 = positions->appendElement( pos[std::get<1>(quad)] );
			uv->appendElement( math::Vec2f(1.0f, 0.0f) );
			i2 = positions->appendElement( pos[std::get<2>(quad)] );
			uv->appendElement( math::Vec2f(1.0f, 1.0f) );
			i3 = positions->appendElement( pos[std::get<3>(quad)] );
			uv->appendElement( math::Vec2f(0.0f, 1.0f) );

			result->addQuad(i0, i1, i2, i3);			
		}

		result->setAttr( "P", positions);
		result->setAttr( "UV", uv);

		return result;
	}

	//TODO: add uv mapping
	GeometryPtr geo_sphere( int uSubdivisions, int vSubdivisions, float radius, math::Vec3f center, Geometry::PrimitiveType primType )
	{
		GeometryPtr result = GeometryPtr(new Geometry(primType));

		AttributePtr positions = Attribute::createVec3f();
		result->setAttr( "P", positions);

		AttributePtr uvs = Attribute::createVec2f();
		result->setAttr( "UV", uvs );

		float dPhi = MATH_2PIf/uSubdivisions;
		float dTheta = MATH_PIf/vSubdivisions;
		float theta, phi;

		// y
		for (theta=MATH_PIf/2.0f+dTheta;theta<=(3.0f*MATH_PIf)/2.0f-dTheta;theta+=dTheta)
		{
			math::Vec3f p;
			float y = sin(theta);
			// x-z
			phi = 0.0f;
			for( int j = 0; j<uSubdivisions; ++j  )
			{
				p.x = cos(theta) * cos(phi);
				p.y = y;
				p.z = cos(theta) * sin(phi);

				p = p*radius + center;

				positions->appendElement( p );
				phi+=dPhi;
			}
		}

		int pole1 = positions->appendElement( math::Vec3f(0.0f, 1.0f, 0.0f)*radius + center );
		int pole2 = positions->appendElement( math::Vec3f(0.0f, -1.0f, 0.0f)*radius + center );

		if( primType == Geometry::POINT )
		{
			int numVertices = positions->numElements();
			for( int i=0; i< numVertices; ++i )
				result->addPoint( i );
		}else
		if( primType == Geometry::TRIANGLE )
		{
			// add faces
			for( int j=0; j<vSubdivisions-3;++j )
			{
				int offset = j*(uSubdivisions);
				int i = 0;
				for( i=0; i<uSubdivisions-1; ++i )
				{
					result->addTriangle(offset+i+1, offset+i + uSubdivisions, offset+i);
					result->addTriangle(offset+i+1, offset+i+uSubdivisions+1, offset+i + uSubdivisions);
				}
				result->addTriangle(offset+0,offset+i + uSubdivisions,offset+i);
				result->addTriangle(offset,offset + uSubdivisions,offset+i + uSubdivisions);
			}
			for( int i=0; i<uSubdivisions-1; ++i )
			{
				result->addTriangle(i+1, i,pole1);
				result->addTriangle(uSubdivisions*(vSubdivisions-3)+i, uSubdivisions*(vSubdivisions-3)+i+1, pole2);
			}
			result->addTriangle(0, uSubdivisions-1, pole1);
			result->addTriangle(uSubdivisions*(vSubdivisions-2)-1, uSubdivisions*(vSubdivisions-3), pole2);
		}
		return result;
	}










	void apply_transform( GeometryPtr geo, math::Matrix44f tm )
	{
		AttributePtr pAttr = geo->getAttr("P");
		int numElements = pAttr->numElements();
		for( int i=0; i<numElements; ++i )
		{
			math::Vec3f v = math::transform( pAttr->get<math::Vec3f>(i), tm);
			pAttr->set<math::Vec3f>( (unsigned int)i, v );
		}
	}



	//
	// computes vertex normals
	// Assumes geometry to be triangles!
	//
	void apply_normals( GeometryPtr geo )
	{
		// only works with triangles and quads
		if( !((geo->primitiveType() == Geometry::TRIANGLE)||(geo->primitiveType() == Geometry::QUAD)) )
		{
			std::cerr << "apply_normals: can compute normals only on non triangle or quad geometry\n";
			return;
		}

		AttributePtr normalAttr = geo->getAttr("N");
		
		if( !normalAttr )
			normalAttr = Attribute::createVec3f();
		else
			normalAttr->clear();

		AttributePtr positions = geo->getAttr("P");
		int numPoints = positions->numElements();
		for( int i=0; i < numPoints; ++i )
			normalAttr->appendElement( math::Vec3f(0.0f, 0.0f, 0.0f) );

		int numPrimitives = geo->numPrimitives();
		int numPrimitiveVertices = geo->numPrimitiveVertices();
		for( int i=0; i < numPrimitives; ++i )
		{
			int idx[3];
			idx[0] = geo->m_indexBuffer[i*numPrimitiveVertices];
			idx[1] = geo->m_indexBuffer[i*numPrimitiveVertices+1];
			idx[2] = geo->m_indexBuffer[i*numPrimitiveVertices+2];

			math::Vec3f v1 = positions->get<math::Vec3f>( idx[1] )-positions->get<math::Vec3f>( idx[0] );
			math::Vec3f v2 = positions->get<math::Vec3f>( idx[2] )-positions->get<math::Vec3f>( idx[0] );
			math::Vec3f fn = math::normalize( math::crossProduct( v1,v2 ) );

			for( int j=0; j<3; ++j )
				normalAttr->set<math::Vec3f>( idx[j], normalAttr->get<math::Vec3f>(idx[j])+fn );
		}

		for( int i=0; i < numPoints; ++i )
			normalAttr->set<math::Vec3f>( i, math::normalize(  normalAttr->get<math::Vec3f>(i) ) );

		geo->setAttr( "N", normalAttr );
	}





	math::BoundingBox3d compute_bound( GeometryPtr geo )
	{
		math::BoundingBox3d bbox;
		AttributePtr p = geo->getAttr( "P" );
		int numElements = p->numElements();
		for( int i=0;i<numElements;++i )
			bbox.extend(p->get<math::Vec3f>(i));
		return bbox;
	}
}












/*
#include "Shader.h"
#define GEO_ATTRIBUTEHASH_COUNT 50



void Geometry::render( Shader *shader )
{
	if(shader && shader->isOk())
	{
		shader->use();

		// iterate all active attributes
		for( int i=0; i<shader->m_activeAttributes.size(); ++i )
		{
			int attrIndex = shader->m_activeAttributes.m_data[i];
			// getAttr and bind(with index)
			if (hasAttr(attrIndex))
				getAttr(attrIndex)->bindAsAttribute(attrIndex);
		}

		// iterate all active uniforms
		for( int i=0; i<shader->m_activeUniforms.size(); ++i )
			// iterate all uniforms of the geometry
			for( int j=0; j<m_uniforms.size(); ++j )
			{
				char *t1 = (char *)shader->m_activeUniformNames.m_data[i];
				char *t2 = (char *)m_uniformNames.m_data[j];
				if( !strcmp( (char *)shader->m_activeUniformNames.m_data[i], m_uniformNames.m_data[j] ) )
					getUniform(j)->bindAsUniform( shader->m_activeUniforms.m_data[i] );
			}
	}else
		// bind default vertex attributes
		getPAttr()->bindAsAttribute(ATTR_P);


	// render primitives

	// ...using indexbufferobject (does not work on laptop)
	//oglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	//oglBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer.size(), m_indexBuffer.m_data, GL_STATIC_DRAW);
	//glDrawElements(m_primitiveType, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

	// ...works for now
	glDrawElements(m_primitiveType, m_indexBuffer.size(), GL_UNSIGNED_INT, m_indexBuffer.m_data);

	if(shader && shader->isOk())
	{
		// iterate all active attributes
		for( int i=0; i<shader->m_activeAttributes.size(); ++i )
		{
			int attrIndex = shader->m_activeAttributes.m_data[i];
			// getAttr and unbind(with index)
			if (hasAttr(attrIndex))
				getAttr(attrIndex)->unbindAsAttribute(attrIndex);
		}

		// disable shader
		oglUseProgram(0);
	}else
		// unbind default vertex attributes
		getPAttr()->unbindAsAttribute(ATTR_P);
}




//
//!!!!!!!!!!!!! we dont copy them now. skeleton needs reference
//
Geometry *Geometry::copy()
{
	Geometry *ngeo = new Geometry();

	// copy attributes
	for( int i=0; i<GEO_ATTRIBUTEHASH_COUNT;++i )
		if (m_attributes[i])
			ngeo->setAttr( i, m_attributes[i]->copy() );
	//
	// copy uniforms !!!!!!!!!!!!! we dont copy them now. skeleton needs reference
	//
	for( int i=0; i<m_uniforms.size();++i )
		ngeo->setUniform( m_uniformNames.m_data[i], (Attribute*)m_uniforms.m_data[i] );

	// copy primitives
	ngeo->m_primitiveType = m_primitiveType;
	ngeo->resetPrimitiveType( m_primitiveType );
	ngeo->m_indexBuffer.resize( m_indexBuffer.size() );
	msys_memcpy( ngeo->m_indexBuffer.m_data, m_indexBuffer.m_data, m_indexBuffer.size()*sizeof(int) );
	ngeo->m_numPrimitives = m_numPrimitives;

	return ngeo;
}






//
//
//
void Geometry::splitVertex( int vertexIndex, int numCopies )
{
	for( int i=0;i<numCopies-1;++i )
	{
		// create a new vertex by creating a new entry in all attributes (and copying values over)
		for( int j=0; j<GEO_ATTRIBUTEHASH_COUNT;++j )
			if (m_attributes[j])
				getAttr( j )->appendElement( getAttr( j )->getRawPointer(vertexIndex) );
	}
}


void Geometry::removeVertex( int vertexIndex )
{
	for( int j=0; j<GEO_ATTRIBUTEHASH_COUNT;++j )
		if (m_attributes[j])
			getAttr( j )->removeElement( vertexIndex );
	// rearrange indices of all primitives
	for( int i=0; i<m_indexBuffer.size(); ++i )
	{
		if( m_indexBuffer.m_data[i] > vertexIndex )
			--m_indexBuffer.m_data[i];
	}
}


void Geometry::autoWeld( float distance )
{
	int *toRemove = (int *)mmalloc( getPAttr()->numElements()*sizeof(int) );
	int numRemove = 0;
	// for each vertex pair
	for( int i=0; i<getPAttr()->numElements(); ++i )
		for( int j=i+1; j<getPAttr()->numElements(); ++j )
		{
			if( (math::distance( getPAttr()->getVec3f(i), getPAttr()->getVec3f(j) ) < distance) && (fabs(getPAttr()->getVec3f(j).x) < distance ) )
			{
				// remove vertex j - later
				toRemove[numRemove++] = j;

				// bend all prims to point to i instead of j
				for( int k = 0; k<m_indexBuffer.size(); ++k )
				{
					int t = m_indexBuffer.m_data[k];
					if(m_indexBuffer.m_data[k] == j)
						m_indexBuffer.m_data[k] = i;
				}
			}
		}

	//Attribute *t = new Attribute(3);

	//for( int i=0; i<getPAttr()->numElements(); ++i )
	//	t->appendElement(math::Vec3f(0.0f, 0.5f, 0.0f));

	for( int i=0; i<numRemove; ++i )
	{
		//printf( "removing %i\n", toRemove[i] );
		//math::Vec3f c( 1.0f, 0.0f, 0.0f );
		//t->setElement(toRemove[i] , &c);

		//math::Vec3f p = getPAttr()->getVec3f(toRemove[i]);
		//p.z += 1.0f;
		//getPAttr()->setElement( toRemove[i], &p );
		//t->setElement( 0, &c);
		removeVertex(toRemove[i]-i);
	}
	//setAttr( ATTR_Cd, t );


	mfree( toRemove );
}




void Geometry::setAttr( int attrIndex, Attribute *vertexAttrib )
{
	m_attributes[attrIndex] = vertexAttrib;
}

Attribute *Geometry::getAttr( int attrIndex )
{
	return m_attributes[attrIndex];
}

bool Geometry::hasAttr( int attrIndex )
{
	return (m_attributes[attrIndex] != 0);
}

void Geometry::setVertexAttr( int attrIndex, int index, float x, float y, float z )
{
	//p
}




void Geometry::setPAttr( Attribute *posAttrib )
{
	m_attributes[ATTR_P] = posAttrib;
}

Attribute *Geometry::getPAttr()
{
	return m_attributes[ATTR_P];
}




Attribute *Geometry::getUniform( int uniformIndex )
{
	return (Attribute *)m_uniforms.m_data[uniformIndex];
}

void Geometry::setUniform( const char *name, Attribute *uniform )
{
	m_uniformNames.push_back( name );
	m_uniforms.push_back( uniform );	
}

int Geometry::addPoint( int vId0 )
{
	if( m_primitiveType != GL_POINTS )
		resetPrimitiveType( GL_POINTS );
	m_indexBuffer.push_back(vId0);

	return m_numPrimitives++;
}

int Geometry::addLine( int vId0, int vId1 )
{
	if( m_primitiveType != GL_LINES )
		resetPrimitiveType( GL_LINES );
	m_indexBuffer.push_back(vId0);
	m_indexBuffer.push_back(vId1);

	return m_numPrimitives++;
}

int Geometry::addTriangle( int vId0, int vId1, int vId2 )
{
	if( m_primitiveType != GL_TRIANGLES )
		resetPrimitiveType( GL_TRIANGLES );
	m_indexBuffer.push_back(vId0);
	m_indexBuffer.push_back(vId1);
	m_indexBuffer.push_back(vId2);

	return m_numPrimitives++;
}

int Geometry::addQuad( int vId0, int vId1, int vId2, int vId3 )
{
	if( m_primitiveType != GL_QUADS )
		resetPrimitiveType( GL_QUADS );
	m_indexBuffer.push_back(vId0);
	m_indexBuffer.push_back(vId1);
	m_indexBuffer.push_back(vId2);
	m_indexBuffer.push_back(vId3);

	return m_numPrimitives++;
}


void Geometry::resetPrimitiveType( int newPrimitiveType )
{
	m_indexBuffer.clear();
	m_primitiveType = newPrimitiveType;
	switch(m_primitiveType)
	{
	case GL_POINTS: m_numComponents=1;break;
	case GL_LINES: m_numComponents=2;break;
	case GL_TRIANGLES: m_numComponents=3;break;
	case GL_QUADS: m_numComponents=4;break;
	};
}













static int getEdgeId( int a, int b, int *edges, int *nedges, Attribute *p )
{
	// encode a/b into one integer .. limits number of edges to 2^16
	// makes it more elegant to look for the edge (its just an integer compare)
	int n = (a>b)?(a<<16)|b:(b<<16)|a;

	// look for given edge
	int i;
	for( i=0; edges[2*i+0]!=n && i<*nedges; i++ );
	if( i==*nedges )
	{
		edges[2*i+0] = n; // set the edge
		(*nedges)++; // increase number of edges if we just have added the edge
		p->appendElement( 0.0f, 0.0f, 0.0f ); // append edgepoint
	}
	edges[2*i+1]++;   // increase access count (== number of shared faces)
	return i;
}

const static float vpw[4] = { 9.0f, 3.0f, 1.0f, 3.0f };
const static float epw[4] = { 3.0f, 3.0f, 1.0f, 1.0f };

void apply_catmullclark( Geometry *geo, int numIterations )
{
	for( int iter=0; iter<numIterations; ++iter )
	{
		Geometry *org = geo->copy();

		// original points are  left - we just clear out primitives
		geo->m_indexBuffer.clear();
		geo->m_numPrimitives = 0;


		//
		// do catmull clark subdivision
		//
		int abcd[4]; // used to rearrange quad indices so that abcd[0] always points to the current vertex
		int eid[4];
		int *edges = (int *)mmalloc( sizeof(int)*10000 );
		int *faceValences = (int *)mmalloc( sizeof(int)*10000 );
		int numEdges = 0;

		msys_memset( edges, 0, sizeof(int)*10000 );
		msys_memset( faceValences, 0, sizeof(int)*10000 );

		// zero out vertex points
		msys_memset( geo->getPAttr()->m_data.m_data, 0, geo->getPAttr()->m_data.size() );

		// add face center points
		int fpOffset = geo->getPAttr()->numElements();
		int epOffset = geo->getPAttr()->appendElements( org->m_numPrimitives );

		// for each quad
		for(int i = 0; i<org->m_numPrimitives; ++i)
		{
			math::Vec3f faceCenter;
			// for each vertex of current quad
			for(int j=0; j<4;++j)
			{
				// rearrange into convenience structure
				for(int k=0; k<4;++k)
					abcd[k] = org->m_indexBuffer.m_data[i*4 + (j+k)%4];
				// get id of edge between current quad vertex with the next quadvertex
				eid[j] = getEdgeId( abcd[0], abcd[1], edges, &numEdges, geo->getPAttr() );
				// update face valences of current quad vertex
				faceValences[abcd[0]]++;
				// update face center point
				faceCenter += 0.25f * org->getPAttr()->getVec3f( abcd[0] );

				for( int k=0; k<4; ++k )
				{
					// increment vertex point
					geo->getPAttr()->setElement( abcd[0], &(geo->getPAttr()->getVec3f(abcd[0]) + vpw[k]*org->getPAttr()->getVec3f(abcd[k])) );
					// increment edge point
					geo->getPAttr()->setElement( epOffset +eid[j], &(geo->getPAttr()->getVec3f(epOffset +eid[j]) + epw[k]*org->getPAttr()->getVec3f(abcd[k])) );
				}
			}

			// set face centerpoint
			geo->getPAttr()->setElement(fpOffset+i, &faceCenter);

			// add child faces
			for(int j=0; j<4;++j)
			//for(int j=3; j>=0;--j)
				geo->addQuad( fpOffset+i, epOffset+eid[(3+j)&3], org->m_indexBuffer.m_data[i*4+j], epOffset+eid[(0+j)&3] );
		}

		// for each original point
		for(int i=0; i<fpOffset;++i)
		{
			geo->getPAttr()->setElement( i, &(geo->getPAttr()->getVec3f(i)*(0.0625f/(float)faceValences[i])) );
			math::Vec3f pp = geo->getPAttr()->getVec3f( i );

		}
		// for each edge point
		for(int i=epOffset; i<geo->getPAttr()->numElements();++i)
		{
			geo->getPAttr()->setElement( i, &(geo->getPAttr()->getVec3f(i)*(0.1250f/(float)edges[(i-epOffset)*2+1])) );
			math::Vec3f pp = geo->getPAttr()->getVec3f( i );
		}

		delete org;
		mfree(edges);
		mfree(faceValences);
	}
}
*/
