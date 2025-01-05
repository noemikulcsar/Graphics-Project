#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec4 fPosWorld;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;

out vec4 fColor;

// Uniforms
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform samplerCube environmentCubemap;  // Cubemap pentru reflexie
uniform sampler2D shadowMap;
uniform vec3 lightDir;  // Direcția luminii
uniform vec3 lightColor;  // Culoarea luminii
uniform vec3 viewPos;  // Poziția camerei (privitorului)
uniform mat4 view;  // Matricea de vizualizare
uniform float shininess;  // Șininess pentru iluminare
uniform float ambientStrength;  // Puterea iluminării ambientale

// Funcția pentru calculul reflexiei
vec3 getReflectedColor(vec3 viewDir, vec3 normal)
{
    // Direcția de reflexie
    vec3 reflectDir = reflect(viewDir, normal);
    // Alege culoarea din cubemap pe baza reflexiei
    return texture(environmentCubemap, reflectDir).rgb;
}

// Funcția pentru calculul iluminării și a umbrelor
vec3 computeLighting()
{
    vec3 ambient = ambientStrength * lightColor;

    // Calculul iluminării diffuse
    vec3 norm = normalize(fNormal);
    vec3 lightDirN = normalize(lightDir);
    float diff = max(dot(norm, lightDirN), 0.0f);
    vec3 diffuse = diff * lightColor;

    // Calculul iluminării speculare
    vec3 viewDir = normalize(viewPos - fPosWorld.xyz);
    vec3 reflectDir = reflect(-lightDirN, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
    vec3 specular = spec * lightColor;

    return ambient + diffuse + specular;
}

// Funcția pentru calculul umbrelor
float computeShadow()
{
    vec3 fragPosLightSpace = fragPosLightSpace.xyz / fragPosLightSpace.w;  // Normalizare pentru coordonatele de lumină
    fragPosLightSpace = fragPosLightSpace * 0.5 + 0.5;  // Transformare în intervalul [0, 1]

    float closestDepth = texture(shadowMap, fragPosLightSpace.xy).r;  // Adâncimea minimă a umbrei
    float currentDepth = fragPosLightSpace.z;  // Adâncimea curentă a fragmentului

    // Compară adâncimea curentă cu adâncimea minimă
    return currentDepth > closestDepth + 0.005 ? 1.0f : 0.0f;  // Afișează umbra dacă este cazul
}

void main()
{
    // Calculăm direcția privitorului (camera)
    vec3 viewDir = normalize(viewPos - fPosWorld.xyz);
    
    // Calculăm iluminarea pe obiect
    vec3 lighting = computeLighting();

    // Calculăm reflexia mediului
    vec3 reflectedColor = getReflectedColor(viewDir, normalize(fNormal));
    
    // Calculăm umbrele
    float shadow = computeShadow();
    
    // Combinăm iluminarea și reflexia
    vec3 finalColor = mix(lighting, reflectedColor, 0.5);  // Combinație între iluminare și reflexie
    finalColor *= (1.0 - shadow);  // Aplicăm umbrele

    // Setăm culoarea finală a fragmentului
    fColor = vec4(finalColor, 1.0f);
}
