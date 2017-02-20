#include "GL/glew.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "scene.hpp"
#include "object.hpp"
#include "node.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "vox.hpp"

#include <vector>

gfx::ObjectPtr gfx::createCube()
{
	ObjectPtr obj = std::make_shared<Object>();

	VertexAttribDescriptionVec desc;
	desc.push_back({ GL_FLOAT, 3 });

	auto VAO = createVAO(data::BoxVertices, data::BoxIndices, desc);

	if (VAO)
	{
		obj->setVao(*VAO);
		obj->setIndexCount(data::BoxIndices.size());
	}

	return obj;
}

gfx::NodePtr gfx::createNode()
{
	NodePtr node = std::make_shared<Node>();
	return node;
}

gfx::CameraPtr gfx::createCamera()
{
	CameraPtr cam = std::make_shared<Camera>();
	return cam;
}

gfx::ShaderPtr gfx::createShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	ShaderPtr shader = std::make_shared<Shader>();
	shader->load(vertexShaderPath, fragmentShaderPath);
	return shader;
}

void gfx::Scene::render()
{
	Eigen::Matrix4f persp = perspective(static_cast<float>(45.0f * M_PI / 180.0f), 1.0f, 1.0f, 100.0f);

	Eigen::Matrix4f look = lookAt(m_primaryCamera->getTarget(), m_primaryCamera->getPosition(), { 0.0f, 1.0f, 0.0f });

	persp *= look;

	m_renderParamObjectPairs.begin()->first.getShader()->setUniformM4f("persp", &persp(0));

	for (auto& nodeObjPair : m_renderParamObjectPairs)
	{
		auto& renderParams = nodeObjPair.first;
		auto& obj = nodeObjPair.second;

		Eigen::Matrix4f finalTransform = renderParams.getNode()->getTransform();
		renderParams.getShader()->setUniformM4f("transform", &finalTransform(0));
		renderParams.setColorUniform("objcolor");
		renderParams.getShader()->use();
		obj->render();
	}
}