#pragma once
#include "forward.hpp"
#include "Eigen/Core"

namespace gfx
{
	class Camera
	{
	public:
		Camera() {}
		~Camera() {}
	private:
		Eigen::Vector3f m_target;
	};

	// from http://spointeau.blogspot.ro/2013/12/hello-i-am-looking-at-opengl-3.html
	template<class T>
	Eigen::Matrix<T, 4, 4> perspective
	(
		T fovy,
		T aspect,
		T zNear,
		T zFar
	)
	{
		typedef Eigen::Matrix<T, 4, 4> Matrix4;

		assert(aspect > 0);
		assert(zFar > zNear);

		T radf = fovy; // Math::degToRad(fovy);

		T tanHalfFovy = tan(radf / 2.0);
		Matrix4 res = Matrix4::Zero();
		res(0, 0) = 1.0 / (aspect * tanHalfFovy);
		res(1, 1) = 1.0 / (tanHalfFovy);
		res(2, 2) = -(zFar + zNear) / (zFar - zNear);
		res(3, 2) = -1.0;
		res(2, 3) = -(2.0 * zFar * zNear) / (zFar - zNear);
		return res;
	}

	template<class T>
	Eigen::Matrix<T, 4, 4> lookAt
	(
		Eigen::Matrix<T, 3, 1> const & eye,
		Eigen::Matrix<T, 3, 1> const & center,
		Eigen::Matrix<T, 3, 1> const & up
	)
	{
		typedef Eigen::Matrix<T, 4, 4> Matrix4;
		typedef Eigen::Matrix<T, 3, 1> Vector3;

		Vector3 f = (center - eye).normalized();
		Vector3 u = up.normalized();
		Vector3 s = f.cross(u).normalized();
		u = s.cross(f);

		Matrix4 res;
		res << s.x(), s.y(), s.z(), -s.dot(eye),
			u.x(), u.y(), u.z(), -u.dot(eye),
			-f.x(), -f.y(), -f.z(), f.dot(eye),
			0, 0, 0, 1;

		return res;
	}
}