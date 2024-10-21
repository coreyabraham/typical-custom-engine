// FRAGMENT SHADER
#version 450

// Make sure this is consistent with the program this shader is being used with!
const int lightCountMax = 100;

out vec4 PixelColour;

in vec3 interpolatedNormal;
in vec2 interpolatedUVs;
in vec3 worldPos;

in vec3 interpolatedTangent;
in vec3 interpolatedBiTangent;

uniform vec3 lightDirections[lightCountMax];
uniform vec3 lightColours[lightCountMax];

uniform float glossiness;
uniform vec3 ambientLight;

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;
uniform sampler2D normalSampler;

uniform vec3 cameraPos;

uniform int shaderOutput;
uniform int lightCount;

// FRAGMENT
void main()
{
	vec3 diffuseMaterialColour = texture(diffuseSampler, interpolatedUVs).rgb;
	vec3 specularMaterialColour = texture(specularSampler, interpolatedUVs).rgb;
	vec3 normalMaterialColour = texture(normalSampler, interpolatedUVs).rgb;
	
	vec3 trueTangent = normalize(interpolatedTangent);
	vec3 trueBiTangent = normalize(interpolatedBiTangent);
	
	vec3 geometricNormal = normalize(interpolatedNormal);
	mat3 TBN = mat3(trueTangent, trueBiTangent, geometricNormal);
	vec3 detailedNormal = normalize(TBN * (normalMaterialColour * 2 - 1));
	
	vec3 viewDirection = normalize(worldPos - cameraPos);
	
	vec3 diffuseLightIntensity;
	vec3 specularLightIntensity;
	
	if (lightCount < lightCountMax)
	{
		for (int i = 0; i < lightCount; i++)
		{	
			vec3 reflectedRay = reflect(lightDirections[i], detailedNormal) * -1;
		
			diffuseLightIntensity += clamp(dot(-lightDirections[i], detailedNormal), 0, 1) * lightColours[i];
			specularLightIntensity += pow(clamp(dot(viewDirection, reflectedRay), 0, 1), glossiness) * lightColours[i];
		}
	}
	
	//PixelColour = vec4(specularMaterialColour, 1.0); // Specular Only
	//PixelColour = vec4(diffuseMaterialColour, 1.0); // Diffuse Only
	//PixelColour = vec4(geometricNormal, 1.0); // Geometric Normal Only
	//PixelColour = vec4(detailedNormal, 1.0); // Detailed Normal Only
	
	PixelColour = vec4(diffuseMaterialColour * diffuseLightIntensity + specularMaterialColour * specularLightIntensity + ambientLight * diffuseMaterialColour, 1.0); // All Channels (Finalized Mapping)
}