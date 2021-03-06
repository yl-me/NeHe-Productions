
/* 代码中只在三角形三个顶点设置了颜色，而图像上的渐变是片段着色器自动插值的结果 */


// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

const GLuint WIDTH = 800, HEIGHT = 600;

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
// Shaders
const GLchar * vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 position;\n"
	"layout (location = 1) in vec3 color;\n"
	"out vec3 ourColor;\n"       // 为片段着色器指定一个颜色输出
	"void main()\n"
	"{\n"
	"gl_Position = vec4(position, 1.0);\n"
	"ourColor = color;\n"
	"}\0";

//const GLchar * fragmentShaderSource = "#version 330 core\n"
//	"in vec4 vertexColor;\n"     // 从顶点着色器传输来的变量
//	"out vec4 color;\n"          // 片段着色器输出的变量必须是vec4
//	"void main()\n"
//	"{\n"
//	"color = vertexColor;\n"
//	"}\0";
//const GLchar * fragmentShaderSource = "#version 330 core\n"
//	"out vec4 color;\n"
//	"uniform vec4 ourColor;\n"            // 片段着色器中的全局变量
//	"void main()\n"
//	"{\n"
//	"color = ourColor;\n"
//	"}\0";
const GLchar * fragmentShaderSource = "#version 330 core\n"
	"in vec3 ourColor;\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"color = vec4(ourColor, 1.0f);\n"
	"}\0";


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);      // opengl主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);      // opengl副版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);     // 设置为核心模式
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);           // 不能调整窗口大小

	GLFWwindow * window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;          // 设置为GL_TRUE是为了更好的使用核心模式
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);   // 渲染窗口大小
	glViewport(0, 0, width, height);

	// Vertex shader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);    // 创建一个顶点着色器
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);   // 把着色器源码附件到着色器对象上
	glCompileShader(vertexShader);       // 编译

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);     // 获取错误消息
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment shader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 着色器
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();    // 返回创建程序ID的引用

	glAttachShader(shaderProgram, vertexShader);    // 把编译的着色器附加到程序
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);               // 链接

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);           // 链接以后就删除
	glDeleteShader(fragmentShader);

	//GLfloat vertices[] = {
	//	-0.5f, -0.5f, 0.0f, // Left  
	//	0.5f, -0.5f, 0.0f, // Right 
	//	0.0f,  0.5f, 0.0f  // Top   
	//};

	//GLfloat vertices[] = {
	//	0.5f,  0.5f, 0.0f,  // Top Right
	//	0.5f, -0.5f, 0.0f,  // Bottom Right
	//	-0.5f, -0.5f, 0.0f,  // Bottom Left
	//	-0.5f,  0.5f, 0.0f   // Top Left 
	//};
	//GLuint indices[] = {  // Note that we start from 0!
	//	0, 1, 3,  // First Triangle
	//	1, 2, 3   // Second Triangle
	//};

	GLfloat vertices[] = {
		// 位置              // 颜色
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};

	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);       // 生成一个VBO对象
	glGenVertexArrays(1, &VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);     // 把VBO绑定到GL_ARRAY_BUFFER
	glBindVertexArray(VAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 把顶点数据复制到缓冲的内存中
	
	/*GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid *)0);   // 解析顶点数据
	glEnableVertexAttribArray(0);           // 启用顶点属性
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GLfloat)));   // 解析顶点数据
	glEnableVertexAttribArray(1);           // 启用颜色属性

	glBindBuffer(GL_ARRAY_BUFFER, 0);  // 可以安全解绑，对于glVertexAttribPointer绑定的VBO对象

	glBindVertexArray(0);  // 解绑VAO

	int count = 0;
	while (!glfwWindowShouldClose(window)) {   // 检查GLFW是否被要求退出
		//if (++count > 500) {
		//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   // 直到条件满足， 线框模式变为原来默认模式
		//	if (count > 1000) {
		//		count -= 1000;
		//	}
		//}
		//else {
		//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    // 设置线框模式
		//}
		glfwPollEvents();                      // 检查是否触发事件，来调用回调函数

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);


		GLfloat timeValue = glfwGetTime();
		GLfloat RedValue = (sin(timeValue) / 2) + 0.5f;
		GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, RedValue, 0.0f, 0.0f, 1.0f);


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);    // 指明索引缓冲渲染
		glBindVertexArray(0);

		glfwSwapBuffers(window);               // 颜色双缓冲
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
//	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_TRUE) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}