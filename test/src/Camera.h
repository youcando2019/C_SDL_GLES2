#ifndef CAMERA_H
#define CAMERA_H

    // Includes GLM
    #include <cglm/cglm.h>
    #include "Input.h"

    // Classe
    typedef struct
    {
        // Attributs d'orientation
        float m_phi;
        float m_theta;
        vec3 m_orientation;
        // Attributs de déplacement
        vec3 m_axeVertical;
        vec3 m_deplacementLateral;

        vec3 m_position;
        vec3 m_pointCible;

        float m_sensibilite;
        float m_vitesse;
    } Camera;

    //void Camera_init(Camera * me);
    //void Camera_init(Camera * me, vec3 position, vec3 pointCible, vec3 axeVertical);
    void Camera_init(Camera * me, vec3 position, vec3 pointCible, vec3 axeVertical, float sensibilite, float vitesse);
    void Camera_destroy(Camera * me);
    void Camera_orienter(Camera * me, int xRel, int yRel);
    void Camera_deplacer(Camera * me, Input *input);
    void Camera_lookat(Camera * me, mat4 * modelview);
    void Camera_setPointcible(Camera * me, vec3 pointCible);
    void Camera_setPosition(Camera * me, vec3 position);
    // Getters et Setters
    float const Camera_getSensibilite(Camera * me);
    float const Camera_getVitesse(Camera * me);
    void Camera_setSensibilite(Camera * me, float sensibilite);
    void Camera_setVitesse(Camera * me, float vitesse);

#endif // CAMERA_H_INCLUDED
