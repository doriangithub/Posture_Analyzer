#version 410

out vec4 drawColor;
in vec3 theColor;
in vec3 theNormal;
in vec3 thePosition;

uniform vec3 lightPosition;


void main()
{
	vec3 lightVector = normalize(lightPosition - thePosition);
	float brightness = dot(lightVector, theNormal);

	vec3 newColor = theColor * vec3(brightness, brightness, brightness);

	drawColor = vec4(newColor, 1.0);

	//drawColor = vec4( brightness, brightness, brightness, 1.0);
}
