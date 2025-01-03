#version 330 core

out vec4 fragColor;

uniform vec3 particleColor; // Culoarea particulelor

void main() {
    fragColor = vec4(particleColor, 1.0); // Alpha control pentru transparență
}
