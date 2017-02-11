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
}