varying vec4 pw;  // position in worldspace
//varying vec3 n;
varying vec2 uv;

uniform sampler2D color;


void main()
{
	vec3 c = texture2D(color,uv).xyz;
	//vec3 c = vec3(uv.x, uv.y, 0.1);
	//vec3 n = vec3(0.1, 0.1, 0.1);
	//gl_FragData[0] = vec4(n.x, n.y, n.z, 1.0);
	//gl_FragColor = vec4(n.x, n.y, n.z, 1.0);
	//gl_FragColor = vec4(c.x, c.y, c.z, 1.0);
	//gl_FragColor = vec4(1.0) - vec4(c.x)*0.75;
	gl_FragColor = vec4(c.x, c.y, c.z, 1.0);
	//gl_FragColor = vec4(texture2D(ao,uv).w);
	//gl_FragColor = vec4( 0.2, 0.5, 0.3, 1.0);
	//gl_FragColor = vec4( position.x, position.y, position.z, 1.0);
	//gl_FragData[0] = vec4(1.0/float(tmp), 0.0, 0.0, 1.0);
	//gl_FragData[0] = vec4(1.0, 0.0, 0.0, 1.0);
	//gl_FragData[1] = vec4(pw.x, pw.y, pw.z, (gl_FragCoord.z / gl_FragCoord.w));
	//gl_FragData[2] = vec4(n.x, n.y, n.z, 1.0);
	//gl_FragData[3] = vec4(uv.x, uv.y, 0.0, 1.0);
}
