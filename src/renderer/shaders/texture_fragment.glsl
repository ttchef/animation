#version 460 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform vec4 uColor;
uniform sampler2D ourTexture;

void main() {
    FragColor = texture(ourTexture, TexCoord);
}
