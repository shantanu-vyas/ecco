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
#include "glWrappers/GLBuffers.hpp"
#include "appdelegate/AppDelegate.hpp"


#include "shaderManager/Shader.hpp"
#include "shaderManager/ShaderProgram.hpp"
// #include "shaderManager/ShaderProgramManager.hpp"

#include "turbo-ui/TurboView.hpp"
#include "turbo-ui/PrePostTestView.hpp"

#include "eventSystem/EccoEvent.hpp"
#include "eventSystem/EventDispatcher.hpp"

#include "ecco/EccoOutcome.hpp"

using ecco::OpenGL::VAOSubBuffer;

std::shared_ptr<ecco::OpenGL::FrameBufferManager> m_fboManager;
std::shared_ptr<ecco::OpenGL::AttachmentManager> m_attachmentManager;
std::shared_ptr<ecco::Base::AppDelegate> m_appDelegate;

void TestSceneGraph();
void InitializeShaders();
void TestTurboView();
void TestEventSystem();
void TestGLBuffers();
void TestOutcome();

int main()
{
    TestOutcome();
    exit(0);
    TestGLBuffers();
    // std::shared_ptr<ecco::EccoManager<ecco::EccoProduct>> manager = ecco::EccoManager<ecco::EccoProduct>::GetInstance();

    m_fboManager = ecco::OpenGL::FrameBufferManager::GetInstance();
    m_attachmentManager = ecco::OpenGL::AttachmentManager::GetInstance();
    //not using auto here to confirm the type is correct
    std::shared_ptr<ecco::OpenGL::FrameBuffer> fbo = m_fboManager->CreateProduct("asdf", 1,1);
    std::shared_ptr<ecco::OpenGL::Attachment> attachment = m_attachmentManager->CreateProduct("asdfasdf", 1,1);

    std::cout << fbo->GetName() << std::endl;
    std::cout << attachment->GetName() << std::endl;

    TestSceneGraph();
    TestEventSystem();

    ecco::Base::AppDelegate::SetAppName("MY APP");
    m_appDelegate = ecco::Base::AppDelegate::GetInstance();
    m_appDelegate->SetInitializeShadersCB([]() {InitializeShaders();});
    TestTurboView();
    m_appDelegate->Run();


}

void InitializeShaders() {
    ecco::OpenGL::Shader::SetShaderLoadDir("../shaders/general/");

    auto vshader = std::make_shared<ecco::OpenGL::Shader>("Vertex", "vert-Vertex.glsl", ecco::OpenGL::ST_Vertex);
    auto fshader = std::make_shared<ecco::OpenGL::Shader>("Fragment", "frag-Frag.glsl", ecco::OpenGL::ST_Fragment);
    std::cout << "Vertex" << std::endl;
    std::cout << vshader->GetCompileLog() << std::endl;
    std::cout << "Shader Loaded: " << vshader->GetIsLoaded() << std::endl;
    std::cout << "Shader Compilation: " << vshader->GetIsCompiled() << std::endl;
    std::cout << "Fragment" << std::endl;
    std::cout << fshader->GetCompileLog() << std::endl;
    std::cout << "Shader Loaded: " << fshader->GetIsLoaded() << std::endl;
    std::cout << "Shader Compilation: " << fshader->GetIsCompiled() << std::endl;

    std::vector<std::shared_ptr<ecco::OpenGL::Shader>> geom;
    auto shaderProgram = std::make_shared<ecco::OpenGL::ShaderProgram>("Shader", vshader, fshader, geom);
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

void TestTurboView() {
    std::cout << "Testing Turbo View " << std::endl;
    //std::shared_ptr<ecco::Turbo::RootTurboView> root = std::make_shared<ecco::Turbo::RootTurboView>("root");
    std::shared_ptr<ecco::Turbo::TurboView> l1Child1 = std::make_shared<ecco::Turbo::TurboView>("L1C1");
    std::shared_ptr<ecco::Turbo::TurboView> l1Child2 = std::make_shared<ecco::Turbo::TurboView>("L1C2");
    std::shared_ptr<ecco::Turbo::PrePostTestView> l2Child1 = std::make_shared<ecco::Turbo::PrePostTestView>("L2C1");
    std::shared_ptr<ecco::Turbo::PrePostTestView> l2Child2 = std::make_shared<ecco::Turbo::PrePostTestView>("L2C2");

    //renders are too many called getting called more than it should
    //something is funky with the hasparent assert, if ! it works for this but it breaks the root cycle case i think


    m_appDelegate->AddChild(l1Child1);
    m_appDelegate->AddChild(l1Child2);
    l1Child1->AddChild(l2Child1);
    l1Child1->AddChild(l2Child2);

    // root->RenderAll();
    //Add Root as child test
    // root->AddChild(l2Child1);
    // l2Child1->AddChild(root);

    //Cyclic Test
    // l1Child1->AddChild(l1Child2);
    // l1Child2->AddChild(l1Child1);

    // l1Child1->PrintTree();
    // m_appDelegate->PrintTree();
}

void TestEventSystem() {
    std::cout << "Testing Events" << std::endl;
    ecco::Event::EventDispatcher m_globalDispatcher("0");
    // m_globalDispatcher.SetEventCallback<ecco::Event::KeyEvent>([](const ecco::Event::KeyEvent& event){
    //     std::cout << "Global Dispatcher got key event"  << std::endl;
    // });

    ecco::Event::EventDispatcher m_uiElement1Dispatcher("1");
    m_uiElement1Dispatcher.SetEventCallback<ecco::Event::KeyEvent>([](const ecco::Event::EccoEvent& event) {
        std::cout << "1) UI element got event 1" << std::endl;
    });

    ecco::Event::EventDispatcher m_uiElement1Dispatcher2("1");
    m_uiElement1Dispatcher2.SetEventCallback<ecco::Event::KeyEvent>([](const ecco::Event::EccoEvent& event) {
        std::cout << "2) UI element got event 1" << std::endl;
    });


    m_globalDispatcher.AddListener<ecco::Event::KeyEvent>(m_uiElement1Dispatcher);
    m_globalDispatcher.AddListener<ecco::Event::KeyEvent>(m_uiElement1Dispatcher2);
    ecco::Event::KeyEvent::ModList mods;
    ecco::Event::KeyEvent::KeyList keys;
    m_globalDispatcher.dispatch(ecco::Event::KeyEvent("Key", mods, keys));

}

void TestGLBuffers() {
    using namespace ecco::OpenGL;
    auto vao = std::make_shared<ecco::OpenGL::VAO>();
    auto vao1 = std::make_shared<ecco::OpenGL::VAO>();

    auto vboV = std::make_shared<VAOSubBuffer<VBOSpecifier::VertexInfo>>();
    auto vboN = std::make_shared<VAOSubBuffer<VBOSpecifier::NormalInfo>>();
    auto vboT = std::make_shared<VAOSubBuffer<VBOSpecifier::TriangleInfo>>();


    vao1->SetAttachment(vboV);
    vao1->Attach(vboV);

    //so should doing this assert ot return false
    //if normals doesnt exist we return false
    //if it does exist but the specific vao doesnt we assert
    // vao1->Attach(vboN);


    vao1->PrintAllAttachments();
    vao1->RemoveAttachment(vboV);
    vao1->PrintAllAttachments();

}

void TestOutcome() {
    std::cout << "running" << std::endl;
    auto pass = []() -> ecco::StatusOutcome {
        return ecco::StatusOutcome::Success();
    };
    auto fail = []() -> ecco::StatusOutcome {
        return ecco::StatusOutcome::Failure("shit");
    };

    auto passData = []() -> ecco::OutcomeData<int> {
        return ecco::OutcomeData<int>::Success(420);
    };
    auto failData = []() -> ecco::OutcomeData<int> {
        return ecco::OutcomeData<int>::Failure("fuck");
    };

    //& so we get fail()
    auto failMacroTest = [&]() -> ecco::StatusOutcome {
        RETURN_ON_FAIL(fail());
        return ecco::StatusOutcome::Success();
    };

    auto failMacroDataTest = [&]() -> ecco::OutcomeData<int> {
    RETURN_ON_FAIL(failData());
    return ecco::OutcomeData<int>::Success(420);

};

    std::cout << "pass" << std::endl;
    auto passTest = pass();
    passTest.Print();

    std::cout << "fail" << std::endl;
    auto failTest = fail();
    failTest.Print();

    std::cout << "pass data" << std::endl;
    auto passDataTest = passData();
    passDataTest.Print();

    std::cout << "fail data" << std::endl;
    auto failDataTest = failData();
    failDataTest.Print();

    std::cout << "fail macro " << std::endl;
    auto failMacro = failMacroTest();
    failMacro.Print();

    std::cout << "fail macro data" << std::endl;
    auto failMacroData = failMacroDataTest();
    failMacroData.Print();


}
