#include <stdlib.h>

#include "texture_manager.h"

#define MAX_TEXTURES 128
static Texture2D _textures[MAX_TEXTURES];

static uint64_t hash (string string) {
    uint64_t hash = 5381;
    char character;

    while ((character = *string++)) {
        hash = ((hash << 5) + hash) + character; /* hash * 33 + c */
    }

    return hash;
}

Texture2D GetTexture (string filename) {
    uint8_t index = (hash (filename) % MAX_TEXTURES);

    if (!IsTextureReady (_textures[index])) _textures[index] = LoadTexture (filename);

    return _textures[index];
}

// NOTE: Does no texture checking, should only be used if texture will 110% exist
Texture2D GetTextureFromID (int64_t index) {
    return _textures[index];
}

void UnloadTextureManager (void) {
    for (size_t i = 0; i < MAX_TEXTURES; i++) {
        if (IsTextureReady (_textures[i])) {
            UnloadTexture (_textures[i]);

            TraceLog (LOG_DEBUG, "\033[32mTexture[%d] free'd!\033[0m", i);
        }
    }
}
