from conan import ConanFile

class CVPrototype(ConanFile):
  name = "cvp"
  version = "0.1.0"

  settings = "os", "compiler", "build_type", "arch"
  generators = "CMakeToolchain", "CMakeDeps"

  def requirements(self):
    self.requires("engine/0.1.0")