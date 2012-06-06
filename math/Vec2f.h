/*---------------------------------------------------------------------



----------------------------------------------------------------------*/
#pragma once



namespace math
{
	struct Vec2f
	{
		Vec2f( float _x, float _y ) : x(_x), y(_y)
		{
		}
		Vec2f()
		{
			x = y = 0.0f;
		}

		union
		{
			struct
			{
				float x, y;
			};
			float v[2];
		};

		float                                 getLength( void ) const // returns the cartesian length of the vector
		{
			return sqrt(x*x + y*y);
		}
		float                          getSquaredLength( void ) const // returns the un-square-rooted length of the vector 
		{
			return x*x + y*y;
		}


		//bool                         operator==( const Vec3f &rhs );
		//bool                         operator!=( const Vec3f &rhs );
		
		bool operator+=( const Vec2f &rhs )
		{
			x+=rhs.x;
			y+=rhs.y;

			return true;
		}

		bool operator-=( const Vec2f &rhs )
		{
			x-=rhs.x;
			y-=rhs.y;

			return true;
		}
		bool operator+=( const float &rhs )
		{
			x+=rhs;
			y+=rhs;

			return true;
		}
		bool operator-=( const float &rhs )
		{
			x-=rhs;
			y-=rhs;

			return true;
		}
		bool operator*=( const float &rhs )
		{
			x*=rhs;
			y*=rhs;

			return true;
		}
		bool operator/=( const float &rhs )
		{
			x/=rhs;
			y/=rhs;

			return true;
		}

		const float&                      operator[]( int i ) const
		{
			return v[i];
		}
		float&                                  operator[]( int i )
		{
			return v[i];
		}
	};














	struct Vec2d
	{
		Vec2d( double _x, double _y ) : x(_x), y(_y)
		{
		}
		Vec2d()
		{
			x = y = 0.0f;
		}

		union
		{
			struct
			{
				double x, y;
			};
			double v[2];
		};

		double                                 getLength( void ) const // returns the cartesian length of the vector
		{
			return sqrt(x*x + y*y);
		}
		double                          getSquaredLength( void ) const // returns the un-square-rooted length of the vector 
		{
			return x*x + y*y;
		}


		//bool                         operator==( const Vec3f &rhs );
		//bool                         operator!=( const Vec3f &rhs );
		
		bool operator+=( const Vec2d &rhs )
		{
			x+=rhs.x;
			y+=rhs.y;

			return true;
		}

		bool operator-=( const Vec2d &rhs )
		{
			x-=rhs.x;
			y-=rhs.y;

			return true;
		}
		bool operator+=( const double &rhs )
		{
			x+=rhs;
			y+=rhs;

			return true;
		}
		bool operator-=( const double &rhs )
		{
			x-=rhs;
			y-=rhs;

			return true;
		}
		bool operator*=( const double &rhs )
		{
			x*=rhs;
			y*=rhs;

			return true;
		}
		bool operator/=( const double &rhs )
		{
			x/=rhs;
			y/=rhs;

			return true;
		}

		const double&                      operator[]( int i ) const
		{
			return v[i];
		}
		double&                                  operator[]( int i )
		{
			return v[i];
		}
	};
}
