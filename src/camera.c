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

#include "camera.h"
#include "tools.h"
#include "surface_manip.h"
#include "screen.h"
#include "font.h"

/* ************************************************************************** *
 * Returns a new full-screen camera initialised with screen data              *
 * (resolution/surface)                                                       */
camera_t* camera_init(screen_res_t res,SDL_Surface* s){
   return camera_custom_init( 0, 0 ,screen_get_width(res)
                            , screen_get_height(res)
                            , screen_get_bpp(res),s);
}


/* ************************************************************************** *
 * Returns a new camera initialised with the given parameters                 */
camera_t* camera_custom_init(int x,int y,int w,int h,int bpp,SDL_Surface* s){
  camera_t* c;
  /* Alloc and configure the camera struct */
  c = secure_malloc(sizeof(camera_t));
  c->cam_surface = 0L;
  c->screen_x = x;
  c->screen_y = y;
  c->h = h;
  c->w = w;
  c->bpp = bpp;
  c->alpha = 255;
  c->map_x = 0;
  c->map_y = 0;
  c->shake_mode = 0;
  c->focused_pos.x = w/2;
  c->focused_pos.y = h/2;
  c->cam_surface = SDL_CreateRGBSurface(0,c->w,c->h,32,0,0,0,0);
  ASSERT(c->cam_surface);
  camera_switch_on(c);
  SDL_FillRect( c->cam_surface,NULL
              , SDL_MapRGB( c->cam_surface->format,DEFAULT_CAM_COLOR
                          , DEFAULT_CAM_COLOR,DEFAULT_CAM_COLOR));    
  return c;
}


/* ************************************************************************** *
 * Release camera                                                             */
void camera_release(camera_t* cam){
  ASSERT(cam)
  ASSERT(cam->cam_surface)
  /* release surface */
  SDL_FreeSurface(cam->cam_surface);
}


/* ************************************************************************** *
 * Switch on the camera                                                       */
void camera_switch_on(camera_t* c){
  ASSERT(c);
  c->status = CAMERA_ON;  
}


/* ************************************************************************** *
 * Switch off the camera                                                      */
void camera_switch_off(camera_t* c){
  ASSERT(c);
  c->status = CAMERA_OFF;
}


/* ************************************************************************** *
 * Set number of whanted shaked frames                                        */
void camera_set_shake_frames(camera_t* c,int shake_frames){
  ASSERT(c);
  c->shake_mode = (shake_frames > c->shake_mode) ? shake_frames : c->shake_mode; 
}


/* ************************************************************************** *
 * Clean the camera surface                                                   */
void camera_clean(camera_t* c){
  SDL_FillRect( c->cam_surface,NULL
              , SDL_MapRGB(c->cam_surface->format,DEFAULT_CAM_COLOR
                          ,DEFAULT_CAM_COLOR,DEFAULT_CAM_COLOR));
}


/* ************************************************************************** *
 * Check if camera is on                                                      */
int  camera_is_on(camera_t* c){
  ASSERT(c);
  return (c->status == CAMERA_ON);
}


/* ************************************************************************** *
 * Set camera alpha                                                           */
void camera_set_alpha(camera_t* c,int alpha_value){
  ASSERT(c);
  c->alpha = alpha_value; 
  SDL_SetSurfaceAlphaMod(c->cam_surface, c->alpha);
  SDL_SetSurfaceBlendMode(c->cam_surface, SDL_BLENDMODE_BLEND);
}


/* ************************************************************************** *
 * Move camera to given coordinates                                           */
void camera_move(camera_t* c,int x,int y){
  ASSERT(c);
  c->map_x = x;
  c->map_y = y;
}


/* ************************************************************************** *
 * Center camera on given coordinates (checking screen bounds)                */
void camera_focus_on(camera_t* c,int x,int y,int max_w,int max_h){
  /* Center cam on x,y */
  c->focused_pos.x=x;
  c->focused_pos.y=y;
  c->map_x = x - c->w / 2;
  c->map_x = (c->map_x < 0) ? 0 : c->map_x ;
  c->map_x = (c->map_x + c->w > max_w) ? max_w - c->w : c->map_x ;
  c->map_y = y - c->h / 2;
  c->map_y = (c->map_y < 0)? 0 : c->map_y ;
  c->map_y = (c->map_y + c->h > max_h ) ? max_h - c->h: c->map_y ;
}


/* ************************************************************************** *
 * Blit camera on screen surface                                              */
void 
camera_direct_blit_on(camera_t* c,SDL_Surface * src_surface,Uint8 shadow){
  ASSERT(c);
  ASSERT(src_surface);
  SDL_Rect srcrect ,destrec;
  /* fill dest infos */
  destrec.x =c->screen_x;
  destrec.y =c->screen_y;
  destrec.h =c->h;
  destrec.w =c->w;
  /* compute src infos */      
  srcrect.x = c->map_x; 
  srcrect.y = c->map_y;  
  srcrect.h = c->h;
  srcrect.w = c->w;
  /* Shake frame ? */
  srcrect.x += (c->shake_mode)? rand()%5-2:0; 
  srcrect.y += (c->shake_mode)? rand()%5-2:0;  
  if(c->shake_mode > 0) c->shake_mode--;  
  
  if(!shadow){
    /* Blit Camera */
    SDL_BlitSurface(src_surface, &srcrect ,c->cam_surface, &destrec );
  }else{
    /* Blit shadow */
    srcrect.x +=2; srcrect.y +=2;
    SDL_SetSurfaceAlphaMod(src_surface, 70);
    SDL_SetSurfaceBlendMode(src_surface, SDL_BLENDMODE_BLEND);
    SDL_BlitSurface(src_surface, &srcrect ,c->cam_surface, &destrec );
    SDL_SetSurfaceAlphaMod(src_surface, 255);
  }
}


/* ************************************************************************** *
 * Blit camera on screen surface                                              */
void camera_blit(camera_t* c,SDL_Surface * src_surface){
  ASSERT(c);
  ASSERT(src_surface);
  SDL_Rect origin = {0,0,0,0};
  SDL_Rect offset;
  offset.x = (c->shake_mode)? c->map_x+rand()%5-2:c->map_x; 
  offset.y = (c->shake_mode)? c->map_y+rand()%5-2:c->map_y;  
  offset.h = c->h;
  offset.w = c->w;
  if(c->shake_mode > 0) c->shake_mode--;  
  //SDL_BlitSurface(src_surface, &offset ,c->cam_surface, &origin );    
  camera_blit_surface_on(c,src_surface,&offset,&origin);
}


/* ************************************************************************** *
 * Blit camera on screen surface                                              */
void camera_blit_shadow(camera_t* c,SDL_Surface * src_surface){
  ASSERT(c);
  ASSERT(src_surface);
  SDL_Rect origin = {0,0,0,0};
  SDL_Rect offset;
  offset.x = (c->shake_mode)? c->map_x+2+rand()%5-2:c->map_x+2; 
  offset.y = (c->shake_mode)? c->map_y+2+rand()%5-2:c->map_y+2;  
  offset.h = c->h;
  offset.w = c->w;  
  if(c->shake_mode > 0) c->shake_mode--;  
  SDL_SetSurfaceAlphaMod(src_surface, 80);
  SDL_SetSurfaceBlendMode(src_surface, SDL_BLENDMODE_BLEND);
  //SDL_BlitSurface(src_surface, &offset ,c->cam_surface, &origin );
  camera_blit_surface_on(c,src_surface,&offset,&origin);  
  SDL_SetSurfaceAlphaMod(src_surface, 255);
}


/* ************************************************************************** *
 * Blit surface on camera                                                     */
void camera_blit_surface_on( camera_t *c,SDL_Surface *src_surface
                           , SDL_Rect *skinpos,SDL_Rect *campos){
  ASSERT(c);
  ASSERT(src_surface);
  SDL_Rect destrec;
  if( ( campos->x > c->w ) || ( campos->y > c->h )
      || ( campos->x + skinpos->w < 0 )
      || ( campos->y + skinpos->h < 0 ) ){
    return; // out
  }
  memcpy(&destrec,campos,sizeof(SDL_Rect));
  SDL_BlitSurface(src_surface, skinpos ,c->cam_surface, &destrec );
}


/* ************************************************************************** *
 * Drow horizontal line on camera                                             */
void 
camera_drow_hline(camera_t *c,int x,int y,int w,int h,int cr, int cg, int cb){
  int line_x = x;
  int line_y = y;
  int line_h = h;
  int line_w = w;
  int cam_h = c->h;
  int cam_w = c->w;
  int cam_x = 0;
  int cam_y = 0;

  
  if(line_x < cam_x){
     line_w -= cam_x - line_x;
     line_x = cam_x;
  }
  if(line_y < cam_y){
     line_h -= cam_y - line_y;
     line_y = cam_y;
  }  
  if(line_x + line_w > cam_x + cam_w)
     line_w -= (cam_x + cam_w) - (line_x + line_w);
  if(line_y + line_h > cam_y + cam_h)
     line_h -= (cam_y + cam_h) - (line_y + line_h);
  line_w = (line_w < cam_w) ? line_w : cam_w;
  line_h = (line_h < cam_h) ? line_h : cam_h;
  drow_hline(c->cam_surface,line_x,line_y,line_w,line_h, cr, cg, cb);
}

/* ************************************************************************** *
 * Put pixel on camera                                                        */
void camera_put_pix_color(camera_t *c, int x, int y, int cr, int cg, int cb){
  int pix_x = x;
  int pix_y = y;
  int cam_h = c->h;
  int cam_w = c->w;
  int cam_x = 0;
  int cam_y = 0;

  if( (pix_x < cam_x+cam_w && pix_y < cam_y+cam_h) 
   && (pix_x >= cam_x && pix_y>= cam_y) )
    put_pix_color(c->cam_surface, pix_x, pix_y, cr, cg, cb);
    
}

/* ************************************************************************** *
 * Fill rectangle on camera                                                   */
void camera_fillrect(camera_t *c,SDL_Rect *rect,Uint32 color){
   SDL_FillRect(c->cam_surface,rect,color);  
}


/* ************************************************************************** *
 * Drow line on camera                                                        */
void camera_drow_line( camera_t *c, int x1, int y1, int x2, int y2
                     , int cr, int cg, int cb){
   drow_line(c->cam_surface, x1, y1, x2, y2, cr, cg, cb);  
}
