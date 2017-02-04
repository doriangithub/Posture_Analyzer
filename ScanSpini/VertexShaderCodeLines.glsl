#version 410

in layout(location=0) vec3 position;

uniform vec3 ambientLight;

uniform mat4 fullTransfomMatrix;

out vec3 theColor;

void main()
{
	gl_Position = vec4(position, 1.0);
	vec4 vp = vec4(position, 1.0);
	gl_Position = fullTransfomMatrix *vp;

	theColor = vec3(1.0, 0.0, 0.0);

	//theNormal = normal;
	//thePosition = position;
}