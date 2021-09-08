//fragment shader
#version 330 core
in vec3 vertexColor;
in vec3 vertexNormal;
in vec3 vertexPos;
in vec3 lightPos;
in vec3 lightColor;
out vec4 FragColor;

void main()
{
	vec3 lightDir = normalize(lightPos - vertexPos);
	float diff = max(dot(normalize(vertexNormal), lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	vec3 ambient = vertexColor * 0.5;
	vec3 result = (ambient + diffuse) * vertexColor;
	FragColor = vec4(result, 1.0);
}								