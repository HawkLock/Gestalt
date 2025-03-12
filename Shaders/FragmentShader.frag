#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D targetTexture;
uniform bool isPaused;
uniform float tintIntensity;

void main()
{
    vec4 color = texture(targetTexture, TexCoord); // Fixed typo

    if (isPaused) {
        float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114)); // Compute grayscale
        color = vec4(vec3(gray), color.a); // Convert back to vec4
    }

    color = mix(color, vec4(0, 1.0, 0.0, 1.0), tintIntensity);

    FragColor = color;
}
