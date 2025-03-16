#version 330 core

layout(location = 0) in vec3 position;  // Pozitia initiala a vertexului
layout(location = 2) in vec3 color;  // Culoarea asociata vertexului

//  Variabilele calculate in acest shader si transmise in fragment
out vec3 fragColor;  // Culoarea vertexului
out float noiseValue;  // Valoarea de noise folosita pentru modificarea inaltimii

uniform mat4 model;  // Matricea de transformare a obiectului
uniform mat4 view;  // Matricea care defineste pozitia si orientarea camerei
uniform mat4 projection;  // Matricea de proiectie

// Genereaza o valoarea de noise aleatoare pentru a introduce variatii in inaltimea terenului
float Noise(vec2 uv) {
    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    vec3 newPosition = position;
    float frequency = 0.2; // Frecventa noise-ului
    noiseValue = Noise(newPosition.xz * frequency); 
    newPosition.y += noiseValue; // Modificam inaltimea

    fragColor = color; // Pasam culoarea in fragment
    gl_Position = projection * view * model * vec4(newPosition, 1.0);
}
