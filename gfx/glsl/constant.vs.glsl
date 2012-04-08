
attribute vec3 P;


uniform mat4 mvpm;

void main()
{
	gl_Position = mvpm * vec4(P,1.0);
}
