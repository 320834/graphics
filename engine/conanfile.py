from conan import ConanFile

class EngineRecipe(ConanFile):
  settings = "os", "compiler", "build_type", "arch"
  generators = "CMakeToolchain", "CMakeDeps"

  def requirements(self):
    self.requires("zlib/1.3.1")
    self.requires("assimp/5.4.3")
    self.requires("glm/1.0.1")
    self.requires("glfw/3.4")
    self.requires("stb/cci.20240531", force=True)