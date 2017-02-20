#include "vox.hpp"
#include <iostream>
#include <fstream>
#include <tuple>
#include <algorithm>
#include <iterator>

namespace data
{
	std::pair<std::string, int> loadChunkNameAndSize(std::ifstream& file)
	{
		char id[5];
		file >> id[0] >> id[1] >> id[2] >> id[3];
		id[4] = 0;
		int size = 0;
		file.read((char*)(&size), 4);
		return{ std::string((char*)id), size };
	}

	std::tuple<std::string, int, int> loadChunkNameAndSizes(std::ifstream& file)
	{
		auto chunk = loadChunkNameAndSize(file);
		int size = 0;
		file.read((char*)(&size), 4);
		return{ chunk.first, chunk.second, size };
	}
}

std::pair<std::vector<data::RawVoxel>, std::vector<char>> data::loadVoxel(const std::string& filename)
{
	std::pair<std::vector<data::RawVoxel>, std::vector<char>> ret;

	std::ifstream file;
	file.open(filename, std::ios::in | std::ios::binary);
	file >> std::noskipws;
	if (!file.is_open())
	{
		return ret;
	}

	std::streampos fileSize = file.tellg();
	file.seekg(0, std::ios::end);
	fileSize = file.tellg() - fileSize;
	file.seekg(0, std::ios::beg);

	auto firstChunk = loadChunkNameAndSize(file);
	if (firstChunk.first != "VOX ")
	{
		return ret;
	}

	if (firstChunk.second != 150)
	{
		return ret;
	}

	auto secondChunk = loadChunkNameAndSizes(file);

	if (std::get<0>(secondChunk) != "MAIN")
	{
		return ret;
	}

	if (std::get<2>(secondChunk) + file.tellg() > fileSize)
	{
		return ret;
	}

	RawVoxel rawVoxel;
	while (file.tellg() < fileSize)
	{
		auto currentChunk = loadChunkNameAndSizes(file);

		// if the size of this chunk would be too big, probably the file is corrupted, give up
		if (std::get<2>(currentChunk) + file.tellg() > fileSize)
		{
			return ret;
		}

		if (std::get<0>(currentChunk) == "PACK")
		{
			continue;  //probably we can do without this right now
		}
		else if (std::get<0>(currentChunk) == "SIZE")
		{
			// in theory, the SIZE chunks come first, so we reset the current mesh
			rawVoxel = RawVoxel();
			
			// must have 12 bytes in size
			if (std::get<1>(currentChunk) != 12)
			{
				return ret;
			}

			file.read((char*)(&rawVoxel.m_sizeX), 4);
			file.read((char*)(&rawVoxel.m_sizeY), 4);
			file.read((char*)(&rawVoxel.m_sizeZ), 4);
		}
		else if (std::get<0>(currentChunk) == "XYZI")
		{
			int num = 0;
			file.read((char*)(&num), 4);
			
			// validate the number of voxels
			if ((num * 4) + file.tellg() > fileSize)
			{
				return ret;
			}

			rawVoxel.m_voxels.resize(num);
			file.read((char *)(&rawVoxel.m_voxels[0]), num * 4);

			ret.first.push_back(rawVoxel);
		}
		else if (std::get<0>(currentChunk) == "RGBA")
		{
			// must have 1024 bytes in size
			if (std::get<1>(currentChunk) != 1024)
			{
				return ret;
			}
			
			ret.second.resize(1024);
			file.read((char *)(&ret.second[0]), 1024);
		}
	}

	return ret;
}

std::pair<std::vector<float>, std::vector<int>> data::convertVoxelsToMeshNaively(const data::RawVoxel& voxel, const data::VoxelPalette& pal)
{
	std::vector<float> vertices;
	std::vector<int> indices;

	vertices.reserve(BoxVertices.size() * voxel.m_voxels.size());
	indices.reserve(BoxIndices.size() * voxel.m_voxels.size());


	for (const auto& v : voxel.m_voxels)
	{
		std::transform(BoxVertices.begin(), BoxVertices.end(), std::back_inserter(vertices), [v](float vertex) {
			static int idx = 0;
			idx = (idx+1)%3;
			if (idx == 0)
			{
				return vertex + v.z;
			}
			else if (idx == 1)
			{
				return vertex + v.x;
			}
			else
			{
				return vertex + v.y;
			}
		});
	}

	return{vertices, indices};
}