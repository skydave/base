varying vec4 pw;  // position in worldspace
varying vec3 n;
varying vec2 uv;

varying vec3 ep; // eye position (on image plane) in worldspace
varying vec3 ed; // eye direction in worldspace


//uniform sampler2D color;
uniform sampler3D volumeData;



struct Ray
{
	vec3 origin;
	vec3 dir;
};

bool hitbox(Ray r, vec3 m1, vec3 m2, out float tmin, out float tmax) 
{
	float tymin, tymax, tzmin, tzmax; 
	float flag = 1.0; 

	if (r.dir.x >= 0.0) 
	{
		tmin = (m1.x - r.origin.x) / r.dir.x;
		tmax = (m2.x - r.origin.x) / r.dir.x;
	}
	else 
	{
		tmin = (m2.x - r.origin.x) / r.dir.x;
		tmax = (m1.x - r.origin.x) / r.dir.x;
	}
	if (r.dir.y >= 0.0) 
	{
		tymin = (m1.y - r.origin.y) / r.dir.y; 
		tymax = (m2.y - r.origin.y) / r.dir.y; 
	}
	else 
	{
		tymin = (m2.y - r.origin.y) / r.dir.y; 
		tymax = (m1.y - r.origin.y) / r.dir.y; 
	}

	if ((tmin > tymax) || (tymin > tmax)) flag = -1.0; 
	if (tymin > tmin) tmin = tymin; 
	if (tymax < tmax) tmax = tymax; 

	if (r.dir.z >= 0.0) 
	{
		tzmin = (m1.z - r.origin.z) / r.dir.z; 
		tzmax = (m2.z - r.origin.z) / r.dir.z; 
	}
	else 
	{
		tzmin = (m2.z - r.origin.z) / r.dir.z; 
		tzmax = (m1.z - r.origin.z) / r.dir.z; 
	}
	if ((tmin > tzmax) || (tzmin > tmax)) flag = -1.0; 
	if (tzmin > tmin) tmin = tzmin; 
	if (tzmax < tmax) tmax = tzmax; 

	return (flag > 0.0); 
}


void main()
{
	

	// intersect ray with box
	Ray ray;
	ray.origin = ep;
	ray.dir = normalize(ed);
	float tmin;
	float tmax;
	bool res = hitbox( ray, vec3(-0.5, -0.5, -0.5), vec3(0.5, 0.5, 0.5), tmin, tmax );
	

	vec3 c = vec3(0.0, 0.0, 0.0);
	if(res)
	{
		vec3 start, end;
		c = vec3(1.0, 0.0, 0.1);

		// do we sit within volume?
		if( tmin < 0.0)
		{
			// start from eye position
			start = ray.origin;
			end = ray.origin + ray.dir*tmax;
			c = vec3(0.0, 1.0, 0.6);
		}else
		{
			// start from tmin
			start = ray.origin + ray.dir*tmin;
			end = ray.origin + ray.dir*tmax;
		}
			
		// do ray marching
		vec4 value;
		float scalar;

		//Initialize accumulated color and opacity
		vec4 dst = vec4(0.0,0.0,0.0,0.0);

		//Determine volume entry position
		vec3 position = start;

		//Compute ray direction
		vec3 direction = ray.dir;
		float stepsize = 0.01;
		
		int numSteps = int(length(end - start)/stepsize);

		//Loop for ray traversal
		for(int i = 0;i < numSteps;i++) //Some large number
		{
			//Data access to scalare value in 3D volume texture
			value = texture3D(volumeData, position+vec3(0.5));
			scalar = value.x*0.1;

			//Apply transfer function
			//float4 src = tex1D(SamplerTransferFunction, scalar);
			vec4 src = vec4(scalar);

			//Front-to-back compositing
			dst = (1.0 - dst.a) * src + dst;

			//Advance ray position along ray direction
			position = position + direction * stepsize;
		}
		
		c = dst.xyz;
		
	}


	//vec3 c = texture2D(color,uv).xyz;
	//vec3 c = vec3(0.5, 0.5, 0.1);
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
	return;
}
