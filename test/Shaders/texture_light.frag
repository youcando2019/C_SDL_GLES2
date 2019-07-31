// Version du GLSL

#version 100

// Precision pour les variables float
precision mediump float; 

// Entre
varying vec3 position;
varying vec3 color;
varying vec2 texCoord0;
varying vec3 normal;
varying vec3 view_position;
varying vec3 light_position;
varying vec3 light_color;

// Uniform
uniform sampler2D texture0;

// Fonction main
void main()
{
    //AMBIENT
	float ambientStrength = 0.2f;
	vec3 ambient = ambientStrength * light_color;

	//DIFFUSE	
	vec3 norm = normalize(normal);
	vec3 light_direction = normalize(light_position - position);
	float diff = max(dot(norm, light_direction), 0.0);
	vec3 diffuse = diff * light_color;
	
	//SPECULAR
	float specularStrength = 0.8f;
	vec3 view_direction = normalize(view_position - position);
	vec3 reflect_direction = reflect(-light_direction, norm);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32.0);
	vec3 specular = specularStrength * spec * light_color;
	
	// Couleur finale du pixel
	vec4 result = vec4((ambient + diffuse + specular), 1.0) * texture2D(texture0, texCoord0);

    // Couleur finale du pixel
    gl_FragColor = result;
}
