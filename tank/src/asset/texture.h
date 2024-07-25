#ifndef TEXTURE_H
#define TEXTURE_H

#include <raylib.h>

#ifdef __cplusplus
extern "C" {
#endif

Texture2D GetTexture (const char* filename);

void UnloadTextures (void);

#ifdef __cplusplus
}
#endif


#endif // TEXTURE_H
