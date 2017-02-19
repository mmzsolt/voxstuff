#pragma once

#include <string>
#include <vector>

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

	std::pair<std::vector<RawVoxel>, std::vector<char>> loadVoxel(const std::string& filename);
}
