#include <GL/glew.h>  /* must be the 1st #include BEFORE ANY other #includes */
#include <GLFW/glfw3.h>
#include <iostream>

/* Vid#8 add includes to read, parse C++ external file: Basic.shader, and add to each Shader buffer */
#include <fstream> 
#include <string>
#include <sstream>

/* Vid#13: (8:00) add include "Renderer.h" */
#include "Renderer.h"

/* Vid#13: (18:30) Add #includes for two new classes */
#include "VertexBuffer.h"
#include "IndexBuffer.h"

/* Vid#14: (23:50) Add #includes for new VertexArray class */
#include "VertexArray.h"

/* Vid#15: (15:40) Add #includes for new Shader class */
#include "Shader.h"

/* Vid#16: (10:20) #include VertexBufferLayout.h" - removes the infinite include looping error */
#include "VertexBufferLayout.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Vid#12 (8:10) OpenGL GLFW  Version 3.3 create an Open Context and Window with the Core Profile 
        GLFW_OPENGL_CORE_PROFILE 
        Note:  ONLY (GLFW_CONTEXT_VERSION_MAJOR, 2) and (GLFW_CONTEXT_VERSION_MINOR, 1) WORKS!!!
        All other combinations of ints (e.g. 2, 3) of later major/minor versions Fails 
        with the following console output msg:

        C:\Dev\Cherno\OpenGL\bin\Win32\Debug\OpenGL.exe (process 4936) exited with code -1.
        To automatically close the console when debugging stops, 
        enable Tools->Options->Debugging->Automatically close the console when debugging stops.
        Press any key to close this window . . .
     */

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);


/****** Create a windowed mode window and its OpenGL context 
    glfwCreateWindow MUST BE PERFORMED BEFORE ANY glfwWindowHint(s) ******/

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
 
    if (window==NULL)
    {
        return -1;
    }

    /*** Make the window's context current - this MUST BE PEFORMED BEFORE glewInit() !!! ***/
    glfwMakeContextCurrent(window);

    /* Vid#11 (9:00) - should sync our Swap with the monitor's refresh rate
        and produce a smooth color change transition */
    GLCall(glfwSwapInterval(1));

    /*** Vid#3: JT Added Modern OpenGL code here - MUST FOLLOW glfwMakeContextCurrent(window) ***/
    if (glewInit() != GLEW_OK)
    {
        std::cout << "glewInit() Error!" << std::endl;
    }

    /*** Vid#3: JT Added Print Modern OpenGL Version code here ***/
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    /* Vid#9B: Vertex Buffer - remove 2 duplicate vertices of the 6 vertices in position[] to implement
            an Index Buffer */
    /* Create a New Scope {...} FROM Here TO BEFORE glfwTerminate(); */
    {
        float positions[] = {
            -0.5f, -0.5f, 0.0f, // vertex 0
             0.5f, -0.5f, 0.0f, // vertex 1
             0.5f,  0.5f, 0.0f, // vertex 2
            -0.5f,  0.5f, 0.0f, // vertex 3
        };

        /* Vid9B: create Index Buffer using new indices[] array
            note: must be unsigned but can use char, short, int, etc. */

        unsigned int indices[] = {
            0, 1, 2,        // 1st right triangle drawn CCW
            2, 3, 0         // 2nd inverted right triangle drawn CCW
        };
     
        /* Vid#14: (5:00) create VertexArray va and VertexBuffer vb AFTER creating Vertex Array Object (vao) */
        VertexArray va;

        //VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        VertexBuffer vb(positions, sizeof(positions));

        /* Vid#14: (24:00) create VertexBufferLayout */

        VertexBufferLayout layout;
        layout.Push<float>(3);
        va.AddBuffer(vb, layout);

        /* Vid#13: (19:35) Delete OR Comment Out IndexBuffer creation code,
            move to the new IndexBuffer Class, and replace with: */

        IndexBuffer ib(indices, 6);

    /* Vid#15: (12:15) Moved ParseShader to Shader.cpp constructor */

        /* Vid#15: (15:50) create Shader class instance and Bind it */

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        /* Vid#15: (16:00) call shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f) */
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    /* Vid#15: (17:00) Unbind VertexArray, the Unbind Shader, Unbind VertexBuffer , and Unbind IndexBuffer
       by setting each = 0 and re-bind all (3) inside the Rendering while loop before the glDraw cmd */

        va.Unbind();

        /* Vid#15: (16:50) delete glBindBuffers and add vb.UnBind() and ib.Unbind() */
        //GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        //GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        vb.Unbind(); 
        ib.Unbind();

        /* Vid#15: (16:15) changed GLCall(glUseProgram(0) to shader.Unbind() */
        shader.Unbind();

        /* Vid#16: (8:20) create instance of Renderer */
        Renderer renderer;

        /* Vid#11 (8:00) - Animate Loop: 1st define 4 float variables: r, g, b, and i */
        float r = 0.0f;             // red color float var initially set to zero
        float increment = 0.05f;    // color animation float increment var initially set to 0.05

        /* Games Render Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Vid#16: (10:30) Move glClear from Application.cpp to Renderer.h class Renderer public method
                and replace with renderer.Clear() Call */
            //GLCall(glClear(GL_COLOR_BUFFER_BIT))

            renderer.Clear();

            /* Vid#12: (4:45) Bind Shader (shader), Uniform (location), Vertex Buffer (buffer)
               and Index Buffer (ibo) BEFORE calling glDrawElements... */

            /* Vid#15: (16:20) changed GLCall(glUseProgram(shader) to shader.Bind() */
            shader.Bind();      // bind our shader
             
            /* Vid#15: (16:35) changed GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f))
                to hader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f)  */
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);    // setup uniform(s)

            /* Vid#16: (8:30) renderer.Draw() call passing in:
                VertexArray va, IndexBuffer ib, and Shader shader */

            renderer.Draw(va, ib, shader);

            /* Vid#16: (8:35) delete OR comment out glDrawElements b/c it's now inside the Renderer class */
            //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));  // Draw Elements call

            /* Vid#11 (8:30) check if r value > 1.0f --> set increment = -0.05f
                else if r value < 0.0f --> set increment = 0.05f */

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }

        /* Vid#15: (16:45) removed delete Shader b/c when the code hits end of scope,
            it will automatically be deleted by the ~destructor of the Shader class */
        //GLCall(glDeleteProgram(shader));
    }

    /* OpenGL GLFW Terminate destroys OpenGL context BEFORE IndexBuffer ~destructor is Called
        We now DO NOT have an OpenGL Context */
    glfwTerminate();
    return 0;
}