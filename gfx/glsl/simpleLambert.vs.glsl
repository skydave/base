attribute vec3     P;
attribute vec3     N;

varying vec3      pv; // position in eyespace
varying vec3      pw; // position in  worldspace
varying vec3       n; // normal in eyespace

uniform mat4    mvpm;
uniform mat4     mvm;
uniform mat3 mvminvt;

void main()
{
	pw = P; // position in  worldspace
	pv = (mvm * vec4(P,1.0)).xyz; // position in eyespace
	n = mvminvt * N; // normal in eyespace
	gl_Position = mvpm * vec4(P, 1.0);
}