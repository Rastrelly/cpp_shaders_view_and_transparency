#include "ourGraphics.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

OGLManager::OGLManager(int pwx, int pwy, GLFWframebuffersizefun callback)
{
	initOGL(pwx, pwy, callback);
}

unsigned int makeTexture(string fileName)
{
	unsigned int tex;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int iw, ih, nChan;
	unsigned char *data = stbi_load(fileName.c_str(), &iw, &ih, &nChan, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iw, ih, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Texture load failed " << fileName << endl;
	}

	stbi_image_free(data);

	return tex;
}

bool OGLManager::initOGL(int pwx, int pwy, GLFWframebuffersizefun callback)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(pwx, pwy, "Main Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	gladLoaded = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!gladLoaded)
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	glfwSetFramebufferSizeCallback(window, callback);

	return true;
}

void OGLManager::endDraw()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void drawOurVBO(flarr verts, int verts_block_size)
{
	//verts_block_size defines how many layouts
	//are transmited:

	/*
	 3- XYZ
	 6- XYZRGB
	 8- XYZRGBUV
	*/


	//buffers
	unsigned int VBO; //vertex buffer
	unsigned int VAO; //vertex array

	glGenBuffers(1, &VBO); //generate vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //assign data type to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*verts.size(), verts.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	if (verts_block_size >= 3)
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, verts_block_size * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	if (verts_block_size >= 6)
	{
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, verts_block_size * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	if (verts_block_size >= 8)
	{
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, verts_block_size * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
	glDrawArrays(GL_TRIANGLES, 0, verts.size() / verts_block_size);

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void drawOurEBO(flarr verts, intarr inds, unsigned int tex, int verts_block_size)
{

	//buffers
	unsigned int VBO; //vertex buffer
	unsigned int EBO; //elemnt buffer
	unsigned int VAO; //vertex array

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO); //generate vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //assign data type to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*verts.size(), verts.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO); //generate element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //assgn data type
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*inds.size(), inds.data(), GL_STATIC_DRAW);

	glBindVertexArray(VAO);

	if (verts_block_size >= 3)
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, verts_block_size * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	if (verts_block_size >= 6)
	{
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, verts_block_size * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	if (verts_block_size >= 8)
	{
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, verts_block_size * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	glDrawElements(GL_TRIANGLES, inds.size(), GL_UNSIGNED_INT, 0);

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
}