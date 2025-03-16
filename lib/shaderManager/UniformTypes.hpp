#include <map>
#include <string>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <glm/glm.hpp>


namespace ecco
{
    namespace OpenGL
    {
        enum class UniformType
        {
            UNKNOWN,
            FLOAT,
            INT,
            BOOL,
            VEC2,
            VEC3,
            VEC4,
            MAT2,
            MAT3,
            MAT4,
            SAMPLER2D,
            OTHER
        };

        template <UniformType>
        struct UniformCppType;

        // Typedef the actual type to the keyword type
        template <> struct UniformCppType<UniformType::FLOAT>     { using type = float; };
        template <> struct UniformCppType<UniformType::INT>       { using type = int; };
        template <> struct UniformCppType<UniformType::BOOL>      { using type = bool; };
        template <> struct UniformCppType<UniformType::VEC2>      { using type = glm::vec2; };
        template <> struct UniformCppType<UniformType::VEC3>      { using type = glm::vec3; };
        template <> struct UniformCppType<UniformType::VEC4>      { using type = glm::vec4; };
        template <> struct UniformCppType<UniformType::MAT2>      { using type = glm::mat2; };
        template <> struct UniformCppType<UniformType::MAT3>      { using type = glm::mat3; };
        template <> struct UniformCppType<UniformType::MAT4>      { using type = glm::mat4; };
        template <> struct UniformCppType<UniformType::SAMPLER2D> { using type = int; };

        /**
         * I would prefer to do
         *   UniformCpp<UniformType::FLOAT> m_float;
         *   vs.
         *   UniformCpp<UniformType::FLoat>::type m_float
         * Note this isnt actually the usecase. This will be for error checking that we are setting uniforms with the correct types.
         **/
        template <UniformType T>
        using UniformCpp = typename UniformCppType<T>::type;

        static const std::unordered_map<std::string, UniformType> uniformTypeLookup{
            {"float", UniformType::FLOAT},
            {"int", UniformType::INT},
            {"bool", UniformType::BOOL},
            {"vec2", UniformType::VEC2},
            {"vec3", UniformType::VEC3},
            {"vec4", UniformType::VEC4},
            {"mat2", UniformType::MAT2},
            {"mat3", UniformType::MAT3},
            {"mat4", UniformType::MAT4},
            {"sampler2D", UniformType::SAMPLER2D}
        };


        //Converts a UniformType to a UniformCppType (cpp type) [Templated Version] auto type = GetCppType<UniformType::FLOAT>();
        template <UniformType T>
        inline std::type_index GetCppType()
        {
            return typeid(typename UniformCppType<T>::type);
        }

        //Converts a UniformType to a UniformCppType (cpp type) [Argument Version] auto type = GetCppType(UniformType::FLOAT);
        inline std::type_index GetCppType(UniformType uniformType)
        {
            switch (uniformType)
            {
                case UniformType::FLOAT:      return GetCppType<UniformType::FLOAT>();
                case UniformType::INT:        return GetCppType<UniformType::INT>();
                case UniformType::BOOL:       return GetCppType<UniformType::BOOL>();
                case UniformType::VEC2:       return GetCppType<UniformType::VEC2>();
                case UniformType::VEC3:       return GetCppType<UniformType::VEC3>();
                case UniformType::VEC4:       return GetCppType<UniformType::VEC4>();
                case UniformType::MAT2:       return GetCppType<UniformType::MAT2>();
                case UniformType::MAT3:       return GetCppType<UniformType::MAT3>();
                case UniformType::MAT4:       return GetCppType<UniformType::MAT4>();
                case UniformType::SAMPLER2D:  return GetCppType<UniformType::SAMPLER2D>();
                default:                      return typeid(void);
            }
        }

        //Converts a string to a UniformCppType
        inline std::type_index GetCppType(const std::string& stringType)
        {
            auto it = uniformTypeLookup.find(stringType);
            if (it != uniformTypeLookup.end())
                return GetCppType(it->second);
            return typeid(void);
        }
    }
}
