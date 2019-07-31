// Version du GLSL

#version 100


// Entre Shader (lie au tableau Vertex Attrib)
attribute vec3 in_Vertex;
attribute vec3 in_Color;


// Uniform

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// Sortie

varying vec3 out_Color;


// Fonction main

void main()
{
    // Position finale du vertex en 3D

    gl_Position = projection * view * model * vec4(in_Vertex, 1.0);
    //gl_Position = vec4(in_Vertex, 1.0);


    // Envoi de la couleur au Fragment Shader

    out_Color = in_Color;
}
