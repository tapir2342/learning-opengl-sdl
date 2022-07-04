#ifndef TEXTURE_H
#define TEXTURE_H

#ifndef __glad_h_
#include <glad/glad.h>
#endif

class Texture {
public:
  unsigned int id;
  unsigned int width, height;
  unsigned int format;
  unsigned int image_format;
  unsigned int wrap_s;
  unsigned int wrap_t;
  unsigned int filter_min;
  unsigned int filter_max;

  Texture();
  void generate(unsigned int width, unsigned int height, unsigned char *data);
  void bind() const;
};

#endif
