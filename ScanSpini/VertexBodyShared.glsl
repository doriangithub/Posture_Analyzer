#version 410

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 normal;

uniform vec3 ambientLight;
uniform float minZ;
uniform float maxZ;

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


	vec3 vertexColorNew = vec3( +1.0f, +1.0f, +1.0f);

	float range = abs((minZ - maxZ));
	float step = range / 10.0;
	float calc = maxZ - position.z;
	float calc2 = abs(calc / step);
	
	if (calc2 > 0)
	{
	}
	if (calc2 > 1)
	{
		vertexColorNew.x = +0.5f;
		vertexColorNew.y = +0.0f;
		vertexColorNew.z = +0.5f;
	}
	if (calc2 > 2)
	{
		vertexColorNew.x = +0.5f;
		vertexColorNew.y = +0.3f;
		vertexColorNew.z = +0.7f;
	}
	if (calc2 > 3)
	{
		vertexColorNew.x = +0.0f;
		vertexColorNew.y = +0.0f;
		vertexColorNew.z = +0.5f;
	}
	
	if (calc2 > 4 )
	{
		vertexColorNew.x = +0.0f;
		vertexColorNew.y = +0.5f;
		vertexColorNew.z = +0.0f;
	}
	if (calc2 > 5)
	{
		vertexColorNew.x = +0.5f;
		vertexColorNew.y = +0.0f;
		vertexColorNew.z = +0.0f;
	}
	if (calc2 > 6)
	{
		vertexColorNew.x = +0.5f;
		vertexColorNew.y = +0.0f;
		vertexColorNew.z = +0.5f;
	}
	if (calc2 > 7)
	{
		vertexColorNew.x = +0.5f;
		vertexColorNew.y = +0.5f;
		vertexColorNew.z = +0.0f;
	}
	if (calc2 > 8)
	{
		vertexColorNew.x = +0.0f;
		vertexColorNew.y = +0.5f;
		vertexColorNew.z = +0.5f;
	}
	if (calc2 > 9)
	{
		vertexColorNew.x = +0.5f;
		vertexColorNew.y = +0.3f;
		vertexColorNew.z = +0.7f;
	}	

	//vertexColorNew.y = +0.5f;

	theColor = vertexColorNew * ambientLight;

	theNormal = normal;
	thePosition = position;
}