#shader vertex
#version 330 core 
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
out vec3 fragNormal; // Передача нормали во фрагментный шейдер
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
void main() {
	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
	fragNormal = normal;
};

#shader fragment
#version 330 core
out vec4 color;
in vec3 fragNormal; // Нормали для каждого фрагмента
uniform vec3 lightDirection; // Направление света
uniform vec3 lightColor; // Цвет света
uniform float ambientStrength; // Сила фонового освещения
void main() {
	float diffuseStrength = max(dot(fragNormal, -lightDirection), 0.0); // Вычисление диффузного освещения
	vec3 result = (ambientStrength + diffuseStrength) * lightColor; // Итоговый цвет с учетом освещения
	color = vec4(result, 1.0);
};
