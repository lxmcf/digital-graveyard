#ifndef SCENES_H
#define SCENES_H

#define DEFINE_SCENE(name)                                                                                              \
    void Init##name (void);                                                                                             \
    void Update##name (void);                                                                                           \
    void Draw##name (void);                                                                                             \
    void Draw##name##Gui (void);                                                                                        \
    void Unload##name (void);                                                                                           \
    GameSceneId Finish##name (void);

#define SCENE_STRUCTURE(name,init,update,draw,draw_gui,unload,finish)                                                   \
    void Init##name (void) init                                                                                         \
    void Update##name (void) update                                                                                     \
    void Draw##name (void) draw                                                                                         \
    void Draw##name##Gui (void) draw_gui                                                                                \
    void Unload##name (void) unload                                                                                     \
    GameSceneId Finish##name (void) finish

#define EMPTY_BLOCK { }

typedef enum GameSceneId {
    SCENE_UNKNOWN = -1,
    SCENE_MENU,
    SCENE_WORLD,

    SCENE_COUNT,
} GameSceneId;

#ifdef __cplusplus
extern "C" {
#endif

DEFINE_SCENE (Menu)
DEFINE_SCENE (World)

#ifdef __cplusplus
}
#endif

#undef DEFINE_SCENE

#endif // SCENES_H
