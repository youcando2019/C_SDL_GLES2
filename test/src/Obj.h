#ifndef OBJ_H_INCLUDED
#define OBJ_H_INCLUDED

    // Include OpenGL ES iOS
#ifdef __IPHONE_NA
#include <OpenGLES/ES2/gl.h>

// Include OpenGL ES Android
#elif __ANDROID__
#include <GLES2/gl2.h>

#elif WIN32
    #include <glad/glad.h>
    #include <KHR/khrplatform.h>

#else
#include <GLES2/gl2.h>

#endif

// Includes CGLM
#include <cglm/cglm.h>

#include <stdio.h>
#include <stdlib.h>
#include "Shader.h"
#include "Texture.h"
#include <malloc.h>

typedef struct
{
    // Attributs
    Shader m_shader;
    Texture m_texture;
    char* m_path_image;
    vec3* m_vertices_indices;
    vec2* m_uvs_indices;
    vec3* m_normals_indices;
    float* m_vertices;
    float* m_uvs;
    float* m_normals;
    unsigned int m_total_vertex;
    vec3 m_color;
    float* m_colors;
    vec3 m_position;
    vec3 m_angle;
    vec3 m_scale;
} Obj;

void Obj_init(Obj * me, char* const path, char* const path_image, vec3 color, vec3 position, vec3 angle, vec3 scale, char* const vertexShader, char* const fragmentShader);
void Obj_destroy(Obj * me);
void Obj_afficher(Obj * me, mat4 projection, mat4 view, mat4 model_f, vec3 view_position);
int Obj_load_colored(Obj * me, const char * path);
int Obj_load(Obj * me, const char * path);
void Obj_move(Obj * me, vec3 distance);
void Obj_rotate(Obj * me, vec3 angle);
void Obj_scale(Obj * me, vec3 scale);
void Obj_set_color(Obj * me, vec3 color);


#endif // OBJ_H_INCLUDED
