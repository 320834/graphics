void Engine::add_cube(
  const glm::vec3 position, const std::string texture
)
{
  m_cubes.emplace_back(
    m_shader.get_program(),
    position,
    texture
  );
}

void Engine::add_cube(
  const glm::vec3 position, const Color& color
)
{
  m_cubes.emplace_back(
    m_shader.get_program(),
    position,
    color
  );
}