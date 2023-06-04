#include <GLFW/glfw3.h>
#include <stdio.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#define GLAD_GL_IMPLEMENTATION
#include <gl.h>
#undef GLAD_GL_IMPLEMENTATION
#include "../include/imgui.h"
#include "../include/imgui_impl_glfw.h"
#include "../include/imgui_impl_opengl3.h"
#include "camera.h"
#include "context.h"
#include "gl_helper.h"
#include "model.h"
#include "opengl_context.h"
#include "program.h"
#include "utils.h"
#include "icons.h"
#include "stb_image.h"

void initOpenGL();
void resizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int, int action, int);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
Context ctx;

Material mFlatwhite;
Material mShinyred;
Material mClearblue;

void loadMaterial() {
  mFlatwhite.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
  mFlatwhite.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
  mFlatwhite.specular = glm::vec3(0.0f, 0.0f, 0.0f);
  mFlatwhite.shininess = 10;

  mShinyred.ambient = glm::vec3(0.1985f, 0.0000f, 0.0000f);
  mShinyred.diffuse = glm::vec3(0.5921f, 0.0167f, 0.0000f);
  mShinyred.specular = glm::vec3(0.5973f, 0.2083f, 0.2083f);
  mShinyred.shininess = 100.0f;

  mClearblue.ambient = glm::vec3(0.0394f, 0.0394f, 0.3300f);
  mClearblue.diffuse = glm::vec3(0.1420f, 0.1420f, 0.9500f);
  mClearblue.specular = glm::vec3(0.1420f, 0.1420f, 0.9500f);
  mClearblue.shininess = 10;
}

void loadPrograms() {
  ctx.programs.push_back(new BasicProgram(&ctx));

  for (auto iter = ctx.programs.begin(); iter != ctx.programs.end(); iter++) {
    if (!(*iter)->load()) {
      std::cout << "Load program fail, force terminate" << std::endl;
      exit(1);
    }
  }
  glUseProgram(0);
}

void loadModels() {
  Model* m1 = new Model();
  if (!ctx.Square) {
    float radius = ctx.cylinder_R;
    float height = ctx.cylinder_height;
    int numSegments = 30;
    glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0);
    float angleStep = (2 * M_PI) / numSegments;

    for (int i = 0; i < numSegments; i++) {
      glm::vec3 top_1 = glm::vec3(pos.x + radius * cos(2 * M_PI * i / double(numSegments)), pos.y + (height / 2.0),
                                  pos.z + radius * sin(2 * M_PI * i / double(numSegments)));
      glm::vec3 top_2 =
          glm::vec3(pos.x + radius * cos(2 * M_PI * (i + 1.0) / double(numSegments)), pos.y + (height / 2.0),
                    pos.z + radius * sin(2 * M_PI * (i + 1.0) / double(numSegments)));
      glm::vec3 bottom_1 = glm::vec3(pos.x + radius * cos(2 * M_PI * i / double(numSegments)), pos.y - (height / 2.0),
                                     pos.z + radius * sin(2 * M_PI * i / double(numSegments)));
      glm::vec3 bottom_2 =
          glm::vec3(pos.x + radius * cos(2 * M_PI * (i + 1.0) / double(numSegments)), pos.y - (height / 2.0),
                    pos.z + radius * sin(2 * M_PI * (i + 1.0) / double(numSegments)));

      glm::vec3 cross_top_bottom = glm::cross(top_2 - top_1, bottom_1 - top_1);

      // Top triangle
      m1->positions.push_back(pos.x);
      m1->positions.push_back(pos.z);
      m1->positions.push_back(pos.y + (height / 2.0));
      m1->normals.push_back(0.0f);
      m1->normals.push_back(-1.0f);
      m1->normals.push_back(0.0f);

      m1->positions.push_back(top_1.x);
      m1->positions.push_back(top_1.z);
      m1->positions.push_back(top_1.y);
      m1->normals.push_back(0.0f);
      m1->normals.push_back(-1.0f);
      m1->normals.push_back(0.0f);

      m1->positions.push_back(top_2.x);
      m1->positions.push_back(top_2.z);
      m1->positions.push_back(top_2.y);
      m1->normals.push_back(0.0f);
      m1->normals.push_back(-1.0f);
      m1->normals.push_back(0.0f);

      // Side triangle
      m1->positions.push_back(top_2.x);
      m1->positions.push_back(top_2.z);
      m1->positions.push_back(top_2.y);
      m1->normals.push_back(cross_top_bottom.x);
      m1->normals.push_back(cross_top_bottom.y);
      m1->normals.push_back(cross_top_bottom.z);

      m1->positions.push_back(top_1.x);
      m1->positions.push_back(top_1.z);
      m1->positions.push_back(top_1.y);
      m1->normals.push_back(cross_top_bottom.x);
      m1->normals.push_back(cross_top_bottom.y);
      m1->normals.push_back(cross_top_bottom.z);

      m1->positions.push_back(bottom_1.x);
      m1->positions.push_back(bottom_1.z);
      m1->positions.push_back(bottom_1.y);
      m1->normals.push_back(cross_top_bottom.x);
      m1->normals.push_back(cross_top_bottom.y);
      m1->normals.push_back(cross_top_bottom.z);

      m1->positions.push_back(bottom_2.x);
      m1->positions.push_back(bottom_2.z);
      m1->positions.push_back(bottom_2.y);
      m1->normals.push_back(cross_top_bottom.x);
      m1->normals.push_back(cross_top_bottom.y);
      m1->normals.push_back(cross_top_bottom.z);

      m1->positions.push_back(top_2.x);
      m1->positions.push_back(top_2.z);
      m1->positions.push_back(top_2.y);
      m1->normals.push_back(cross_top_bottom.x);
      m1->normals.push_back(cross_top_bottom.y);
      m1->normals.push_back(cross_top_bottom.z);

      m1->positions.push_back(bottom_1.x);
      m1->positions.push_back(bottom_1.z);
      m1->positions.push_back(bottom_1.y);
      m1->normals.push_back(cross_top_bottom.x);
      m1->normals.push_back(cross_top_bottom.y);
      m1->normals.push_back(cross_top_bottom.z);

      // Bottom triangle
      m1->positions.push_back(pos.x);
      m1->positions.push_back(pos.z);
      m1->positions.push_back(pos.y - (height / 2.0));
      m1->normals.push_back(0);
      m1->normals.push_back(-1);
      m1->normals.push_back(0);

      m1->positions.push_back(bottom_2.x);
      m1->positions.push_back(bottom_2.z);
      m1->positions.push_back(bottom_2.y);
      m1->normals.push_back(0);
      m1->normals.push_back(-1);
      m1->normals.push_back(0);

      m1->positions.push_back(bottom_1.x);
      m1->positions.push_back(bottom_1.z);
      m1->positions.push_back(bottom_1.y);
      m1->normals.push_back(0);
      m1->normals.push_back(-1);
      m1->normals.push_back(0);
    }

    m1->numVertex = numSegments * 12;
    m1->drawMode = GL_TRIANGLES;
    m1->modelMatrix = glm::scale(m1->modelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
  } else {
    float size_w = 1.0;  // 控制立方体的大小
    float size_h = ctx.square_h;
    // float halfSize = size / 2.0;

    // 顶点位置
    float pos[] = {
        // 正面
        -size_w, -size_w, size_w,  // V0
        size_h, -size_w, size_w,   // V1
        size_h, size_w, size_w,    // V2
        -size_w, size_w, size_w,   // V3
        // 背面
        -size_w, -size_w, -size_w,  // V4
        size_h, -size_w, -size_w,   // V5
        size_h, size_w, -size_w,    // V6
        -size_w, size_w, -size_w,   // V7
        // 左侧
        -size_w, -size_w, -size_w,  // V8
        -size_w, -size_w, size_w,   // V9
        -size_w, size_w, size_w,    // V10
        -size_w, size_w, -size_w,   // V11
                                    // 右侧
        size_h, -size_w, -size_w,   // V12
        size_h, -size_w, size_w,    // V13
        size_h, size_w, size_w,     // V14
        size_h, size_w, -size_w,    // V15
        // 顶部
        -size_w, size_w, size_w,   // V16
        size_h, size_w, size_w,    // V17
        size_h, size_w, -size_w,   // V18
        -size_w, size_w, -size_w,  // V19
        // 底部
        -size_w, -size_w, size_w,  // V20
        size_h, -size_w, size_w,   // V21
        size_h, -size_w, -size_w,  // V22
        -size_w, -size_w, -size_w  // V23
    };

    // 顶点索引
    unsigned int indices[] = {
        // 正面
        0, 1, 2,  // 第一个三角形
        0, 2, 3,  // 第二个三角形
        // 背面
        4, 6, 5,  // 第一个三角形
        4, 7, 6,  // 第二个三角形
        // 左侧
        8, 9, 10,   // 第一个三角形
        8, 10, 11,  // 第二个三角形
        // 右侧
        12, 14, 13,  // 第一个三角形
        12, 15, 14,  // 第二个三角形
        // 顶部
        16, 17, 18,  // 第一个三角形
        16, 18, 19,  // 第二个三角形
        // 底部
        20, 22, 21,  // 第一个三角形
        20, 23, 22   // 第二个三角形
    };

    for (int i = 0; i < 36; i++) {
      int index = indices[i];
      m1->positions.push_back(pos[3 * index]);
      m1->positions.push_back(pos[3 * index + 1]);
      m1->positions.push_back(pos[3 * index + 2]);
    }

    // 法线
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 3; j++) {
        m1->normals.push_back(0);
        m1->normals.push_back(1);
        m1->normals.push_back(0);
      }
    }
    m1->numVertex = 36;
    m1->drawMode = GL_TRIANGLES;
    m1->modelMatrix = glm::scale(m1->modelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
    // m1->modelMatrix *= glm::translate(glm::identity<glm::mat4>(), glm::vec3(2, 0, 0));
    m1->modelMatrix *= glm::rotate(m1->modelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  }

  if (ctx.woodmap == 0) {
    m1->textures.push_back(createTexture("../assets/models/plank/wood_color_maps/wood_bar_color.bmp", false, false));
  } else if (ctx.woodmap == 1) {
    m1->textures.push_back(createTexture("../assets/models/plank/wood_color_maps/wood_bar_color_1.bmp", false, false));
  } else if (ctx.woodmap == 2) {
    m1->textures.push_back(
        createTexture("../assets/models/plank/wood_color_maps/wood_bar_color_gray.bmp", false, false));
  } else if (ctx.woodmap == 3) {
    m1->textures.push_back(
        createTexture("../assets/models/plank/wood_color_maps/wood_bar_color_red.bmp", false, false));
  } else if (ctx.woodmap == 4) {
    m1->textures.push_back(
        createTexture("../assets/models/plank/wood_color_maps/wood_bar_color_gray_1.bmp", false, false));
  } else if (ctx.woodmap == 5) {
    m1->textures.push_back(
        createTexture("../assets/models/plank/wood_color_maps/Inkedwood_bar_color_1.jpg", false, false));
  }

  m1->textures.push_back(createTexture("../assets/models/plank/wood_color_maps/wood_bar_specular.bmp", false, false));
  m1->textures.push_back(createTexture("../assets/models/plank/wood_color_maps/wood_bar_normal.bmp", false, false));
  m1->textures.push_back(createTexture("../assets/models/plank/tree_geo_maps/pith_and_radius_map.bmp", false, true));
  m1->textures.push_back(createTexture("../assets/models/plank/tree_geo_maps/knot_height_map.bmp", false, false));
  m1->textures.push_back(createTexture("../assets/models/plank/tree_geo_maps/knot_orientation_map.bmp", false, false));
  m1->textures.push_back(createTexture("../assets/models/plank/tree_geo_maps/knot_state_map.bmp", true, false));
  ctx.models.push_back(m1);
}

void setupObjects() {
  ctx.objects.push_back(new Object(0, glm::translate(glm::identity<glm::mat4>(), glm::vec3(0, 0, 0))));
  (*ctx.objects.rbegin())->material = mFlatwhite;
}
void shutdown();
void mainPanel();
void renderUI(GLFWwindow* window);

int main() {
  initOpenGL();
  GLFWwindow* window = OpenGLContext::getWindow();
  glfwSetWindowTitle(window, "Group 39 - Tree Growth Rings");

  // Init Camera helper
  Camera camera(glm::vec3(0, 2, 5));
  camera.initialize(OpenGLContext::getAspectRatio());
  // Store camera as glfw global variable for callbasks use
  glfwSetWindowUserPointer(window, &camera);
  ctx.camera = &camera;
  ctx.window = window;

  loadMaterial();
  loadModels();
  loadPrograms();
  setupObjects();

  // Main rendering loop
  while (!glfwWindowShouldClose(window)) {
    if (ctx.model_change) {
      ctx.model_change = !ctx.model_change;
      ctx.models.clear();
      ctx.objects.clear();
      loadModels();
      loadPrograms();
      setupObjects();
    }
    // Polling events.
    glfwPollEvents();
    // Update camera position and view
    if (ctx.camera_move) {
      camera.move(window);
    }

    // GL_XXX_BIT can simply "OR" together to use.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /// TO DO Enable DepthTest
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearDepth(1.0f);
    ctx.spotLightDirection = normalize(glm::vec3(3, 0.3, 3) - ctx.spotLightPosition);
    ctx.pointLightPosition = glm::vec3(6 * glm::cos(glm::radians(ctx._pointLightPosisionDegree)), 3.0f,
                                       6 * glm::sin(glm::radians(ctx._pointLightPosisionDegree)));
    ctx.programs[ctx.currentProgram]->doMainLoop();

#ifdef __APPLE__
    // Some platform need explicit glFlush
#endif
    renderUI(window);
    glFlush();
    glfwSwapBuffers(window);
  }
  shutdown();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

void keyCallback(GLFWwindow* window, int key, int, int action, int) {
  // There are three actions: press, release, hold(repeat)
  if (action == GLFW_REPEAT) {
    switch (key) {
      case GLFW_KEY_K:
        ctx._pointLightPosisionDegree += 1.0f;
        break;
      case GLFW_KEY_L:
        ctx._pointLightPosisionDegree -= 1.0f;
        break;
      case GLFW_KEY_I:
        ctx.spotLightPosition.x = glm::clamp(ctx.spotLightPosition.x - 0.1f, -2.0f, 8.0f);
        break;
      case GLFW_KEY_O:
        ctx.spotLightPosition.x = glm::clamp(ctx.spotLightPosition.x + 0.1f, -2.0f, 8.0f);
        break;
      default:
        break;
    }
    return;
  }
  // Press ESC to close the window.
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    return;
  }

  if (action == GLFW_PRESS) {
    switch (key) {
      case GLFW_KEY_F9: {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
          // Show the mouse cursor
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
          // Hide the mouse cursor
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        break;
      }
      case GLFW_KEY_H:
        ctx.spotLightColor = glm::vec3(0.5f, 0.5f, 0.5f);
        break;
      case GLFW_KEY_J:
        ctx.spotLightColor = glm::vec3(0.9f, 0.9f, 0.9f);
        break;
      case GLFW_KEY_Y:
        ctx.spotLightColor = glm::vec3(0.8f, 0.6f, 0.6f);
        break;
      case GLFW_KEY_U:
        ctx.spotLightColor = glm::vec3(0.7f, 0.6f, 0.5f);
      default:
        break;
    }
  }
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    ctx.camera_move = !ctx.camera_move;
    if (ctx.camera_move) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }
}
void resizeCallback(GLFWwindow* window, int width, int height) {
  OpenGLContext::framebufferResizeCallback(window, width, height);
  auto ptr = static_cast<Camera*>(glfwGetWindowUserPointer(window));
  if (ptr) {
    ptr->updateProjectionMatrix(OpenGLContext::getAspectRatio());
  }
}

void initOpenGL() {
  // Initialize OpenGL context, details are wrapped in class.
#ifdef __APPLE__
  // MacOS need explicit request legacy support
  OpenGLContext::createContext(21, GLFW_OPENGL_ANY_PROFILE);
#else
  OpenGLContext::createContext(21, GLFW_OPENGL_ANY_PROFILE);
//  OpenGLContext::createContext(43, GLFW_OPENGL_COMPAT_PROFILE);
#endif
  GLFWwindow* window = OpenGLContext::getWindow();

  glfwSetKeyCallback(window, keyCallback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetFramebufferSizeCallback(window, resizeCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Initialize dear-ImGui
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 410 core");

  // Setup Icon fonts
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->AddFontDefault();
  ImFontConfig config;
  config.MergeMode = true;
  config.PixelSnapH = true;
  config.GlyphMinAdvanceX = 13.0f;  // monospaced
#ifndef NDEBUG
  OpenGLContext::printSystemInfo();
  OpenGLContext::enableDebugCallback();
#endif
}
void shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void mainPanel() {
  ImGui::Begin("Group 39");
  ImGui::SameLine();
  if (ImGui::Button("Stop")) ctx.Stop = !ctx.Stop;
  ImGui::SameLine();
  if (ctx.Stop) {
    ImGui::Text("Stop\n");
  } else {
    ImGui::Text("Play\n");
  }
  ImGui::SameLine();
  ImGui::SliderFloat("speed", &ctx.speed_control, 0.1f, 10.f);

  ImGui::SliderFloat("rmin", &ctx.rmin, 0.f, 150.f);
  ImGui::SliderFloat("rmax", &ctx.rmax, ctx.rmin, 250.f);
  ImGui::SliderFloat("hmax", &ctx.hmax, 3500.f, 4500.f);
  ImGui::SliderInt("knum", &ctx.knum, 50, 450);

  if (ImGui::SliderFloat("cylinder_height", &ctx.cylinder_height, 0.2f, 8.f)) {
    ctx.model_change = true;
  }

  if (ImGui::SliderFloat("cylinder_R", &ctx.cylinder_R, 0.1f, 1.8f)) {
    ctx.model_change = true;
  }

  if (ImGui::Button("Shape")) {
    ctx.Square = !ctx.Square;
    ctx.model_change = true;
  }
  ImGui::SameLine();
  if (ctx.Square) {
    ImGui::Text("Square\n");
  } else {
    ImGui::Text("Cylinder\n");
  }

  if (ImGui::SliderFloat("Rectangle height", &ctx.square_h, -0.5f, 1.f)) {
    ctx.model_change = true;
  }

  if (ImGui::Button("map1")) {
    ctx.woodmap = 0;
    ctx.model_change = true;
  }
  ImGui::SameLine();
  if (ImGui::Button("map2")) {
    ctx.woodmap = 1;
    ctx.model_change = true;
  }
  ImGui::SameLine();
  if (ImGui::Button("map3")) {
    ctx.woodmap = 2;
    ctx.model_change = true;
  }
  ImGui::SameLine();
  if (ImGui::Button("map4")) {
    ctx.woodmap = 3;
    ctx.model_change = true;
  }
  ImGui::SameLine();
  if (ImGui::Button("map5")) {
    ctx.woodmap = 4;
    ctx.model_change = true;
  }
  ImGui::SameLine();
  if (ImGui::Button("map6")) {
    ctx.woodmap = 5;
    ctx.model_change = true;
  }
  ImGui::SameLine();

  char wood_num = static_cast<char>(ctx.woodmap);
  if (ctx.woodmap == 0) {
    ImGui::Text("Map= 1");

  } else if (ctx.woodmap == 1) {
    ImGui::Text("Map= 2");
  } else if (ctx.woodmap == 2) {
    ImGui::Text("Map= 3");
  } else if (ctx.woodmap == 3) {
    ImGui::Text("Map= 4");
  } else if (ctx.woodmap == 4) {
    ImGui::Text("Map= 5");
  } else if (ctx.woodmap == 5) {
    ImGui::Text("Map= 6");
  }

  ImGui::SliderFloat("Diffuse_roughness", &ctx.Diffuse_roughness, 0.1f, 1.0f);

  ImGui::SliderFloat("range_k0", &ctx.range_k0, 0.3f, 1.5f);
  ImGui::SliderFloat("range_k1", &ctx.range_k1, ctx.range_k0, 2.0f);
  ImGui::SliderFloat("range_d0", &ctx.range_d0, 0.0f, 0.5f);
  ImGui::SliderFloat("range_d1", &ctx.range_d1, ctx.range_d0, 1.f);

  ImGui::SliderFloat("k_s", &ctx.k_s, 1.f, 2.5f);
  ImGui::SliderFloat("k_b", &ctx.k_b, 3.f, 6.5f);

  ImGui::SliderFloat("dead_color_factor", &ctx.dead_color_factor, 0.f, 3.f);
  ImGui::SliderFloat("dead_outline_factor", &ctx.dead_outline_factor, 0.f, 3.5f);
  ImGui::SliderFloat("dead_outline_thickness", &ctx.dead_outline_thickness, 0.f, 1.f);

  if (ImGui::Button("Dark")) {
    ctx.knotColor = ctx.knotColor + glm::vec3(0.01, 0.01, 0.01);
  }
  ImGui::SameLine();
  if (ImGui::Button("Bright")) {
    ctx.knotColor = ctx.knotColor - glm::vec3(0.01, 0.01, 0.01);
  }
  ImGui::SameLine();
  ImGui::Text("Knot Color");

  ImGui::End();
}
void renderUI(GLFWwindow* window) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  mainPanel();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
