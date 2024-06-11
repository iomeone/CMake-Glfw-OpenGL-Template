#include <Project/render_manager.h>


namespace RenderManager
{


	Rendering::Shader* GetShader(unsigned int shaderType)
	{
		return &g_shaders[shaderType];
	}
	Rendering::VertexBatcher* GetBatcher()
	{
		return &g_batcher;
	}
};
