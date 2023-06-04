#include <iostream>
#include <fstream>
#include "context.h"
#include "program.h"
bool first_pause = true;
double pause_time = 0;
double start_time = 0;
bool BasicProgram::load() {
  programId = quickCreateProgram(vertProgramFile, fragProgramFIle);
  
  int num_model = (int)ctx->models.size();
  VAO = new GLuint[num_model];

  /* TODO#2-2: Pass model vertex data to vertex buffer
   *           1. Generate and bind vertex array object (VAO) for each model
   *           2. Generate and bind three vertex buffer objects (VBOs) for each model
   *           3. Pass model positions, normals and texcoords to to VBOs
   * Note:
   *           1. You may refer to Programs/example.cpp and example.vert
   *           2. You may checkout basic.vert to get location index first
   *           3. You can get model data from ctx->models[i]->{posisions,normals,texcoords}
   * Keywords:
   *           - glGenVertexArrays
   *           - glBindVertexArray
   *           - glGenBuffers
   *           - glBufferData
   *           - glEnableVertexAttribArray
   *           - glVertexAttribPointer
   */
  
  glGenVertexArrays(num_model, VAO);
  for (int i = 0; i < num_model; i++) {
    glBindVertexArray(VAO[i]);
    Model* model = ctx->models[i];

    GLuint VBO[3];
    glGenBuffers(3, VBO); // ¦³position, normal, texcoord ¤T­Ó

    // position attribute
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model->positions.size(), model->positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    // normal attribute
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model->normals.size(), model->normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // texcoord attribute
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model->texcoords.size(), model->texcoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  }

  return programId != 0;
}

void BasicProgram::doMainLoop() {
  /* TODO#2-3: Render objects with shader
   *           1. use and bind program (BasicProgram::programId)
   *           2. Iterate all objects (ctx->objects)
   *           3. Load currect model VAO for object
   *           4. Pass projection, view, model matrix to shaders
   *           5. Pass model texture to shaders
   *           6. Draw with glDrawArrays
   * Note:
   *           1. You may refer to Programs/example.cpp and example.vert
   *           2. You may checkout definition of Context class, Model class and Object class first
   *           3. You can get object class from ctx->objects[i]
   *           4. You can get texture index of Model::textures for current object by Object::textureIndex
   *           5. you may also need model->drawMode, model->numVertex
   * Keywords:
   *           - glUseProgram
   *           - glBindVertexArray
   *           - glGetUniformLocation
   *           - glUniformMatrix4fv
   *           - glActiveTexture
   *           - glBindTexture
   *           - glUniform1i
   *           - glDrawArrays
   */
  glUseProgram(programId);

  /*Read json file*/
  
  GLint rminLoc = glGetUniformLocation(programId, "rmin");
  glUniform1f(rminLoc, ctx->rmin);
  GLint rmaxLoc = glGetUniformLocation(programId, "rmax");
  glUniform1f(rmaxLoc, ctx->rmax);
  GLint hmaxLoc = glGetUniformLocation(programId, "hmax");
  glUniform1f(hmaxLoc, ctx->hmax);
  GLint knumLoc = glGetUniformLocation(programId, "N");
  glUniform1i(knumLoc, ctx->knum);
  
  if (!ctx->Stop) {
    GLuint timeLoc = glGetUniformLocation(programId, "time");
    
    if (first_pause == false) {
      first_pause = true;
      start_time = (glfwGetTime() / ctx->speed_control);
    }
    glUniform1f(timeLoc, pause_time + ((glfwGetTime() / ctx->speed_control) - start_time));
    
    //float 
  } else {
    if (first_pause == true) {
      pause_time = pause_time + ((glfwGetTime() / ctx->speed_control) - start_time);
      first_pause = false;
    }
    
  }
  
  GLint roughnessLoc = glGetUniformLocation(programId, "roughness");
  glUniform1f(roughnessLoc, ctx->Diffuse_roughness);

  // light pos
  float lrot = 5.2;
  glm::vec3 light_pos_vec(cos(lrot), sin(lrot), 1.2f);
  GLint lightPosLoc = glGetUniformLocation(programId, "lightPos");
  glUniform3f(lightPosLoc, light_pos_vec.x, light_pos_vec.y, light_pos_vec.z);

  // dir light color
  glm::vec3 light_color(0.8f, 0.8f, 0.8f);
  GLint lightColLoc = glGetUniformLocation(programId, "lightColor");
  glUniform3f(lightColLoc, ctx->spotLightColor.x, ctx->spotLightColor.y, ctx->spotLightColor.z);

  // range_k & d
  GLint range_k0Loc = glGetUniformLocation(programId, "range_k0");
  glUniform1f(range_k0Loc, ctx->range_k0);
  GLint range_k1Loc = glGetUniformLocation(programId, "range_k1");
  glUniform1f(range_k1Loc, ctx->range_k1);

  GLint range_d0Loc = glGetUniformLocation(programId, "range_d0");
  glUniform1f(range_d0Loc, ctx->range_d0);
  GLint range_d1Loc = glGetUniformLocation(programId, "range_d1");
  glUniform1f(range_d1Loc, ctx->range_d1);

  // smooth k value
  GLint k_sLoc = glGetUniformLocation(programId, "k_s");
  glUniform1f(k_sLoc, ctx->k_s);
  GLint k_bLoc = glGetUniformLocation(programId, "k_b");
  glUniform1f(k_bLoc, ctx->k_b);

  // dead color
  GLint dead_color_factorLoc = glGetUniformLocation(programId, "dead_color_factor_1");
  glUniform1f(dead_color_factorLoc, ctx->dead_color_factor);

  GLint dead_outline_factorLoc = glGetUniformLocation(programId, "dead_outline_factor_1");
  glUniform1f(dead_outline_factorLoc, ctx->dead_outline_factor);

  GLint dead_outline_thicknessLoc = glGetUniformLocation(programId, "dead_outline_thickness_1");
  glUniform1f(dead_outline_thicknessLoc, ctx->dead_outline_thickness);

  // knots color
  GLint knotColorLoc = glGetUniformLocation(programId, "knotColor");
  glUniform3f(knotColorLoc, ctx->knotColor.x, ctx->knotColor.y, ctx->knotColor.z);

  int obj_num = (int)ctx->objects.size();
  
  for (int i = 0; i < obj_num; i++) {
    int modelIndex = ctx->objects[i]->modelIndex;
    glBindVertexArray(VAO[modelIndex]);

    Model* model = ctx->models[modelIndex];
    const float* p = ctx->camera->getProjectionMatrix();
    GLint pmatLoc = glGetUniformLocation(programId, "projection");
    glUniformMatrix4fv(pmatLoc, 1, GL_FALSE, p);

    const float* v = ctx->camera->getViewMatrix();
    GLint vmatLoc = glGetUniformLocation(programId, "view");
    glUniformMatrix4fv(vmatLoc, 1, GL_FALSE, v);

    //view pos
    const float* v_p = ctx->camera->getPosition();
    GLint vpmatLoc = glGetUniformLocation(programId, "viewPos");
    glUniform3f(vpmatLoc, v_p[0], v_p[1], v_p[2]);


    const float* m = glm::value_ptr(ctx->objects[i]->transformMatrix * model->modelMatrix);
    GLint mmatLoc = glGetUniformLocation(programId, "model");
    glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, m);

    // Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model->textures[0]);
    glUniform1i(glGetUniformLocation(programId, "ColorMap"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, model->textures[1]);
    glUniform1i(glGetUniformLocation(programId, "SpecularMap"), 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, model->textures[2]);
    glUniform1i(glGetUniformLocation(programId, "NormalMap"), 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, model->textures[3]);
    glUniform1i(glGetUniformLocation(programId, "PithRadiusMap"), 3);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, model->textures[4]);
    glUniform1i(glGetUniformLocation(programId, "KnotHeightMap"), 4);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, model->textures[5]);
    glUniform1i(glGetUniformLocation(programId, "KnotOrientMap"), 5);

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, model->textures[6]);
    glUniform1i(glGetUniformLocation(programId, "KnotStateMap"), 6);


    glDrawArrays(model->drawMode, 0, model->numVertex);
  }
  glUseProgram(0);

}