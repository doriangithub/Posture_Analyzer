#version 410

out vec4 drawColor;
in vec3 theColor;
//in vec3 theNormal;
//in vec3 thePosition;

//uniform vec3 lightPosition;

void main()
{
	//vec3 lightVector = normalize(lightPosition - thePosition);
	//float brightness = dot(lightVector, theNormal);

	drawColor = vec4(theColor, 1.0);

	//drawColor = vec4(brightness, brightness, brightness, 1.0);
}
