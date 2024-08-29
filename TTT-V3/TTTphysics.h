#ifndef TTT_PHYSICS_H
#define TTT_PHYSICS_H

#include "TTT.h"
#include "TTTcog.h"
#include <numeric>

class TTTphysics : public TTTcog
{
private:
    float m_lastTick;
protected:
    // 3d variables
    glm::vec3 p_position;            // meters
    glm::vec3 p_rotation;            // radians
    glm::vec3 p_velocity;            // meters/second
    glm::vec3 p_acceleration;        // meters/second^2
    glm::vec3 p_angularAcceleration; // radians/second^2
    glm::vec3 p_angularVelocity;     // radians/second
    
    // physics properties            
    float p_weight;                  // kg
    float p_friction;                // coefficient of friction
    float p_drag;                    // coefficient of drag
    float p_angularDrag;             // coefficient of angular drag
    float p_gravity;                 // gravitational acceleration (m/s^2)
    float p_terminalVelocity;        // meters/second
    float p_terminalRotationalVelocity; // radians/second^2
    bool p_useGravity;               // is the object affected by gravity

public:
    TTTphysics();

    void FixedUpdate();
 
    glm::vec3 Position() { return p_position; }
    glm::vec3 Rotation() { return p_rotation; }
    glm::vec3 Velocity() { return p_velocity; }
    glm::vec3 Acceleration() { return p_acceleration; }
    glm::vec3 AngularAcceleration() { return p_angularAcceleration; }
    glm::vec3 AngularVelocity() { return p_angularVelocity; }

    float Weight() { return p_weight; }
    float Friction() { return p_friction; }
    float Drag() { return p_drag; }
    float AngularDrag() { return p_angularDrag; }
    float Gravity() { return p_gravity; }
    bool UseGravity() { return p_useGravity; }

    void SetPosition(glm::vec3 position) { p_position = position; }
    void SetRotation(glm::vec3 rotation) { p_rotation = rotation; }
    void SetVelocity(glm::vec3 velocity) { p_velocity = velocity; }
    void SetAcceleration(glm::vec3 acceleration) { p_acceleration = acceleration; }

    virtual ~TTTphysics();
};

#endif