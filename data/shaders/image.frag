#version 330 core
out vec4 FragColor;

uniform sampler2D texture0;
in vec2 texCoords;

void main()
{
    FragColor = texture(texture0, texCoords);
}




//#version 330 core
//in vec2 texCoord;
//
//out vec4 FragColor;
//
//uniform sampler2D texture1;
//
//void main()
//{
//    
//    //FragColor = texture(texture1, texCoord);
//    FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
//}
//