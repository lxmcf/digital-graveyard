#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <raylib.h>

#include "defines.h"

#ifdef __cplusplus
extern "C" {
#endif

Texture2D GetTexture (string filename);
Texture2D GetTextureFromID (int64_t index);

void UnloadTextureManager (void);

#ifdef __cplusplus
}
#endif


#endif // TEXTURE_MANAGER_H
