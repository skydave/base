varying vec4 pw;  // position in worldspace
varying vec3 n;
varying vec2 uv;

vec3 lightPos1 = vec3(10.0, 10.0, 10.0);
vec3 lightDir = vec3(-1.0, -1.0, -1.0);
uniform mat4 vminv;

vec3 getCameraPos()
{
	// vmatrix transforms from camera to world space
	// vmatrix inverse transforms from world to camera space
	// we transpose it to inverse the inverse
	mat4 t = transpose(vminv);
	return vec3( t[0].w, t[1].w, t[2].w );
}

vec4 phong(in vec3 n,in vec3 v,in vec3 l)
{
	vec3      ambient;  // ambient color
	vec3      diffuse;  // diffuse color
	vec3      specular; // specular color
	float     exponent; // exponent value
	float           ka;
	float           kd;
	float           ks;
	ambient = vec3( 0.05, 0.05, 0.05 ); ka = 1.0;
	diffuse = vec3( 0.5, 0.5, 0.5 ); kd = 1.0;
	specular = vec3( 1.0, 1.0, 1.0 ); ks = 0.0;
	exponent = 10.1;
	vec3 a = ka*ambient;
	vec3 d = kd*max(dot(n,l),0.0)*diffuse;
	vec3 s = exponent==0.0 ? vec3(0.0) : ks*pow(max(dot(reflect(-l,n),v),0.0),exponent)*specular;

	return vec4(a+d+s,1.0);
}


void main()
{
	//vec3 L = normalize(lightPos1 - pw.xyz);
	vec3 L = normalize(-lightDir);
	vec3 N = normalize(n);
	vec3 V = normalize(getCameraPos() - pw.xyz);

	gl_FragData[0] = phong(N, V, L);
}
