#define NUM_LIGHTS 3
varying vec3 vecNormal, vecLight, vecEye;

void main()
{
    vec3 viewVex = vec3(gl_ModelViewMatrix * gl_Vertex);
	vecNormal = gl_NormalMatrix * gl_Normal;
	vecLight  = vec3(gl_LightSource[0].position.xyz - viewVex);
	vecEye    = -viewVex;

	gl_Position = ftransform();
}
