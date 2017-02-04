#version 410

out vec4 drawColor;
in vec3 theColor;


void main()
{
	drawColor = vec4(theColor, 1.0);
}

