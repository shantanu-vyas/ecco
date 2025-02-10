#ifndef MODELMANAGER_HPP
#define MODELMANAGER_HPP

#include "../ecco/EccoObject.hpp"
#include "../glWrappers/VAOModel.hpp"
#include "Model.hpp"

namespace ecco
{
    namespace Model
    {
        class ModelManager : EccoObject
        {
            enum ModelType {
            Model,
            Instanced_Model
            };

            public:
                ModelManager();
                ~ModelManager();
                //TODO implement the ecco outcome class, everything should return an outcome
                void AddModel(const std::shared_ptr<ecco::Model::Model> model);
                void DeleteModel(const std::shared_ptr<ecco::Model::Model> model);
                void CreateAndAddModel();



            private:
                //Do we want to keep this as a map of model:vao? only downside is then we have to store the model in memory maybe we can write a cache
                std::vector<ecco::OpenGL::VAOModel> m_vaoModels;

        };
    }
}
#endif
