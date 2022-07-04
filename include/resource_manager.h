#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include <stb_image.h>

#include "shader.h"
#include "texture.h"

class ResourceManager {
public:
  static std::map<std::string, Shader> shaders;
  static std::map<std::string, Texture> textures;
  static Shader load_shader(std::string name, const char *path_vs,
                            const char *path_fs);
  static Shader get_shader(std::string name);
  static Texture load_texture(std::string name, const char *path, bool alpha);
  static Texture get_texture(std::string name);
  static void clear();

private:
  ResourceManager() {}
  static Shader load_shader_from_file(const char *path_vs, const char *path_fs);
  static Texture load_texture_from_file(const char *path, bool alpha);
};

#endif
