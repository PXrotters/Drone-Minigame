#version 330 core

in vec3 fragColor;  // Culoarea interpolata a fragmentului din vertex
in float noiseValue;  // Valoarea de zgomot generata in vertex

out vec4 outColor;  // Culoarea finala a fragmentului afisata pe ecran

void main() {
    vec3 baseColor = vec3(0, 0.4, 0);
    vec3 topColor = vec3(0.4, 0.8, 0);
    outColor = vec4(mix(baseColor, topColor, noiseValue), 1.0);  // Realizeaza o interpolare liniara intre 2 culori
}
