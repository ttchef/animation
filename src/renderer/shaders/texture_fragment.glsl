#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec4 uColor;
uniform sampler2D ourTexture;

void main() {
    vec4 texColor = texture(ourTexture, TexCoord);
    FragColor = texColor * uColor;
}
