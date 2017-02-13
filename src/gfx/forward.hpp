#pragma once

#include <memory>
#include <vector>

namespace gfx
{
	class Object;
	typedef std::shared_ptr<Object> ObjectPtr;
	typedef std::vector<ObjectPtr> ObjectPtrVec;

	class Scene;
	typedef std::shared_ptr<Scene> ScenePtr;

	class Node;
	typedef std::shared_ptr<Node> NodePtr;
	typedef std::vector<NodePtr> NodePtrVec;

	class Camera;
	typedef std::shared_ptr<Camera> CameraPtr;
	typedef std::vector<CameraPtr> CameraPtrVec;

	class Shader;
	typedef std::shared_ptr<Shader> ShaderPtr;
	typedef std::vector<ShaderPtr> ShaderPtrVec;
}