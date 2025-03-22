#version 400 core

layout (location=0) in vec3 aPos;

out vec4 glPosition;

void main()
{

    glPosition = vec4(aPos,1.0);

}
