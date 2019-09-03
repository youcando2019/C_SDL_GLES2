// Version du GLSL
 
#version 100


// Entrée Shader (liée au tableau Vertex Attrib)
attribute vec2 in_Vertex;
attribute vec3 in_Color;

// Sortie
varying vec3 out_Color;

// Fonction main
void main()
{
    // Position finale du vertex
    gl_Position = vec4(in_Vertex, 0.0, 1.0);

    out_Color = in_Color;
}