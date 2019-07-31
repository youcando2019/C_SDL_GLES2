#include "Camera.h"
/*
void Camera_init(Camera * me)
{
    me->m_phi = 0.0;
    me->m_theta = 0.0;
    glm_vec3_zero(me->m_orientation);
    glm_vec3_copy((vec3){0, 0, 1}, me->m_axeVertical);
    glm_vec3_zero(me->m_deplacementLateral);
    glm_vec3_zero(me->m_position);
    glm_vec3_zero(me->m_pointCible);
    me->m_sensibilite = 0.0;
    me->m_vitesse = 0.0;
}
*/
/*Camera::Camera(glm::vec3 position, glm::vec3 pointCible, glm::vec3 axeVertical) :
    m_phi(-35.26), m_theta(-135), m_orientation(), m_axeVertical(axeVertical), m_deplacementLateral(), m_position(position), m_pointCible(pointCible)
*/
void Camera_init(Camera * me, vec3 position, vec3 pointCible, vec3 axeVertical, float sensibilite, float vitesse)
{
    me->m_phi = 0.0;
    me->m_theta = 0.0;
    glm_vec3_zero(me->m_orientation);
    glm_vec3_copy(axeVertical, me->m_axeVertical);
    glm_vec3_zero(me->m_deplacementLateral);
    glm_vec3_copy(position, me->m_position);
    glm_vec3_copy(pointCible, me->m_pointCible);
    me->m_sensibilite = sensibilite;
    me->m_vitesse = vitesse;
    // Actualisation du point cibl�
    Camera_setPointcible(me, pointCible);
}

void Camera_destroy(Camera * me)
{

}

void Camera_orienter(Camera * me, int xRel, int yRel)
{
    // Modification des angles
    me->m_phi += -yRel * me->m_sensibilite;
    me->m_theta += -xRel * me->m_sensibilite;

    // Limitation de l'angle phi
    if(me->m_phi > 89.0)
    {
        me->m_phi = 89.0;
    }
    else if(me->m_phi < -89.0)
    {
        me->m_phi = -89.0;
    }

    // Conversion des angles en radian
    float phiRadian = me->m_phi * M_PI / 180;
    float thetaRadian = me->m_theta * M_PI / 180;

    // Si l'axe vertical est l'axe X
    if(me->m_axeVertical[0] == 1.0)
    {
        // Calcul des coordonn�es sph�riques
        me->m_orientation[0] = sin(phiRadian);
        me->m_orientation[1] = cos(phiRadian) * cos(thetaRadian);
        me->m_orientation[2] = cos(phiRadian) * sin(thetaRadian);
    }
    // Si c'est l'axe Y
    else if(me->m_axeVertical[1] == 1.0)
    {
        // Calcul des coordonn�es sph�riques
        me->m_orientation[0] = cos(phiRadian) * sin(thetaRadian);
        me->m_orientation[1] = sin(phiRadian);
        me->m_orientation[2] = cos(phiRadian) * cos(thetaRadian);
    }
    // Sinon c'est l'axe Z
    else
    {
        // Calcul des coordonn�es sph�riques
        me->m_orientation[0] = cos(phiRadian) * cos(thetaRadian);
        me->m_orientation[1] = cos(phiRadian) * sin(thetaRadian);
        me->m_orientation[2] = sin(phiRadian);
    }

    // Calcul de la normale
    glm_vec3_cross(me->m_axeVertical, me->m_orientation, me->m_deplacementLateral);
    glm_normalize(me->m_deplacementLateral);
    // Calcul du point cibl� pour OpenGL
    glm_vec3_add(me->m_position, me->m_orientation, me->m_pointCible);
}

void Camera_deplacer(Camera * me, Input *input)
{
    // Gestion de l'orientation
    if(Input_mouvementSouris(input))
    {
        Camera_orienter(me, Input_getXRel(input), Input_getYRel(input));
    }

    // Avanc�e de la cam�ra
    if(Input_getTouche(input, SDL_SCANCODE_UP))
    {
        glm_vec3_muladds(me->m_orientation, me->m_vitesse, me->m_position);
        glm_vec3_add(me->m_position, me->m_orientation, me->m_pointCible);
    }
    // Recul de la cam�ra
    if(Input_getTouche(input, SDL_SCANCODE_DOWN))
    {
        vec3 orVit;
        glm_vec3_zero(orVit);
        glm_vec3_muladds(me->m_orientation, me->m_vitesse, orVit);
        glm_vec3_sub(me->m_position, orVit, me->m_position);
        glm_vec3_add(me->m_position, me->m_orientation, me->m_pointCible);
    }
    // D�placement vers la gauche
    if(Input_getTouche(input, SDL_SCANCODE_LEFT))
    {
        glm_vec3_muladds(me->m_deplacementLateral, me->m_vitesse, me->m_position);
        glm_vec3_add(me->m_position, me->m_orientation, me->m_pointCible);
    }
    // D�placement vers la droite
    if(Input_getTouche(input, SDL_SCANCODE_RIGHT))
    {
        vec3 depVit;
        glm_vec3_zero(depVit);
        glm_vec3_muladds(me->m_deplacementLateral, me->m_vitesse, depVit);
        glm_vec3_sub(me->m_position, depVit, me->m_position);
        glm_vec3_add(me->m_position, me->m_orientation, me->m_pointCible);
    }
}

void Camera_lookat(Camera * me, mat4 * modelview)
{
    // Actualisation de la vue dans la matrice
    glm_lookat(me->m_position, me->m_pointCible, me->m_axeVertical, *modelview);
}

void Camera_setPointcible(Camera * me, vec3 pointCible)
{
    // Calcul du vecteur orientation
    glm_vec3_sub(me->m_pointCible, me->m_position, me->m_orientation);
    glm_normalize(me->m_orientation);

    // Si l'axe vertical est l'axe X
    if(me->m_axeVertical[0] == 1.0)
    {
        // Calcul des angles
        me->m_phi = asin(me->m_orientation[0]);
        me->m_theta = acos(me->m_orientation[1] / cos(me->m_phi));
        if(me->m_orientation[1] < 0)
        {
            me->m_theta *= -1;
        }
    }

    // Si c'est l'axe Y
    else if(me->m_axeVertical[1] == 1.0)
    {
        // Calcul des angles
        me->m_phi = asin(me->m_orientation[1]);
        me->m_theta = acos(me->m_orientation[2] / cos(me->m_phi));
        if(me->m_orientation[2] < 0)
        {
            me->m_theta *= -1;
        }
    }

    // Sinon c'est l'axe Z
    else
    {
        // Calcul des angles
        me->m_phi = asin(me->m_orientation[0]);
        me->m_theta = acos(me->m_orientation[2] / cos(me->m_phi));
        if(me->m_orientation[2] < 0)
        {
            me->m_theta *= -1;
        }
    }
    // Conversion en degr�s
    me->m_phi = me->m_phi * 180 / M_PI;
    me->m_theta = me->m_theta * 180 / M_PI;
}

void Camera_setPosition(Camera * me, vec3 position)
{
    // Mise � jour de la position
    glm_vec3_copy(position, me->m_position);
    // Actualisation du point cibl�
    glm_vec3_add(me->m_position, me->m_orientation, me->m_pointCible);
}

float const Camera_getSensibilite(Camera * me)
{
    return me->m_vitesse;
}

float const Camera_getVitesse(Camera * me)
{
    return me->m_vitesse;
}

void Camera_setSensibilite(Camera * me, float sensibilite)
{
    me->m_sensibilite = sensibilite;
}

void Camera_setVitesse(Camera * me, float vitesse)
{
    me->m_vitesse = vitesse;
}
