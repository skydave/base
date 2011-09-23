#pragma once

#include "Geometry.h"
#include <util/Object.h>



namespace base
{






	BASE_DECL_SMARTPTR(Mesh);
	class Mesh : public Object
	{
	public:
		typedef math::Vec3f vec3_t;

		struct Point
		{
			vec3_t pos;
		};
		typedef Point* PointHandle;
		typedef std::vector<PointHandle> PointList;

		struct Vertex
		{
			PointHandle point;
		};
		typedef Vertex* VertexHandle;
		typedef std::vector<VertexHandle> VertexList;

		struct Face
		{
			VertexHandle addVertex( PointHandle p );
			std::vector<VertexHandle> vertices;
		};
		typedef Face* FaceHandle;
		typedef std::vector<FaceHandle> FaceList;

		//Mesh();


		PointHandle                                                                                                        addPoint( vec3_t );
		FaceHandle                                                                 addTriangle( PointHandle a, PointHandle b, PointHandle c );



		GeometryPtr                                                                                                             getGeometry();




		static MeshPtr sphere( float radius = 1.0f, int nSubdivsX = 30, int nSubdivsY = 30, vec3_t center = vec3_t(), float uMin = 0.0f, float uMax = 1.0f, float vMin = 0.0f, float vMax = 1.0f );
	private:
		FaceList                  m_faces;
		PointList                m_points;

	};
}
