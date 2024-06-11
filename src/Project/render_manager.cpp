#include <Project/render_manager.h>


namespace RenderManager
{

	Rendering::VertexBatcher g_batcher;

	Rendering::Shader	g_shaders[SHADER_TYPES];



	void initializeDefaultShader()
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

	}





	void initializeBILLBOARDShader()
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

uniform vec3 point_center;

void main()
{
	color_val = color;
	tex_coord = texture;

	mat4 transform = cameraTransform * modelview;

	vec3 camera_right = vec3(transform[0][0], transform[1][0], transform[2][0]);
	vec3 camera_up = vec3(transform[0][1], transform[1][1], transform[2][1]);

	pos = (projection * transform) *
		vec4(
			point_center +
			(camera_right * position.x) +
			(camera_up * position.y),
			1
		);

	gl_Position = screenPos * pos;
}


)";


			const char* fragmentShaderSource = R"(


			#version 150

			uniform sampler2D textureSample;
			uniform float fogPos;

			in vec2 tex_coord;
			in vec4 color_val;
			in vec4 pos;

			out vec4 fragColor;

			void main()
			{
				//if ((pos.y / pos.w) < -1)
				//	discard;

				vec4 outcolor = texture(textureSample, tex_coord);
				if (outcolor.a <= 0)
					discard;
	
				float z = (gl_FragCoord.z * gl_FragCoord.w) * 13;
				float fogVal = clamp((fogPos - z) / -0.2, 0, 1);
	
				if (fogVal <= 0)
					discard;
				fragColor = outcolor * color_val * fogVal;// + vec4(0,0,0,0.5);
			}


)";


			g_shaders[SHADER_TYPE_BILLBOARD].CreateShader(vertexShaderSource, fragmentShaderSource);


			g_shaders[SHADER_TYPE_BILLBOARD].Use();
			g_shaders[SHADER_TYPE_BILLBOARD].SetModelView(glm::mat4x4(1.0f));
			g_shaders[SHADER_TYPE_BILLBOARD].SetScreenPos(glm::mat4x4(1.0f));
			g_shaders[SHADER_TYPE_BILLBOARD].SetCameraTransform(glm::mat4x4(1.0f));
		}

	}

	bool Initialize()
	{
 

		initializeDefaultShader();


		initializeBILLBOARDShader();

		/*
				
			g_shaders[SHADER_TYPE_DEFAULT].CreateShader("batcher");
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
