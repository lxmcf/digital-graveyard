#include <raylib.h>
#include <stdint.h>
#include <stdlib.h>

#include "texture.h"

#define MAX_TEXTURES 64
static Texture2D _texture_list[MAX_TEXTURES];

static uint64_t hash (const char* string) {
    uint64_t hash = 5381;
    char character;

    while ((character = *string++)) {
        hash = ((hash << 5) + hash) + character; /* hash * 33 + c */
    }

    return hash;
}

Texture2D GetTexture (const char* filename) {
    uint8_t index = (hash (filename) % MAX_TEXTURES);

    if (!IsTextureReady (_texture_list[index])) _texture_list[index] = LoadTexture (filename);

    return _texture_list[index];
}

void UnloadTextures (void) {
    for (size_t i = 0; i < MAX_TEXTURES; i++) {
        if (IsTextureReady (_texture_list[i])) {
            UnloadTexture (_texture_list[i]);

            TraceLog (LOG_DEBUG, "\033[32mTexture[%d] free'd!\033[0m", i);
        }
    }
}
