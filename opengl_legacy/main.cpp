#include<GLFW/glfw3.h>
#include<Windows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<vector>

#include"ObjLoader.h"

// Inicializar 
void init(GLFWwindow* window);
void teclado_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
void reSize(int w, int h);

// Fun��es


// Vari�veis
static float angulo = 0;
static unsigned blenderModelId;

void Desenha(float dt, float x, float r = 0.0f, float g = 0.0f, float b = 0.0f) {


	glLoadIdentity();


	glColorMask(r, g, b, 1);

	//glEnable(GL_COLOR_MATERIAL);


	vec3 position(0.f, 0.f, 0.f);
	vec3 direction(0.f, 0.f, -3.f);
	vec3 up(0.f, 1.f, 0.f);
	vec3 look = position + direction;// direction;

	
	gluLookAt(position.x, position.y, position.z, look.x, look.y, look.z, up.x, up.y, up.z);
	

	float veloc_ang = 25.f*dt;
	angulo += veloc_ang;
	
	glPushMatrix();
		glTranslatef(x, -8, 8.f);
		glCallList(blenderModelId);
	glPopMatrix();

	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	

	// glColorMask(1, 0, 0, 1);

	if (angulo >= 360.0)
		angulo = 0.0;


}

int main() {

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "Tutorial Light - Blender", NULL, NULL);

	init(window);

	float lastTime = 0.0;

	bool running = true;
	while (running)
	{
		float currentTime = (float)glfwGetTime();

		float dt = currentTime - lastTime;
		lastTime = currentTime;

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		reSize(width, height);
		

		// -- Draw Objects --

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
		//Desenha o azul primeiro
		Desenha(dt, -5.08, 0.0f, 0.0f, 1.0f);

		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);
		//glClearColor(0, 0, 0, 0);

		Desenha(dt, -5, 1.0f, 0.0, 0.0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glfwSwapBuffers(window);
		glDisable(GL_BLEND);

		running = !glfwWindowShouldClose(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}


void init(GLFWwindow* window) {
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, teclado_callback);

	glClearColor(0, 0, 0, 0.f);


	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);


	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	float globalAmb[] = { 0.1f, 0.1f, 0.1f, 1.f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);

	float light0[4][4] = {
				{0.1f, 0.1f, 0.1f, 1.f}, // ambient
				{0.8f, 0.8f, 0.8f, 1.f}, // diffuse
				{ 1.f,  1.f,  1.f, 1.f }, // specular
				{0.f, 0.f, 1.f, 1.f}    // position
	};

	glLightfv(GL_LIGHT0, GL_AMBIENT, &light0[0][0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &light0[1][0]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, &light0[2][0]);
	glLightfv(GL_LIGHT0, GL_POSITION, &light0[3][0]);


	ObjLoader::loadOBJ(blenderModelId, "Blender/3DModel.obj");
}




void reSize(int w, int h)
{
	glViewport(0, 0, w, h);

	float aspect = (float)w / (float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 500.0);
	glMatrixMode(GL_MODELVIEW);
}


void teclado_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}
 