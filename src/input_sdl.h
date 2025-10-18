/*
 *  __      _ .__  .__
 * /  \    / \(__) (__)  ____ ______  ____
 * \   \/\/  /|  | |  |_/ __ \\_  _ \/  _ \
 *  \       //   |/   |\  ___/ |  |\(  <_> )
 *   \__/\_/ \__ \\__ \ \___  >|__|  \____/
 *              \/   \/     \/ By BScrk
 * Copyright (C) 2008-2009 Luca Benevolo <wiiero@free.fr>
 *
 * Original game concept (Liero) : Joosa Riekkinen in 1998
 * Wiiero is Liero game clone developed for Wii platform.
 * Wiiero is also avaiable on PSP and PC (linux/Windows) platform.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef INPUT_H
#define INPUT_H

#include "common.h"
#include <SDL2/SDL.h>
#include "player.h"
#include "game.h"
#include "font.h"

typedef enum
{
  MODE_CMD,
  EXTRA_INFO,
  FIRE_CMD,
  JUMP_CMD,
  CROP_CMD,
  WSHOW_CMD,
  WLEFT_CMD,
  WRIGHT_CMD,
  NINJA_CMD,
  UP_CMD,
  DOWN_CMD,
  LEFT_CMD,
  RIGHT_CMD,
  MENU_CMD,
  PAUSE_CMD,
  OK_CMD,
  CANCEL_CMD,
  MAX_CMD
} lab_id_t;

char *cmd_labels[MAX_CMD] = {
    "PC controls", "", "fire:", "jump:", "Dig:", "show weapon:", "prev weapon:", "next weapon:", "ninja hook:", "up:", "down:", "left:", "right:", "pause:", "menu:", "ok:", "cancel:"};

char *cmd_key[2][MAX_CMD] = {
    {"", "Player 1", "[d]", "[a]", "[j+l]", "[z]", "[z+j]", "[z+l]", "[a+z]", "[i]", "[k]", "[j]", "[l]", "[space]", "[escape]", "[d]", "[a]"}, {"", "Player 2", "[page up]", "[insert]", "[4+6]", "[home]", "[home+4]", "[home+6]", "[insert+home]", "[8]", "[5]", "[4]", "[6]", "[space]", "[escape]", "[page up]", "[insert]"}};

enum
{ /* - -- --- WIIERO EVENTS ---- --- -- - */
  /* SDL2: Using scancodes for SDL_GetKeyboardState */
  P1_ACTION_KEY_UP = SDL_SCANCODE_I,
  P1_ACTION_KEY_DOWN = SDL_SCANCODE_K,
  P1_ACTION_KEY_LEFT = SDL_SCANCODE_J,
  P1_ACTION_KEY_RIGTH = SDL_SCANCODE_L,
  P1_ACTION_KEY_JUMP = SDL_SCANCODE_A, 
  P1_ACTION_KEY_JUMP_BIS = SDL_SCANCODE_Q, 
  P1_ACTION_KEY_CHANGE = SDL_SCANCODE_Z,
  P1_ACTION_KEY_CHANGE_BIS = SDL_SCANCODE_W,
  P1_ACTION_KEY_FIRE = SDL_SCANCODE_D,
  P2_ACTION_KEY_UP = SDL_SCANCODE_KP_8,
  P2_ACTION_KEY_DOWN = SDL_SCANCODE_KP_5,
  P2_ACTION_KEY_LEFT = SDL_SCANCODE_KP_4,
  P2_ACTION_KEY_RIGTH = SDL_SCANCODE_KP_6,
  P2_ACTION_KEY_JUMP = SDL_SCANCODE_INSERT,
  P2_ACTION_KEY_CHANGE = SDL_SCANCODE_HOME,
  P2_ACTION_KEY_FIRE = SDL_SCANCODE_PAGEUP,
  GAME_ACTION_KEY_EXIT = SDL_SCANCODE_ESCAPE,
  GAME_ACTION_KEY_FLIP = SDL_SCANCODE_TAB,
  GAME_ACTION_KEY_PAUSE = SDL_SCANCODE_SPACE,
}; /* - -- --- WIIERO EVENTS ---- --- -- */

/* SDL2 GameController support */
#define MAX_GAMEPADS 2
static SDL_GameController* gamepads[MAX_GAMEPADS] = {NULL, NULL};
static int gamepad_count = 0;

static __inline__ void init_controllers()
{
  font_console_print_debug("Initializing SDL2 GameController support...\n", FONT_SMALL);
  
  // Enable game controller events
  SDL_GameControllerEventState(SDL_ENABLE);
  
  // Scan for connected controllers
  int num_joysticks = SDL_NumJoysticks();
  char msg[128];
  snprintf(msg, sizeof(msg), "Detected %d joystick(s)\n", num_joysticks);
  font_console_print_debug(msg, FONT_SMALL);
  
  gamepad_count = 0;
  for (int i = 0; i < num_joysticks && gamepad_count < MAX_GAMEPADS; i++) {
    if (SDL_IsGameController(i)) {
      gamepads[gamepad_count] = SDL_GameControllerOpen(i);
      if (gamepads[gamepad_count]) {
        const char* name = SDL_GameControllerName(gamepads[gamepad_count]);
        snprintf(msg, sizeof(msg), "Gamepad %d: %s\n", gamepad_count, name ? name : "Unknown");
        font_console_print_debug(msg, FONT_SMALL);
        gamepad_count++;
      }
    }
  }
  
  if (gamepad_count > 0) {
    snprintf(msg, sizeof(msg), "✓ %d gamepad(s) ready\n", gamepad_count);
    font_console_print_debug(msg, FONT_SMALL);
  } else {
    font_console_print_debug("No gamepads detected\n", FONT_SMALL);
  }
  
  SDL_Delay(1000);
}

static __inline__ int are_controls_ready()
{
  return 1;
}

/* PC event control */
static __inline__ void game_check_event(game_t *g)
{
  /* CHECK GAME EVENTS */
  const Uint8 *keystate;
  ASSERT(g);

  /* FLUSH EVENTS */
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0)
  {
    switch (event.type)
    {
    case SDL_QUIT:
      g->wiiero_exit = 1;
      break;
      
    case SDL_CONTROLLERDEVICEADDED:
      // Hot-plug support: add new controller
      if (gamepad_count < MAX_GAMEPADS) {
        int device_index = event.cdevice.which;
        if (SDL_IsGameController(device_index)) {
          // Check if this controller is not already open
          SDL_GameController* new_controller = SDL_GameControllerOpen(device_index);
          if (new_controller) {
            SDL_Joystick* new_joy = SDL_GameControllerGetJoystick(new_controller);
            int new_instance_id = SDL_JoystickInstanceID(new_joy);
            
            // Check if already in our list
            int already_open = 0;
            for (int i = 0; i < gamepad_count; i++) {
              if (gamepads[i]) {
                SDL_Joystick* existing_joy = SDL_GameControllerGetJoystick(gamepads[i]);
                if (SDL_JoystickInstanceID(existing_joy) == new_instance_id) {
                  already_open = 1;
                  break;
                }
              }
            }
            
            if (already_open) {
              // Already have this one, close the duplicate
              SDL_GameControllerClose(new_controller);
            } else {
              // New controller, add it
              gamepads[gamepad_count] = new_controller;
              gamepad_count++;
            }
          }
        }
      }
      break;
      
    case SDL_CONTROLLERDEVICEREMOVED:
      // Hot-plug support: remove disconnected controller
      for (int i = 0; i < gamepad_count; i++) {
        if (gamepads[i] && SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gamepads[i])) == event.cdevice.which) {
          SDL_GameControllerClose(gamepads[i]);
          gamepads[i] = NULL;
          // Shift remaining gamepads
          for (int j = i; j < gamepad_count - 1; j++) {
            gamepads[j] = gamepads[j + 1];
          }
          gamepads[gamepad_count - 1] = NULL;
          gamepad_count--;
          break;
        }
      }
      break;
    }
  }
  /* GET KEYS */
  keystate = SDL_GetKeyboardState(NULL);

  /* Player 1 keyboard */
  if (keystate[P1_ACTION_KEY_UP])
    g->worms[PLAYER_1]->worms_action |= (ACTION_UP | ACTION_FROM_KEYBOARD);
  if (keystate[P1_ACTION_KEY_DOWN])
    g->worms[PLAYER_1]->worms_action |= (ACTION_DOWN | ACTION_FROM_KEYBOARD);
  if (keystate[P1_ACTION_KEY_LEFT])
    g->worms[PLAYER_1]->worms_action |= (ACTION_LEFT | ACTION_FROM_KEYBOARD);
  if (keystate[P1_ACTION_KEY_RIGTH])
    g->worms[PLAYER_1]->worms_action |= (ACTION_RIGHT | ACTION_FROM_KEYBOARD);
  if (keystate[P1_ACTION_KEY_JUMP] || keystate[P1_ACTION_KEY_JUMP_BIS])
    g->worms[PLAYER_1]->worms_action |= (ACTION_JUMP | ACTION_CANCEL | ACTION_FROM_KEYBOARD);
  if (keystate[P1_ACTION_KEY_CHANGE] || keystate[P1_ACTION_KEY_CHANGE_BIS])
    g->worms[PLAYER_1]->worms_action |= (ACTION_CHANGE | ACTION_FROM_KEYBOARD);
  if (keystate[P1_ACTION_KEY_FIRE])
    g->worms[PLAYER_1]->worms_action |= (ACTION_FIRE | ACTION_OK | ACTION_FROM_KEYBOARD);

  /* Player 2 keyboard */
  if (keystate[P2_ACTION_KEY_UP])
    g->worms[PLAYER_2]->worms_action |= (ACTION_UP | ACTION_FROM_KEYBOARD);
  if (keystate[P2_ACTION_KEY_DOWN])
    g->worms[PLAYER_2]->worms_action |= (ACTION_DOWN | ACTION_FROM_KEYBOARD);
  if (keystate[P2_ACTION_KEY_LEFT])
    g->worms[PLAYER_2]->worms_action |= (ACTION_LEFT | ACTION_FROM_KEYBOARD);
  if (keystate[P2_ACTION_KEY_RIGTH])
    g->worms[PLAYER_2]->worms_action |= (ACTION_RIGHT | ACTION_FROM_KEYBOARD);
  if (keystate[P2_ACTION_KEY_JUMP])
    g->worms[PLAYER_2]->worms_action |= (ACTION_JUMP | ACTION_CANCEL | ACTION_FROM_KEYBOARD);
  if (keystate[P2_ACTION_KEY_CHANGE])
    g->worms[PLAYER_2]->worms_action |= (ACTION_CHANGE | ACTION_FROM_KEYBOARD);
  if (keystate[P2_ACTION_KEY_FIRE])
    g->worms[PLAYER_2]->worms_action |= (ACTION_FIRE | ACTION_OK | ACTION_FROM_KEYBOARD);

  // [OTHER]
  if (keystate[GAME_ACTION_KEY_EXIT])
  {
    g->worms[PLAYER_1]->worms_action |= ACTION_MENU;
  }

  if (keystate[GAME_ACTION_KEY_FLIP])
    screen_flip_mode(g->wiiero_screen);

  if (keystate[GAME_ACTION_KEY_PAUSE])
  {
    g->worms[PLAYER_1]->worms_action |= ACTION_PAUSE;
  }
  

  /* === GAMEPAD INPUT === */
  // Iterate over all players and their corresponding gamepads
  for (int player_id = 0; player_id < NB_PLAYERS; player_id++) {
    if (player_id < gamepad_count && gamepads[player_id]) {
      SDL_GameController* pad = gamepads[player_id];
      
      // D-Pad and Left Stick for movement
      if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
        g->worms[player_id]->worms_action |= ACTION_UP;
      }
      if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
        g->worms[player_id]->worms_action |= ACTION_DOWN;
      }
      if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
        g->worms[player_id]->worms_action |= ACTION_LEFT | ACTION_L_ACT;
      }
      if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
        g->worms[player_id]->worms_action |= ACTION_RIGHT | ACTION_R_ACT;
      }

      
      if (SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTY) < -8000) {
        g->worms[player_id]->worms_action |= ACTION_UP;
      }
      if (SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTY) > 8000) {
        g->worms[player_id]->worms_action |= ACTION_DOWN;
      }

      if (SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTX) < -8000) {
        g->worms[player_id]->worms_action |= ACTION_LEFT;
      }
      if (SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTX) > 8000) {
        g->worms[player_id]->worms_action |= ACTION_RIGHT;
      }
      // A (Cross) = Jump/Cancel
      if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_A)) {
        g->worms[player_id]->worms_action |= (ACTION_JUMP | ACTION_CANCEL);
        //printf("A button pressed [Player %d]\n", player_id);
      }
      
      // B (Circle) = Fire/OK
      if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_B)) {
        g->worms[player_id]->worms_action |= (ACTION_FIRE | ACTION_OK);
        //printf("B button pressed [Player %d]\n", player_id);
      }
      
      // X (Square) = Jump/Cancel
      if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_X)) {
        g->worms[player_id]->worms_action |= (ACTION_CROP | ACTION_CANCEL);
        //printf("X button pressed [Player %d]\n", player_id);
      }
      
      // Y (Triangle) = Fire/OK
      if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_Y)) {
        g->worms[player_id]->worms_action |= (ACTION_CHANGE | ACTION_OK);
        //printf("Y button pressed [Player %d]\n", player_id);
      }
      
      
      // L1/R1 = Change weapon
      if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) &&
          SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) {
        g->worms[player_id]->worms_action |= (ACTION_CHANGE | ACTION_L_ACT);
      }else if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) ||
                SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) {
        g->worms[player_id]->worms_action |= ACTION_CHANGE;
      }else 

      // Start = Pause
      if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_START)) {
        g->worms[player_id]->worms_action |= ACTION_PAUSE;
        //printf("Start button pressed [Player %d]\n", player_id);
      }
      
      // Back/Select = Menu
      if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_BACK)) {
        g->worms[player_id]->worms_action |= ACTION_MENU;
        //printf("Back button pressed [Player %d]\n", player_id);
      }

      
      // Triggers L2 for ninja hook
      if( SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 8000){
        g->worms[player_id]->worms_action |= ACTION_HOOK;
        //printf("L2 Trigger pressed [Player %d]\n", player_id);
      }

      // Triggers R2 for Fire
      if( SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 8000){
        g->worms[player_id]->worms_action |= ACTION_FIRE;
        //printf("R2 Trigger pressed [Player %d]\n", player_id);
      }

      // Left Stick Button = Jump
      if(SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_LEFTSTICK)) {
        g->worms[player_id]->worms_action |= ACTION_JUMP;
        //printf("L3 button pressed [Player %d]\n", player_id);
      }

      // Right Stick Button = Jump
      if(SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_RIGHTSTICK)) {
        g->worms[player_id]->worms_action |= ACTION_HOOK;
        //printf("R3 button pressed [Player %d]\n", player_id);
      }


      // translate Right Stick X & Y in angle for aiming
      float right_stick_x = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_RIGHTX); // range -32768 (Left) to 32767 (Right)
      float right_stick_y = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_RIGHTY);// range -32768 (Up) to 32767 (Down)
      if (fast_abs(right_stick_x) > 4000 || fast_abs(right_stick_y) > 4000) {
        float angle = ftrigo_atan2(right_stick_y, right_stick_x);
        player_look_at(g->worms[player_id], angle);
      }
    }
  }
}


/* Player keyboard event update PC */
static __inline__ void player_kb_event_update(player_t *p, map_t *m, player_t **other_pls)
{
  ASSERT(p);
  ASSERT(m);

  /* Up & Down events */
  if (p->worms_action & ACTION_CHANGE)
  {
    if (p->worms_action & ACTION_UP)
      player_change_rope_len(p, -ROPE_LEN_CHANGE_PITCH);
    if (p->worms_action & ACTION_DOWN)
      player_change_rope_len(p, ROPE_LEN_CHANGE_PITCH);
  }
  else
  {
    if ((p->worms_action & ACTION_UP))
    {
      player_look_up(p);
    }
    else
    {
      if ((p->worms_action & ACTION_DOWN))
        player_look_down(p);
      else
        p->reticle_pitch = ANGLE_PITCH;
    }
  }
  
  /* Ninja hook */
  if ((p->worms_action & ACTION_JUMP) && (p->worms_action & ACTION_CHANGE))
  {
    if (!(p->worms_status & STATUS_NINJA_ACTION))
    {
      if (p->ninja_hook->last_bullet == 0l)
      {
        /* Launch */
        // printf("lauch\n");
        player_launch_hook(p);
      }
      else
      {
        /* Launch new */
        // printf("lauch new\n");
        player_remove_hook(p, other_pls);
        player_launch_hook(p);
      }
      p->worms_status |= STATUS_NINJA_ACTION;
    }
  }
  else
  {
    /* jump */
    if ((p->worms_action & ACTION_JUMP))
    {
      player_jump(p);

      /* Remove hook */
      if (!(p->worms_status & STATUS_NINJA_ACTION))
        if (p->ninja_hook->last_bullet != 0l)
        {
          // printf("remove\n");
          player_remove_hook(p, other_pls);
        }
    }
    p->worms_status &= ~STATUS_NINJA_ACTION;
  }
  /* crop , change or move */
  if ((p->worms_action & ACTION_RIGHT) && (p->worms_action & ACTION_LEFT))
  {
    /* if crop, no weapon change */
    p->worms_status &= ~STATUS_CHANGING_W;
    if (!(p->worms_status & STATUS_CROPING))
      player_crop(p, m);
  }
  else
  {
    /* no crop */
    p->worms_status &= ~STATUS_CROPING;
    /* change or move */
    if (!(p->worms_action & ACTION_CHANGE))
    {
      /* if moving, no weapon change */
      p->worms_status &= ~STATUS_CHANGING_W;
      if ((p->worms_action & ACTION_RIGHT))
        player_move_right(p);
      if ((p->worms_action & ACTION_LEFT))
        player_move_left(p);
    }
    else
    {
      /* change */
      if (!(p->worms_action & ACTION_JUMP))
      {
        if ((p->worms_action & ACTION_RIGHT))
        {
          if (!(p->worms_status & STATUS_CHANGING_W))
            player_change(p, 1);
        }
        else
        {
          if ((p->worms_action & ACTION_LEFT))
          {
            if (!(p->worms_status & STATUS_CHANGING_W))
              player_change(p, -1);
          }
          else
          {
            p->worms_status &= ~STATUS_CHANGING_W;
          }
        }
        p->worms_status |= STATUS_SHOW_W;
      }
    }
  }
  /* FIRE */
  if (p->worms_action & ACTION_FIRE)
  {
    player_fire(p);
  }
}

static __inline__ void player_gp_event_update(player_t *p, map_t *m, player_t **other_pls)
{
  /* ============= GAMEPAD CONTROLS (similar to Wii Nunchuk/Classic) ============= */
  ASSERT(p);
  ASSERT(m);

  /* Up & Down events */
  if ((p->worms_action & ACTION_CHANGE) && !(p->ninja_hook->last_bullet == 0l)) 
  { // ROPE
    if (p->worms_action & ACTION_UP)
      player_change_rope_len(p, -ROPE_LEN_CHANGE_PITCH);
    if (p->worms_action & ACTION_DOWN)
      player_change_rope_len(p, ROPE_LEN_CHANGE_PITCH);
  }
  else
  { // No ROPE
    if ((p->worms_action & ACTION_UP))
      player_look_up(p);
    else if ((p->worms_action & ACTION_DOWN))
      player_look_down(p);
    else
      p->reticle_pitch = ANGLE_PITCH;
  }

  /* NINJA HOOK (Triggers L/R) */
  if (p->worms_action & ACTION_HOOK)
  {
    if (!(p->worms_status & STATUS_NINJA_ACTION))
    {
      if (p->ninja_hook->last_bullet == 0l)
      {
        player_launch_hook(p);
      }
      else
      {
        player_remove_hook(p, other_pls);
        player_launch_hook(p);
      }
      p->worms_status |= STATUS_NINJA_ACTION;
    }
  }else{
    /* JUMP (B button) */
    if (p->worms_action & ACTION_JUMP)
    {
      player_jump(p);
      if (!(p->worms_status & STATUS_NINJA_ACTION))
      {
        if (p->ninja_hook->last_bullet != 0l)
        {
          player_remove_hook(p, other_pls);
        }
      }
    }
    p->worms_status &= ~STATUS_NINJA_ACTION;
  }

  /* CROP (A button) */
  if (p->worms_action & ACTION_CROP)
    player_crop(p, m);
  else
    p->worms_status &= ~STATUS_CROPING;



  /* Weapon change with X button + direction */
  if (p->worms_action & ACTION_CHANGE){
    p->worms_status |= STATUS_SHOW_W;
    //printf("Change weapon mode\n");
    if (p->worms_action & ACTION_L_ACT) {
      //printf("Change weapon left [Player %d]\n", p->id);
      if (!(p->worms_status & STATUS_CHANGING_W)){
        player_change(p, 1);
        //printf("Weapon changed to the left [Player %d]\n", p->id);
      }else{
        //printf("Already changing weapon, ignoring input [Player %d]\n", p->id);
      }
    } else if (p->worms_action & ACTION_R_ACT) {
      //printf("Change weapon right [Player %d]\n", p->id);
      if (!(p->worms_status & STATUS_CHANGING_W)){
        player_change(p, -1);
        //printf("Weapon changed to the right [Player %d]\n", p->id);
      }else{
        //printf("Already changing weapon, ignoring input [Player %d]\n", p->id);
      }
    } else {
      //printf("No change, clearing changing status [Player %d]\n", p->id);
      p->worms_status &= ~STATUS_CHANGING_W;
    }
  }else {
    /* Move left/right */
    if ((p->worms_action & ACTION_RIGHT))
      player_move_right(p);
    if ((p->worms_action & ACTION_LEFT))
      player_move_left(p);
  }
  

  /* FIRE (Y button) */
  if ((p->worms_action & ACTION_FIRE))
    player_fire(p);

 
  // printf(" p->worms_action: %d%d%d%d%d%d%d%d%d%d%d%d\n",
  //        (p->worms_action & ACTION_UP) ? 1 : 0,
  //        (p->worms_action & ACTION_DOWN) ? 1 : 0,
  //        (p->worms_action & ACTION_LEFT) ? 1 : 0,
  //        (p->worms_action & ACTION_RIGHT) ? 1 : 0,
  //        (p->worms_action & ACTION_JUMP) ? 1 : 0,
  //        (p->worms_action & ACTION_CHANGE) ? 1 : 0,
  //        (p->worms_action & ACTION_FIRE) ? 1 : 0,
  //        (p->worms_action & ACTION_CROP) ? 1 : 0,
  //        (p->worms_action & ACTION_HOOK) ? 1 : 0,
  //        (p->worms_action & ACTION_L_ACT) ? 1 : 0,
  //        (p->worms_action & ACTION_R_ACT) ? 1 : 0,
  //        (p->worms_action & ACTION_FROM_KEYBOARD) ? 1 : 0
  //        );
}

// ----------------------------------------------------------------------------
/* Player general event update PC */
static __inline__ void player_event_update(player_t *p, map_t *m, player_t **other_pls)
{
  ASSERT(p);
  ASSERT(m);

  if (!(p->worms_status & STATUS_ALIVE))
  {
    if (p->ninja_hook->last_bullet)
      player_remove_hook(p, other_pls);
    return;
  }

  p->worms_status &= ~STATUS_SHOW_W;

  if(p->worms_action & ACTION_FROM_KEYBOARD){
    player_kb_event_update(p,m,other_pls);
  }else{
    player_gp_event_update(p,m,other_pls);
  }

}

#endif
