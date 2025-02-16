# LIB INFO 


## appdelegate (Mostly impl)
### AppDelegate - Launches UI and handles input using GLFW 

## assetLoaders (not impl)

## ecco - Base Classes
### EccoLogger (not impl)
### EccoManager - Templated singleton class for making EccoProducts, i think not entirely correctly written 
### EccoProduct - Base class for types (FBOs, Materials, Models etc...) that Managers can template on and create/manage
### EccoObject - Base Ecco Type for all classes
### EccoTimer - (not impl)
### EccoOutcome - (not impl) Should return a status + return value like a tuple but more structured

## glWrappers C++ wrappers around GL types that will be managed by managers
### Attachment - Not fully impl 
### AttachmentManager - Not fully 
### FrameBuffer - Not fully impl
### FrameBufferManager - not fully impl
### Texture - not not impl
### TextureManager - not impl
### VAOModel? idk if this is even right this should just go in models but not impl

## materials
### Material
### MaterialManager
### Then make some kind of specific materials that will be reused

## pipeline
### BasePipelineStage 
### DeferredGeometry : BasePipelineStage - not impl
### DeferredLighting : BasePipelineStage - not impl
### FinalComposition : BasePipelineStage - not impl
### ForwardRendering : BasePipelineStage - not impl
### PostProcessingBase : BasePipelineStage - not impl
### AntiAliasingPostProcess : PostProcessingBase - not impl
### SimplePipeline : BasePipeline (will combine all these stages) - not impl

## properties - not impl

## sceneModel - Handling of all models
### SceneGraph - impl mostly
### SceneNode - impl mostly
### Model - not impl
### ModelManager - not impl
### Camera - not impl, maybe shouldnt be here either 

## Need a folder for lights
### Lights can be scene lights or object lights.
#### Objects lights will be for objects that create light mostly for GI? 

## Need a folder for cameras? 

## shaderManager
### Shader - not impl
### ShaderManager - not impl? not sure if needed depends on how the shader caching is structured
### ShaderProgram - not impl
### ShaderProgramManager - not impl

# Thoughts

General Ideas
Model has vert, tri, norm, tex coords, VAO setup 
model has to own ref to its materials 
model has to manage a texCoord -> material mapping (this might have to be duplicated, the same texCoords at different stages might have different materials) 
model has to own ref to textures which also ties into texCoord to texture which can have the same issue
... need to plan this out more in detail before working on the materials side of everything




