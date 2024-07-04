#include "include/shaders.h"
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

static char *loadShaderSource(char *filename) {
  FILE *file;
  file = fopen(filename, "r");

  if (NULL == file) {
    fprintf(stderr, "Failed to Open Shader File\n");
    exit(1);
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    fprintf(stderr, "Failed to seek end of file\n");
    fclose(file);
    exit(1);
  }

  long fileSize = ftell(file);
  if (fileSize == -1) {
    fprintf(stderr, "Failed to get file size\n");
    fclose(file);
    exit(1);
  }

  rewind(file);

  char *content_heap = malloc(fileSize + 1); // +1 for null terminator
  if (content_heap == NULL) {
    fprintf(stderr, "Failed to allocate memory\n");
    fclose(file);
    exit(1);
  }

  // Read bytes into memory
  size_t bytesRead = fread(content_heap, sizeof(char), fileSize, file);
  if (bytesRead < fileSize) {
    if (feof(file)) {
      fprintf(stderr, "Unexpected end of file.\n");
    } else if (ferror(file)) {
      fprintf(stderr, "Error reading shader file\n");
    }
    free(content_heap);
    fclose(file);
    exit(1);
  }

  content_heap[bytesRead] = '\0';

  fclose(file);

  return content_heap;
}

u32 compileAndLinkShaders(char *vertexShaderPath, char *fragmentShaderPath) {
  // Compiling Vertex Shader
  char *vertexShaderSource_heap = loadShaderSource(vertexShaderPath);
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1,
                 (const GLchar *const *)&vertexShaderSource_heap, NULL);
  glCompileShader(vertexShader);

  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
  }

  // Compiling Fragment Shader
  char *fragmentShaderSource_heap = loadShaderSource(fragmentShaderPath);
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1,
                 (const GLchar *const *)&fragmentShaderSource_heap, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
  }

  free(vertexShaderSource_heap);
  free(fragmentShaderSource_heap);

  // Linking Shader Program
  GLuint shaderProgramID = glCreateProgram();
  glAttachShader(shaderProgramID, vertexShader);
  glAttachShader(shaderProgramID, fragmentShader);
  glLinkProgram(shaderProgramID);
  glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgramID, 512, NULL,
                        infoLog); // Correct function to get linking log
    printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgramID;
}
