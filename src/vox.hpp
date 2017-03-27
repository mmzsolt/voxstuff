#pragma once

#include <string>
#include <vector>

#include "GL/glew.h"

namespace data
{

	class RawVoxel
	{
	public:
		struct voxel
		{
			unsigned char x, y, z, i;
		};
		std::vector<voxel> m_voxels;
		int m_sizeX;
		int m_sizeY;
		int m_sizeZ;
	};

	static std::vector<GLfloat> BoxVertices = {
		0.5f, 0.5f, -0.5f, // Top Right Back
		0.5f, -0.5f, -0.5f, // Bottom Right Back
		-0.5f, -0.5f, -0.5f, // Bottom Left Back
		-0.5f, 0.5f, -0.5f, // Top Left Back
		0.5f, 0.5f, 0.5f, // Top Right Front
		0.5f, -0.5f, 0.5f, // Bottom Right Front
		-0.5f, -0.5f, 0.5f, // Bottom Left Front
		-0.5f, 0.5f, 0.5f, // Top Left Front
	};

	// strange how hard it is to find the index representation of the faces of a cube on the net
	static std::vector<GLuint> BoxIndices = {
		0, 1, 3,
		1, 2, 3,
		4, 5, 7,
		5, 6, 7,
		0, 1, 4,
		1, 4, 5,
		2, 3, 7,
		2, 6, 7,
		0, 4, 7,
		0, 7, 3,
		1, 5, 6,
		1, 6, 2
	};

	typedef std::vector<unsigned char> VoxelPalette;

	std::pair<std::vector<RawVoxel>, VoxelPalette> loadVoxel(const std::string& filename);

	std::pair<std::vector<float>, std::vector<int>> convertVoxelsToMeshNaively(const RawVoxel& model, const VoxelPalette& pal);
}
