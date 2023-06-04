#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "camera.h"
#include "model.h"
#include "program.h"

// Global varaibles share between main.cpp and shader programs
class Context {
 public:
  // Index of program id in Context::programs vector
  // which is currently use to render all objects
  int currentProgram = 0;

 public:
  std::vector<Program*> programs;
  std::vector<Model*> models;
  std::vector<Object*> objects;

 public:
  // Parameter of lights use in light shader (TODO#4)
  int directionLightEnable = 1;
  glm::vec3 directionLightDirection = glm::vec3(-0.3f, -0.5f, -0.2f);
  glm::vec3 directionLightColor = glm::vec3(0.6f, 0.6f, 0.6f);

  int pointLightEnable = 0;
  glm::vec3 pointLightPosition = glm::vec3(6, 3, 0);
  glm::vec3 pointLightColor = glm::vec3(1.0f, 0.0f, 0.0f);
  float pointLightConstant = 0.9f;
  float pointLightLinear = 0.027f;
  float pointLightQuardratic = 0.0128f;
  float _pointLightPosisionDegree = 0.0f;

  int spotLightEnable = 0;
  glm::vec3 spotLightPosition = glm::vec3(0, 4, 5);
  glm::vec3 spotLightDirection = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 spotLightColor = glm::vec3(0.9f, 0.9f, 0.9f);
  float spotLightCutOff = glm::cos(glm::radians(12.5f));
  float spotLightOuterCutOff = glm::cos(glm::radians(17.5f));
  float spotLightConstant = 0.9f;
  float spotLightLinear = 0.014f;
  float spotLightQuardratic = 0.007f;
  float MixVal = 0.5;

  int Stop = 0;
  float speed_control = 5;
  float rmin = 104.06, rmax = 143.375, hmax = 4000;
  int knum = 108;
  float cylinder_height = 3.0;
  float square_h = 0.5;
  float cylinder_R = 1.0;
  bool model_change = false;
  bool Square = false;
  int woodmap = 0;
  bool camera_move = true;
  float Diffuse_roughness = 0.5;
  float range_k0 = 0.4;
  float range_k1 = 0.73;
  float range_d0 = 0.1;
  float range_d1 = 0.2;
  float k_s = 1.5;
  float k_b = 5.0;

  float dead_color_factor = 0;
  float dead_outline_factor = 0.65;
  float dead_outline_thickness = 0.02;
  glm::vec3 knotColor = glm::vec3(0.20, 0.20, 0.15); 
 public:
  Camera* camera = 0;
  GLFWwindow* window = 0;
};