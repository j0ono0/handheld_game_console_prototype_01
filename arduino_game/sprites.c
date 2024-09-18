#include "sprites.h"

#define ARRAYSIZE(frameset) (sizeof(frameset) / sizeof(frameset[0]))
#define NEWSPRITE(speed, frameset) ((Sprite){ARRAYSIZE(frameset), speed, frameset})

// Frames ////////////////////////////////////////////////////////////////////

extern const SpriteFrame sprite_frame_strongman;
extern const SpriteFrame sprite_frame_target;
extern const SpriteFrame sprite_frame_crate;
extern const SpriteFrame sprite_frame_crate_active;
extern const SpriteFrame sprite_frame_powerconverter;
extern const SpriteFrame sprite_frame_powerconverter_active;
extern const SpriteFrame sprite_frame_office_chair;
extern const SpriteFrame sprite_frame_desktop_panel;
extern const SpriteFrame sprite_frame_desktop_terminal;
extern const SpriteFrame sprite_frame_inbuilt_terminal;
extern const SpriteFrame sprite_frame_desktop_clutter;
extern const SpriteFrame sprite_frame_prof_stationary_west;
extern const SpriteFrame sprite_frame_prof_stationary_east;
extern const SpriteFrame sprite_frame_prof_walk_east_1;
extern const SpriteFrame sprite_frame_prof_walk_east_2;
extern const SpriteFrame sprite_frame_prof_walk_west_1;
extern const SpriteFrame sprite_frame_prof_walk_west_2;
extern const SpriteFrame sprite_frame_prof_walk_south_1;
extern const SpriteFrame sprite_frame_prof_walk_south_2;
extern const SpriteFrame sprite_frame_prof_walk_north_1;
extern const SpriteFrame sprite_frame_prof_walk_north_2;

// Framesets /////////////////////////////////////////////////////////////////

const SpriteFrame *sprite_frameset_prof_stationary_west[] = {&sprite_frame_prof_stationary_west};
const SpriteFrame *sprite_frameset_prof_stationary_east[] = {&sprite_frame_prof_stationary_east};
const SpriteFrame *sprite_frameset_target[] = {&sprite_frame_target};
const SpriteFrame *sprite_frameset_crate[] = {&sprite_frame_crate};
const SpriteFrame *sprite_frameset_crate_active[] = {&sprite_frame_crate_active};
const SpriteFrame *sprite_frameset_powerconverter[] = {&sprite_frame_powerconverter};
const SpriteFrame *sprite_frameset_powerconverter_active[] = {&sprite_frame_powerconverter_active};

const SpriteFrame *sprite_frameset_prof_walk_north[] = {&sprite_frame_prof_walk_north_1, &sprite_frame_prof_walk_north_2};
const SpriteFrame *sprite_frameset_prof_walk_east[] = {&sprite_frame_prof_walk_east_1, &sprite_frame_prof_walk_east_2};
const SpriteFrame *sprite_frameset_prof_walk_south[] = {&sprite_frame_prof_walk_south_1, &sprite_frame_prof_walk_south_2};
const SpriteFrame *sprite_frameset_prof_walk_west[] = {&sprite_frame_prof_walk_west_1, &sprite_frame_prof_walk_west_2};

const SpriteFrame *sprite_frameset_strongman[] = {&sprite_frame_strongman};
const SpriteFrame *sprite_frameset_office_chair[] = {&sprite_frame_office_chair};
const SpriteFrame *sprite_frameset_desktop_panel[] = {&sprite_frame_desktop_panel};
const SpriteFrame *sprite_frameset_desktop_terminal[] = {&sprite_frame_desktop_terminal};
const SpriteFrame *sprite_frameset_inbuilt_terminal[] = {&sprite_frame_inbuilt_terminal};
const SpriteFrame *sprite_frameset_desktop_clutter[] = {&sprite_frame_desktop_clutter};

const SpriteFrame *sprite_frameset_crate_flash[] = {&sprite_frame_crate, &sprite_frame_crate_active};


// Sprites /////////////////////////////////////////////////////////////////

const Sprite sprite_prof_walk_north = NEWSPRITE(2, sprite_frameset_prof_walk_north);
const Sprite sprite_prof_walk_east = NEWSPRITE(2, sprite_frameset_prof_walk_east);
const Sprite sprite_prof_walk_south = NEWSPRITE(2, sprite_frameset_prof_walk_south);
const Sprite sprite_prof_walk_west = NEWSPRITE(2, sprite_frameset_prof_walk_west);
const Sprite sprite_prof_stationary_west = NEWSPRITE(1, sprite_frameset_prof_stationary_west);
const Sprite sprite_prof_stationary_east = NEWSPRITE(1, sprite_frameset_prof_stationary_east);

const Sprite sprite_target = NEWSPRITE(1, sprite_frameset_target);
const Sprite sprite_crate = NEWSPRITE(1, sprite_frameset_crate);
const Sprite sprite_crate_active = NEWSPRITE(1, sprite_frameset_crate_active);
const Sprite sprite_powerconverter = NEWSPRITE(1, sprite_frameset_powerconverter);
const Sprite sprite_powerconverter_active = NEWSPRITE(1, sprite_frameset_powerconverter_active);

const Sprite sprite_strongman = NEWSPRITE(1, sprite_frameset_strongman);
const Sprite sprite_office_chair = NEWSPRITE(1, sprite_frameset_office_chair);
const Sprite sprite_desktop_panel = NEWSPRITE(1, sprite_frameset_desktop_panel);
const Sprite sprite_desktop_terminal = NEWSPRITE(1, sprite_frameset_desktop_terminal);
const Sprite sprite_inbuilt_terminal = NEWSPRITE(1, sprite_frameset_inbuilt_terminal);
const Sprite sprite_desktop_clutter = NEWSPRITE(1, sprite_frameset_desktop_clutter);

const Sprite sprite_crate_1 = NEWSPRITE(2, sprite_frameset_crate_flash);
const Sprite sprite_crate_2 = NEWSPRITE(4, sprite_frameset_crate_flash);
const Sprite sprite_crate_3 = NEWSPRITE(8, sprite_frameset_crate_flash);
const Sprite sprite_crate_4 = NEWSPRITE(16, sprite_frameset_crate_flash);
const Sprite sprite_crate_5 = NEWSPRITE(32, sprite_frameset_crate_flash);
const Sprite sprite_crate_6 = NEWSPRITE(64, sprite_frameset_crate_flash);
