
#include <iostream>
#include <memory>
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


#include "shaderManager/Shader.hpp"
// #include "shaderManager/ShaderProgram.hpp"
// #include "shaderManager/ShaderProgramManager.hpp"

std::shared_ptr<ecco::OpenGL::FrameBufferManager> m_fboManager;
std::shared_ptr<ecco::OpenGL::AttachmentManager> m_attachmentManager;
std::shared_ptr<ecco::Base::AppDelegate> m_appDelegate;

void TestSceneGraph();
void InitializeShaders();

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

    TestSceneGraph();

    ecco::Base::AppDelegate::SetAppName("MY APP");
    m_appDelegate = ecco::Base::AppDelegate::GetInstance();

    m_appDelegate->SetInitializeShadersCB([]() {InitializeShaders();});

    m_appDelegate->Run();


}

void InitializeShaders() {
    ecco::OpenGL::Shader::SetShaderLoadDir("../shaders/general/");

    auto shader = ecco::OpenGL::Shader("Vertex", "vert-Vertex.glsl", ecco::OpenGL::ST_Vertex);
    std::cout << shader.GetCompileLog() << std::endl;
    std::cout << "Shader Loaded: " << shader.GetIsLoaded() << std::endl;
    std::cout << "Shader Compilation: " << shader.GetIsCompiled() << std::endl;
}

void TestSceneGraph() {
    std::shared_ptr<ecco::SceneGraph::SceneGraph> sceneGraph = std::make_shared<ecco::SceneGraph::SceneGraph>("");
    std::shared_ptr<ecco::SceneGraph::SceneNode> rootNode = std::make_shared<ecco::SceneGraph::SceneNode>("root");
    std::shared_ptr<ecco::SceneGraph::SceneNode> l1Child1 = std::make_shared<ecco::SceneGraph::SceneNode>("L1C1");
    std::shared_ptr<ecco::SceneGraph::SceneNode> l1Child2 = std::make_shared<ecco::SceneGraph::SceneNode>("L1C2");
    std::shared_ptr<ecco::SceneGraph::SceneNode> l2Child1 = std::make_shared<ecco::SceneGraph::SceneNode>("L2C1");
    std::shared_ptr<ecco::SceneGraph::SceneNode> l2Child2 = std::make_shared<ecco::SceneGraph::SceneNode>("L2C2");


    sceneGraph->SetRootNode(rootNode);
    rootNode->AddChild(l1Child1);
    rootNode->AddChild(l1Child2);

    l1Child1->AddChild(l2Child1);
    l1Child1->AddChild(l2Child2);

    sceneGraph->PrintTree();
}
