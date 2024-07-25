#include <raylib.h>
#include <raymath.h>

#include "texture_manager.h"
#include "defines.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "Settle"
#define WINDOW_CENTRE CLITERAL(Vector2){ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 }

#define GRID_WIDTH 100
#define GRID_HEIGHT 100
#define GRID_CELL_SIZE 64
#define GRID_START 0
#define GRID_SHIFT 6

#define MINIMUM_FPS 30

static int8_t grid[GRID_WIDTH][GRID_HEIGHT];
static uint8_t grid_values[GRID_WIDTH][GRID_HEIGHT];
static int8_t grid_tiles[GRID_WIDTH][GRID_HEIGHT];

#define TILES_COUNT 16
static Rectangle rectangle_paths[TILES_COUNT];
static Camera2D camera;

void SetFPSLimit ();

int main (int argc, string argv[]) {
    InitWindow (WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    #ifdef BUILD_DEBUG
    SetTraceLogLevel (LOG_DEBUG);
    #else
    SetTraceLogLevel (LOG_ERROR);
    #endif

    #ifndef NO_FPS_CAP
    SetFPSLimit ();
    #endif

    for (size_t i = 0; i < TILES_COUNT; i++) {
        rectangle_paths[i] = CLITERAL(Rectangle) {
            .x = GRID_CELL_SIZE * i,
            .y = 0,
            .width = GRID_CELL_SIZE,
            .height = GRID_CELL_SIZE
        };
    }

    camera = CLITERAL(Camera2D){ 0 };
    camera.target = Vector2Zero ();
    camera.offset = Vector2Zero ();
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    for (size_t x = 0; x < GRID_WIDTH; x++) {
        for (size_t y = 0; y < GRID_HEIGHT; y++) {
            grid[x][y] = 0;
            grid_values[x][y] = -1;

            grid_tiles[x][y] = GetRandomValue (0, 3);
        }
    }

    Rectangle bounds = CLITERAL(Rectangle) {
        .x = 0, .y = 0,
        .width = GRID_WIDTH << GRID_SHIFT,
        .height = GRID_HEIGHT << GRID_SHIFT
    };

    int mouse_x = 0;
    int mouse_y = 0;

    while (!WindowShouldClose ()) {
        Vector2 mouse_position_window = GetMousePosition ();
        Vector2 mouse_position = GetScreenToWorld2D (mouse_position_window, camera);

        if (CheckCollisionPointRec (mouse_position, bounds)) {
            mouse_x = (int)mouse_position.x >> GRID_SHIFT;
            mouse_y = (int)mouse_position.y >> GRID_SHIFT;

            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                grid[mouse_x][mouse_y] = 1 - IsKeyDown (KEY_LEFT_CONTROL);
            }

            if (IsKeyPressed (KEY_ONE)) {
                grid[mouse_x][mouse_y] = 2;
            }

            if (IsKeyPressed (KEY_TWO)) {
                grid[mouse_x][mouse_y] = 3;
            }
        }

        if (IsMouseButtonDown (MOUSE_BUTTON_MIDDLE) || IsKeyDown (KEY_LEFT_ALT)) {
            Vector2 delta = GetMouseDelta ();
            delta = Vector2Scale (delta, -1.0f / camera.zoom);

            camera.target = Vector2Add (camera.target, delta);
        }

        float wheel = (IsKeyPressed (KEY_EQUAL) - IsKeyPressed (KEY_MINUS)) * IsKeyDown (KEY_LEFT_CONTROL) + (int)GetMouseWheelMove ();
        if (wheel != 0.0f) {
            #define ZOOM_INCREMENT 0.1f

            camera.zoom += (wheel * ZOOM_INCREMENT);
            camera.zoom = Clamp (camera.zoom, 0.5f, 2.0f);

            if (camera.zoom >= 0.5f && camera.zoom <= 2.0f) {
                Vector2 offset = CLITERAL(Vector2){
                    mouse_position_window.x / camera.zoom,
                    mouse_position_window.y / camera.zoom
                };

                camera.target = Vector2Subtract (mouse_position, offset);
            }
        }

        #define PADDING 256
        camera.target = Vector2Clamp (camera.target, CLITERAL(Vector2){ -PADDING, -PADDING }, CLITERAL(Vector2){ bounds.width - ((WINDOW_WIDTH / camera.zoom) - PADDING), bounds.height - ((WINDOW_HEIGHT / camera.zoom) - PADDING) });
        #undef PADDING

        int start_x = MAX ((int)camera.target.x >> GRID_SHIFT, 0);
        int start_y = MAX ((int)camera.target.y >> GRID_SHIFT, 0);

        int end_x = MAX (((int)(camera.target.x + (WINDOW_WIDTH / camera.zoom))) >> GRID_SHIFT, 0);
        int end_y = MAX (((int)(camera.target.y + (WINDOW_HEIGHT / camera.zoom))) >> GRID_SHIFT, 0);

        // TODO: Make this only update on cell change
        for (int16_t x = start_x; x <= end_x; x++) {
            for (int16_t y = start_y; y <= end_y; y++) {
                if (!grid[x][y]) {
                    grid_values[x][y] = -1;

                    continue;
                }

                uint8_t tile_value = 0;

                if ((x + 1) < GRID_WIDTH)   if (grid[x + 1][y] == 1) tile_value |= 1;
                if ((y - 1) >= GRID_START)  if (grid[x][y - 1] == 1) tile_value |= 2;
                if ((x - 1) >= GRID_START)  if (grid[x - 1][y] == 1) tile_value |= 4;
                if ((y + 1) < GRID_WIDTH)   if (grid[x][y + 1] == 1) tile_value |= 8;

                grid_values[x][y] = tile_value;
            }
        }

        BeginDrawing ();
        BeginMode2D (camera);
            ClearBackground (BLACK);

            for (size_t x = start_x; x <= end_x; x++) {
                bool within_bounds_x = (x >= 0) && (x < GRID_WIDTH);
                if (!within_bounds_x) continue;

                uint16_t adjusted_x = x << GRID_SHIFT;

                for (size_t y = start_y; y <= end_y; y++) {
                    bool within_bounds_y = (y >= 0) && (y < GRID_HEIGHT);
                    if (!within_bounds_y) continue;

                    uint16_t adjusted_y = y << GRID_SHIFT;

                    DrawTexture (GetTexture ("assets/textures/grass_01.png"), adjusted_x, adjusted_y, WHITE);

                    if (grid[x][y] == 1) {
                        DrawTextureRec (GetTexture ("assets/textures/path_64.png"), rectangle_paths[grid_values[x][y]], CLITERAL(Vector2){ adjusted_x, adjusted_y }, WHITE);
                    }

                    if (grid[x][y] == 2) DrawTexture (GetTexture ("assets/textures/structure.png"), adjusted_x, adjusted_y, WHITE);
                    if (grid[x][y] == 3) DrawTexture (GetTexture ("assets/textures/unit.png"), adjusted_x, adjusted_y, WHITE);
                }
            }

            DrawRectangleRec (CLITERAL(Rectangle){ mouse_x << GRID_SHIFT, mouse_y << GRID_SHIFT, GRID_CELL_SIZE, GRID_CELL_SIZE }, Fade (WHITE, SINE (GetTime () * 4) * 0.25f));

            DrawRectangleLinesEx (bounds, 2.0f, WHITE);

            DrawText (TextFormat ("%d - %d", mouse_x, mouse_y), mouse_x << GRID_SHIFT, mouse_y << GRID_SHIFT, 20, WHITE);
        EndMode2D ();

        #ifdef BUILD_DEBUG
        int fps = GetFPS ();

        string text = TextFormat ("%d FPS", fps);
        int width = MeasureText (text, 20);

        DrawRectangle (0, 0, width + 16, 32, Fade (BLACK, 0.25f));
        DrawText (text, 8, 8, 20, fps <= MINIMUM_FPS ? ORANGE : GREEN);
        #endif

        EndDrawing ();
    }

    UnloadTextureManager ();

    CloseWindow ();

    return 0;
}

void SetFPSLimit () {
    int current_monitor = GetCurrentMonitor ();
    int fps = GetMonitorRefreshRate (current_monitor);

    SetTargetFPS (fps);
}
