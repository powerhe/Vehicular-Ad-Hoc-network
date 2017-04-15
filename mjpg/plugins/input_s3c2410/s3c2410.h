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

#ifndef S3C2410_H
#define S3C2410_H 

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <stdarg.h>
#include <linux/types.h>
#include <linux/videodev2.h>
//#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

/* in case default setting */
#define WIDTH 1280
#define HEIGHT 1024
#define BPPIN 8
#define OUTFRMNUMB 1
#define NB_BUFFER 4


struct frame_t{
        char header[5];
        int nbframe;
        double seqtimes;
        int deltatimes;
        int w;
        int h;
        int size;
        int format;
        unsigned short bright;
        unsigned short contrast;
        unsigned short colors;
        unsigned short exposure;
        unsigned char wakeup;
        int acknowledge;
        } __attribute__ ((packed)); 
	
struct vdIn {
	int fd;
	char *videodevice ;
	unsigned char *pFramebuffer;
	unsigned char *ptframe[OUTFRMNUMB];
  unsigned char *mem[NB_BUFFER];
	
	int framelock[OUTFRMNUMB];
	pthread_mutex_t grabmutex;
	int          framesizeIn ;
	volatile int frame_cour;
	int  bppIn;
	int  hdrwidth;
	int  hdrheight;
	int  formatIn;
	int  signalquit;	
	struct v4l2_capability cap;
	struct v4l2_format fmt;
	struct v4l2_buffer buf;
	struct v4l2_requestbuffers rb;
	
  int  grayscale;
	uint32_t quality;
	
	};
	
int init_s3c2410  (struct vdIn *vd,char *device,int width,int height);
int s3c2410_Grab  (struct vdIn *vd );
int close_s3c2410 (struct vdIn *vd);

#endif /* S3C2410_H */