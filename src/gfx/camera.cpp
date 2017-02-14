#include "camera.hpp"
#include "Eigen/Dense"

Eigen::Vector3f gfx::Camera::Up(0.0f, 1.0f, 0.0f);

void gfx::Camera::move(const Eigen::Vector3f& diff)
{
	Eigen::Matrix4f look = lookAt(m_target, m_pos, Up);
	Eigen::Matrix4f lookInv = look.inverse();
	Eigen::Vector4f tempDiff(diff.x(), diff.y(), diff.z(), 0.0f);
	Eigen::Vector4f transfDiff = lookInv * tempDiff;
	m_pos += Eigen::Vector3f(transfDiff.x(), transfDiff.y(), transfDiff.z());
	m_target += Eigen::Vector3f(transfDiff.x(), transfDiff.y(), transfDiff.z());
}

void gfx::Camera::rotate(const Eigen::Quaternionf& quat)
{
	m_target = quat * (m_target - m_pos) + m_pos;
}
