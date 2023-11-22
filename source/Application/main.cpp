#include "main.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern Timer                PhysicsTime;
extern Timer                Time;
extern Logger               Log;
extern ActorSystem          AI;
extern InputSystem          Input;
extern AudioSystem          Audio;
extern EngineSystemManager  Engine;
extern PhysicsSystem        Physics;
extern RenderSystem         Renderer;
extern ScriptSystem         Scripting;
extern ResourceManager      Resources;
extern ApplicationLayer     Application;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    
    // Create the main render window
    HWND wHndl = Application.CreateWindowHandle("windowFrame", "Render window");
    
    // Console window
    char consoleTitle[500];
    GetConsoleTitleA( consoleTitle, 500 );
    HWND cHnd = FindWindowA( NULL, consoleTitle );
    
    ShowWindow(cHnd, SW_SHOW);
    SetWindowPos(cHnd, NULL, WINDOW_CONSOLE_LEFT, WINDOW_CONSOLE_TOP, WINDOW_CONSOLE_WIDTH, WINDOW_CONSOLE_HEIGHT, SWP_SHOWWINDOW);
    
    Application.SetWindowCenterScale(0.85, 1.1);
    Viewport windowSz = Application.GetWindowArea();
    
    // Clear the event log output file
    Log.Clear();
    
    // Initiate and set the target context for rendering
    if (Renderer.SetRenderTarget(wHndl) != GLEW_OK) {
        DestroyWindow(wHndl);
        MessageBox(NULL, "Cannot locate the OpenGL library. Please update your graphics drivers...", "Error", MB_OK);
        return 0;
    }
    
    // Set the render window view port size
    Renderer.SetViewport(0, 0, windowSz.w, windowSz.h);
    
    // Initiate engine sub systems
    PhysicsTime.SetRefreshRate(PHYSICS_UPDATES_PER_SECOND);
    Time.SetRefreshRate(RENDER_FRAMES_PER_SECOND);
    
    Physics.Initiate();
    Renderer.Initiate();
    Audio.Initiate();
    Resources.Initiate();
    
    // Initiate the engine last
    Engine.Initiate();
    
#ifdef RUN_UNIT_TESTS
    TestFramework testFrameWork;
    testFrameWork.Initiate();
    
    testFrameWork.AddTest( &testFrameWork.TestEngineFunctionality );
    
    testFrameWork.AddTest( &testFrameWork.TestRenderSystem );
    testFrameWork.AddTest( &testFrameWork.TestScriptSystem );
    
    testFrameWork.AddTest( &testFrameWork.TestGameObject );
    testFrameWork.AddTest( &testFrameWork.TestComponentObject );
    
    testFrameWork.AddTest( &testFrameWork.TestPhysicsSystem );
    testFrameWork.AddTest( &testFrameWork.TestTransform );
    
    testFrameWork.AddTest( &testFrameWork.TestSerializerSystem );
    
    // Run test suite
    testFrameWork.RunTestSuite();
    
    // Finish the test
    if (!testFrameWork.hasTestFailed) {
        testFrameWork.Complete();
    } else {
        DestroyWindow(wHndl);
        std::string freeze;
        std::cin >> freeze;
    }
#endif
    
    Start();
    
    // Render timer
    float renderUpdateTimeout = 1000.0f / RENDER_FRAMES_PER_SECOND;
    float renderAccumulator=0;
    Time.Update();
    
    // Physics timer
    float physicsUpdateTimeout = 1000.0f / PHYSICS_UPDATES_PER_SECOND;
    float physicsAccumulator=0;
    float physicsAlpha=0;
    PhysicsTime.Update();
    
    // Tick update timer
    Timer tickTimer;
    float tickUpdateTimeout = 1000.0f / TICK_UPDATES_PER_SECOND;
    float tickAccumulator=0;
    tickTimer.Update();
    
    MSG messages;
    
    while (Application.isActive) {
        
        while (PeekMessage(&messages, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&messages);
            DispatchMessage(&messages);
        }
        
        //
        // Tick update timer
        //
        
        tickAccumulator += tickTimer.GetCurrentDelta();
        tickTimer.Update();
        
        if (tickAccumulator >= tickUpdateTimeout) {
            
            // Update application space
            Run();
            
            Scripting.Update();
            
            Engine.Update();
            
            // Call updates on accumulated time
            for (int i=0; i < 2; i++) {
                
                AI.Update();
                
                TickUpdate();
                
                tickAccumulator -= tickUpdateTimeout;
                if (tickAccumulator > tickUpdateTimeout) 
                    continue;
                break;
            }
        }
        
        //
        // Physics timer
        //
        
        physicsAccumulator += PhysicsTime.GetCurrentDelta();
        PhysicsTime.Update();
        
        while (physicsAccumulator >= physicsUpdateTimeout) {
            
            physicsAccumulator -= physicsUpdateTimeout;
            
            Physics.world->update( physicsUpdateTimeout );
            
            physicsAlpha = physicsAccumulator / physicsUpdateTimeout;
            
        }
        
        //
        // Render timer
        //
        
        renderAccumulator += Time.GetCurrentDelta();
        Time.Update();
        
        if (renderAccumulator >= renderUpdateTimeout) {
            
            renderAccumulator -= renderUpdateTimeout;
            
            Renderer.RenderFrame( renderUpdateTimeout );
        }
        
        continue;
    }
    
    
    // Shutdown engine & sub systems
    Shutdown();
    
    Engine.Shutdown();
    
    Resources.DestroyAssets();
    
    Physics.Shutdown();
    
    Audio.Shutdown();
    
    Renderer.ReleaseRenderTarget();
    
    Application.DestroyWindowHandle();
    
    return messages.wParam;
}


