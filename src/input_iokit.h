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
 * Wiiero is also avaiable on PSP, PC (linux/Windows) and macOS platform.
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

#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <CoreFoundation/CoreFoundation.h>

#define MAX_GAMEPADS 2
#define MAX_MAC_CONTROLS 2
#define AXIS_DEAD_ZONE 64
#define AXIS_CENTER 127

enum
{
  MAC_CONTROL_MODE_KEYBOARD,
  MAC_CONTROL_MODE_GAMEPAD
};

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

static char *cmd_labels[MAX_CMD] = {
    "macOS controls", "", "fire:", "jump:", "Dig:", "show weapon:", "prev weapon:", "next weapon:", "ninja hook:", "up:", "down:", "left:", "right:", "pause:", "menu:", "ok:", "cancel:"};

static char *cmd_key[MAX_MAC_CONTROLS][MAX_CMD] = {
    {"", "Keyboard", "[d] or Y", "[a] or B", "[j+l] or A", "[z] or X", "[z+j] or X+←", "[z+l] or X+→", "[a+z] or L/R", "[i] or ↑", "[k] or ↓", "[j] or ←", "[l] or →", "[space] or Select", "[escape] or Start", "[d] or B/Y", "[a] or A/X"}, {"", "Gamepad", "Y", "B", "A", "X", "X+←", "X+→", "L/R", "stick ↑", "stick ↓", "stick ←", "stick →", "Select", "Start", "B or Y", "A or X"}};

/* IOKit Gamepad structure */
typedef struct
{
  IOHIDDeviceRef device;
  int connected;
  int control_mode;
  /* Current button states */
  int buttons[12];
  int button_down[12]; /* Edge detection for pressed event */
  /* Current axis values (0-255) */
  int axis_x;
  int axis_y;
  int axis_z;
  int axis_rx;
  int axis_ry;
  int axis_rz;
  /* Hat switch */
  int hat_value;
} MacGamepad;

static MacGamepad gamepads[MAX_GAMEPADS];
static IOHIDManagerRef hid_manager = NULL;
static int gamepads_initialized = 0;

/* Keyboard keys (SDL2 Scancodes for SDL_GetKeyboardState) */
enum
{
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
};

// ----------------------------------------------------------------------------
/* IOKit callback for input value changes */
static void inputValueCallback(void *context, IOReturn result, void *sender, IOHIDValueRef value)
{
  IOHIDElementRef element = IOHIDValueGetElement(value);
  IOHIDDeviceRef device = IOHIDElementGetDevice(element);

  uint32_t usagePage = IOHIDElementGetUsagePage(element);
  uint32_t usage = IOHIDElementGetUsage(element);
  CFIndex logicalValue = IOHIDValueGetIntegerValue(value);

  /* Find which gamepad */
  int gamepad_id = -1;
  for (int i = 0; i < MAX_GAMEPADS; i++)
  {
    if (gamepads[i].device == device && gamepads[i].connected)
    {
      gamepad_id = i;
      break;
    }
  }

  if (gamepad_id == -1)
    return;

  MacGamepad *pad = &gamepads[gamepad_id];

  /* Process input based on type */
  switch (usagePage)
  {
  case kHIDPage_GenericDesktop:
    switch (usage)
    {
    case kHIDUsage_GD_X:
      pad->axis_x = (int)logicalValue;
      break;
    case kHIDUsage_GD_Y:
      pad->axis_y = (int)logicalValue;
      break;
    case kHIDUsage_GD_Z:
      pad->axis_z = (int)logicalValue;
      break;
    case kHIDUsage_GD_Rx:
      pad->axis_rx = (int)logicalValue;
      break;
    case kHIDUsage_GD_Ry:
      pad->axis_ry = (int)logicalValue;
      break;
    case kHIDUsage_GD_Rz:
      pad->axis_rz = (int)logicalValue;
      break;
    case kHIDUsage_GD_Hatswitch:
      pad->hat_value = (int)logicalValue;
      break;
    }
    break;

  case kHIDPage_Button:
    if (usage >= 1 && usage <= 12)
    {
      int btn_idx = usage - 1;
      int old_state = pad->buttons[btn_idx];
      pad->buttons[btn_idx] = (int)logicalValue;
      /* Edge detection: button just pressed */
      pad->button_down[btn_idx] = (logicalValue && !old_state);
    }
    break;
  }
}

// ----------------------------------------------------------------------------
/* IOKit callback for device matching */
static void deviceMatchingCallback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device)
{
  for (int i = 0; i < MAX_GAMEPADS; i++)
  {
    if (!gamepads[i].connected)
    {
      gamepads[i].device = device;
      gamepads[i].connected = 1;
      gamepads[i].control_mode = MAC_CONTROL_MODE_GAMEPAD;
      /* Initialize values */
      memset(gamepads[i].buttons, 0, sizeof(gamepads[i].buttons));
      memset(gamepads[i].button_down, 0, sizeof(gamepads[i].button_down));
      gamepads[i].axis_x = AXIS_CENTER;
      gamepads[i].axis_y = AXIS_CENTER;
      gamepads[i].axis_z = AXIS_CENTER;
      gamepads[i].axis_rx = AXIS_CENTER;
      gamepads[i].axis_ry = AXIS_CENTER;
      gamepads[i].axis_rz = AXIS_CENTER;
      gamepads[i].hat_value = -1;

      IOHIDDeviceRegisterInputValueCallback(device, inputValueCallback, NULL);
      break;
    }
  }
}

// ----------------------------------------------------------------------------
/* IOKit callback for device removal */
static void deviceRemovalCallback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device)
{
  for (int i = 0; i < MAX_GAMEPADS; i++)
  {
    if (gamepads[i].device == device)
    {
      gamepads[i].connected = 0;
      gamepads[i].device = NULL;
      gamepads[i].control_mode = MAC_CONTROL_MODE_KEYBOARD;
      break;
    }
  }
}

// ----------------------------------------------------------------------------
/* Initialize IOKit HID manager for gamepads */
static __inline__ void init_controllers()
{
  if (gamepads_initialized)
    return;

  font_console_print_debug("init macOS gamepads ...\n", FONT_SMALL);
  SDL_Delay(500);

  /* Initialize gamepad structures */
  memset(gamepads, 0, sizeof(gamepads));
  for (int i = 0; i < MAX_GAMEPADS; i++)
  {
    gamepads[i].control_mode = MAC_CONTROL_MODE_KEYBOARD;
    gamepads[i].axis_x = AXIS_CENTER;
    gamepads[i].axis_y = AXIS_CENTER;
  }

  /* Create HID manager */
  hid_manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
  if (!hid_manager)
  {
    font_console_print_debug("Warning: Could not create IOHIDManager\n", FONT_SMALL);
    return;
  }

  /* Create matching dictionaries for joysticks and gamepads */
  CFMutableArrayRef matchingArray = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);

  /* Joystick (UsagePage=0x01, Usage=0x04) */
  CFMutableDictionaryRef matchingDict1 = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                                                                   &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
  int usagePage1 = kHIDPage_GenericDesktop;
  int usage1 = kHIDUsage_GD_Joystick;
  CFNumberRef usagePageRef1 = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usagePage1);
  CFNumberRef usageRef1 = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usage1);
  CFDictionarySetValue(matchingDict1, CFSTR(kIOHIDDeviceUsagePageKey), usagePageRef1);
  CFDictionarySetValue(matchingDict1, CFSTR(kIOHIDDeviceUsageKey), usageRef1);
  CFArrayAppendValue(matchingArray, matchingDict1);

  /* Gamepad (UsagePage=0x01, Usage=0x05) */
  CFMutableDictionaryRef matchingDict2 = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                                                                   &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
  int usagePage2 = kHIDPage_GenericDesktop;
  int usage2 = kHIDUsage_GD_GamePad;
  CFNumberRef usagePageRef2 = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usagePage2);
  CFNumberRef usageRef2 = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usage2);
  CFDictionarySetValue(matchingDict2, CFSTR(kIOHIDDeviceUsagePageKey), usagePageRef2);
  CFDictionarySetValue(matchingDict2, CFSTR(kIOHIDDeviceUsageKey), usageRef2);
  CFArrayAppendValue(matchingArray, matchingDict2);

  /* Multi-axis Controller (UsagePage=0x01, Usage=0x08) */
  CFMutableDictionaryRef matchingDict3 = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                                                                   &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
  int usagePage3 = kHIDPage_GenericDesktop;
  int usage3 = kHIDUsage_GD_MultiAxisController;
  CFNumberRef usagePageRef3 = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usagePage3);
  CFNumberRef usageRef3 = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usage3);
  CFDictionarySetValue(matchingDict3, CFSTR(kIOHIDDeviceUsagePageKey), usagePageRef3);
  CFDictionarySetValue(matchingDict3, CFSTR(kIOHIDDeviceUsageKey), usageRef3);
  CFArrayAppendValue(matchingArray, matchingDict3);

  IOHIDManagerSetDeviceMatchingMultiple(hid_manager, matchingArray);

  /* Register callbacks */
  IOHIDManagerRegisterDeviceMatchingCallback(hid_manager, deviceMatchingCallback, NULL);
  IOHIDManagerRegisterDeviceRemovalCallback(hid_manager, deviceRemovalCallback, NULL);

  /* Open HID manager */
  IOReturn ret = IOHIDManagerOpen(hid_manager, kIOHIDOptionsTypeNone);
  if (ret == kIOReturnSuccess)
  {
    /* Schedule with run loop */
    IOHIDManagerScheduleWithRunLoop(hid_manager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    gamepads_initialized = 1;
    font_console_print_debug("Gamepads ready (keyboard fallback)\n", FONT_SMALL);
  }
  else
  {
    font_console_print_debug("Warning: IOHIDManager open failed\n", FONT_SMALL);
  }

  /* Cleanup */
  CFRelease(usagePageRef1);
  CFRelease(usageRef1);
  CFRelease(matchingDict1);
  CFRelease(usagePageRef2);
  CFRelease(usageRef2);
  CFRelease(matchingDict2);
  CFRelease(usagePageRef3);
  CFRelease(usageRef3);
  CFRelease(matchingDict3);
  CFRelease(matchingArray);

  SDL_Delay(500);
}

// ----------------------------------------------------------------------------
/* Check if controls are ready */
static __inline__ int are_controls_ready()
{
  return 1; /* Always ready (keyboard fallback) */
}

// ----------------------------------------------------------------------------
/* Handle gamepad events for one player */
static __inline__ void handle_gamepad_event(int gamepad_id, player_t *p)
{
  if (gamepad_id < 0 || gamepad_id >= MAX_GAMEPADS)
    return;
  if (!gamepads[gamepad_id].connected)
    return;

  MacGamepad *pad = &gamepads[gamepad_id];

  /* Analog stick movement (axis_x and axis_y) */
  if (pad->axis_x < AXIS_CENTER - AXIS_DEAD_ZONE)
    p->worms_action |= ACTION_LEFT;
  else if (pad->axis_x > AXIS_CENTER + AXIS_DEAD_ZONE)
    p->worms_action |= ACTION_RIGHT;

  if (pad->axis_y < AXIS_CENTER - AXIS_DEAD_ZONE)
    p->worms_action |= ACTION_UP;
  else if (pad->axis_y > AXIS_CENTER + AXIS_DEAD_ZONE)
    p->worms_action |= ACTION_DOWN;

  /* Buttons (held state) */
  if (pad->buttons[3]) /* Y button - FIRE */
    p->worms_action |= ACTION_FIRE;
  if (pad->buttons[2]) /* B button - JUMP */
    p->worms_action |= ACTION_JUMP;
  if (pad->buttons[0]) /* X button - CHANGE/SHOW WEAPON */
    p->worms_action |= ACTION_CHANGE;

  /* Buttons (pressed state for single actions) */
  if (pad->button_down[1])
  { /* A button - CROP */
    p->worms_action |= ACTION_CROP;
  }
  if (pad->button_down[2])
  { /* B button - OK */
    p->worms_action |= ACTION_OK;
  }
  if (pad->button_down[3])
  { /* Y button - OK */
    p->worms_action |= ACTION_OK;
  }
  if (pad->button_down[0] || pad->button_down[1])
  { /* X or A - CANCEL */
    p->worms_action |= ACTION_CANCEL;
  }

  /* Triggers (L/R) for ninja hook - Button 4 and 5 */
  if (pad->button_down[4] || pad->button_down[5])
  {
    p->worms_action |= ACTION_HOOK;
  }

  /* Start/Select buttons - Button 9 and 8 */
  if (pad->button_down[9]) /* Start (button 10) - MENU */
    p->worms_action |= ACTION_MENU;
  if (pad->button_down[8]) /* Select (button 9) - PAUSE */
    p->worms_action |= ACTION_PAUSE;

  /* Weapon change with X + direction */
  if (pad->buttons[0])
  { /* X held */
    if (pad->axis_x < AXIS_CENTER - AXIS_DEAD_ZONE ){
      p->worms_action &= ~ACTION_LEFT;
      p->worms_action |= ACTION_L_ACT;
    }
    else if (pad->axis_x > AXIS_CENTER + AXIS_DEAD_ZONE){
      p->worms_action &= ~ACTION_RIGHT;
      p->worms_action |= ACTION_R_ACT;
    }
  }

  /* Clear button_down flags for next frame */
  memset(pad->button_down, 0, sizeof(pad->button_down));
}

// ----------------------------------------------------------------------------
/* Main game event handler */
static __inline__ void game_check_event(game_t *g)
{
  ASSERT(g);

  /* Process IOKit events */
  if (gamepads_initialized)
  {
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, true);
  }

  /* Handle SDL keyboard events */
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0)
  {
    switch (event.type)
    {
    case SDL_QUIT:
      g->wiiero_exit = 1;
      break;
    }
  }

  /* Get keyboard state */
  const Uint8 *keystate = SDL_GetKeyboardState(NULL);

  /* Player 1: Try gamepad first, fallback to keyboard */
  handle_gamepad_event(0, g->worms[PLAYER_1]);
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

  /* Player 2: Try gamepad 2, fallback to keyboard */
  handle_gamepad_event(1, g->worms[PLAYER_2]);
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

  /* Global keyboard shortcuts */
  if (keystate[GAME_ACTION_KEY_EXIT])
    g->worms[PLAYER_1]->worms_action |= ACTION_MENU | ACTION_FROM_KEYBOARD;
  if (keystate[GAME_ACTION_KEY_FLIP])
    screen_flip_mode(g->wiiero_screen);
  if (keystate[GAME_ACTION_KEY_PAUSE])
    g->worms[PLAYER_1]->worms_action |= ACTION_PAUSE | ACTION_FROM_KEYBOARD;
}


// ----------------------------------------------------------------------------
/* Player keyboard event update Mac */
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


// ----------------------------------------------------------------------------

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
  
    if (p->worms_action & ACTION_L_ACT) {
      if (!(p->worms_status & STATUS_CHANGING_W))
        player_change(p, 1);
    } else if (p->worms_action & ACTION_R_ACT) {
      if (!(p->worms_status & STATUS_CHANGING_W))
        player_change(p, -1);
    } else {
      p->worms_status &= ~STATUS_CHANGING_W;
    }
  }

  /* FIRE (Y button) */
  if ((p->worms_action & ACTION_FIRE))
    player_fire(p);

  {
    /* Move left/right */
    if ((p->worms_action & ACTION_RIGHT))
      player_move_right(p);
    if ((p->worms_action & ACTION_LEFT))
      player_move_left(p);
  }
  
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
/* Player general event update Mac */
static __inline__ void player_event_update(player_t *p, map_t *m, player_t **other_plsls)
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
