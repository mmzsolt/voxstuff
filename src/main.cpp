#include "Eigen/Core"
#include "vox.hpp"
#include <iostream>

int main()
{
	Eigen::Matrix4f mat = Eigen::Matrix4f::Identity();
	Eigen::Vector4f vec(1.0f, 2.0f, 3.0f, 4.0f);
	vec = 2 * mat * vec;
	std::cout << vec;

	return 0;
}