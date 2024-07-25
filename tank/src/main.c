#include <raylib.h>

#include "scenes/scene_manager.h"
#include "asset/texture.h"

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

#define TARGET_FPS 60

void UpdateDrawFrame (void);

int main (int argc, const char* argv[]) {
    InitWindow (1280, 720, "Hello World");

    SceneManagerInit ();

    #ifdef PLATFORM_WEB
    emscripten_set_main_loop (UpdateDrawFrame, 0, 1);
    #else
    SetTargetFPS (TARGET_FPS);

    while (!WindowShouldClose ()) {
        UpdateDrawFrame ();
    }
    #endif

    SceneManagerUnload ();
    UnloadTextures ();

    CloseWindow ();

    return 0;
}

void UpdateDrawFrame (void) {
    SceneManagerUpdate ();
}
