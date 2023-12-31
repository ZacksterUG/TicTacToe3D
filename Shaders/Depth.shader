#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;

void main()
{
	gl_Position = model * vec4(aPos, 1.0);
};

#shader fragment
#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
	float lightDistance = length(FragPos.xyz - lightPos);
	lightDistance = lightDistance / far_plane;
	gl_FragDepth = lightDistance;
};

#shader geometry
#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos;

void main()
{
	for (int face = 0; face < 6; ++face)
	{
		gl_Layer = face;
		for (int i = 0; i < 3; ++i)
		{
			FragPos = gl_in[i].gl_Position;
			gl_Position = shadowMatrices[face] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
};