#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

#include <string>
#include <unordered_map>

#include <glad/glad.h>

#include "utils.h"

struct TextureStatus {
  unsigned int texture_id;
  bool success;
};

struct TextureLoadData {
  int width;
  int height;
  int nr_channels;
  unsigned char* data;
};

class TextureManager {

public:
  static TextureStatus get_texture(const std::string& texture_name);
private:

  static TextureStatus import_texture_file(
    const std::string& filename
  );
  static TextureStatus load_texture(
    TextureLoadData& texture_data,
    const std::string extension
  );

  static std::unordered_map<std::string, unsigned int> m_textures;
};

#endif //TEXTURE_MANAGER_H_
