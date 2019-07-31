// Version du GLSL

#version 100

// Entre Shader (lie au tableau Vertex Attrib)
attribute vec3 in_Vertex;
attribute vec3 in_Color;
attribute vec2 in_TexCoord0;
attribute vec3 in_Normal;

// Variable uniform
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 transpose_inverse_model;
uniform vec3 in_view_position;
uniform vec3 in_light_position;
uniform vec3 in_light_color;

// Sortie
varying vec3 position;
varying vec3 color;
varying vec2 texCoord0;
varying vec3 normal;
varying vec3 view_position;
varying vec3 light_position;
varying vec3 light_color;

// Fonction main
void main()
{
	// Envoi de la couleur au Fragment Shader
	position = vec3(model * vec4(in_Vertex, 1.0));
	color = in_Color;
	normal = in_Normal;
	view_position = in_view_position;
	light_position = in_light_position;
	light_color = in_light_color;
	
	// Envoi de la texture au Fragment Shader
    texCoord0 = in_TexCoord0;

	// Position finale du vertex
	gl_Position = projection * view * model * vec4(in_Vertex, 1.0);
}
