#include "Transform.h"
#include <cstdio>

Transform * Transform_init()
{
    return (Transform *) calloc(1, sizeof(Transform));
}

Transform * Transform_create( float rotation_Angle, glm::vec3 rotation_Axis, glm::vec3 translation_vector, glm::vec3 scaling_vector)
{
    Transform * T = Transform_init();

    T->rotation = glm::angleAxis( glm::radians(rotation_Angle), normalize(rotation_Axis) );
    T->translation = translation_vector;
    T->scale = scaling_vector;

    return T;
}

glm::mat4 Transform_getMatrix(const Transform * T)
{
    glm::mat4 identity(1.0f);
    return glm::translate(identity, T->translation) * glm::mat4_cast(T->rotation) * glm::scale(identity, T->scale);
}

glm::quat Rotation_from2vec(glm::vec3 v1, glm::vec3 v2)
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
		rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), v1);

		if ( glm::length2(rotationAxis) < 0.01 )
			rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), v1);

		rotationAxis = normalize( rotationAxis );
		return glm::angleAxis( glm::radians(180.0f), rotationAxis );
	}

	rotationAxis = glm::cross( v1, v2 );

	float s = sqrt( ( 1 + cosTheta) * 2 );
	float invs = 1 / s;

	return glm::quat
    (
		s * 0.5f, 
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
    
}

Transform * Transform_interpolate(const float t, const Transform * T1, const Transform * T2, const bool linear)
{
	Transform * Tf = Transform_init();

	if (linear)
		Tf->rotation = T1->rotation * (1-t) + T2->rotation * t;
	else
		Tf->rotation = glm::mix(T1->rotation, T2->rotation, t);

	Tf->scale = T1->scale * (1-t) + T2->scale * t;
	Tf->translation = T1->translation * (1-t) + T2->translation * t;

	return Tf;
}

IKSolver * IKSolver_init(Transform ** T_init, glm::vec3 target, unsigned char n_transform)
{
	IKSolver * solver = (IKSolver *) calloc(1, sizeof(IKSolver));

	solver->transforms_initial 	= T_init;
	solver->target 				= target;
	solver->n_transform 		= n_transform;

	return solver;
}

Transform ** IKSolver_solve(IKSolver * IK)
{

	glm::vec4 * points = (glm::vec4 *) calloc( 1 + IK->n_transform / 2, sizeof(glm::vec4));
	glm::vec4 * new_points = (glm::vec4 *) calloc( 1 + IK->n_transform / 2, sizeof(glm::vec4));
	glm::mat4 M(1.0f);

	points[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// Calculate the points position for t0
	for (unsigned char i = 0; i < IK->n_transform / 2; i++)
	{
		M = M * Transform_getMatrix(IK->transforms_initial[2*i]);
		M = M * Transform_getMatrix(IK->transforms_initial[2*i + 1]);
		points[i + 1] = M * points[0];
	}

	glm::vec3 current_target = IK->target;

	// Calculate target position for each points at t1
	for (unsigned char k = IK->n_transform / 2; k < 255; k--)
	{

		new_points[k] = glm::vec4(
			current_target.x,
			current_target.y,
			current_target.z,
			1.0f
		);

		printf("K = %i\t P [%3.2f, %3.2f, %3.2f]\t", k, points[k].x, points[k].y, points[k].z);
		printf("N [%3.2f, %3.2f, %3.2f]\n", new_points[k].x, new_points[k].y, new_points[k].z);

		if (k >= 1)
		{
			glm::vec3 trans = glm::normalize( glm::vec3( points[k-1].x, points[k-1].y, points[k-1].z ) - current_target );
			float L =  glm::length( IK->transforms_initial[ 2 * k - 1 ]->translation );
			current_target = current_target + ( L * trans );
		}

	}

	Transform ** new_transforms = (Transform **) calloc( IK->n_transform, sizeof(Transform *) );

	new_transforms[0] = Transform_init();
	new_transforms[0]->translation = IK->transforms_initial[0]->translation;
	new_transforms[0]->scale = IK->transforms_initial[0]->scale;

	glm::quat tmp = Rotation_from2vec( 
				glm::vec3( points[0] 		- points[1] ),
				glm::vec3( new_points[0] 	- new_points[1] )
			);

	new_transforms[0]->rotation = tmp;

	for (int i = 1; i < IK->n_transform; i++)
	{

		new_transforms[i] = Transform_init();
		new_transforms[i]->translation = IK->transforms_initial[i]->translation;
		new_transforms[i]->scale = IK->transforms_initial[i]->scale;

		if (i % 2 == 1)
		{
			new_transforms[i]->rotation = IK->transforms_initial[i]->rotation;
		}
		else
		{
			new_transforms[i]->rotation = Rotation_from2vec( 
				glm::vec3( new_points[ 	(i / 2) - 1] - new_points[ (i / 2) ] ),
				glm::vec3( new_points[ 	(i / 2) ] 	 - new_points[ (i / 2) + 1] )
			);
		}
	}

	free(points);
	free(new_points);

	return new_transforms;

}