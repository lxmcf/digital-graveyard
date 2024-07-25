#include <raylib.h>

#include "scenes.h"
#include "../asset/texture.h"

static bool has_tank;
static bool is_placing_tank;
static Rectangle tank_dimensions;

SCENE_STRUCTURE (
    World,
    {
        is_placing_tank = false;
    },
    {
        if (!has_tank) {
            Vector2 mouse = GetMousePosition ();

            if (!is_placing_tank) {
                if (IsMouseButtonPressed (MOUSE_BUTTON_LEFT)) {
                    is_placing_tank = true;
                }
            }
        }
    },
    {
        DrawText("Hello from the world!", 32, 32, 40, WHITE);

        DrawTextureV (GetTexture ("assets/textures/debug.png"), CLITERAL(Vector2){ 32, 256 }, WHITE);
    },
    {

    },
    {

    },
    {
        return SCENE_WORLD;
    }
)
