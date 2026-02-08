#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out vec3 vNormal;
out vec3 vFragPos;

void main()
{
    vec4 worldPos = u_Model * vec4(aPos, 1.0);
    gl_Position = u_ViewProjection * worldPos;
    
    vFragPos = vec3(worldPos);
    vNormal = mat3(transpose(inverse(u_Model))) * aNormal; // normal transform
}