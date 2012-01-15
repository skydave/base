// The numbers below are 1/256 and 0.5/256, change accordingly if permutationTableTexture changes
#define ONE 0.00390625
#define ONEHALF 0.001953125

uniform sampler2D noise_permTexture; // permutation texture used for perlin noise

//
// The interpolation function. This could be a 1D texture lookup
// to get some more speed, but it's not the main part of the algorithm.
//
float fade(float t)
{
	//return t*t*(3.0-2.0*t); // Old fade, yields discontinuous second derivative
	return t*t*t*(t*(t*6.0-15.0)+10.0); // Improved fade, yields C2-continuous noise
}


float perlinnoise2d( vec2 P )
{
  vec2 Pi = ONE*floor(P)+ONEHALF; // Integer part, scaled and offset for texture lookup
  vec2 Pf = fract(P);             // Fractional part for interpolation

  // Noise contribution from lower left corner
  vec2 grad00 = texture2D(noise_permTexture, Pi).rg * 4.0 - 1.0;
  float n00 = dot(grad00, Pf);

  // Noise contribution from lower right corner
  vec2 grad10 = texture2D(noise_permTexture, Pi + vec2(ONE, 0.0)).rg * 4.0 - 1.0;
  float n10 = dot(grad10, Pf - vec2(1.0, 0.0));

  // Noise contribution from upper left corner
  vec2 grad01 = texture2D(noise_permTexture, Pi + vec2(0.0, ONE)).rg * 4.0 - 1.0;
  float n01 = dot(grad01, Pf - vec2(0.0, 1.0));

  // Noise contribution from upper right corner
  vec2 grad11 = texture2D(noise_permTexture, Pi + vec2(ONE, ONE)).rg * 4.0 - 1.0;
  float n11 = dot(grad11, Pf - vec2(1.0, 1.0));

  // Blend contributions along x
  vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade(Pf.x));

  // Blend contributions along y
  float n_xy = mix(n_x.x, n_x.y, fade(Pf.y));

  // We're done, return the final noise value.
  return n_xy;
}


float perlinnoise3d( vec3 P )
{
	vec3 Pi = ONE*floor(P)+ONEHALF; // Integer part, scaled so +1 moves one texel
								  // and offset 1/2 texel to sample texel centers
	vec3 Pf = fract(P);     // Fractional part for interpolation

	// Noise contributions from (x=0, y=0), z=0 and z=1
	float perm00 = texture2D(noise_permTexture, Pi.xy).a ;
	vec3  grad000 = texture2D(noise_permTexture, vec2(perm00, Pi.z)).rgb * 4.0 - 1.0;
	float n000 = dot(grad000, Pf);
	vec3  grad001 = texture2D(noise_permTexture, vec2(perm00, Pi.z + ONE)).rgb * 4.0 - 1.0;
	float n001 = dot(grad001, Pf - vec3(0.0, 0.0, 1.0));

	// Noise contributions from (x=0, y=1), z=0 and z=1
	float perm01 = texture2D(noise_permTexture, Pi.xy + vec2(0.0, ONE)).a ;
	vec3  grad010 = texture2D(noise_permTexture, vec2(perm01, Pi.z)).rgb * 4.0 - 1.0;
	float n010 = dot(grad010, Pf - vec3(0.0, 1.0, 0.0));
	vec3  grad011 = texture2D(noise_permTexture, vec2(perm01, Pi.z + ONE)).rgb * 4.0 - 1.0;
	float n011 = dot(grad011, Pf - vec3(0.0, 1.0, 1.0));

	// Noise contributions from (x=1, y=0), z=0 and z=1
	float perm10 = texture2D(noise_permTexture, Pi.xy + vec2(ONE, 0.0)).a ;
	vec3  grad100 = texture2D(noise_permTexture, vec2(perm10, Pi.z)).rgb * 4.0 - 1.0;
	float n100 = dot(grad100, Pf - vec3(1.0, 0.0, 0.0));
	vec3  grad101 = texture2D(noise_permTexture, vec2(perm10, Pi.z + ONE)).rgb * 4.0 - 1.0;
	float n101 = dot(grad101, Pf - vec3(1.0, 0.0, 1.0));

	// Noise contributions from (x=1, y=1), z=0 and z=1
	float perm11 = texture2D(noise_permTexture, Pi.xy + vec2(ONE, ONE)).a ;
	vec3  grad110 = texture2D(noise_permTexture, vec2(perm11, Pi.z)).rgb * 4.0 - 1.0;
	float n110 = dot(grad110, Pf - vec3(1.0, 1.0, 0.0));
	vec3  grad111 = texture2D(noise_permTexture, vec2(perm11, Pi.z + ONE)).rgb * 4.0 - 1.0;
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




float fbm3d( vec3 p, int octaves, float lacunarity, float gain )
{
	float amp = 1.0;
	float sum = 0.0;
	vec3 point = p;
	for(int i = 0; i<octaves; ++i)
	{
		float noise = perlinnoise3d(point);
		sum += (noise * amp);
		amp *= gain;
		point *= lacunarity;
	}
	return sum;
}
