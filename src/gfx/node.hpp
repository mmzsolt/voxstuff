#pragma once
#include "forward.hpp"
#include "Eigen/Core"
#include "Eigen/Geometry"

namespace gfx
{
	class Node
	{
	public:
		Node()
			: m_pos({ 0.0f, 0.0f, 0.0f })
			, m_rot({ 0.0f, 0.0f, 0.0f, 1.0f })
			, m_scale({ 1.0f, 1.0f, 1.0f })
		{}

		~Node() {}

		void setPosition(const Eigen::Vector3f& pos) { m_pos = pos; }
		const Eigen::Vector3f& getPosition() const { return m_pos; }

		void setScale(const Eigen::Vector3f& scale) { m_scale = scale; }
		const Eigen::Vector3f& getScale() const { return m_scale; }

		void setRotation(const Eigen::Quaternionf& rot) { m_rot = rot; }
		const Eigen::Quaternionf& getRotation() const { return m_rot; }

		void rotateBy(const Eigen::Quaternionf& rot) { m_rot *= rot; }
		void rotateBy(const Eigen::Vector3f& axis, float angle) { Eigen::AngleAxisf rot(angle, axis); m_rot = m_rot * rot; }
		void rotateAroundX(float radians) { rotateBy({ 1.0f, 0.0f, 0.0f }, radians); }
		void rotateAroundY(float radians) { rotateBy({ 0.0f, 1.0f, 0.0f }, radians); }
		void rotateAroundZ(float radians) { rotateBy({ 0.0f, 0.0f, 1.0f }, radians); }

		Eigen::Matrix4f getTransform() const;

	private:
		Eigen::Vector3f m_pos;
		Eigen::Vector3f m_scale;
		Eigen::Quaternionf m_rot;
	};
}
