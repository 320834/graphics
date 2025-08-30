from conan import ConanFile

class CVPrototype(ConanFile):
  name = "cvp"
  version = "0.1.0"

  settings = "os", "compiler", "build_type", "arch"
  generators = "CMakeToolchain", "CMakeDeps"

  def requirements(self):
    self.requires("engine/0.1.0")
    self.requires("opencv/4.12.0")

  def configure(self):
     self.options["opencv"].face = True
     self.options["opencv"].with_gtk = True
     self.options["opencv"].with_wayland = False
     self.options["opencv"].with_v4l = True
