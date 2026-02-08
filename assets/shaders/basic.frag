#version 330 core

in vec3 vNormal;
in vec3 vFragPos;

out vec4 FragColor;

uniform vec3 u_Color;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

void main()
{
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(u_LightPos - vFragPos);
    vec3 viewDir = normalize(u_ViewPos - vFragPos);
    
    // Ambient
    float ambient = 0.15;
    
    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    
    // Specular (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    
    vec3 result = (ambient + diff + 0.5 * spec) * u_Color;
    FragColor = vec4(result, 1.0);
}