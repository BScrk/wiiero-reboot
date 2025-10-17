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

#include "tools.h"
#include "screen.h"
#include "image.h"
#include "ressources.h"
#include "font.h"
#include <unistd.h>

static int screen_resolution[MAX_RES][3]={{320,220,32}
                                          ,{480,272,32}
                                          ,{480,320,32}
                                          ,{600,400,32}
                                          ,{640,420,32}
                                          ,{640,480,32}
                                          ,{720,480,32}
                                          ,{800,600,32}
                                          ,{1024,768,32}
                                        };
static int screen_is_init = 0;
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void screen_set_gamma(int gamma){
  // SDL2: Gamma is now per-window, skipping for compatibility
  // Can be implemented with SDL_SetWindowBrightness if needed
  (void)gamma; // Suppress unused warning
}



int screen_get_width(screen_res_t res){
  return screen_resolution[res][0];
}

int screen_get_height(screen_res_t res){
  return screen_resolution[res][1];
}

int screen_get_bpp(screen_res_t res){
  return screen_resolution[res][2];
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
screen_t* screen_init(screen_res_t res){
  return screen_custom_init( screen_get_width(res)
                           , screen_get_height(res)
                           , screen_get_bpp(res));
}/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void screen_release(screen_t* s){
  ASSERT(s);
  if(s->surface)
    SDL_FreeSurface(s->surface);
  if(s->renderer)
    SDL_DestroyRenderer(s->renderer);
  if(s->window)
    SDL_DestroyWindow(s->window);
  secure_free(s);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void screen_reset_mode(screen_t* s){
  ASSERT(s);
  
  // Destroy existing window/renderer if switching modes
  if(s->window){
    if(s->renderer){
      SDL_DestroyRenderer(s->renderer);
      s->renderer = NULL;
    }
    SDL_DestroyWindow(s->window);
    s->window = NULL;
  }
  if(s->surface){
    SDL_FreeSurface(s->surface);
    s->surface = NULL;
  }
  
  // Create window
  Uint32 window_flags = SDL_WINDOW_SHOWN;
  if(s->mode == FULL_SCREEN_MODE){
    window_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_ShowCursor(SDL_DISABLE);
  }else{
    SDL_ShowCursor(SDL_ENABLE);
  }
  
  s->window = SDL_CreateWindow("Wiiero",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                s->w, s->h,
                                window_flags);
  if(!s->window){
    HARD_DBG("SDL_CreateWindow Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  
  // Create renderer with VSync
  s->renderer = SDL_CreateRenderer(s->window, -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(!s->renderer){
    HARD_DBG("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  
  // Set logical size to maintain aspect ratio
  SDL_RenderSetLogicalSize(s->renderer, s->w, s->h);
  
  // Set debug renderer for font console output (SDL2)
  font_set_debug_renderer(s->renderer);
  
  // Create a surface for compatibility with existing code
  s->surface = SDL_CreateRGBSurface(0, s->w, s->h, 32,
                                    0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
  if(!s->surface){
    HARD_DBG("SDL_CreateRGBSurface Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  
  // Force window to be visible immediately (SDL2)
  SDL_ShowWindow(s->window);
  SDL_RaiseWindow(s->window);
  
  // Clear and present to make window visible immediately (SDL2)
  SDL_SetRenderDrawColor(s->renderer, 0, 0, 0, 255);
  SDL_RenderClear(s->renderer);
  SDL_RenderPresent(s->renderer);
  
  // Pump events to ensure window is shown (critical on macOS)
  SDL_PumpEvents();
}/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void screen_flip_mode(screen_t* s){
  ASSERT(s);
  s->mode = (s->mode == WIN_SCREEN_MODE) ? FULL_SCREEN_MODE : WIN_SCREEN_MODE;
  screen_reset_mode(s);
}/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void screen_loading_splash(screen_t* s){
  ASSERT(s);
  SDL_Rect offset;
  Uint8 alpha=0; 
  SDL_Surface* splash = image_load( DEFAULT_SPLASH );
  /* Recuperation des coordonnées */
  offset.x = (s->surface->w - splash->w)/2;
  offset.y = (s->surface->h - splash->h)/2;
  offset.h = splash->h;
  offset.w = splash->w;
  
  
  for(alpha=0; alpha<255;alpha++){
#ifndef PSP_MODE
    /* Wii, PC */
    usleep(500);
#else
    SDL_Delay(5);
#endif
  // Clear entire surface before blitting (clean slate for each frame)
  SDL_FillRect(s->surface, NULL, SDL_MapRGB(s->surface->format, 0, 0, 0));
  
  SDL_SetSurfaceAlphaMod(splash, alpha);
  SDL_SetSurfaceBlendMode(splash, SDL_BLENDMODE_BLEND);
  SDL_BlitSurface( splash, 0 , s->surface, &offset );
  // Create texture and present (SDL2)
  if(s->renderer){
    SDL_Texture* tex = SDL_CreateTextureFromSurface(s->renderer, s->surface);
    // Don't clear - just update to preserve debug text
    SDL_RenderCopy(s->renderer, tex, NULL, NULL);
    SDL_RenderPresent(s->renderer);
    SDL_DestroyTexture(tex);
  }
  }

  SDL_FreeSurface(splash);  
}/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void screen_clean(screen_t* s){
  ASSERT(s);
  ASSERT(s->surface);
  SDL_FillRect( s->surface,NULL
              , SDL_MapRGB(s->surface->format,DEFAULT_CAM_COLOR
                          ,DEFAULT_CAM_COLOR,DEFAULT_CAM_COLOR));
}/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
screen_t* screen_custom_init(int w,int h,int bpp){
  ASSERT(!screen_is_init);
  screen_t* s = secure_malloc(sizeof(screen_t));
  s->window = NULL;
  s->renderer = NULL;
  s->surface = NULL;
  screen_is_init = 1;
  s->h = h;
  s->w = w;
  s->bpp = bpp;
  s->screen_cam_list = 0L;
  screen_set_gamma(1);
#ifdef DESKTOP_MODE
  s->mode = WIN_SCREEN_MODE;
#else
  s->mode = FULL_SCREEN_MODE;
#endif
  screen_reset_mode(s);
  return s;
}/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

camera_t* screen_add_camera(screen_t* s,int cx,int cy){
  return screen_add_custom_camera(s,cx,cy,s->w,s->h, s->bpp);
}/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

camera_t* screen_add_custom_camera(screen_t* s,int cx,int cy,int cw,int ch,int cbpp){
  camera_list_t * new_cam_cell;
  DBG("- adding Cams!\n");
  ASSERT(s);
  ASSERT(screen_is_init);
  new_cam_cell = secure_malloc(sizeof(camera_list_t));
  new_cam_cell->next = 0L;
  new_cam_cell->cam = camera_custom_init( cx, cy, cw, ch, cbpp,s->surface);

  if(!s->screen_cam_list){
    s->screen_cam_list = new_cam_cell;
  }else{
    new_cam_cell->next = s->screen_cam_list;
    s->screen_cam_list = new_cam_cell;
  }

  return s->screen_cam_list->cam;
  
}/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void screen_display(screen_t* s){
  ASSERT(s);
  ASSERT(screen_is_init);
  ASSERT(s->renderer);
  ASSERT(s->surface);
  
  // First, blit all cameras onto the screen surface
  if(s->screen_cam_list){
    camera_list_t * cur_cam_slot = s->screen_cam_list;
    
    while(cur_cam_slot){
      if( camera_is_on(cur_cam_slot->cam) ){
        SDL_Rect offset;
        /* Get camera coordinates */
        offset.x = cur_cam_slot->cam->screen_x;
        offset.y = cur_cam_slot->cam->screen_y;
        offset.h = cur_cam_slot->cam->h;
        offset.w = cur_cam_slot->cam->w;    
        /* Blit camera surface onto screen surface */
        SDL_BlitSurface(cur_cam_slot->cam->cam_surface, NULL, s->surface, &offset);
      }
      cur_cam_slot = cur_cam_slot->next;
    }
  }
  
  // Create texture from the complete screen surface
  SDL_Texture* texture = SDL_CreateTextureFromSurface(s->renderer, s->surface);
  if(!texture){
    HARD_DBG("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    return;
  }
  
  // Clear renderer
  SDL_RenderClear(s->renderer);
  
  // Copy texture to renderer
  SDL_RenderCopy(s->renderer, texture, NULL, NULL);
  
  // Present
  SDL_RenderPresent(s->renderer);
  
  // Cleanup
  SDL_DestroyTexture(texture);
}/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
