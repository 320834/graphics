int test()
{
  glm::mat4 translation {
    1.0f, 2.0f, 0.0f, -5,
    3.0f, 1.0f, 6.0f, -10,
    0.0f, 0.0f, 1.0f, -15,
    0.0f, 0.0f, 0.0f, 1
  };

  std::cout << translation[1][2] << std::endl;
}