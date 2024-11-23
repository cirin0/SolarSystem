#version 330 core

out vec4 FragColor; // Вихідний колір фрагменту

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D ourTexture; //  Rename for clarity
uniform vec3 objectColor;
uniform bool hasTexture;  // Rename for clarity

void main() {
    //vec2 flippedTexCoords = vec2(TexCoords.x, 1.0 - TexCoords.y);
    vec3 color = objectColor;
    if (hasTexture)
    {
        color = texture(ourTexture, TexCoords).rgb;
    } else {
        color = objectColor;
    }
    FragColor = vec4(color, 1.0);
}