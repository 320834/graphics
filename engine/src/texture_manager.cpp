#include "texture_manager.h"
#include "stb_image.h"

#include <unordered_map>

std::unordered_map<std::string, unsigned int> 
  TextureManager::m_textures;

TextureStatus TextureManager::get_texture(
  const std::string& texture_name
)
{
  // std::string a = texture_name;
  auto search = m_textures.find(texture_name);
  if(search != m_textures.end()) {
    return {
      .texture_id = search->second,
      .success = true
    };
  }

  TextureStatus status =
    import_texture_file(texture_name);

  if(status.success) {

    utils::log(
      "Loading Texture " + texture_name + " from disk",
      "Texture Manager");
    // Saving texture in map
    m_textures.insert({texture_name, status.texture_id});
  }

  return status;
}

TextureStatus TextureManager::import_texture_file(
  const std::string& filename
)
{
  if(filename.empty()) {
    return {
      .texture_id = 0,
      .success = false
    };
  }

  const std::string path = filename;
  int width, height, nr_channels;
  
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(path.c_str(), &width, &height,
    &nr_channels, 0);

  // null that means something bad happened
  if(!data) {
    return {
      .texture_id = 0,
      .success = false
    };
  }

  auto pos = filename.find(".");
  if(pos == std::string::npos) {
    return {
      .texture_id = 0,
      .success = false
    };
  }

  const std::string extension =
    filename.substr(pos);

  TextureLoadData texture_data = {
    .width = width,
    .height = height,
    .nr_channels = nr_channels,
    .data = data
  };

  return load_texture(texture_data, extension);
}

TextureStatus TextureManager::load_texture(
  TextureLoadData& texture_data,
  const std::string extension
)
{
  unsigned int texture;
  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if(auto search = extension.find(".png"); search != std::string::npos) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_data.width, texture_data.height, 0, GL_RGBA,
      GL_UNSIGNED_BYTE, texture_data.data);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_data.width, texture_data.height, 0, GL_RGB,
      GL_UNSIGNED_BYTE, texture_data.data);
  }

  glGenerateMipmap(GL_TEXTURE_2D);
  // unsigned int use_texture_id = glGetUniformLocation(texture, "use_texture");
  // glUniform1i(use_texture_id, (int)true);

  stbi_image_free(texture_data.data);

  return {
    .texture_id = texture,
    .success = true
  };
}
