// User application code space
//

#include "Engine/Engine.h"

extern EngineComponents     Components;
extern ColorPreset          Colors;
extern RandomGen            Random;
extern Logger               Log;
extern Timer                PhysicsTime;
extern Timer                Time;

extern EngineSystemManager  Engine;
extern ApplicationLayer     Application;
extern ResourceManager      Resources;
extern Serialization        Serializer;
extern ScriptSystem         Scripting;
extern RenderSystem         Renderer;
extern PhysicsSystem        Physics;
extern InputSystem          Input;
extern MathCore             Math;
extern ActorSystem          AI;



// User globals
Scene* sceneMain;
Scene* sceneOverlay;

GameObject*  cameraController;
Camera*      mainCamera;

Material* skyMaterial;

GameObject* plain;
Mesh*       plainMesh;
SubMesh     meshPlainPart;

GameObject* overlayObject[5];

MeshRenderer* overlayRenderer;
Vector3 overlayPosition;


// Application entry point
//

void Start() {
    
    sceneMain = Engine.Create<Scene>();
    Renderer.AddSceneToRenderQueue(sceneMain);
    
    
    //
    // Create a sky
    
    Color skyHigh(Colors.blue);
    skyHigh += Colors.MakeGrayScale(0.2);
    
    Color skyLow(Colors.blue);
    skyLow  += Colors.MakeGrayScale(0.6);
    
    GameObject* skyObject = Engine.CreateSky("sky", skyLow, skyHigh, 0.0001);
    sceneMain->AddMeshRendererToSceneRoot( skyObject->GetComponent<MeshRenderer>() );
    
    skyMaterial = skyObject->GetComponent<MeshRenderer>()->material;
    skyMaterial->diffuse = Color(0.087, 0.087, 0.087);
    skyMaterial->EnableDepthTest();
    
    
    
    //
    // Create a camera controller
    cameraController = Engine.CreateCameraController(Vector3(-100, 50, -100), Vector3(1, 8, 1));
    mainCamera = cameraController->GetComponent<Camera>();
    sceneMain->camera = mainCamera;
    cameraController->DisableGravity();
    
    // Attach sky object to the camera object
    skyObject->parent = cameraController;
    
    
    cameraController->AddComponent( Engine.CreateComponent<Light>() );
    Light* cameraLight = cameraController->GetComponent<Light>();
    sceneMain->AddLightToSceneRoot(cameraLight);
    cameraLight->intensity   = 100;
    cameraLight->range       = 1000;
    cameraLight->attenuation = 0.8;
    cameraLight->color       = Colors.blue;
    
    
    
    
    GameObject* plain = Engine.Create<GameObject>();
    plain->transform.scale = Vector3(10, 10, 10);
    plain->AddComponent( Engine.CreateComponent<MeshRenderer>( Resources.CreateMeshFromTag("cube"), Resources.CreateMaterialFromTag("grassy") ) );
    plainMesh = plain->GetComponent<MeshRenderer>()->mesh;
    plain->GetComponent<MeshRenderer>()->material->shader = Engine.shaders.color;
    plain->GetComponent<MeshRenderer>()->material->ambient = Colors.MakeGrayScale(0.02);
    plain->GetComponent<MeshRenderer>()->material->diffuse = Colors.MakeGrayScale(0.02);
    plain->GetComponent<MeshRenderer>()->material->DisableCulling();
    
    sceneMain->AddMeshRendererToSceneRoot( plain->GetComponent<MeshRenderer>() );
    
    plainMesh->CopySubMesh(0, meshPlainPart);
    
    Engine.Destroy<Mesh>( plainMesh );
    plainMesh = Engine.Create<Mesh>();
    
    
    
    
    
    float depthThreshold = 0.4;
    
    float xNoise = 0.1;
    float yNoise = 0.1;
    float zNoise = 0.1;
    
    int width  = 30;
    int depth  = 30;
    int height = 30;
    
    
    for (int z=0; z < height; z++) {
        
        for (int y=0; y < depth; y++) {
            
            for (int x=0; x < width; x++) {
                
                float CoordX = x * xNoise;
                float CoordY = y * yNoise;
                float CoordZ = z * zNoise;
                
                float noiseTotal = Random.Perlin(CoordX, CoordY, CoordZ);
                
                if (noiseTotal > depthThreshold) 
                    continue;
                
                // Create new color for the sub mesh
                Color color = Colors.MakeRandomGrayScale();
                color *= Color(0.2, 0.2, 0.2);
                color += Color(0.04, 0.04, 0.04);
                
                if (Random.Range(0, 10) > 8) 
                    color += Colors.yellow;
                
                if (Random.Range(0, 10) > 4) 
                    color += Colors.blue * Color(0.4, 0.4, 0.4);
                
                // Change sub mesh color
                for (int v=0; v < meshPlainPart.vertexCount; v++) {
                    
                    meshPlainPart.vertexBuffer[v].r = color.r;
                    meshPlainPart.vertexBuffer[v].g = color.g;
                    meshPlainPart.vertexBuffer[v].b = color.b;
                    
                }
                
                
                plainMesh->AddSubMesh((float)x, (float)y, (float)z, meshPlainPart, false);
                
            }
        }
    }
    plainMesh->UploadToGPU();
     
    
    
    
    
    
    //
    // Overlay example
    
    sceneOverlay = Engine.Create<Scene>();
    sceneOverlay->camera = Engine.Create<Camera>();
    sceneOverlay->camera->isOrthographic = true;
    sceneOverlay->camera->isFixedAspect  = true;
    
    sceneOverlay->camera->viewport.w = Renderer.displaySize.x;
    sceneOverlay->camera->viewport.h = Renderer.displaySize.y;
    
    sceneOverlay->camera->clipFar  =  100;
    sceneOverlay->camera->clipNear = -100;
    
    
    Renderer.AddSceneToRenderQueue(sceneOverlay);
    
    // Create some text renderers
    overlayObject[0] = Engine.CreateOverlayTextRenderer("Render window", 13, Colors.black, "font");
    overlayObject[1] = Engine.CreateOverlayTextRenderer("", 10, Colors.black, "font");
    overlayObject[2] = Engine.CreateOverlayTextRenderer("", 10, Colors.black, "font");
    overlayObject[3] = Engine.CreateOverlayTextRenderer("", 10, Colors.black, "font");
    overlayObject[4] = Engine.CreateOverlayTextRenderer("", 10, Colors.black, "font");
    
    sceneOverlay->AddMeshRendererToSceneRoot( overlayObject[0]->GetComponent<MeshRenderer>() );
    sceneOverlay->AddMeshRendererToSceneRoot( overlayObject[1]->GetComponent<MeshRenderer>() );
    sceneOverlay->AddMeshRendererToSceneRoot( overlayObject[2]->GetComponent<MeshRenderer>() );
    sceneOverlay->AddMeshRendererToSceneRoot( overlayObject[3]->GetComponent<MeshRenderer>() );
    sceneOverlay->AddMeshRendererToSceneRoot( overlayObject[4]->GetComponent<MeshRenderer>() );
    
    Text* text[5];
    for (int i=0; i < 5; i++) 
        text[i] = overlayObject[i]->GetComponent<Text>();
    
    text[0]->color = Colors.red;
    text[1]->color = Colors.black;
    text[2]->color = Colors.black;
    text[3]->color = Colors.black;
    text[4]->color = Colors.black;
    
    text[0]->canvas.position.x =  5;
    text[0]->canvas.position.y =  13;
    
    text[1]->canvas.position.x =  20;
    text[1]->canvas.position.y =  20;
    
    text[2]->canvas.position.x =  20;
    text[2]->canvas.position.y =  21;
    
    text[3]->canvas.position.x =  20;
    text[3]->canvas.position.y =  22;
    
    text[4]->canvas.position.x =  80;
    text[4]->canvas.position.y =  20;
    
    text[0]->canvas.anchorBottom = false;
    text[0]->canvas.anchorRight  = false;
    text[1]->canvas.anchorBottom = true;
    text[1]->canvas.anchorRight  = true;
    text[2]->canvas.anchorBottom = true;
    text[2]->canvas.anchorRight  = true;
    text[3]->canvas.anchorBottom = true;
    text[3]->canvas.anchorRight  = true;
    text[4]->canvas.anchorBottom = true;
    text[4]->canvas.anchorRight  = true;
    
    
    
    // World physics
    
    Physics.SetWorldGravity(0, 0, 0);
    
    
    return;
}




// Camera movement force
float cameraSpeed   = 10;



//
// Application loop
//

void Run() {
    
    glm::vec3 force(0);
    if (mainCamera != nullptr) {
        
        // Directional movement
        if (Input.CheckKeyCurrent(VK_W)) {force += mainCamera->forward;}
        if (Input.CheckKeyCurrent(VK_S)) {force -= mainCamera->forward;}
        if (Input.CheckKeyCurrent(VK_A)) {force += mainCamera->right;}
        if (Input.CheckKeyCurrent(VK_D)) {force -= mainCamera->right;}
        
        // Elevation
        if (Input.CheckKeyCurrent(VK_SPACE)) {force += mainCamera->up;}
        if (Input.CheckKeyCurrent(VK_SHIFT)) {force -= mainCamera->up;}
        
        if (cameraController != nullptr) 
            force *= cameraSpeed;
        
        if (Input.CheckKeyCurrent(VK_CONTROL)) force *= 2 * Time.delta;
        
        
        
        cameraController->AddForce(force.x, force.y, force.z);
        
    }
    
    
    
    // Update player position on screen
    overlayObject[1]->GetComponent<Text>()->text = "x " + FloatToString( mainCamera->transform.position.x );
    overlayObject[2]->GetComponent<Text>()->text = "y " + FloatToString( mainCamera->transform.position.y );
    overlayObject[3]->GetComponent<Text>()->text = "z " + FloatToString( mainCamera->transform.position.z );
    overlayObject[4]->GetComponent<Text>()->text = "Draw calls " + IntToString( Renderer.GetNumberOfDrawCalls() );
    
    if (Input.CheckKeyCurrent(VK_I)) sceneOverlay->camera->transform.scale += 0.01;
    if (Input.CheckKeyCurrent(VK_K)) sceneOverlay->camera->transform.scale -= 0.01;
    
    
    
    
    // Escape key pause
    
    if (Input.CheckKeyPressed(VK_ESCAPE)) {
        
        Application.Pause();
        
        if (Application.isPaused) {
            mainCamera->DisableMouseLook();
            Input.ClearKeys();
            
            Application.ShowMouseCursor();
            
        } else {
            SetCursorPos(Renderer.displayCenter.x, Renderer.displayCenter.y);
            mainCamera->EnableMouseLook();
            
            Application.HideMouseCursor();
            
            Time.Update();
            PhysicsTime.Update();
        }
        
    }
    
    return;
}


//
// Called once every tick
//


void TickUpdate(void) {
    
    
    
    return;
}





void Shutdown(void) {
    
    
    
    return;
}



