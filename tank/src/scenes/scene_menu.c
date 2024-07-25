#include <raylib.h>

#include "scenes.h"

static GameSceneId next_scene;

SCENE_STRUCTURE (
    Menu,
    {
        next_scene = SCENE_MENU;
    },
    {
        if (IsKeyPressed (KEY_SPACE)) {
            next_scene = SCENE_WORLD;
        }
    },
    {
        DrawText ("Hello from the menu!", 32, 32, 40, WHITE);
    },
    {

    },
    {

    },
    {
        return next_scene;
    }
)
