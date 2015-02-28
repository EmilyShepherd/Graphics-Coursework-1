#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include "utils.hpp"
#include <math.h>
#include <time.h>
#include <vector>
#include "Sphere.hpp"
#include "Cone.h"

#define A 0
#define B 1
#define C 2
#define D 3
#define E 4

GLchar *vertexsource, *fragmentsource;
GLuint vertexshader, fragmentshader;
GLuint shaderprogram;
GLuint vbo[1];

int mode           = A;        //

clock_t startTime, startTime2; // Animation clocks
int spin   = 0;                // Flag to turn spin on / off
int moveUp = 1;                // In E, is the sphere going up or down?

Sphere sphere = 0;
Cone cone = 0;

/**
 * Sets up boring OpenGL geometry stuff
 */
void SetupGeometry(void)
{
    std::vector<Vertex> buffer;
    buffer.reserve(sphere.getBuffer()->size() + sphere.getNormals()->size() + cone.getBuffer()->size());
    buffer.insert(buffer.end(), sphere.getBuffer()->begin(),  sphere.getBuffer()->end());
    buffer.insert(buffer.end(), sphere.getNormals()->begin(), sphere.getNormals()->end());
    buffer.insert(buffer.end(), cone.getBuffer()->begin(),    cone.getBuffer()->end());

    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(struct Vertex), &buffer[0], GL_STATIC_DRAW);

    glVertexAttribPointer((GLuint)0, 3, GL_DOUBLE, GL_FALSE, sizeof(struct Vertex), (const GLvoid*) offsetof (struct Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer((GLuint)1, 3, GL_DOUBLE, GL_TRUE, sizeof(struct Vertex), (const GLvoid*) offsetof(struct Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer((GLuint)2, 2, GL_DOUBLE, GL_FALSE, sizeof(struct Vertex), (const GLvoid*) offsetof(struct Vertex, UV));
    glEnableVertexAttribArray(2);
}

/**
 * Loads the shaders
 */
void SetupShaders(void)
{
    shaderprogram = glCreateProgram();

    vertexsource = filetobuf((char *)"main.vert");
    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
    glCompileShader(vertexshader);
    glAttachShader(shaderprogram, vertexshader);

    fragmentsource = filetobuf((char *)"main.frag");
    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
    glCompileShader(fragmentshader);
    glAttachShader(shaderprogram, fragmentshader);

    glBindAttribLocation(shaderprogram, 0, "in_Position");
    glBindAttribLocation(shaderprogram, 1, "in_Normal");
    glBindAttribLocation(shaderprogram, 2, "in_UV");
    glLinkProgram(shaderprogram);
    glUseProgram(shaderprogram);
}

void RenderSphere()
{
    float diff = (((float)clock() - (float)startTime) / 1000000.0F ) * 1000;

    glm::mat4 View       = glm::mat4(1.);
        if (spin) View       = glm::rotate(View, diff * 20, glm::vec3(0.f, 1.0f, 0.f));
    glm::mat4 Model      = glm::scale(glm::mat4(1.), glm::vec3(0.8));

    glm::mat4 MVP = View * Model;
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
    glUniform1i(glGetUniformLocation(shaderprogram, "shading"), mode == C ? 1 : 0);

    if (mode == E)
    {
        glUniform1i(glGetUniformLocation(shaderprogram, "texture"), 0);
        glUniform1i(glGetUniformLocation(shaderprogram, "shading"), 2);
    }

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mode == B)
    {
        glUniform3fv(glGetUniformLocation(shaderprogram, "color"), 1, glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));
        glDrawArrays(GL_LINES, sphere.getBuffer()->size(), sphere.getNormals()->size());
    }

    glUniform3fv(glGetUniformLocation(shaderprogram, "color"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 0.0)));
    glDrawArrays(mode == C || mode == E ? GL_TRIANGLES : GL_LINES, 0, sphere.getBuffer()->size());
}

void RenderAnimation()
{
    float diff = (((float)clock() - (float)startTime) / 1000000.0F ) * 1000;

    glUniform1i(glGetUniformLocation(shaderprogram, "shading"), 1);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 MVP = glm::mat4(1.);
    MVP = glm::scale(MVP, glm::vec3(0.75));
    MVP = glm::translate(MVP, glm::vec3(sin(diff), cos(diff), 0.0));
    MVP = glm::scale(MVP, glm::vec3(0.13));

    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
    glUniform3fv(glGetUniformLocation(shaderprogram, "color"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 1.0)));
    glDrawArrays(GL_TRIANGLES, 0, sphere.getBuffer()->size());

    MVP = glm::mat4(1.);
    MVP = glm::scale(MVP, glm::vec3(0.75));
    MVP = glm::translate(MVP, glm::vec3(sin(diff), cos(diff), 0.0));
    MVP = glm::scale(MVP, glm::vec3(0.1));
    MVP = glm::rotate(MVP, 100 * diff, glm::vec3(0.0, -1.0, .1));
    MVP = glm::translate(MVP, glm::vec3(0., 0.0, -2.0));
    MVP = glm::rotate(MVP, 90.0f, glm::vec3(0.0, 0.0, -1.0));

    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
    glUniform3fv(glGetUniformLocation(shaderprogram, "color"), 1, glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));
    glDrawArrays(GL_TRIANGLES, sphere.getBuffer()->size() + sphere.getNormals()->size(), cone.getBuffer()->size());

    MVP = glm::mat4(1.);
    MVP = glm::rotate(MVP, diff * 57.3f, glm::vec3(0.0, 0.0, -1.0));
    MVP = glm::translate(MVP, glm::vec3(0.0, 0.5, 0.0));
    MVP = glm::scale(MVP, glm::vec3(0.1, 0.17, 0.1));

    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
    glUniform3fv(glGetUniformLocation(shaderprogram, "color"), 1, glm::value_ptr(glm::vec3(0.0, 1.0, .5)));
    glDrawArrays(GL_TRIANGLES, sphere.getBuffer()->size() + sphere.getNormals()->size(), cone.getBuffer()->size());

    MVP = glm::mat4(1.);
    MVP = glm::rotate(MVP, diff * 57.3f / 12.f, glm::vec3(0.0, 0.0, -1.0));
    MVP = glm::translate(MVP, glm::vec3(0.0, 0.43, 0.0));
    MVP = glm::scale(MVP, glm::vec3(0.1, 0.1, 0.1));

    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
    glUniform3fv(glGetUniformLocation(shaderprogram, "color"), 1, glm::value_ptr(glm::vec3(0.0, 1.0, .5)));
    glDrawArrays(GL_TRIANGLES, sphere.getBuffer()->size() + sphere.getNormals()->size(), cone.getBuffer()->size());

    MVP = glm::mat4(1.);
    MVP = glm::scale(MVP, glm::vec3(0.3));

    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
    glUniform3fv(glGetUniformLocation(shaderprogram, "color"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 0.0)));
    glDrawArrays(GL_TRIANGLES, 0, sphere.getBuffer()->size());
}

void Render()
{
    if (mode == D)
    {
        RenderAnimation();
    }
    else
    {
        RenderSphere();
    }
}

/**
 * Handles key presses
 *
 * Accepts:
 *   + A - E / 1 - 5  to switch modes
 *   + Q / ESC        to close window
 *   + S              to spin model in modes A, B, C and E
 *
 * @param window   The window
 * @param key      The key that was pressed
 * @param scancode Lol, no idea what this is
 * @param action   Key Press / Key down / etc...
 * @param mods     Also something
 */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_1:
            case GLFW_KEY_A:
                mode = A;RenderSphere();
                break;

            case GLFW_KEY_2:
            case GLFW_KEY_B:
                mode = B;RenderSphere();
                break;

            case GLFW_KEY_3:
            case GLFW_KEY_C:
                mode = C;RenderSphere();
                break;

            case GLFW_KEY_4:
            case GLFW_KEY_D:
                mode = D;
                startTime = clock();
                spin = 0;
                break;

            case GLFW_KEY_5:
            case GLFW_KEY_E:
                mode       = E;
                break;

            case GLFW_KEY_S:
                if ( mode != D)
                {
                    spin      = spin ? 0 : 1;
                    startTime = clock();
                }
                break;
        }
    }
}

/**
 * main
 */
int main(void)
{
    if(!glfwInit())
    {
        fprintf(stderr, "Failed to start GLFW\n");
        exit(EXIT_FAILURE);
    }

    GLFWwindow* window = glfwCreateWindow(640, 640, "COMP3214 - Emily Shepherd", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "Failed to open window\n");
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);

    startTime = clock();

    SetupShaders();

    sphere = Sphere(30);
    cone   = Cone(20);

    loadBMP("images/world.bmp");
    SetupGeometry();

    while(!glfwWindowShouldClose(window))
    {
        Render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
