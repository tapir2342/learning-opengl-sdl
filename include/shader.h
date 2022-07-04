#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <HandmadeMath.h>

#ifndef __glad_h_
#include <glad/glad.h>
#endif

class Shader {
public:
  unsigned int id;
  Shader() {}
  Shader &use();
  void compile(const char *source_fs, const char *source_vs);
  void integer(const char *name, int value);
  void matrix4(const char *name, const hmm_mat4 &matrix);
  void vector3f(const char *name, const hmm_vec3 &value);

private:
  void check_compile_errors(unsigned int object, std::string type);
};

#endif
