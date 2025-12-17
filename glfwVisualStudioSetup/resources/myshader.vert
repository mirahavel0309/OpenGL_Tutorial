#version 330 core

layout(location = 0) in vec3 in_postion;
layout(location = 1) in vec3 in_color;

void main()
{
	gl_Position = vec4(in_postion, 1);
}