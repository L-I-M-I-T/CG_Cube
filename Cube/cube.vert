//vertex shader
#version 330 core
layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 Color;
layout(location = 2) in vec3 Normal;
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model; 
uniform vec3 LightPos;
uniform vec3 LightColor;
out vec3 vertexColor;
out vec3 vertexNormal;
out vec3 vertexPos;
out vec3 lightPos;
out vec3 lightColor;

void main()
{
	gl_Position = Projection * View * Model * vec4(Pos.x, Pos.y, Pos.z, 1.0);
	vertexColor = Color;
	vertexNormal = Normal;
	vertexPos = vec3(Model * vec4(Pos, 1.0));
	lightPos = LightPos;
	lightColor = LightColor;
}