/**
 * @file Transform.h
 * @brief Header for struct Transform
 * @author Antony Madaleno
 * @version 1.0
 * @date 22-02-2024
 *
 * Header pour les struct Transform
 *
 */

#pragma once

//GLM INCLUDES
#include "include/glm/vec3.hpp" // glm::vec3
#include "include/glm/vec4.hpp" // glm::vec4
#include "include/glm/mat4x4.hpp" // glm::mat4
#include "include/glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "include/glm/gtx/quaternion.hpp"

typedef struct Transform
{
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 scale;
} Transform;

typedef struct IKSolver
{
    Transform ** transforms_initial;
    glm::vec3 target;
    unsigned char n_transform;
} IKSolver;

/**
 * @brief Create an empty transform (better use for debug only)
 * 
 * @return Transform * 
 */
Transform * Transform_init();

/**
 * @brief 
 * 
 * @param rotation_Angle Angle of rotation for the quaternion in degrees
 * @param rotation_Axis  Axis of rotation for the quaternion
 * @param translation_vector Translation vector
 * @param scaling_vector Scaling vector
 * @return Transform * the pointer to the Transformation object created
 */
Transform * Transform_create( float rotation_Angle, glm::vec3 rotation_Axis, glm::vec3 translation_vector, glm::vec3 scaling_vector);

/**
 * @brief 
 * 
 * @return glm::mat4 
 */
glm::mat4 Transform_getMatrix(const Transform *);

/**
 * @brief find the rotation necessary to go from v1 to v2 (they will be normalized)
 * 
 * @param v1  starting vector
 * @param v2  ending vector
 * @return glm::quat 
 */
glm::quat Rotation_from2vec(glm::vec3 v1, glm::vec3 v2);

/**
 * @brief 
 * @param t  Interpolation time
 * @param T1 Transformation at t = 0.0
 * @param T2 Transformation at t = 1.0
 * @param linear use linear instead of slerp for quaternion
 * @return Transform * interpolated transform
 */
Transform * Transform_interpolate(const float t, const Transform * T1, const Transform * T2, const bool linear);

/**
 * @brief 
 * 
 * @param T_init Transforms at T0
 * @param target Effector Target position
 * @return IKSolver* 
 */
IKSolver * IKSolver_init(Transform ** T_init, glm::vec3 target, unsigned char n_transform);

/**
 * @brief Solve the IK by cyclic
 * 
 * @param IK 
 * @return Transform** 
 */
Transform ** IKSolver_solve(IKSolver * IK);