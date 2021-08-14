#include "RenderCommand.hpp"
#include "glad/glad.h"

namespace FloraEngine {
void RenderCommand::DrawIndices(Mesh *mesh, Shader *shader) {

  glBindVertexArray(mesh->VAO);
  glGenBuffers(1, &mesh->VBO);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(mesh->mVertices),
               mesh->mVertices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  shader->Use();
  glBindVertexArray(mesh->VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
} // namespace FloraEngine