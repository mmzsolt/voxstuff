#pragma once

#include "forward.hpp"

namespace gfx
{
	ObjectPtr createCube();
	NodePtr createNode();
	ShaderPtr createShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

	class Scene
	{
	public:
		Scene() {}
		~Scene() {}
		
		void addNodeAndX(const NodePtr& node, const ObjectPtr& obj) { m_nodeObjectPairs.push_back({ node, obj }); }

		void render();

	private:
		NodeObjectPairVec m_nodeObjectPairs;
	};
}