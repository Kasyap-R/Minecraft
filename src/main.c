#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#include "common.h"
#include "shaders.h"
#include "stb_image.h"
#include <cglm/cglm.h>
#include <cglm/mat4.h>

void framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

i32 main() {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Minecraft", NULL, NULL);
  if (!window) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // Initialize GLAD before calling any OpenGL functions
  // This loads function pointers specific to our OpenGL verison (located in the
  // GPU driver)
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

  // Unused currently
  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  // Generate and bind VAO
  u32 VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Generate and bind VBO
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Generate and bind EBO
  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // Set vertex attribute pointers for coords, color, and texture
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Unbind VAO and EBO (good practice)
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  u32 shader =
      compileAndLinkShaders("shaders/triangle.vert", "shaders/triangle.frag");

  // Configure texture wrapping for both the s and t axes
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  // Configure texture filtering when upscaling (using interpolation)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Configure texture filtering when downscaling (using mipmaps)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);

  // Load Texture
  int width, height, nrChannels;
  unsigned char *data =
      stbi_load("textures/Wood_Texture.png", &width, &height, &nrChannels, 0);

  // Create, bind and generate texture object
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);

  free(data);

  // Create minmaps for currently bound texture
  glGenerateMipmap(GL_TEXTURE_2D);

  // Setting up view matrix
  // note that we're translating the scene in the reverse direction of where we
  // want to move (i.e. if we want to move forward, move the scene backward)
  mat4 view_mat;
  glm_mat4_identity(view_mat);
  vec3 view_vec = {0.0, 0.0f, -3.0f};
  glm_translate(view_mat, view_vec);

  // Setting up projection matrix
  mat4 projection_mat;
  glm_mat4_identity(projection_mat);
  glm_perspective(glm_rad(95.0f), 800.0f / 600.0f, 0.1f, 100.0f,
                  projection_mat);

  // Enabel Depth Testing
  glEnable(GL_DEPTH_TEST);

  // Set up Camera
  vec3 cameraPos = {0.0f, 0.0f, 3.0f};
  vec3 cameraTarget = {0.0f, 0.0f, 0.0f};
  vec3 cameraDirection;
  glm_vec3_zero(cameraDirection);
  glm_vec3_sub(cameraPos, cameraTarget, cameraDirection);
  glm_normalize(cameraDirection);

  vec3 up = {0.0f, 1.0f, 0.0f};
  vec3 cameraRight;
  glm_vec3_zero(cameraRight);
  glm_cross(cameraDirection, cameraRight, cameraRight);
  glm_normalize(cameraRight);

  vec3 cameraUp;
  glm_vec3_zero(cameraUp);
  glm_cross(cameraDirection, cameraRight, cameraUp);

  // Uncomment to draw in wireframe mode
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {
    // Process input
    processInput(window);

    // Clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Setting up model matrix
    mat4 model_mat;
    glm_mat4_identity(model_mat);
    vec3 rotation_axis = {0.5f, 1.0f, 0.0f}; // x-axis
    glm_rotate(model_mat, (float)glfwGetTime() * glm_rad(50.0f), rotation_axis);

    // Setting up transformation matrix
    mat4 transform_mat;
    glm_mat4_identity(transform_mat);
    glm_mat4_mul(model_mat, transform_mat, transform_mat);
    glm_mat4_mul(view_mat, transform_mat, transform_mat);
    glm_mat4_mul(projection_mat, transform_mat, transform_mat);

    // Use the shader program
    glUseProgram(shader);

    // Set transformation matrix in vertex shader
    unsigned int transformLoc = glGetUniformLocation(shader, "transformation");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform_mat[0]);

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Swap buffers and poll events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
