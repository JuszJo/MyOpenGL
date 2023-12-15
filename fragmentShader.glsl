#version 330 core

in vec2 myTextureCoord;

out vec4 fragColor;

uniform sampler2D myTexture;

void main() {
    // fragColor = vec4(0.7, 0.3, 0.3, 1.0);
    fragColor = texture(myTexture, myTextureCoord);
}