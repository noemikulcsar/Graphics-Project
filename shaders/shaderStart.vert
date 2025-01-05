#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

out vec3 fNormal;
out vec4 fPosEye;
out vec4 fPosWorld;
out vec2 fTexCoords;
out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 lightSpaceTrMatrix; // Transform matrix for light space

void main() 
{
    // Transform position to light space for shadow mapping
    fragPosLightSpace = lightSpaceTrMatrix * model * vec4(vPosition, 1.0f);

    // Compute eye space coordinates
    fPosEye = view * model * vec4(vPosition, 1.0f);

    // Transform the normal to eye space using the normal matrix
    fNormal = normalize(normalMatrix * vNormal);
    // Pass texture coordinates
    fTexCoords = vTexCoords;

    // Final position in clip space
    gl_Position = projection * view * model * vec4(vPosition, 1.0f);
}
