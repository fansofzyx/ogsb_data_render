#pragma once
#include <vec3.h>
#include <mat4.h>

class xCamera
{
public:
	xCamera():MouseSensitivity(0.005), WalkSpeed(3.5), view( 0.425733656 ,0.279169768 ,-0.860706151 ), pos(38.3913116 , -97.0132446 ,111.233620 ), fov(60), near_dist(0.2), far_dist(2000000.0)
	{

	}
	mat4f GetLookAtMat(vec3f &up = vec3f(0.0, 0.0, 1.0))
	{
		return mat4f::lookAt(pos, pos+view, up);
	}
	mat4f GetPerspectiveMat(float ratio)
	{
		return mat4f::perspectiveProjection(fov, ratio, near_dist, far_dist);
	}
	void RotateView(float angle, vec3f axis)
	{
		vec3f tempview = view.rotateVec(angle, axis.x, axis.y, axis.z);
		view = tempview.normalize();
	}
	void GoFront ()
	{
		pos += (view * WalkSpeed);
	}
	void GoBack()
	{
		pos -= (view * WalkSpeed);
	}
	void GoLeft()
	{
		pos.x -= (view.y * WalkSpeed);
		pos.y += (view.x * WalkSpeed);
	}
	void GoRight()
	{
		pos.x += (view.y * WalkSpeed);
		pos.y -= (view.x * WalkSpeed);
	}
	vec3f getPos()
	{
		return pos;
	}

protected:
public:
	float MouseSensitivity;
	float WalkSpeed;
	vec3f view;
	vec3f pos;
	float fov;
	float near_dist;
	float far_dist;
};