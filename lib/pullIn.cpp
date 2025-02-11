// #include "assetLoaders/ImageLoader.hpp"
// #include "assetLoaders/ModelLoader.hpp"
// #include "assetLoaders/ObjLoader.hpp"
// #include "assetLoaders/STLLoader.hpp"
// #include "glWrappers/FrameBuffer.hpp"
// #include "glWrappers/FrameBufferManager.hpp"
// #include "glWrappers/Texture.hpp"
// #include "glWrappers/TextureManager.hpp"
// #include "glWrappers/VAOModel.hpp"
// #include "pipeline/BasePipline.hpp"
// #include "pipeline/DefferedGeometry.hpp"
// #include "pipeline/DefferedLighting.hpp"
// #include "pipeline/FinalComposition.hpp"
// #include "pipeline/ForwardRendering.hpp"
// #include "pipeline/PostProcessing.hpp"
// #include "sceneModel/Camera.hpp"
#include "sceneModel/Model.hpp"
#include "sceneModel/ModelManager.hpp"
#include "sceneModel/SceneGraph.hpp"
#include "sceneModel/SceneNode.hpp"
#include "ecco/EccoObject.hpp"
#include "ecco/EccoOutcome.hpp"
#include "ecco/EccoManager.hpp"
#include "ecco/EccoProduct.hpp"
#include "glWrappers/FrameBuffer.hpp"
#include "glWrappers/FrameBufferManager.hpp"
// #include "shaderManager/Shader.hpp"
// #include "shaderManager/ShaderProgram.hpp"
// #include "shaderManager/ShaderProgramManager.hpp"


void func()
{
    ecco::SceneGraph::SceneNode node("");
    ecco::SceneGraph::SceneGraph graph("");
    std::shared_ptr<ecco::EccoManager<ecco::EccoProduct>> manager = ecco::EccoManager<ecco::EccoProduct>::GetInstance();

    //Doesnt work
    // ecco::OpenGL::FrameBuffer m("",1,1);

    //dont do this
    auto m_fboManager = ecco::EccoManager<ecco::OpenGL::FrameBuffer>::GetInstance();
    // m_fboManager->CreateProduct("my fbo", 10,10,20);


    ecco::OpenGL::FrameBufferManager m_fboManager1 = ecco::OpenGL::FrameBufferManager::GetInstance();


    //fucking the template is pulling the function for the base class and not the specific
    //do i have to rewrite this for each impl?

}
