#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>  
#include <string>
#include <sstream>

struct ShaderSrc {
    std::string VertexSrc;
    std::string FragmentSrc;
};

static ShaderSrc ParseShader(const std::string& filePath) {
    std::ifstream input(filePath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(input, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if(line.find("fragment") != std::string::npos){
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& str_src) {
    unsigned int id = glCreateShader(type);
    const char* src = str_src.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    /* debug code ! */
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << std::endl << message << std::endl;
        glDeleteShader(id);
        return 0;
    }


    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;



    /* Initialize the library */
    if (!glfwInit())
        return -1;

    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Baro Lasha", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "ERROR!\n";

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };


    /* generate a vertex buffer */
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0
    };

    glfwSwapInterval(1);

    /* generate index buffer */
    unsigned int idx_buff;
    glGenBuffers(1, &idx_buff);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buff);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

    ShaderSrc source = ParseShader("Source/Resources/Basic.shader");
    

    unsigned int shader = CreateShader(source.VertexSrc, source.FragmentSrc);
    glUseProgram(shader);

    int u_id = glGetUniformLocation(shader, "u_Color");

    float red_val[] = {
        0.0f, 0.0f, 0.0f
    };
    int idx = 0;
    float diff = 0.02f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        

        glUniform4f(u_id, red_val[0], red_val[1], red_val[2], 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        if (red_val[idx] > 1.0f) {
            ++idx;
            if (idx % 3 == 0) {
                idx = 0;
                diff = -0.02f;
            }
        }
        else if (red_val[idx] < 0.0f) {
            ++idx;
            if (idx % 3 == 0) {
                idx = 0;
                diff = 0.02f;
            }
        }
        red_val[idx] += diff;
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}