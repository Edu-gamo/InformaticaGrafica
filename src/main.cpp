//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>

#include <SOIL.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

using namespace glm;
using namespace std;
const GLint WIDTH = 800, HEIGHT = 600;
bool WIDEFRAME = false;
float textureChange = 0.0f;
float rotationY = 0.0f;
float rotationX = 0.0f;
float autoRotation = false;
Camera myCamera;

Model model1;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void printVAO(GLuint VAO);
mat4 GenerateModelMatrix(vec3 scale, vec3 rotation, vec3 position);

int main() {
	//initGLFW
//TODO

	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//create a window
//TODO
	GLFWwindow* window;

	if (!glfwInit()) exit(EXIT_FAILURE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Ventana", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	//set GLEW and inicializate
//TODO
	glfwMakeContextCurrent(window);

	glewExperimental = GLU_TRUE;

	if (GLEW_OK != glewInit()) {
		cout << "Error al iniciar glew" << endl;
		glfwTerminate();
		return NULL;
	}

	//set function when callback
//TODO
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//set windows and viewport
//TODO
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	glViewport(0, 0, screenWidth, screenHeight);

	//fondo
//TODO
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 1.0, 1.0, 1.0);

	glCullFace(GL_FRONT_AND_BACK);
	glFrontFace(GL_CCW);

	//cargamos los shader
	Shader myShader = Shader("./src/3DVertex.vertexshader", "./src/3DFragment.fragmentshader");

	// Definir el buffer de vertices
	/*GLfloat VertexBufferCube[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	vec3 CubesPositionBuffer[] = {
		vec3(0.0f ,  0.0f,  0.0f),
		vec3(2.0f ,  5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(2.4f , -0.4f, -3.5f),
		vec3(-1.7f,  3.0f, -7.5f),
		vec3(1.3f , -2.0f, -2.5f),
		vec3(1.5f ,  2.0f, -2.5f),
		vec3(1.5f ,  0.2f, -1.5f),
		vec3(-1.3f,  1.0f, -1.5f)
	};

	// Crear los VBO, VAO y EBO
	GLuint VBO;
	GLuint VAO;

	//reservar memoria para el VAO, VBO y EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//Establecer el objeto
	glBindVertexArray(VAO);
		//Declarar el VBO y el EBO
		//Enlazar el buffer con openGL
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferCube), VertexBufferCube, GL_STATIC_DRAW);

		//Establecer las propiedades de los vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);

		//liberar el buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//liberar el buffer de vertices
	glBindVertexArray(0);


	//Textura
	GLuint texture1;
	GLuint texture2;

	//Reservar memoria
	glGenTextures(1, &texture1);

	//Enlazar textura reservada en memoria
	glBindTexture(GL_TEXTURE_2D, texture1);

	//Propiedades (Wrapping y filtrado)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Cargar imagen
	int width, height;
	unsigned char* image = SOIL_load_image("./src/Luffy.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	//Liberar imagen
	SOIL_free_image_data(image);

	//Reservar memoria
	glGenTextures(1, &texture2);

	//Enlazar textura reservada en memoria
	glBindTexture(GL_TEXTURE_2D, texture2);

	//Propiedades (Wrapping y filtrado)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Cargar imagen
	image = SOIL_load_image("./src/Coby.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	//Liberar imagen
	SOIL_free_image_data(image);

	//Liberar puntero de textura
	glBindTexture(GL_TEXTURE_2D, 0);*/

	//Matriz Projection
	float aspectRatio = screenWidth / screenHeight;
	if (screenWidth < screenHeight) aspectRatio = screenHeight / screenWidth;
	mat4 proj;


	//Camara
	vec3 cPos = vec3(0.0f, 0.0f, 3.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraDir = normalize(cPos - cameraTarget);
	myCamera = Camera(cPos, cameraDir, 0.05f, 60.0f, 3.0f);

	model1 = Model("./src/models/spider/spider.obj");
	//model1 = Model("./src/models/car/Porsche_911_GT2.obj");

	//Bloquear cursor a la ventana
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//bucle de dibujado
	while (!glfwWindowShouldClose(window)) {
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Establecer el color de fondo
		glViewport(0, 0, screenWidth, screenHeight);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(1.0, 1.0, 1.0, 1.0);

		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(myShader.Program, "Texture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(myShader.Program, "Texture2"), 1);
		glUniform1f(glGetUniformLocation(myShader.Program, "textureChange"), textureChange);*/

		proj = perspective(radians(myCamera.GetFOV()), aspectRatio, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "proj"), 1, GL_FALSE, value_ptr(proj));

		//Camara
		myCamera.DoMovement(window);
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "view"), 1, GL_FALSE, value_ptr(myCamera.LookAt()));

		myShader.USE();
		mat4 model = GenerateModelMatrix(vec3(1.0f), vec3(0.0f), vec3(0.0f));
		model1.Draw(myShader, GL_FILL);

		/*mat4 model = GenerateModelMatrix(vec3(1.0f), vec3(rotationX, rotationY, 0.0f), CubesPositionBuffer[0]);
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "matriz"), 1, GL_FALSE, value_ptr(model));
		printVAO(VAO);

		for (int i = 1; i < 10; i++) {
			mat4 model = GenerateModelMatrix(vec3(1.0f), vec3(glfwGetTime() * 100, glfwGetTime() * 100, 0.0f), CubesPositionBuffer[i]);
			glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "matriz"), 1, GL_FALSE, value_ptr(model));
			printVAO(VAO);
		}*/

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// liberar la memoria de los VAO, EBO y VBO
	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);*/
	//glDeleteBuffers(1, &EBO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//TODO, comprobar que la tecla pulsada es escape para cerrar la aplicación y la tecla w para cambiar a modo widwframe
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE); 

	//if (key == GLFW_KEY_W && action == GLFW_PRESS) WIDEFRAME = !WIDEFRAME;

	//Cambio de textura
	if (key == GLFW_KEY_1 && (action == GLFW_PRESS || action == GLFW_REPEAT)) if (textureChange < 1.0f) textureChange += 0.1f;
	if (key == GLFW_KEY_2 && (action == GLFW_PRESS || action == GLFW_REPEAT)) if (textureChange > 0.0f) textureChange -= 0.1f;

	//Rotacion del cubo
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) rotationX -= 1.0f;
	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) rotationX += 1.0f;
	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) rotationY -= 1.0f;
	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) rotationY += 1.0f;
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) autoRotation = !autoRotation;
}

void printVAO(GLuint VAO) {
	glBindVertexArray(VAO);
	if (WIDEFRAME) {
		//pintar con lineas
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	else {
		//pintar con triangulos
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
}

mat4 GenerateModelMatrix(vec3 scal, vec3 rotation, vec3 position) {
	mat4 scaleMat;
	scaleMat = scale(scaleMat, scal);

	mat4 rotationMat;
	rotationMat = rotate(rotationMat, radians(rotation[0]), vec3(1.0f, 0.0f, 0.0f));
	rotationMat = rotate(rotationMat, radians(rotation[1]), vec3(0.0f, 1.0f, 0.0f));
	rotationMat = rotate(rotationMat, radians(rotation[2]), vec3(0.0f, 0.0f, 1.0f));

	mat4 traslationMat;
	traslationMat = translate(traslationMat, position);

	return traslationMat * rotationMat * scaleMat;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	myCamera.MouseMove(window, xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	myCamera.MouseScroll(window, yoffset);
}