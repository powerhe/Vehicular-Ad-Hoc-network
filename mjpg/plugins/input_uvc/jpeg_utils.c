/*******************************************************************************
# Linux-UVC streaming input-plugin for MJPG-streamer                           #
#                                                                              #
# This package work with the Logitech UVC based webcams with the mjpeg feature #
#                                                                              #
#   Orginally Copyright (C) 2005 2006 Laurent Pinchart &&  Michel Xhaard       #
#   Modifications Copyright (C) 2006  Gabriel A. Devenyi                       #
#   Modifications Copyright (C) 2007  Tom Stöveken                             #
#   Modifications Copyright (C) 2010  Vladimir S, Fonov                        #
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

#include <stdio.h>
//#include <jpeglib.h>
#include <stdlib.h>

#include "v4l2uvc.h"
#include "../../simplified_jpeg_encoder.h"


/******************************************************************************
Input Value.: video structure from v4l2uvc.c/h, destination buffer and buffersize
              the buffer must be large enough, no error/size checking is done!
Return Value: the buffer will contain the compressed data
******************************************************************************/
int compress_yuyv_to_jpeg(struct vdIn *vd, unsigned char *buffer, int size, int quality,uint32_t format) 
{
	//TODO: figure out how to convert quality to simple_jpeg_encoder quality
  static int written;
	/*char fourcc[5]={0,0,0,0,0};
	memmove(fourcc,&format,4);
	printf("Compressing %s\n",fourcc);*/
	switch(format)
	{
		case V4L2_PIX_FMT_YUYV: 
			written=s_encode_image(vd->framebuffer,buffer,1024,FORMAT_CbCr422,vd->width,vd->height,vd->framesizeIn);
			break;
			
		case V4L2_PIX_FMT_YUV422P: 
			written=s_encode_image(vd->framebuffer,buffer,1024,FORMAT_CbCr422p,vd->width,vd->height,vd->framesizeIn);
			break;
			
		case V4L2_PIX_FMT_RGB565:
			//do inplace conversion
			RGB565_2_YCbCr420(vd->framebuffer,vd->framebuffer,vd->width,vd->height);
			written=s_encode_image(vd->framebuffer,buffer,1024,FORMAT_CbCr422,vd->width,vd->height,vd->framesizeIn);
			break;
			
		case V4L2_PIX_FMT_RGB24:
			//do inplace conversion
			RGB24_2_YCbCr422(vd->framebuffer,vd->framebuffer,vd->width,vd->height);
			written=s_encode_image(vd->framebuffer,buffer,1024,FORMAT_CbCr422,vd->width,vd->height,vd->framesizeIn);
			break;
			
		case V4L2_PIX_FMT_RGB32:
			//do inplace conversion
			RGB32_2_YCbCr420(vd->framebuffer,vd->framebuffer,vd->width,vd->height);
			written=s_encode_image(vd->framebuffer,buffer,1024,FORMAT_CbCr420,vd->width,vd->height,vd->framesizeIn);
			break;
			
		case V4L2_PIX_FMT_YVU420://WARNING: this is not properly implemented yet
		case V4L2_PIX_FMT_Y41P: //don't know how to handle
		case V4L2_PIX_FMT_GREY:
		default:
			written=s_encode_image(vd->framebuffer,buffer,1024,FORMAT_CbCr400,vd->width,vd->height,vd->framesizeIn);
			break;
	};
  return (written);
}

