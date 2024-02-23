#include "Transform.h"

Transform * Transform_init()
{
    return (Transform *) calloc(1, sizeof(Transform));
}

Transform * Transform_create( float rotation_Angle, glm::vec3 rotation_Axis, glm::vec3 translation_vector, glm::vec3 scaling_vector)
{
    Transform * T = Transform_init();

    T->rotation = glm::angleAxis(glm::radians(rotation_Angle), rotation_Axis);
    T->translation = translation_vector;
    T->scale = scaling_vector;

    return T;
}

glm::mat4 Transform_getMatrix(const Transform * T)
{
    glm::mat4 identity(1.0f);
    return glm::translate(identity, T->translation) * glm::mat4_cast(T->rotation) * glm::scale(identity, T->scale);
}

glm::quat Rotation_from2vec(const glm::vec3 v1, const glm::vec3 v2)
{
    v1 = normalize(v1);
	v2 = normalize(v2);

	float cosTheta = dot(v1, v2);
	glm::vec3 rotationAxis;

	if ( cosTheta < -1 + 0.001f )
    {
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to v1
		rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), v1);

		if ( gtx::norm::length2(rotationAxis) < 0.01 )
			rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), v1);

		rotationAxis = normalize( rotationAxis );
		return gtx::quaternion::angleAxis( glm::radians(180.0f), rotationAxis );
	}

	rotationAxis = cross( v1, v2 );

	float s = sqrt( ( 1 + cosTheta) * 2 );
	float invs = 1 / s;

	return quat
    (
		s * 0.5f, 
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
    
}