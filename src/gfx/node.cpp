#include "node.hpp"

Eigen::Matrix4f gfx::Node::getTransform() const
{
	Eigen::Transform<float, 3, Eigen::Affine> transform = transform.Identity();	
	transform.prescale(m_scale).prerotate(m_rot).pretranslate(m_pos);
	return transform.matrix();
}