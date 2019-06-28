// Version du GLSL
 
#version 100

// Précision pour les variables float
precision mediump float;

// Entre
varying vec3 out_Color;

// Fonction main
void main()
{
    // Couleur finale du pixel
    gl_FragColor = vec4(out_Color, 1.0);
}