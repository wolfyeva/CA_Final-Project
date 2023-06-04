#include "model.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <glm/vec3.hpp>

Model* Model::fromObjectFile(const char* obj_file) {
  Model* m = new Model();
  m->texcoords.clear();
  std::ifstream ObjFile(obj_file);

  if (!ObjFile.is_open()) {
    std::cout << "Can't open File !" << std::endl;
    return NULL;
  }

  /* TODO#1: Load model data from OBJ file
   *         You only need to handle v, vt, vn, f
   *         Other fields you can directly ignore
   *         Fill data into m->positions, m->texcoords m->normals and m->numVertex
   *         Data format:
   *           For positions and normals
   *         | 0    | 1    | 2    | 3    | 4    | 5    | 6    | 7    | 8    | 9    | 10   | 11   | ...
   *         | face 1                                                       | face 2               ...
   *         | v1x  | v1y  | v1z  | v2x  | v2y  | v2z  | v3x  | v3y  | v3z  | v1x  | v1y  | v1z  | ...
   *         | vn1x | vn1y | vn1z | vn2x | vn2y | vn2z | vn3x | vn3y | vn3z | vn1x | vn1y | vn1z | ...
   *           For texcoords
   *         | 0    | 1    | 2    | 3    | 4    | 5    | 6    | 7    | ...
   *         | face 1                                  | face 2        ...
   *         | v1x  | v1y  | v2x  | v2y  | v3x  | v3y  | v1x  | v1y  | ...
   * Note:
   *        OBJ File Format (https://en.wikipedia.org/wiki/Wavefront_.obj_file)
   *        For simplification, all faces from OBJ file are triangle (vertex per face = 3)
   */

  std::string line = "";
  std::string prefix = "";
  std::stringstream ss;

  std::vector<float> Pos; // 存讀取到的position(v)
  std::vector<float> Texcoord; // 存讀取到的texcoord(vt)
  std::vector<float> Normal; // 存讀取到的normal(vn)
  std::vector<int> Faces; // 存讀取到的face(f)
  while (getline(ObjFile, line)) {
    ss.clear();
    ss.str(line);
    ss >> prefix;

    if (prefix == "v") {//load position

      for (int i =0 ;i<3;i++) {
        float vertex_position;
        ss >> vertex_position;
        Pos.push_back(vertex_position);
        //std::cout << "v" << Pos.size()-1<< " Pos " << vertex_position << std::endl;
      
      }
      
    } else if (prefix == "vt") {//load texcoord

      for (int i = 0; i < 2; i++) {
        float texturecoordinates;
        ss >> texturecoordinates;
        Texcoord.push_back(texturecoordinates);
        //std::cout << "vt" << Texcoord.size() - 1 << " Texcoord " << texturecoordinates << std::endl;
      }

    } else if (prefix == "vn") {//load normal

      for (int i = 0; i < 3; i++) {
        float vertex_normal;
        ss >> vertex_normal;
        Normal.push_back(vertex_normal);
        //std::cout << "vn" << Normal.size() - 1 << " Normal " << vertex_normal << std::endl;
      }

    } else if (prefix == "f") {//load faces
      for (int i = 0; i < 3; i++) {
        std::string face;
        ss >> face;
        std::vector<std::string> face_vectorindex;

        face_vectorindex.push_back(face.substr(0, face.find("/"))); 
        face = face.substr(face.find("/") + 1, face.length()); 
        face_vectorindex.push_back(face.substr(0, face.find("/")));
        face = face.substr(face.find("/") + 1, face.length()); 
        face_vectorindex.push_back(face);

        Faces.push_back(stoi(face_vectorindex[0]) - 1);
        Faces.push_back(stoi(face_vectorindex[1]) - 1);
        Faces.push_back(stoi(face_vectorindex[2]) - 1);
        //std::cout << "face_vectorindex " << face_vectorindex[0] << std::endl;
        //std::cout << "face_vectorindex " << face_vectorindex[1] << std::endl;
        //std::cout << "face_vectorindex " << face_vectorindex[2] << std::endl;
      }
    }
    
  }

  for (int i = 0; i < Faces.size();i++) {

    //std::cout << "i" << i << " Faces" << Faces[i] << std::endl;

    m->positions.push_back(Pos[3 * Faces[i]]);      // pos.x
    m->positions.push_back(Pos[3 * Faces[i] + 1]);  // pos.y
    m->positions.push_back(Pos[3 * Faces[i] + 2]);  // pos.z

    i++;

    //std::cout << "i" << i << " Faces" << Faces[i] << std::endl;
    m->texcoords.push_back(Texcoord[2 * Faces[i]]);  // texcoord.x
    m->texcoords.push_back(Texcoord[2 * Faces[i] + 1]);  // texcoord.y
    i++;

    //std::cout << "i" << i << " Faces" << Faces[i] << std::endl;
    m->normals.push_back(Normal[3 * Faces[i]]);   // normal.x
    m->normals.push_back(Normal[3 * Faces[i] + 1]);  // normal.y
    m->normals.push_back(Normal[3 * Faces[i] + 2]);  // normal.z

  }
  m->numVertex = Faces.size() / 3 ;

  if (ObjFile.is_open()) ObjFile.close();

  return m;
}
