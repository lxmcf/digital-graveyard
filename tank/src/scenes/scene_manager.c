#include <raylib.h>
#include <raymath.h>

#include "scene_manager.h"

#define REGISTER_SCENE(name,id)                                                                                         \
    _RegisterScene (id,Init##name,Update##name,Draw##name,Draw##name##Gui,Unload##name,Finish##name);                   \

typedef void (*VoidFunction)(void);
typedef GameSceneId (*GameSceneIdFunction)(void);

static GameScene scene_list[SCENE_COUNT];
static GameSceneId current_scene;

static Camera2D default_camera;

static void _RegisterScene (GameSceneId id, VoidFunction init, VoidFunction update, VoidFunction draw, VoidFunction draw_gui, VoidFunction unload, GameSceneIdFunction finish) {
    scene_list[id] = (GameScene) {
        .id = id,

        .init = init,
        .update = update,
        .draw = draw,
        .draw_gui = draw_gui,
        .unload = unload,

        .finish = finish,
    };
}

void SceneManagerInit (void) {
    REGISTER_SCENE (Menu, SCENE_MENU);
    REGISTER_SCENE (World, SCENE_WORLD);

    default_camera = CLITERAL(Camera2D){ 0 };
    default_camera.zoom = 1.0f;
    default_camera.rotation = 0.0f;
    default_camera.target = Vector2Zero ();
    default_camera.offset = Vector2Zero ();
}

void SceneManagerUpdate (void) {
    scene_list[current_scene].update ();

    BeginDrawing ();
        ClearBackground (SKYBLUE);

        BeginMode2D (default_camera);
            scene_list[current_scene].draw ();
        EndMode2D ();

        scene_list[current_scene].draw_gui ();
    EndDrawing ();

    GameSceneId next_scene = scene_list[current_scene].finish ();

    if (next_scene != current_scene) {
        scene_list[current_scene].unload ();

        current_scene = next_scene;

        scene_list[current_scene].init ();
    }
}

void SceneManagerUnload (void) {
    scene_list[current_scene].unload ();
}
