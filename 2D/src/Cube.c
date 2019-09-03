#include "Cube.h"

void Cube_init(float taille, char* const image, float repetition, char* const vertexShader, char* const fragmentShader, Cube * me)
{
    Shader_init(&me->m_shader, vertexShader, fragmentShader);
    Texture_init(&me->m_texture, image);
    Shader_charger(&me->m_shader);
    Texture_charger(&me->m_texture);

    // Division du paramètre taille
    taille /= 2;

    // Vertices temporaires
    float verticesTmp[] = {-taille, -taille, -taille, taille, -taille, -taille, taille, taille, -taille, // Face 1
                        -taille, -taille, -taille, -taille, taille, -taille, taille, taille, -taille, // Face 1
                        taille, -taille, taille, taille, -taille, -taille, taille, taille, -taille, // Face 2
                        taille, -taille, taille, taille, taille, taille, taille, taille, -taille, // Face 2
                        -taille, -taille, taille, taille, -taille, taille, taille, -taille, -taille, // Face 3
                        -taille, -taille, taille, -taille, -taille, -taille, taille, -taille, -taille, // Face 3
                        -taille, -taille, taille, taille, -taille, taille, taille, taille, taille, // Face 4
                        -taille, -taille, taille, -taille, taille, taille, taille, taille, taille, // Face 4
                        -taille, -taille, taille, -taille, -taille, -taille, -taille, taille, -taille, // Face 5
                        -taille, -taille, taille, -taille, taille, taille, -taille, taille, -taille, // Face 5
                        -taille, taille, taille, taille, taille, taille, taille, taille, -taille, // Face 6
                        -taille, taille, taille, -taille, taille, -taille, taille, taille, -taille}; // Face 6

    // Couleurs temporaires
    float coordTextureTmp[] = {0.0, 0.0, 1.0, 0.0, 1.0, 1.0, // Face 1
                            0.0, 0.0, 0.0, 1.0, 1.0, 1.0, // Face 1
                            0.0, 0.0, 1.0, 0.0, 1.0, 1.0, // Face 2
                            0.0, 0.0, 0.0, 1.0, 1.0, 1.0, // Face 2
                            0.0, 0.0, 1.0, 0.0, 1.0, 1.0, // Face 3
                            0.0, 0.0, 0.0, 1.0, 1.0, 1.0, // Face 3
                            0.0, 0.0, 1.0, 0.0, 1.0, 1.0, // Face 4
                            0.0, 0.0, 0.0, 1.0, 1.0, 1.0, // Face 4
                            0.0, 0.0, 1.0, 0.0, 1.0, 1.0, // Face 5
                            0.0, 0.0, 0.0, 1.0, 1.0, 1.0, // Face 5
                            0.0, 0.0, 1.0, 0.0, 1.0, 1.0, // Face 6
                            0.0, 0.0, 0.0, 1.0, 1.0, 1.0}; // Face 6

    // Copie des valeurs dans les tableaux finaux
    for(int i=0; i < 108; i++)
    {
        me->m_vertices[i] = verticesTmp[i];
        if(i < 72)
            me->m_coordTexture[i] = coordTextureTmp[i] * repetition;
    }
}

void Cube_destroy(Cube * me)
{

}

void Cube_afficher(mat4 * projection, mat4 * modelview, Cube * me)
{
    // Activation du shader
    glUseProgram(Shader_getProgramID(&me->m_shader));

        // On remplie puis on active le tableau Vertex Attrib 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, me->m_vertices);
        glEnableVertexAttribArray(0);

        // Envoi des coordonnées de texture
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, me->m_coordTexture);
        glEnableVertexAttribArray(2);

        // On envoie les matrices au shader
        glUniformMatrix4fv(glGetUniformLocation(Shader_getProgramID(&me->m_shader), "projection"), 1, GL_FALSE, *projection);
        glUniformMatrix4fv(glGetUniformLocation(Shader_getProgramID(&me->m_shader), "modelview"), 1, GL_FALSE, *modelview);

        // Verrouillage de la texture
        glBindTexture(GL_TEXTURE_2D, Texture_getID(&me->m_texture));

            // On affiche le triangle
            glDrawArrays(GL_TRIANGLES, 0, 36);

        // Déverrouillage de la texture
        glBindTexture(GL_TEXTURE_2D, 0);

        // On désactive le tableau Vertex Attrib puisque l'on en a plus besoin
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(0);

    // Désactivation du shader
    glUseProgram(0);
}

