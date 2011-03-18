/*
 * To create offsets of one texel and one half texel in the
 * texture lookup, we need to know the texture image size.
 */
#define ONE 0.00390625
#define ONEHALF 0.001953125
// The numbers above are 1/256 and 0.5/256, change accordingly
// if you change the code to use another texture size.






uniform int scene;
uniform float tmp;
uniform sampler2D permTexture; // permutation texture used for perlin noise

/*
 * The interpolation function. This could be a 1D texture lookup
 * to get some more speed, but it's not the main part of the algorithm.
 */
float fade(float t)
{
	//return t;
	//return t*t*(3.0-2.0*t); // Old fade, yields discontinuous second derivative
	return t*t*t*(t*(t*6.0-15.0)+10.0); // Improved fade, yields C2-continuous noise
}






vec3 noise2f( vec2 P )
{
	vec2 Pi = ONE*floor(P)+ONEHALF; // Integer part, scaled and offset for texture lookup
	vec2 Pf = fract(P);             // Fractional part for interpolation


	float du = 30.0*Pf.x*Pf.x*(Pf.x*(Pf.x-2.0)+1.0);
	float dv = 30.0*Pf.y*Pf.y*(Pf.y*(Pf.y-2.0)+1.0);
	//float du = 30.0*Pf.x*Pf.x*Pf.x*Pf.x - 60.0*Pf.x*Pf.x*Pf.x + 30.0*Pf.x*Pf.x;
	//float dv = 30.0*Pf.y*Pf.y*Pf.y*Pf.y - 60.0*Pf.y*Pf.y*Pf.y + 30.0*Pf.y*Pf.y;
	//float du = 6.0*Pf.x - 6.0*Pf.x*Pf.x;
	//float dv = 6.0*Pf.y - 6.0*Pf.y*Pf.y;

	//float du = 1.0;
	//float dv = 1.0;

	float u = fade(Pf.x);
	float v = fade(Pf.y);
	
	
/*
	float a = texture2D(permTexture, Pi).a;
	float b = texture2D(permTexture, Pi + vec2(ONE, 0.0)).a;
	float c = texture2D(permTexture, Pi + vec2(0.0, ONE)).a;
	float d = texture2D(permTexture, Pi + vec2(ONE, ONE)).a;
*/
	
	

	// Noise contribution from lower left corner
	vec2 grad00 = texture2D(permTexture, Pi).rg * 4.0 - 1.0;
	float a = dot(grad00, Pf);

	// Noise contribution from lower right corner
	vec2 grad10 = texture2D(permTexture, Pi + vec2(ONE, 0.0)).rg * 4.0 - 1.0;
	float b = dot(grad10, Pf - vec2(1.0, 0.0));

	// Noise contribution from upper left corner
	vec2 grad01 = texture2D(permTexture, Pi + vec2(0.0, ONE)).rg * 4.0 - 1.0;
	float c = dot(grad01, Pf - vec2(0.0, 1.0));

	// Noise contribution from upper right corner
	vec2 grad11 = texture2D(permTexture, Pi + vec2(ONE, ONE)).rg * 4.0 - 1.0;
	float d = dot(grad11, Pf - vec2(1.0, 1.0));


	// WHY?
	float k0 = a;
	float k1 = b-a;
	float k2 = c-a;
	float k3 = a - b - c + d;

	float n = k0 + k1*u + k2*v + k3*u*v;
	float dn_dx = du * (k1 + k3*v);
	float dn_dy = dv * (k2 + k3*u);

	// We're done, return the final noise value.
	return vec3(n, dn_dx, dn_dy);
	//return vec3(n, u, v);
	//return vec3(Pf.x, 0.0, 0.0);
	
	//return texture2D(permTexture, Pi).xyz;
}




vec3 noise2f_org( vec2 P )
{
  vec2 Pi = ONE*floor(P)+ONEHALF; // Integer part, scaled and offset for texture lookup
  vec2 Pf = fract(P);             // Fractional part for interpolation

  // Noise contribution from lower left corner
  vec2 grad00 = texture2D(permTexture, Pi).rg * 4.0 - 1.0;
  float n00 = dot(grad00, Pf);

  // Noise contribution from lower right corner
  vec2 grad10 = texture2D(permTexture, Pi + vec2(ONE, 0.0)).rg * 4.0 - 1.0;
  float n10 = dot(grad10, Pf - vec2(1.0, 0.0));

  // Noise contribution from upper left corner
  vec2 grad01 = texture2D(permTexture, Pi + vec2(0.0, ONE)).rg * 4.0 - 1.0;
  float n01 = dot(grad01, Pf - vec2(0.0, 1.0));

  // Noise contribution from upper right corner
  vec2 grad11 = texture2D(permTexture, Pi + vec2(ONE, ONE)).rg * 4.0 - 1.0;
  float n11 = dot(grad11, Pf - vec2(1.0, 1.0));

  // Blend contributions along x
  vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade(Pf.x));

  // Blend contributions along y
  float n_xy = mix(n_x.x, n_x.y, fade(Pf.y));

  // We're done, return the final noise value.
  return vec3(n_xy);
}




vec4 noise3f( vec3 P )
{
	vec3 Pi = ONE*floor(P)+ONEHALF; // Integer part, scaled so +1 moves one texel
								  // and offset 1/2 texel to sample texel centers
	vec3 Pf = fract(P);     // Fractional part for interpolation

	float du = 30.0*Pf.x*Pf.x*(Pf.x*(Pf.x-2.0)+1.0);
	float dv = 30.0*Pf.y*Pf.y*(Pf.y*(Pf.y-2.0)+1.0);
	float dw = 30.0*Pf.z*Pf.z*(Pf.z*(Pf.z-2.0)+1.0);

	float u = fade(Pf.x);
	float v = fade(Pf.y);
	float w = fade(Pf.z);


	float tmp = 0.0;
	tmp = texture2D(permTexture, Pi.xy).b;
	float a = texture2D(permTexture, vec2(tmp, Pi.z) ).a;
	float e = texture2D(permTexture, vec2(tmp, Pi.z+ONE) ).a;

	tmp = texture2D(permTexture, Pi.xy + vec2(ONE, 0.0)).b;
	float b = texture2D(permTexture, vec2(tmp, Pi.z)).a;
	float f = texture2D(permTexture, vec2(tmp, Pi.z+ONE)).a;

	tmp = texture2D(permTexture, Pi.xy + vec2(0.0, ONE)).b;
	float c = texture2D(permTexture, vec2(tmp, Pi.z)).a;
	float g = texture2D(permTexture, vec2(tmp, Pi.z+ONE)).a;

	tmp = texture2D(permTexture, Pi.xy + vec2(ONE, ONE)).b;
	float d = texture2D(permTexture, vec2(tmp, Pi.z)).a;
	float h = texture2D(permTexture, vec2(tmp, Pi.z+ONE)).a;

/*
// Noise contributions from (x=0, y=0), z=0 and z=1
float perm00 = texture2D(permTexture, Pi.xy).a ;
vec3  grad000 = texture2D(permTexture, vec2(perm00, Pi.z)).rgb * 4.0 - 1.0;
float a = dot(grad000, Pf);
vec3  grad001 = texture2D(permTexture, vec2(perm00, Pi.z + ONE)).rgb * 4.0 - 1.0;
float e = dot(grad001, Pf - vec3(0.0, 0.0, 1.0));

// Noise contributions from (x=0, y=1), z=0 and z=1
float perm01 = texture2D(permTexture, Pi.xy + vec2(0.0, ONE)).a ;
vec3  grad010 = texture2D(permTexture, vec2(perm01, Pi.z)).rgb * 4.0 - 1.0;
float b = dot(grad010, Pf - vec3(0.0, 1.0, 0.0));
vec3  grad011 = texture2D(permTexture, vec2(perm01, Pi.z + ONE)).rgb * 4.0 - 1.0;
float f = dot(grad011, Pf - vec3(0.0, 1.0, 1.0));

// Noise contributions from (x=1, y=0), z=0 and z=1
float perm10 = texture2D(permTexture, Pi.xy + vec2(ONE, 0.0)).a ;
vec3  grad100 = texture2D(permTexture, vec2(perm10, Pi.z)).rgb * 4.0 - 1.0;
float c = dot(grad100, Pf - vec3(1.0, 0.0, 0.0));
vec3  grad101 = texture2D(permTexture, vec2(perm10, Pi.z + ONE)).rgb * 4.0 - 1.0;
float g = dot(grad101, Pf - vec3(1.0, 0.0, 1.0));

// Noise contributions from (x=1, y=1), z=0 and z=1
float perm11 = texture2D(permTexture, Pi.xy + vec2(ONE, ONE)).a ;
vec3  grad110 = texture2D(permTexture, vec2(perm11, Pi.z)).rgb * 4.0 - 1.0;
float d = dot(grad110, Pf - vec3(1.0, 1.0, 0.0));
vec3  grad111 = texture2D(permTexture, vec2(perm11, Pi.z + ONE)).rgb * 4.0 - 1.0;
float h = dot(grad111, Pf - vec3(1.0, 1.0, 1.0));
*/
	float k0 = a;
	float k1 = b - a;
	float k2 = c - a;
	float k3 = e - a;
	float k4 = a - b - c + d;
	float k5 = a - c - e + g;
	float k6 = a - b - e + f;
	float k7 = -a + b + c - d + e - f - g + h;

	float n = k0 + k1*u + k2*v + k3*w + k4*u*v + k5*v*w + k6*w*u + k7*u*v*w;
	float dn_dx = du * (k1 + k4*v + k6*w + k7*v*w);
	float dn_dy = dv * (k2 + k5*w + k4*u + k7*w*u);
	float dn_dz = dw * (k3 + k6*u + k5*v + k7*u*v);


	// We're done, return the final noise value.
	return vec4(n, dn_dx, dn_dy, dn_dz);
}



float noise3f_org( vec3 P )
{
  vec3 Pi = ONE*floor(P)+ONEHALF; // Integer part, scaled so +1 moves one texel
								  // and offset 1/2 texel to sample texel centers
  vec3 Pf = fract(P);     // Fractional part for interpolation

  // Noise contributions from (x=0, y=0), z=0 and z=1
  float perm00 = texture2D(permTexture, Pi.xy).a ;
  vec3  grad000 = texture2D(permTexture, vec2(perm00, Pi.z)).rgb * 4.0 - 1.0;
  float n000 = dot(grad000, Pf);
  vec3  grad001 = texture2D(permTexture, vec2(perm00, Pi.z + ONE)).rgb * 4.0 - 1.0;
  float n001 = dot(grad001, Pf - vec3(0.0, 0.0, 1.0));

  // Noise contributions from (x=0, y=1), z=0 and z=1
  float perm01 = texture2D(permTexture, Pi.xy + vec2(0.0, ONE)).a ;
  vec3  grad010 = texture2D(permTexture, vec2(perm01, Pi.z)).rgb * 4.0 - 1.0;
  float n010 = dot(grad010, Pf - vec3(0.0, 1.0, 0.0));
  vec3  grad011 = texture2D(permTexture, vec2(perm01, Pi.z + ONE)).rgb * 4.0 - 1.0;
  float n011 = dot(grad011, Pf - vec3(0.0, 1.0, 1.0));

  // Noise contributions from (x=1, y=0), z=0 and z=1
  float perm10 = texture2D(permTexture, Pi.xy + vec2(ONE, 0.0)).a ;
  vec3  grad100 = texture2D(permTexture, vec2(perm10, Pi.z)).rgb * 4.0 - 1.0;
  float n100 = dot(grad100, Pf - vec3(1.0, 0.0, 0.0));
  vec3  grad101 = texture2D(permTexture, vec2(perm10, Pi.z + ONE)).rgb * 4.0 - 1.0;
  float n101 = dot(grad101, Pf - vec3(1.0, 0.0, 1.0));

  // Noise contributions from (x=1, y=1), z=0 and z=1
  float perm11 = texture2D(permTexture, Pi.xy + vec2(ONE, ONE)).a ;
  vec3  grad110 = texture2D(permTexture, vec2(perm11, Pi.z)).rgb * 4.0 - 1.0;
  float n110 = dot(grad110, Pf - vec3(1.0, 1.0, 0.0));
  vec3  grad111 = texture2D(permTexture, vec2(perm11, Pi.z + ONE)).rgb * 4.0 - 1.0;
  float n111 = dot(grad111, Pf - vec3(1.0, 1.0, 1.0));

  // Blend contributions along x
  vec4 n_x = mix(vec4(n000, n001, n010, n011), vec4(n100, n101, n110, n111), fade(Pf.x));

  // Blend contributions along y
  vec2 n_xy = mix(n_x.xy, n_x.zw, fade(Pf.y));

  // Blend contributions along z
  float n_xyz = mix(n_xy.x, n_xy.y, fade(Pf.z));

  // We're done, return the final noise value.
  return n_xyz;
}


//
// ------------------------------------------------
//



vec3 turb2f( vec2 p, int numIterations )
{
	vec3 a = vec3(0.0);
	float b = 1.0;

	for( int i=0; i<numIterations; ++i )
	{
		vec3 n = noise2f( p );
		a.x += b*n.x;
		a.y += b*n.y;
		a.z += b*n.z;

		b *= 0.5;
		p = mat2(1.6, 1.2, -1.2, 1.6)*p;
	}
	return a;
}


vec4 turb3f( vec3 p, int numIterations )
{
	vec4 a = vec4(0.0);
	float b = 1.0;

	for( int i=0; i<numIterations; ++i )
	{
		vec4 n = noise3f( p );
		a.x += b*n.x;
		a.y += b*n.y;
		a.z += b*n.z;
		a.w += b*n.w;
		b *= 0.5;
		//p = mat2(1.6, 1.2, -1.2, 1.6)*p;
		p = p * 2.2 + 1.0;
	}
	return a;
}

/*
float terrain( vec2 p )
{
	float a = 0.0;
	float b = 1.0;
	
	vec2 c = vec2(0.0);
	
	for( int i=0; i<8; ++i )
	{
		float n = noise2f( p );
		a += b*n;
		//a = n;
		b *= 0.5;
		p = mat2(1.6, 1.2, -1.2, 1.6)*p;
	}
	return a;
}
*/

float turbulence3f( vec3 p )
{
	float a = 0.0;
	float b = 1.0;

	for( int i=0; i<1; ++i )
	{
		float n = noise3f( p ).x;
		a += b*n;
		b *= 0.5;
		//p = mat2(1.6, -1.2, 1.2, 1.6)*p;
		//p = mat2(1.6, 1.2, -1.2, 1.6)*p;
		p = p * 2.2 + 1.0;
	}
	return a;
}


// =============================================================
// Convert the cipher range from [-1,1] to [0,1]
float norm(float x)
{
    return x * 0.5 + 0.5;
}

// Generate animated (t) caustic values
float caustic(float u, float v, vec3 Y, float t)
{
	const float pi = 3.1416;
    return pow((
    norm(sin(pi * 2.0 * (u + v + Y.z*t))) +
    norm(sin(pi       * (v - u - Y.z*t))) +
    norm(sin(pi       * (v     + Y.z*t))) +
    norm(sin(pi * 3.0 * (u     - Y.z*t)))) * 0.3, 2.0);
}

float waterHeight( vec2 x, float time )
{
    return  caustic(x.x+time*0.75, x.y*0.5, vec3(0.0,0.0,time), 0.3) * 0.006 +
            caustic(x.x*0.1+time*0.2, x.y*0.1, vec3(0.0,0.0,time), 0.02) * 0.125 -
            0.15;
}
// =============================================================

vec3 terrain( vec2 p, int numIterations )
{
	//vec3 res = turb2f(p*0.01, numIterations);
	//res.x = res.x * 100.0;
	//return res;

	float a = 0.0;
	float b = 1.0;
	vec2 c = vec2(0.0);
	for(int i=0; i<numIterations; ++i )
	{
		vec3 n = noise2f(p);
		c += n.yz;
		a += b*n.x / (1.0+dot(c,c));
		b *= 0.5;
		p = mat2x2(1.6,-1.2,1.2,1.6)*p;
	}
	vec3 result = vec3(a, c.x, c.y);
	

	

	if( scene == 0 )
	{
		//
		// crater
		//
		vec2 center = vec2( 0.0, 0.0 );
		float radius = 500.5;
		
		float d = radius*(1.0+noise2f( p*0.0005 ).x) - length(p-center);
		
		float h = 0.0;
		if( d > 0.0 )
			h = 1.0 - smoothstep( 0.0, 250.0, d );
		else
			h = 1.0 - smoothstep( 0.0, 550.0, -d );
		
		return h + result*smoothstep( radius, 600.0, length(p-center) ) + result*smoothstep( 0.0, radius, length(p-center) )*0.5;
	}else
	if( scene == 1 )
	{
		//
		// beach
		//
		p = p - vec2( -2000.0, -2000.0 );
		float t = 0.0;
		float shore = p.x*p.x*0.00005;
		if( p.y > shore )
		{
			//t = turb3f(vec3(p.x, p.y, tmp), 5).x*0.01;
			//t = noise3f(vec3(p.x, p.y, tmp));
			t = 0.0;
		}else
		{
			// blend in mountains
			return (result+1.0)*smoothstep( 0.0, 500.0, -(p.y-shore) );
		}
		return vec3(t);
	}
	return vec3(0);
}

float smoothstep( float a, float b, float t )
{
	float x = (t-a)/(b-a);
	return clamp(x, 0.0, 1.0);
}





