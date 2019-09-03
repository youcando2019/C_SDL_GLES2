#include "Button.h"

void Button_init(Button * self, int width, int height, char* const image, char* const vertexShader, char* const fragmentShader)
{
    Shader_init(&self->m_shader, vertexShader, fragmentShader);
    Texture_init(&self->m_texture, image);
    Shader_charger(&self->m_shader);
    Texture_charger(&self->m_texture);

    // Division du paramètre taille
    self->m_width = width;
    self->m_height = height;

    float taille_w = ((float)width * pixel) / 2.0;
    float taille_h = ((float)height * pixel) / 2.0;

    // Vertices temporaires
    float verticesTmp[] = {-taille_w, -taille_h, taille_w, -taille_h, taille_w, taille_h,
                           -taille_w, -taille_h, -taille_w, taille_h, taille_w, taille_h};

    // Couleurs temporaires
    float coordTextureTmp[] = {0.0, 0.0, 1.0, 0.0, 1.0, 1.0,
                               0.0, 0.0, 0.0, 1.0, 1.0, 1.0};

    // Copie des valeurs dans les tableaux finaux
    for(int i=0; i < 12; i++)
    {
        self->m_vertices[i] = verticesTmp[i];
        self->m_coordTexture[i] = coordTextureTmp[i];
    }
}

void Button_destroy(Button * self)
{

}

void Button_afficher(Button * self, mat4 * projection, mat4 * modelview)
{
    // Activation du shader
    glUseProgram(Shader_getProgramID(&self->m_shader));

        // On remplie puis on active le tableau Vertex Attrib 0
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, self->m_vertices);
        glEnableVertexAttribArray(0);

        // Envoi des coordonnées de texture
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, self->m_coordTexture);
        glEnableVertexAttribArray(2);

        // On envoie les matrices au shader
        glUniformMatrix4fv(glGetUniformLocation(Shader_getProgramID(&self->m_shader), "projection"), 1, GL_FALSE, **projection);
        glUniformMatrix4fv(glGetUniformLocation(Shader_getProgramID(&self->m_shader), "modelview"), 1, GL_FALSE, **modelview);

        // Verrouillage de la texture
        glBindTexture(GL_TEXTURE_2D, Texture_getID(&self->m_texture));

            // On affiche le triangle
            glDrawArrays(GL_TRIANGLES, 0, 6);

        // Déverrouillage de la texture
        glBindTexture(GL_TEXTURE_2D, 0);

        // On désactive le tableau Vertex Attrib puisque l'on en a plus besoin
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(0);

    // Désactivation du shader
    glUseProgram(0);
}
