// Version du GLSL

#version 100

// Precision pour les variables float
precision mediump float; 

// Entre
varying vec2 vTexCoord0;

// Uniform
uniform sampler2D texture0;

// Fonction main
void main()
{
    // Couleur finale du pixel
    gl_FragColor = texture2D(texture0, vTexCoord0);
}
