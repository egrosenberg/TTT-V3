#include "TTTphysics.h"
#include "main.h"

#define STANDARD_GRAVITY 9.806f

const float TWO_PI = glm::two_pi<float>();

TTTphysics::TTTphysics()
{
    m_lastTick = glfwGetTime();
    // inialize variables to 0
    p_position = glm::vec3(0.0f);            // meters
    p_velocity = glm::vec3(0.0f);            // meters/second
    p_rotation = glm::vec3(0.0f);            // radians
    p_acceleration = glm::vec3(0.0f);        // meters/second^2
    p_angularAcceleration = glm::vec3(0.0f); // radians/second^2
    p_angularVelocity = glm::vec3(0.0f);     // radians/second
    p_weight = 0.0f;                         // kg
    p_friction = 0.0f;                       // coefficient of friction
    p_drag = 0.0f;                           // coefficient of drag
    p_angularDrag = 0.0f;                    // coefficient of angular drag
    p_terminalVelocity = INFINITY;           // meters/second
    p_terminalRotationalVelocity = INFINITY; // radians/second^2
    p_gravity = STANDARD_GRAVITY;            // gravitational acceleration
    p_useGravity = true;
}
/**
 * inherited FixedUpate function
 * update all spacial attributes (position / rotation)
 */
void TTTphysics::FixedUpdate()
{
    // calculate time delta
    float time = glfwGetTime();
    float deltaTime = time - m_lastTick;
    m_lastTick = time;

    // update velocity using acceleration
    p_velocity += p_acceleration * deltaTime;
    // force velocity to respect terminal velocity
    float speed = glm::length(p_velocity);
    if (speed > p_terminalVelocity)
    {
        speed = p_terminalVelocity;
        // normalize velocity vector and then multiply by terminal velocity
        p_velocity = glm::normalize(p_velocity);
        p_velocity *= speed;
    }
    p_angularVelocity += p_angularAcceleration * deltaTime;
    // force rotational velocity to respect terminal velocity
    float rotationalSpeed = glm::length(p_velocity);
    if (rotationalSpeed > p_terminalRotationalVelocity)
    {
        rotationalSpeed = p_terminalRotationalVelocity;
        // normalize velocity vector and then multiply by terminal velocity
        p_angularVelocity = glm::normalize(p_angularVelocity);
        p_angularVelocity *= rotationalSpeed;
    }

    // update position using velocity
    p_position += p_velocity * deltaTime;
    // update rotation using angular velocity
    p_rotation += p_angularVelocity * deltaTime;
    // wrap angles betwen -360 and 360
    p_rotation.x = fmod(p_rotation.x, TWO_PI);
    p_rotation.y = fmod(p_rotation.y, TWO_PI);
    p_rotation.z = fmod(p_rotation.z, TWO_PI);
}

TTTphysics::~TTTphysics()
{

}