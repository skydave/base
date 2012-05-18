varying vec3      pw; // position in worldspace
varying vec3      pv; // position in eyespace
varying vec3       n; // normal in eyespace

uniform vec3       l; // vector into the direction of an infinite distance light in worldspace

uniform mat4   vminv;
uniform mat4     mvm;
uniform mat3 mvminvt;

uniform vec3 ambient;
uniform float     ka;
uniform vec3 diffuse;
uniform float     kd;


void main()
{
	vec4 result = vec4(0.0, 0.0, 0.0, 1.0);
	vec3 N = normalize(n);
	vec3 V = normalize(pv.xyz);
	vec3 L = normalize(mvminvt*l);


	// lambert shading ==============
	vec3 a = ka*ambient;
	vec3 d = kd*max(dot(N,L),0.0)*diffuse;

	if( dot(N,V) > 0.0 )
		result = vec4(a, 1.0);
	else
	{
		result = vec4(a,1.0) + vec4(d, 1.0);
	}


	gl_FragData[0] = result;
}