#include "Mesh.h"








namespace base
{
	Mesh::VertexHandle Mesh::Face::addVertex( PointHandle p )
	{
		VertexHandle v = new Vertex;
		v->point = p;
		vertices.push_back(v);
		return v;
	}

	Mesh::PointHandle Mesh::addPoint( vec3_t pos )
	{
		PointHandle p = new Point;
		p->pos = pos;
		m_points.push_back( p );
		return p;
	}
	Mesh::FaceHandle Mesh::addTriangle( PointHandle a, PointHandle b, PointHandle c )
	{
		FaceHandle f = new Face;
		f->addVertex(a);
		f->addVertex(b);
		f->addVertex(c);
		m_faces.push_back(f);
		return f;
	}


	MeshPtr Mesh::sphere( float radius, int nSubdivsU, int nSubdivsV, vec3_t center, float uMin, float uMax, float vMin, float vMax )
	{
		MeshPtr m = MeshPtr( new Mesh() );

		float dPhi = MATH_2PIf/nSubdivsU;
		float dTheta = MATH_PIf/nSubdivsV;
		float theta, phi;

		std::vector<Mesh::PointHandle> points;

		// y
		for (theta=MATH_PIf/2.0f+dTheta;theta<=(3.0f*MATH_PIf)/2.0f-dTheta;theta+=dTheta)
		{
			vec3_t p;
			float y = sin(theta);
			// x-z
			phi = 0.0f;
			for( int j = 0; j<nSubdivsU; ++j  )
			{
				p.x = cos(theta) * cos(phi);
				p.y = y;
				p.z = cos(theta) * sin(phi);

				p = p*radius + center;

				points.push_back( m->addPoint(p) );
				phi+=dPhi;
			}
		}

		Mesh::PointHandle pole1 = m->addPoint( math::Vec3f(0.0f, 1.0f, 0.0f)*radius + center );
		Mesh::PointHandle pole2 = m->addPoint( math::Vec3f(0.0f, -1.0f, 0.0f)*radius + center );

		points.push_back(pole1);
		points.push_back(pole2);

		// add faces
		for( int j=0; j<nSubdivsV-3;++j )
		{
			int offset = j*(nSubdivsU);
			int i = 0;
			for( i=0; i<nSubdivsU-1; ++i )
			{
				m->addTriangle(points[offset+i+1], points[offset+i + nSubdivsU], points[offset+i]);
				m->addTriangle(points[offset+i+1], points[offset+i+nSubdivsU+1], points[offset+i + nSubdivsU]);
			}
			m->addTriangle(points[offset+0],points[offset+i + nSubdivsU],points[offset+i]);
			m->addTriangle(points[offset],points[offset + nSubdivsU],points[offset+i + nSubdivsU]);
		}
		for( int i=0; i<nSubdivsU-1; ++i )
		{
			m->addTriangle(points[i+1], points[i],pole1);
			m->addTriangle(points[nSubdivsU*(nSubdivsV-3)+i], points[nSubdivsU*(nSubdivsV-3)+i+1], pole2);
		}
		m->addTriangle(points[0], points[nSubdivsU-1], pole1);
		m->addTriangle(points[nSubdivsV*(nSubdivsV-2)-1], points[nSubdivsU*(nSubdivsV-3)], pole2);


		return m;
	}

	GeometryPtr Mesh::getGeometry()
	{
		GeometryPtr g = Geometry::createTriangleGeometry();


		AttributePtr positions = g->getAttr( "P" );

		static std::map<Mesh::PointHandle, int> g_pointIndices;

		g_pointIndices.clear();


		for( PointList::iterator it = m_points.begin(); it != m_points.end(); ++it )
		{
			PointHandle p = *it;
			g_pointIndices[p] = positions->appendElement( p->pos );
		}
		for( FaceList::iterator it = m_faces.begin(); it != m_faces.end(); ++it )
		{
			FaceHandle f = *it;
			if( f->vertices.size() == 3 )
				g->addTriangle( g_pointIndices[f->vertices[0]->point], g_pointIndices[f->vertices[1]->point], g_pointIndices[f->vertices[2]->point] );
		}
		return g;
	}
}