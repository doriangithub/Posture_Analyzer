#version 410

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 normal;

uniform vec3 ambientLight;


//uniform vec3 dominatingColor;

uniform mat4 fullTransfomMatrix;

out vec3 theColor;
out vec3 theNormal;
out vec3 thePosition;

void main()
{
	//gl_Position = vec4(position, 1.0);
	
	vec4 vp = vec4(position, 1.0);
	gl_Position = fullTransfomMatrix *vp;

	theColor = vertexColor * ambientLight;

	theNormal = normal;
	thePosition = position;
}