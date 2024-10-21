// VERTEX SHADER
#version 450

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 vertUVs;
layout (location = 3) in vec4 vertTangent;

uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;

out vec3 interpolatedNormal;
out vec2 interpolatedUVs;

out vec3 interpolatedTangent;
out vec3 interpolatedBiTangent;

out vec3 worldPos;

// VERTEX
void main()
{
	interpolatedUVs = vertUVs;
	interpolatedNormal = (modelMatrix * vec4(vertNormal, 0.0)).xyz;
	
	interpolatedTangent = (modelMatrix * vec4(vertTangent.xyz,0)).xyz;
	interpolatedBiTangent = cross(interpolatedNormal, interpolatedTangent) * vertTangent.w;
	
	worldPos = (modelMatrix * vec4(vertPos, 1.0)).xyz;
	gl_Position = mvpMatrix * vec4(vertPos, 1.0);
}