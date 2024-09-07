#version 460

layout(binding = 0) uniform sampler2D colorTexture;

out vec4 FragColor;

void main() {
    FragColor = texture(colorTexture, gl_FragCoord.xy / vec2(textureSize(colorTexture, 0)));
}
