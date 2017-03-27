#pragma once

#include "forward.hpp"
#include "shader.hpp"
#include "object.hpp"

namespace gfx
{
	ObjectPtr createCube();
	NodePtr createNode();
	CameraPtr createCamera();
	ShaderPtr createShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	
	template <typename VertexContainerType, typename IndexContainerType>
	ObjectPtr createObject(const VertexContainerType& vertices, const IndexContainerType& indices, const VertexAttribDescriptionVec& descriptions)
	{
		ObjectPtr obj = std::make_shared<Object>();

		auto VAO = createVAO(vertices, indices, descriptions);

		if (VAO)
		{
			obj->setVao(*VAO);
			obj->setIndexCount(indices.size());
		}

		return obj;
	}
	
	class Scene
	{
		typedef std::pair<RenderParams, ObjectPtr> RenderParamsObjectPair;
		typedef std::vector<RenderParamsObjectPair> RenderParamsObjectPairVec;

	public:
		Scene() {}
		~Scene() {}
		
		void addRenderParamsObjectPair(const RenderParams& params, const ObjectPtr& obj) { m_renderParamObjectPairs.push_back({ params, obj }); }

		void render();

		void setPrimaryCamera(const CameraPtr& cam) { m_primaryCamera = cam; }
		const CameraPtr& getPrimaryCamera() const { return m_primaryCamera; }

	private:
		RenderParamsObjectPairVec m_renderParamObjectPairs;
		CameraPtr m_primaryCamera;
	};
}