#include "VAOModel.hpp"

using namespace ecco::OpenGL;

// all these constructors are entirely unneeded if were going with model since model will hold all this data we can have a single constructor for model

VAOModel::VAOModel(const std::shared_ptr<ecco::Model::Model> &model)
    : ecco::EccoObject(model->GetName() + "-VBOModel"),
      m_model(model),
      m_hasInstancing(false),
      m_hasPerVertexColors(false),
      m_hasTexCoords(false) {
  // Set up base model VBO
}

VAOModel::VAOModel(const std::shared_ptr<ecco::Model::Model> &model,
                   const ecco::Model::InstancePositions &instancingPositions) :
    VAOModel(model) {

  m_hasInstancing = true;
  // Set up instancing VBO
}

VAOModel::VAOModel(const std::shared_ptr<ecco::Model::Model> &model,
         const ecco::Model::PerVertexColors &perVertexColors) :
    VAOModel(model) {

  m_hasPerVertexColors = true;
  // Set up per-vertex color VBO
}

VAOModel::VAOModel(const std::shared_ptr<ecco::Model::Model> &model,
         const ecco::Model::InstancePositions &instancingPositions,
         const ecco::Model::PerVertexColors &perVertexColors) :
     VAOModel(model, instancingPositions) {

  m_hasPerVertexColors = true;
  // Set up per-vertex color VBO
}

VAOModel::VAOModel(const std::shared_ptr<ecco::Model::Model> &model,
         const ecco::Model::TexCoords &uvCoords) :
     VAOModel(model) {

  m_hasTexCoords = true;
  // Set up UV coords VBO
}

VAOModel::VAOModel(const std::shared_ptr<ecco::Model::Model> &model,
         const ecco::Model::InstancePositions &instancingPositions,
         const ecco::Model::TexCoords &uvCoords) :
     VAOModel(model, instancingPositions) {

  m_hasTexCoords = true;
  // Set up UV coords VBO
}

VAOModel::VAOModel(const std::shared_ptr<ecco::Model::Model> &model,
         const ecco::Model::PerVertexColors &perVertexColors,
         const ecco::Model::TexCoords &uvCoords) :
    VAOModel(model, perVertexColors) {

  m_hasTexCoords = true;
  // Set up UV coords VBO
}

VAOModel::VAOModel(const std::shared_ptr<ecco::Model::Model> &model,
         const ecco::Model::InstancePositions &instancingPositions,
         const ecco::Model::PerVertexColors &perVertexColors,
         const ecco::Model::TexCoords &uvCoords) :
    VAOModel(model, instancingPositions, perVertexColors) {

  m_hasTexCoords = true;
  // Set up UV coords VBO
}

// assert if m_hasInstancing is false
void VAOModel::EnableInstancing() {

}
void VAOModel::DisableInstancing() {

}
bool VAOModel::GetHasInstancing() const {

}
bool VAOModel::GetIsInstancing() const {

}

void VAOModel::EnablePerVertexColors() {

} // assert if m_hasPerVertexColors is false
void VAOModel::DisablePerVertexColors() {

}
bool VAOModel::GetHasPerVertexColors() const {

}
bool VAOModel::GetIsPerVertexColors() const {

}

void CreateVAOWithGeometry(std::shared_ptr<ecco::Model::Model> model) {
    // model->GetTriangles()
    // model->GetVertices()

}
void CreateInstancingVBO(const ecco::Model::InstancePositions& instancePositions) {

}
void CreatePerVertexColorVBO(const ecco::Model::PerVertexColors& perVertexColors) {

}
void CreatePerVertexTexCoords(const ecco::Model::PerVertexColors& perVertexTexCoords) {

}
