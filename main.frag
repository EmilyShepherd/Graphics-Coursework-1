#version 150
precision highp float;

uniform vec3 color;
uniform int  shading;
uniform sampler2D myTextureSampler;

in vec4 normal;
in vec2 UV;
out vec4 FragColor;

void main(void)
{
    if (shading == 0)
    {
        FragColor = vec4(color, 0.0);
    }
    else
	{
		vec4 C;
		
		if (shading == 1)
        {
			C = vec4(color, 0.0);
		}
        else
        {
			C = texture(myTextureSampler, UV);
		}
		
		vec4 L = vec4(0.0, 0.0, -1.0, 0.0);
		vec4 N = normalize(normal);
		
		vec4 AmbientColor  = vec4(0.1) * C;
        vec4 DiffuseColor  = C * clamp(dot(N, L), 0, 1);
		
		FragColor = AmbientColor + DiffuseColor;
	}
}
