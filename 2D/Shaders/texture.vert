// Version du GLSL

#version 100

// Entre Shader (lie au tableau Vertex Attrib)
attribute vec3 in_Vertex;
attribute vec4 in_Color;
attribute vec2 in_TexCoord0;

// Uniform
uniform mat4 projection;
uniform mat4 modelview;

// Sortie
varying vec4 vColor;
varying vec2 vTexCoord0;

// Fonction main
void main()
{
    // Position finale du vertex en 3D
    gl_Position = projection * modelview * vec4(in_Vertex, 1.0);

    // Send the color to fragment shader, not used but must DO
    vColor = in_Color;

    // Envoi de la texture au Fragment Shader
    vTexCoord0 = in_TexCoord0;
}
