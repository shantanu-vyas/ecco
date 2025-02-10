#pragma once
#include <string>

namespace ecco
{
    namespace Model
    {
        class Model
        {
            public:
                Model();
                ~Model();
                void LoadModel(std::string filename);
            private:
        };
    }
}
