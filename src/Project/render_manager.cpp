#include <Project/render_manager.h>


namespace RenderManager
{

	Rendering::VertexBatcher g_batcher;

	Rendering::Shader	g_shaders[SHADER_TYPES];


	bool Initialize()
	{
 
		{
			const char* vertexShaderSource = R"(
#version 150

in vec3 position;
in vec2 texture;
in vec4 color;

out vec2 tex_coord;
out vec4 color_val;
out vec4 pos;

uniform mat4 modelview;
uniform mat4 cameraTransform;
uniform mat4 projection;

uniform mat4 screenPos;

void main()
{
	color_val = color;
	tex_coord = texture;
	pos = (projection * cameraTransform * modelview) * vec4(position, 1);
	gl_Position = screenPos * pos;
}

)";


			const char* fragmentShaderSource = R"(
#version 150

uniform sampler2D textureSample;

uniform float fogPos;
//fogPos = 1.6
//fogSize = 0.2

in vec2 tex_coord;
in vec4 color_val;
in vec4 pos;

out vec4 fragColor;

void main()
{
	if ((pos.y / pos.w) < -1)
		discard;
	float z = (gl_FragCoord.z * gl_FragCoord.w) * 13;
	float fogVal = clamp((fogPos - z) / -0.2, 0, 1);
	fragColor = (texture(textureSample, tex_coord) * color_val * fogVal);
	//fragColor  = vec4(1.0, 0.0, 0.0, 1.0);
}
)";
	

			g_shaders[SHADER_TYPE_DEFAULT].CreateShader(vertexShaderSource, fragmentShaderSource);


			g_shaders[SHADER_TYPE_DEFAULT].Use();
			g_shaders[SHADER_TYPE_DEFAULT].SetModelView(glm::mat4x4(1.0f));
			g_shaders[SHADER_TYPE_DEFAULT].SetScreenPos(glm::mat4x4(1.0f));
			g_shaders[SHADER_TYPE_DEFAULT].SetCameraTransform(glm::mat4x4(1.0f));
		}
		


		/*
				
				g_shaders[SHADER_TYPE_BILLBOARD].CreateShader("billboard");
				g_shaders[SHADER_TYPE_SCREEN].CreateShader("batcher");
				g_shaders[SHADER_TYPE_TEST].CreateShader("batcher-test");
				g_shaders[SHADER_TYPE_FX_GAUSSIAN_9].CreateShader("filter-gaussian");
		
		*/









		g_batcher.Create();

		return true;

	}

	Rendering::Shader* GetShader(unsigned int shaderType)
	{
		return &g_shaders[shaderType];
	}
	Rendering::VertexBatcher* GetBatcher()
	{
		return &g_batcher;
	}
};
