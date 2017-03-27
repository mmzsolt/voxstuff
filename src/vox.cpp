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

std::pair<std::vector<data::RawVoxel>, data::VoxelPalette> data::loadVoxel(const std::string& filename)
{
	std::pair<std::vector<RawVoxel>, VoxelPalette> ret;

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

std::pair<std::vector<float>, std::vector<int>> data::convertVoxelsToMeshNaively(const data::RawVoxel& model, const data::VoxelPalette& pal)
{
	std::vector<float> vertices;
	std::vector<int> indices;
	
	float centerPos[3] = { model.m_sizeX / 2.0f, model.m_sizeY / 2.0f, model.m_sizeZ / 2.0f };

	vertices.reserve(2 * BoxVertices.size() * model.m_voxels.size());
	indices.reserve(BoxIndices.size() * model.m_voxels.size());

	int indexBase = 0;
	// for every little voxel
	for (const auto& voxel : model.m_voxels)
	{
		// let's make a little cube thingy
		float voxelPos[3] = { static_cast<float>(voxel.x) - centerPos[0], static_cast<float>(voxel.y) - centerPos[1], static_cast<float>(voxel.z) - centerPos[2] };
		size_t palIndex = voxel.i * 4;
		float voxelColor[3] = {pal[palIndex] / 255.0f, pal[palIndex+1] / 255.0f, pal[palIndex+2] / 255.0f};

		// first the little vertices
		for (size_t i = 0, n = BoxVertices.size(); i < n; i += 3)
		{
			vertices.push_back(BoxVertices[i] + voxelPos[0]);
			vertices.push_back(BoxVertices[i+1] + voxelPos[1]);
			vertices.push_back(BoxVertices[i+2] + voxelPos[2]);

			// after every vertex comes the color (now)
			std::copy(&voxelColor[0], &voxelColor[3], std::back_inserter(vertices));
		}

		// the little indices just need their values added to the current index
		std::transform(BoxIndices.begin(), BoxIndices.end(), std::back_inserter(indices), [=](auto i) { return i + indexBase; });
		// after all, we've added 8 vertices
		indexBase += 8;
	}

	return{vertices, indices};
}