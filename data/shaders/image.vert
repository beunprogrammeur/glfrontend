#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;

out vec2 texCoords;

uniform mat4 scale;
uniform mat4 translate;
uniform mat4 rotate;

void main()
{
	texCoords = texCoord;
	gl_Position = translate * rotate * scale * vec4(aPos, 1.0f);
}










//#version 330 core
//layout (location = 0) in vec3 position;
//layout (location = 1) in vec2 aTexCoord;
//
//uniform mat4 transform;
//
//out vec2 texCoord;
//
//void main()
//{
//	texCoord = aTexCoord;
//	
//	gl_Position = transform * vec4(position, 1.0f);
//}
