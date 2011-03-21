//
// To create offsets of one texel and one half texel in the
// texture lookup, we need to know the texture image size.
//
// The numbers below are 1/256 and 0.5/256, change accordingly
// if you change the code to use another texture size.
#define ONE 0.00390625
#define ONEHALF 0.001953125







uniform int scene;
uniform float tmp;
uniform sampler2D permTexture; // permutation texture used for perlin noise

//
// The interpolation function. This could be a 1D texture lookup
// to get some more speed, but it's not the main part of the algorithm.
//
float fade(float t)
{
	//return t;
	//return t*t*(3.0-2.0*t); // Old fade, yields discontinuous second derivative
	return t*t*t*(t*(t*6.0-15.0)+10.0); // Improved fade, yields C2-continuous noise
}






vec3 noise2d( vec2 P )
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




vec3 noise2d_org( vec2 P )
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




vec3 turb2d( vec2 p, int numIterations )
{
	vec3 a = vec3(0.0);
	float b = 1.0;

	for( int i=0; i<numIterations; ++i )
	{
		vec3 n = noise2d( p );
		a.x += b*n.x;
		a.y += b*n.y;
		a.z += b*n.z;

		b *= 0.5;
		p = mat2(1.6, 1.2, -1.2, 1.6)*p;
	}
	return a;
}



