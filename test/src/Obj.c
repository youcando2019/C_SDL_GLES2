#include "Obj.h"

void Obj_init(Obj * me, char* const path, char* const path_image, vec3 color, vec3 position, vec3 angle, vec3 scale, char* const vertexShader, char* const fragmentShader)
{
    Shader_init(&me->m_shader, vertexShader, fragmentShader);
    Shader_charger(&me->m_shader);

    me->m_path_image = path_image;
    if(strcmp(me->m_path_image, "") == 0)
    {
        glm_vec3_copy(color, me->m_color);
    }
    else
    {
        Texture_init(&me->m_texture, me->m_path_image);
        Texture_charger(&me->m_texture);
    }

    me->m_vertices_indices = malloc(0 * sizeof(vec3));
    me->m_uvs_indices = malloc(0 * sizeof(vec2));
    me->m_normals_indices = malloc(0 * sizeof(vec3));

    me->m_vertices = malloc(0 * sizeof(float));
    me->m_uvs = malloc(0 * sizeof(float));
    me->m_normals = malloc(0 * sizeof(float));

    me->m_total_vertex = 0;

    me->m_colors = malloc(0 * sizeof(float));

    if(strcmp(me->m_path_image, "") == 0)
    {
        Obj_load_colored(me, path);
    }
    else
    {
        Obj_load(me, path);
    }

    glm_vec3_copy(position, me->m_position);
    glm_vec3_copy(angle, me->m_angle);
    glm_vec3_copy(scale, me->m_scale);
}

void Obj_destroy(Obj * me)
{
    free(me->m_colors);
    free(me->m_normals_indices);
    free(me->m_uvs_indices);
    free(me->m_vertices_indices);
    free(me->m_normals);
    free(me->m_uvs);
    free(me->m_vertices);
}

void Obj_afficher(Obj * me, mat4 projection, mat4 view, mat4 model_f)
{
    mat4 model;
    glm_mat4_copy(model_f, model);
    glm_translate(model, me->m_position);
    glm_rotate(model, me->m_angle[0], (vec3){1, 0, 0});
    glm_rotate(model, me->m_angle[1], (vec3){0, 1, 0});
    glm_rotate(model, me->m_angle[2], (vec3){0, 0, 1});
    glm_scale(model, me->m_scale);

    // Activation du shader
    glUseProgram(Shader_getProgramID(&me->m_shader));

        // On remplie puis on active le tableau Vertex Attrib 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, me->m_vertices);
        glEnableVertexAttribArray(0);

        if(strcmp(me->m_path_image, "") == 0)
        {
            // Envoi des coordonnées de color
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, me->m_colors);
            glEnableVertexAttribArray(1);
        }
        else
        {
            // Envoi des coordonnées de texture
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, me->m_uvs);
            glEnableVertexAttribArray(2);
        }

        // Envoi des coordonnées de normal
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, me->m_normals);
        glEnableVertexAttribArray(3);

        // On envoie les matrices au shader
        glUniformMatrix4fv(glGetUniformLocation(Shader_getProgramID(&me->m_shader), "projection"), 1, GL_FALSE, *projection);
        glUniformMatrix4fv(glGetUniformLocation(Shader_getProgramID(&me->m_shader), "view"), 1, GL_FALSE, *view);
        glUniformMatrix4fv(glGetUniformLocation(Shader_getProgramID(&me->m_shader), "model"), 1, GL_FALSE, *model);

        if(strcmp(me->m_path_image, "") != 0)
        {
            // Verrouillage de la texture
            glBindTexture(GL_TEXTURE_2D, Texture_getID(&me->m_texture));
        }

                // On affiche le triangle
                glDrawArrays(GL_TRIANGLES, 0, me->m_total_vertex);

        if(strcmp(me->m_path_image, "") != 0)
        {
            // Déverrouillage de la texture
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        // On désactive le tableau Vertex Attrib puisque l'on en a plus besoin
        glDisableVertexAttribArray(3);
        if(strcmp(me->m_path_image, "") != 0)
        {
            glDisableVertexAttribArray(2);
        }
        else
        {
            glDisableVertexAttribArray(1);
        }
        glDisableVertexAttribArray(0);

    // Désactivation du shader
    glUseProgram(0);
}

int Obj_load_colored(Obj * me, const char * path)
{
    unsigned int v = 0;
    unsigned int vn = 0;
    unsigned int f = 0;

    FILE * file = fopen(path, "r");
    if( file == NULL )
    {
        printf("Impossible to open the file in path : %s !\n", path);
        return 0;
    }
    while( 1 )
    {
        char lineHeader[128];

        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
        {
            break; // EOF = End Of File. Quit the loop.
        }

        // else : parse lineHeader
        if(strcmp(lineHeader, "v") == 0)
        {
            v += 1;
            me->m_vertices_indices = realloc(me->m_vertices_indices, v * sizeof(vec3));
            fscanf(file, "%f %f %f\n", &me->m_vertices_indices[v-1][0], &me->m_vertices_indices[v-1][1], &me->m_vertices_indices[v-1][2] );
        }
        else if ( strcmp( lineHeader, "vn" ) == 0 )
        {
            vn += 1;
            me->m_normals_indices = realloc(me->m_normals_indices, vn * sizeof(vec3));
            fscanf(file, "%f %f %f\n", &me->m_normals_indices[vn-1][0], &me->m_normals_indices[vn-1][1], &me->m_normals_indices[vn-1][2] );
        }
        else if ( strcmp( lineHeader, "f" ) == 0 )
        {
            unsigned int vertexIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2] );
            if (matches != 6)
            {
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return 0;
            }

            me->m_total_vertex += 3;
            f += 9;
            me->m_vertices = realloc(me->m_vertices, f * sizeof(float));
            me->m_vertices[f - 9] = me->m_vertices_indices[vertexIndex[0]-1][0];
            me->m_vertices[f - 8] = me->m_vertices_indices[vertexIndex[0]-1][1];
            me->m_vertices[f - 7] = me->m_vertices_indices[vertexIndex[0]-1][2];
            me->m_vertices[f - 6] = me->m_vertices_indices[vertexIndex[1]-1][0];
            me->m_vertices[f - 5] = me->m_vertices_indices[vertexIndex[1]-1][1];
            me->m_vertices[f - 4] = me->m_vertices_indices[vertexIndex[1]-1][2];
            me->m_vertices[f - 3] = me->m_vertices_indices[vertexIndex[2]-1][0];
            me->m_vertices[f - 2] = me->m_vertices_indices[vertexIndex[2]-1][1];
            me->m_vertices[f - 1] = me->m_vertices_indices[vertexIndex[2]-1][2];

            me->m_normals = realloc(me->m_normals, f * sizeof(float));
            me->m_normals[f - 9] = me->m_normals_indices[normalIndex[0]-1][0];
            me->m_normals[f - 8] = me->m_normals_indices[normalIndex[0]-1][1];
            me->m_normals[f - 7] = me->m_normals_indices[normalIndex[0]-1][2];
            me->m_normals[f - 6] = me->m_normals_indices[normalIndex[1]-1][0];
            me->m_normals[f - 5] = me->m_normals_indices[normalIndex[1]-1][1];
            me->m_normals[f - 4] = me->m_normals_indices[normalIndex[1]-1][2];
            me->m_normals[f - 3] = me->m_normals_indices[normalIndex[2]-1][0];
            me->m_normals[f - 2] = me->m_normals_indices[normalIndex[2]-1][1];
            me->m_normals[f - 1] = me->m_normals_indices[normalIndex[2]-1][2];
        }
    }

    // Create Colors
    me->m_colors = realloc(me->m_colors, me->m_total_vertex * 3 * sizeof(float));
    Obj_set_color(me, me->m_color);

    return 1;
}

int Obj_load(Obj * me, const char * path)
{
    unsigned int v = 0;
    unsigned int vt = 0;
    unsigned int vn = 0;
    unsigned int f = 0;
    unsigned int ft = 0;

    FILE * file = fopen(path, "r");
    if( file == NULL )
    {
        printf("Impossible to open the file !\n");
        return 0;
    }
    while( 1 )
    {
        char lineHeader[128];

        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
        {
            break; // EOF = End Of File. Quit the loop.
        }

        // else : parse lineHeader
        if(strcmp(lineHeader, "v") == 0)
        {
            v += 1;
            me->m_vertices_indices = realloc(me->m_vertices_indices, v * sizeof(vec3));
            fscanf(file, "%f %f %f\n", &me->m_vertices_indices[v-1][0], &me->m_vertices_indices[v-1][1], &me->m_vertices_indices[v-1][2] );
        }
        else if ( strcmp( lineHeader, "vt" ) == 0 )
        {
            vt += 1;
            me->m_uvs_indices = realloc(me->m_uvs_indices, vt * sizeof(vec2));
            fscanf(file, "%f %f\n", &me->m_uvs_indices[vt-1][0], &me->m_uvs_indices[vt-1][1] );
        }
        else if ( strcmp( lineHeader, "vn" ) == 0 )
        {
            vn += 1;
            me->m_normals_indices = realloc(me->m_normals_indices, vn * sizeof(vec3));
            fscanf(file, "%f %f %f\n", &me->m_normals_indices[vn-1][0], &me->m_normals_indices[vn-1][1], &me->m_normals_indices[vn-1][2] );
        }
        else if ( strcmp( lineHeader, "f" ) == 0 )
        {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9)
            {
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return 0;
            }
            me->m_total_vertex += 3;
            f += 9;
            me->m_vertices = realloc(me->m_vertices, f * sizeof(float));
            me->m_vertices[f - 9] = me->m_vertices_indices[vertexIndex[0]-1][0];
            me->m_vertices[f - 8] = me->m_vertices_indices[vertexIndex[0]-1][1];
            me->m_vertices[f - 7] = me->m_vertices_indices[vertexIndex[0]-1][2];
            me->m_vertices[f - 6] = me->m_vertices_indices[vertexIndex[1]-1][0];
            me->m_vertices[f - 5] = me->m_vertices_indices[vertexIndex[1]-1][1];
            me->m_vertices[f - 4] = me->m_vertices_indices[vertexIndex[1]-1][2];
            me->m_vertices[f - 3] = me->m_vertices_indices[vertexIndex[2]-1][0];
            me->m_vertices[f - 2] = me->m_vertices_indices[vertexIndex[2]-1][1];
            me->m_vertices[f - 1] = me->m_vertices_indices[vertexIndex[2]-1][2];

            ft += 6;
            me->m_uvs = realloc(me->m_uvs, ft * sizeof(float));
            me->m_uvs[ft - 6] = me->m_uvs_indices[uvIndex[0]-1][0];
            me->m_uvs[ft - 5] = me->m_uvs_indices[uvIndex[0]-1][1];
            me->m_uvs[ft - 4] = me->m_uvs_indices[uvIndex[1]-1][0];
            me->m_uvs[ft - 3] = me->m_uvs_indices[uvIndex[1]-1][1];
            me->m_uvs[ft - 2] = me->m_uvs_indices[uvIndex[2]-1][0];
            me->m_uvs[ft - 1] = me->m_uvs_indices[uvIndex[2]-1][1];

            me->m_normals = realloc(me->m_normals, f * sizeof(float));
            me->m_normals[f - 9] = me->m_normals_indices[normalIndex[0]-1][0];
            me->m_normals[f - 8] = me->m_normals_indices[normalIndex[0]-1][1];
            me->m_normals[f - 7] = me->m_normals_indices[normalIndex[0]-1][2];
            me->m_normals[f - 6] = me->m_normals_indices[normalIndex[1]-1][0];
            me->m_normals[f - 5] = me->m_normals_indices[normalIndex[1]-1][1];
            me->m_normals[f - 4] = me->m_normals_indices[normalIndex[1]-1][2];
            me->m_normals[f - 3] = me->m_normals_indices[normalIndex[2]-1][0];
            me->m_normals[f - 2] = me->m_normals_indices[normalIndex[2]-1][1];
            me->m_normals[f - 1] = me->m_normals_indices[normalIndex[2]-1][2];
        }
    }

    // Create Colors
    me->m_colors = realloc(me->m_colors, me->m_total_vertex * 3 * sizeof(float));
    for(int i=0; i<(int)me->m_total_vertex*3; i+=3)
    {
        me->m_colors[i] = me->m_color[0];
        me->m_colors[i+1] = me->m_color[1];
        me->m_colors[i+2] = me->m_color[2];
    }

    return 1;
}


void Obj_move(Obj * me, vec3 distance)
{
    glm_vec3_add(me->m_position, distance, me->m_position);
}

void Obj_rotate(Obj * me, vec3 angle)
{
    glm_vec3_add(me->m_angle, angle, me->m_angle);
    if(me->m_angle[0] >= 360)
    {
        me->m_angle[0] -= 360;
    }
    else if(me->m_angle[0] <= -360)
    {
        me->m_angle[0] += 360;
    }
    if(me->m_angle[1] >= 360)
    {
        me->m_angle[1] -= 360;
    }
    else if(me->m_angle[1] <= -360)
    {
        me->m_angle[1] += 360;
    }
    if(me->m_angle[2] >= 360)
    {
        me->m_angle[2] -= 360;
    }
    else if(me->m_angle[2] <= -360)
    {
        me->m_angle[2] += 360;
    }
}

void Obj_scale(Obj * me, vec3 scale)
{
    glm_vec3_add(me->m_scale, scale, me->m_scale);
}

void Obj_set_color(Obj * me, vec3 color)
{
    glm_vec3_copy(color, me->m_color);
    for(int i=0; i<(int)me->m_total_vertex*3; i+=3)
    {
        me->m_colors[i] = me->m_color[0];
        me->m_colors[i+1] = me->m_color[1];
        me->m_colors[i+2] = me->m_color[2];
    }
}
