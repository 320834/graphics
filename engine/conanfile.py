from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import copy

class EngineRecipe(ConanFile):
  name = "engine"
  version = "0.1.0"

  # Optional metadata
  license = "None"
  author = "JC"
  url = "None"
  description = "Basic rendering engine"
  topics = ("render")

  # Configuration
  settings = "os", "compiler", "build_type", "arch"
  options = {"shared": [True, False], "fPIC": [True, False]}
  default_options = {"shared": True, "fPIC": True}
  generators = "CMakeToolchain", "CMakeDeps"

  def requirements(self):
    self.requires("zlib/1.3.1", transitive_headers=True)
    self.requires("assimp/5.4.3", transitive_headers=True)
    self.requires("glm/1.0.1", transitive_headers=True)
    self.requires("glfw/3.4", transitive_headers=True)
    self.requires("stb/cci.20240531", force=True, transitive_headers=True)

  def export_sources(self):
    copy(self, "CMakeLists.txt", self.recipe_folder, self.export_sources_folder)
    copy(self, "include/*", self.recipe_folder, self.export_sources_folder)
    copy(self, "src/*", self.recipe_folder, self.export_sources_folder)
    copy(self, "README*", self.recipe_folder, self.export_sources_folder)
    copy(self, "demo_game/*", self.recipe_folder, self.export_sources_folder)
    copy(self, "cmake/*", self.recipe_folder, self.export_sources_folder)

  def config_options(self):
    if self.settings.os == "Windows":
        del self.options.fPIC

  def layout(self):
    cmake_layout(self)

  def build(self):
    cmake = CMake(self)
    cmake.configure()
    cmake.build()

  def package(self):
    cmake = CMake(self)
    cmake.install()

  def package_info(self):
    self.cpp_info.libs = ["engine"]

