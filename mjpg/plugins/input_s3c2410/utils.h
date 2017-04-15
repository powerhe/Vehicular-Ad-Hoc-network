/*******************************************************************************
# mini2410 SOC  streaming input-plugin for MJPG-streamer                       #
#                                                                              #
# This package work with the CAM130 connected directly to mini2410 board       #
#                                                                              #
#   Orginally Copyright (C) 2005 2006 Laurent Pinchart &&  Michel Xhaard       #
#   Modifications Copyright (C) 2006  Gabriel A. Devenyi                       #
#   Modifications Copyright (C) 2007  Tom Stöveken                             #
#   Modifications Copyright (C) 2009  Vladimir S. Fonov                        #
#                                                                              #
# This program is free software; you can redistribute it and/or modify         #
# it under the terms of the GNU General Public License as published by         #
# the Free Software Foundation; either version 2 of the License, or            #
# (at your option) any later version.                                          #
#                                                                              #
# This program is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of               #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                #
# GNU General Public License for more details.                                 #
#                                                                              #
# You should have received a copy of the GNU General Public License            #
# along with this program; if not, write to the Free Software                  #
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA    #
#                                                                              #
*******************************************************************************/

#ifndef UTILS_H
#define UTILS_H

#define ERR_NO_SOI 1
#define ERR_NOT_8BIT 2
#define ERR_HEIGHT_MISMATCH 3
#define ERR_WIDTH_MISMATCH 4
#define ERR_BAD_WIDTH_OR_HEIGHT 5
#define ERR_TOO_MANY_COMPPS 6
#define ERR_ILLEGAL_HV 7
#define ERR_QUANT_TABLE_SELECTOR 8
#define ERR_NOT_YCBCR_221111 9
#define ERR_UNKNOWN_CID_IN_SCAN 10
#define ERR_NOT_SEQUENTIAL_DCT 11
#define ERR_WRONG_MARKER 12
#define ERR_NO_EOI 13
#define ERR_BAD_TABLES 14
#define ERR_DEPTH_MISMATCH 15

typedef short indata;
typedef struct Myrgb16 {
	unsigned short blue:5;
	unsigned short green:6;
	unsigned short red:5;
} Myrgb16;
typedef struct Myrgb24 {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
} Myrgb24;
typedef struct Myrgb32 {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char alpha;
} Myrgb32;


void
YUV420toRGB (unsigned char *src, unsigned char *dst, int width, int height,
	     int flipUV, int ColSpace);
int
 get_jpegsize (unsigned char *buf, int insize);
 
void exit_fatal(char *messages);

/* 
decode jpeg data in buf to a rgb24 in pic the caller need to give address of pic buffer
If change are detected in size jpeg_decode realloc the needed buffer and 
 return the real width and height encoded in the jpeg data 
 caller need to check the change before update the screen
 to avoid memory leak caller did free the pic buffer after used 
 */
int jpeg_decode(unsigned char **pic, unsigned char *buf, int *width, int *height);
double ms_time (void);
/* eqalize the picture only works on yuv420p */
void equalize (unsigned char *src, int width, int height, int format);
#endif // UTILS_H
