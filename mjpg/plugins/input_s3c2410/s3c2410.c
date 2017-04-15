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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <getopt.h>
#include <pthread.h>
#include <sched.h>
#include <sys/mman.h>

#include "../../utils.h"
#include "../../mjpg_streamer.h"


#include "s3c2410.h"
#include "utils.h"
#include "../../simplified_jpeg_encoder.h"

/****************************************************************************
*			Public
****************************************************************************/
int init_s3c2410 (struct vdIn *vd, char *device, 
                  int width, int height)
{
  int err = -1;
  int f;
  int i;
	int ret;

  if (vd == NULL || device == NULL)
    return -1;

  if (width == 0 || height == 0)
    return -1;
	vd->hdrwidth=width;
	vd->hdrheight=height;
	
  vd->videodevice=strdup(device);
  DBG("Opening device\n");
  if ((vd->fd = open( vd->videodevice, O_RDWR)) == -1)
    exit_fatal ("ERROR opening V4L interface");

	
  memset(&vd->cap, 0, sizeof(struct v4l2_capability));
  err = ioctl(vd->fd, VIDIOC_QUERYCAP, &vd->cap);
  if (err < 0) {
    fprintf(stderr, "Error opening device %s: unable to query device.\n", vd->videodevice);
    return err;
  }
	
  if ((vd->cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) {
    fprintf(stderr, "Error opening device %s: video capture not supported.\n",
           vd->videodevice);
    return -1;
  }
	
	if (!(vd->cap.capabilities & V4L2_CAP_READWRITE)) {
		fprintf(stderr, "%s does not support read i/o\n", vd->videodevice);
    return -1;
	}
  /*
   * set format in
   */
  memset(&vd->fmt, 0, sizeof(struct v4l2_format));
	
  vd->fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  vd->fmt.fmt.pix.width = vd->hdrwidth;
  vd->fmt.fmt.pix.height = vd->hdrheight;
  vd->fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV422P;
  vd->fmt.fmt.pix.field = V4L2_FIELD_ANY;
	
  err = ioctl(vd->fd, VIDIOC_S_FMT, &vd->fmt);
	
  if (err < 0) {
    perror("Unable to set format");
    return err;
  }

  if ((vd->fmt.fmt.pix.width != vd->hdrwidth) ||
      (vd->fmt.fmt.pix.height != vd->hdrheight)) {
    fprintf(stderr, " format asked unavailable get width %d height %d \n", vd->fmt.fmt.pix.width, vd->fmt.fmt.pix.height);
	
		vd->hdrwidth = vd->fmt.fmt.pix.width;
		vd->hdrheight = vd->fmt.fmt.pix.height;
	}
	

  vd->framesizeIn=vd->hdrwidth*vd->hdrheight*2;  

  vd->pFramebuffer=(unsigned char *) malloc ((size_t) vd->framesizeIn ); //just in case
  

  memset(&vd->rb, 0, sizeof(struct v4l2_requestbuffers));
  vd->rb.count = NB_BUFFER;
  vd->rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  vd->rb.memory = V4L2_MEMORY_MMAP;

  ret = ioctl(vd->fd, VIDIOC_REQBUFS, &vd->rb);
  if (ret < 0) {
    perror("Unable to allocate buffers");
    return ret;
  }

  /*
   * map the buffers
   */
  for (i = 0; i < NB_BUFFER; i++) {
    memset(&vd->buf, 0, sizeof(struct v4l2_buffer));
    vd->buf.index = i;
    vd->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    vd->buf.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(vd->fd, VIDIOC_QUERYBUF, &vd->buf);
    if (ret < 0) {
      perror("Unable to query buffer");
			return 0;
    }

/*    if (debug)
      fprintf(stderr, "length: %u offset: %u\n", vd->buf.length, vd->buf.m.offset);*/

    vd->mem[i] = mmap(0 /* start anywhere */ ,
                      vd->buf.length, PROT_READ, MAP_SHARED, vd->fd,
                      vd->buf.m.offset);
    if (vd->mem[i] == MAP_FAILED) {
      perror("Unable to map buffer");
			return ret;
    }
/*    if (debug)
      fprintf(stderr, "Buffer mapped at address %p.\n", vd->mem[i]);*/
  }
	
  /*
   * Queue the buffers.
   */
  for (i = 0; i < NB_BUFFER; ++i) {
    memset(&vd->buf, 0, sizeof(struct v4l2_buffer));
    vd->buf.index = i;
    vd->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    vd->buf.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(vd->fd, VIDIOC_QBUF, &vd->buf);
    if (ret < 0) {
      perror("Unable to queue buffer");
			return ret;
    }
  }
	

  //DBG("Allocating input buffers\n");
  
  /* allocate the 4 frames output buffer */
	/*
  for (i = 0; i < OUTFRMNUMB; i++)
  {
      vd->ptframe[i] = NULL;
      vd->ptframe[i] = (unsigned char *) malloc ((size_t) vd->framesizeIn+sizeof(struct frame_t) );
      vd->framelock[i] = 0;
  }*/

  vd->frame_cour = 0;
  
  pthread_mutex_init (&vd->grabmutex, NULL);
  //printf("Allocated\n");
	
  int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  ret = ioctl(vd->fd, VIDIOC_STREAMON, &type);
  if (ret < 0) {
    perror("Unable to start capture");
    return ret;
  }
	
  return 0;
}

int close_s3c2410 (struct vdIn *vd)
{
  int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  int ret;

  ret = ioctl(vd->fd, VIDIOC_STREAMOFF, &type);
  if (ret < 0) {
    perror("Unable to stop capture");
  }
	
  int i;
  free(vd->pFramebuffer);
  vd->pFramebuffer = NULL;

  DBG("close video_device\n");  
  close (vd->fd);
  /* dealloc the whole buffers */
  if (vd->videodevice)
  {
      free (vd->videodevice);
      vd->videodevice = NULL;
  }

/*
  for (i = 0; i < OUTFRMNUMB; i++)
  {
   if (vd->ptframe[i])
	 {
	  free (vd->ptframe[i]);
	  vd->ptframe[i] = NULL;
	  vd->framelock[i] = 0;
	  DBG("freeing output buffer %d\n",i);
	 }
  }*/

   pthread_mutex_destroy (&vd->grabmutex);
   return 0;
}

int convertframe(unsigned char *dst,unsigned char *src, 
		 int width,int height, int formatIn, int qualite,int buf_size,
		 int grayscale)
{ 
	 int ret=0;
	 //unsigned char *tmp=malloc(width*height*2);
	 
	 //RGB565_2_YCbCr420(src,src,width,height); //inplace conversion
	 
	 ret=s_encode_image(src,dst,qualite,grayscale?FORMAT_CbCr400:FORMAT_CbCr422p,width,height,buf_size);
	 //free(tmp);
	 return ret;
}

int s3c2410_Grab (struct vdIn *vd )
{
  static int frame = 0;

  int len;
	int ret;
  int size;
  int err = 0;
  int jpegsize = 0;
  int qualite = 1024;
  
  struct frame_t *headerframe;
  double timecourant =0;
  double temps = 0;
  
  timecourant = ms_time();

/* read method */
  size = vd->framesizeIn;
	
  memset(&vd->buf, 0, sizeof(struct v4l2_buffer));
  vd->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  vd->buf.memory = V4L2_MEMORY_MMAP;

  ret = ioctl(vd->fd, VIDIOC_DQBUF, &vd->buf);
  if (ret < 0) {
    perror("Unable to dequeue buffer");
		return 0;
  }
	
  /*
  do
  {
    len = read (vd->fd, vd->pFramebuffer, size);
    
    if(!len ) //not yet ready
      sched_yield();
		
    
  } while(!len)*/
  
  /*if(len<0) 
  {
        printf ("2440 read error\n");
        return -1;
  }*/
  /* Is there someone using the frame */
  while((vd->framelock[vd->frame_cour] != 0)&& vd->signalquit)
      usleep(1000);

  pthread_mutex_lock (&vd->grabmutex);
  /*
    memcpy (vd->ptframe[vd->frame_cour]+ sizeof(struct frame_t), vd->pFramebuffer, vd->framesizeIn);
    jpegsize =jpeg_compress(vd->ptframe[vd->frame_cour]+ sizeof(struct frame_t),len,
    vd->pFramebuffer, vd->hdrwidth, vd->hdrheight, qualite); 
    */
	//memmove(vd->mem[vd->buf.index], (size_t) vd->framesizeIn);
  temps = ms_time();
	
  jpegsize= convertframe(vd->ptframe[vd->frame_cour]+ sizeof(struct frame_t),
    vd->mem[vd->buf.index],//vd->pFramebuffer,
    vd->hdrwidth, vd->hdrheight,
    vd->formatIn,  vd->quality, vd->framesizeIn,vd->grayscale); 
		
  ret = ioctl(vd->fd, VIDIOC_QBUF, &vd->buf);
  if (ret < 0) {
    perror("Unable to requeue buffer");
    return 0;
  }
		
		
    
  headerframe=(struct frame_t*)vd->ptframe[vd->frame_cour];
  
  snprintf(headerframe->header,5,"%s","2410"); 
  
  headerframe->seqtimes = ms_time();
  headerframe->deltatimes=(int)(headerframe->seqtimes-timecourant); 
  headerframe->w = vd->hdrwidth;
  headerframe->h = vd->hdrheight;
  headerframe->size = (( jpegsize < 0)?0:jpegsize);; 
  headerframe->format = vd->formatIn; 
  headerframe->nbframe = frame++; 
    
  DBG("compress frame %d times %f\n",frame, headerframe->seqtimes-temps);
  vd->frame_cour = (vd->frame_cour +1) % OUTFRMNUMB;  
  pthread_mutex_unlock (&vd->grabmutex); 
  /************************************/
     
  return jpegsize;
}
