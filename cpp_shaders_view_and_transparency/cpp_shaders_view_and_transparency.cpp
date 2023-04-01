#include "ourGraphics.h"


Shader * shad;
unsigned int ourTexture;
int winx, winy;


flarr vertices = {
	// positions          // colors           // texture coords
	 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // top right
	 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // bottom right
	-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // bottom left
	-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // top left 
};

intarr indices =
{
	0, 1, 3,
	1, 2, 3
};

void prepShaders()
{
	std::cout << "Preparing shaders..." << std::endl;
	shad = new Shader("shader_vert.gls", "shader_frag.gls");
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	OGLManager ogl(800,600,framebuffer_size_callback);
	prepShaders();
	ourTexture = makeTexture("kusanagi_img_transp.png");

	while (!glfwWindowShouldClose(ogl.window))
	{
		processInput(ogl.window);

		glfwGetWindowSize(ogl.window,&winx,&winy);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shad->use();

		shad->setInt("ourTexture", 0);

		glm::mat4 projection = glm::ortho((float)(-winx/2), (float)(winx/2),
			(float)(-winy/2), (float)(winy/2), -10.0f, 10.0f);
		glm::mat4 view = glm::mat4(1.0f);
		
		glm::mat4 model = glm::mat4(1.0f);


		float modscale = 1.0f;
		if (winx > winy) modscale = (float)winy/2;
		else modscale = (float)winx / 2;

		model = glm::scale(model, glm::vec3(modscale));

		shad->setMatrix4f("projection", projection);
		shad->setMatrix4f("view", view);
		shad->setMatrix4f("model", model);

		shad->setBool("useColour", false);

		drawOurEBO(vertices, indices, ourTexture, 8);

		ogl.endDraw();
	}

	return 0;
}