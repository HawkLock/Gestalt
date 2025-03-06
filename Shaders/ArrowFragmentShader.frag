#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D targetTexture;
uniform float alpha;  // Add a uniform for controlling transparency

void main()
{
    vec4 texColor = texture(targetTexture, TexCoord);
    FragColor = vec4(texColor.rgb, texColor.a * alpha);  // Apply transparency
}
