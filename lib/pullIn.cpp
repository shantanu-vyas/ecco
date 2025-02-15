
#include <iostream>
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
#include "glWrappers/AttachmentManager.hpp"
#include "appdelegate/AppDelegate.hpp"
// #include "shaderManager/Shader.hpp"
// #include "shaderManager/ShaderProgram.hpp"
// #include "shaderManager/ShaderProgramManager.hpp"

std::shared_ptr<ecco::OpenGL::FrameBufferManager> m_fboManager;
std::shared_ptr<ecco::OpenGL::AttachmentManager> m_attachmentManager;
std::shared_ptr<ecco::Base::AppDelegate> m_appDelegate;

int main()
{
    // std::shared_ptr<ecco::EccoManager<ecco::EccoProduct>> manager = ecco::EccoManager<ecco::EccoProduct>::GetInstance();

    m_fboManager = ecco::OpenGL::FrameBufferManager::GetInstance();
    m_attachmentManager = ecco::OpenGL::AttachmentManager::GetInstance();
    //not using auto here to confirm the type is correct
    std::shared_ptr<ecco::OpenGL::FrameBuffer> fbo = m_fboManager->CreateProduct("asdf", 1,1);
    std::shared_ptr<ecco::OpenGL::Attachment> attachment = m_attachmentManager->CreateProduct("asdfasdf", 1,1);

    std::cout << fbo->GetName() << std::endl;
    std::cout << attachment->GetName() << std::endl;
    ecco::Base::AppDelegate::SetAppName("MY APP");
    m_appDelegate = ecco::Base::AppDelegate::GetInstance();


}
