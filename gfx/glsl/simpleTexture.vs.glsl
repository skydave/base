attribute vec3 P;
attribute vec2 UV;
varying vec2 uv;
void main()
{
	uv = UV;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(P,1.0);
}
