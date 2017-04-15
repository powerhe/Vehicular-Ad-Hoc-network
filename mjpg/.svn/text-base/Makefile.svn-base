###############################################################
#
# Purpose: Makefile for "M-JPEG Streamer"
# Author.: Tom Stoeveken (TST)
# Version: 0.3
# License: GPL
#
###############################################################

#CC = arm-linux-gcc
CC ?= gcc

CFLAGS += -O3 -DLINUX -D_GNU_SOURCE -Wall 
#CFLAGS += -O2 -DDEBUG -DLINUX -D_GNU_SOURCE -Wall
LFLAGS +=  -lpthread -ldl 

APP_BINARY=mjpg_streamer
OBJECTS=mjpg_streamer.o utils.o

# define the names and targets of the plugins
PLUGINS = input_uvc.so
PLUGINS += output_file.so
PLUGINS += output_http.so
PLUGINS += input_testpicture.so
PLUGINS += output_autofocus.so
PLUGINS += input_gspcav1.so
PLUGINS += input_file.so
PLUGINS += input_control.so
# PLUGINS += input_http.so
# PLUGINS += output_viewer.so


all: application plugins

clean:
	make -C plugins/input_uvc $@
	make -C plugins/input_testpicture $@
	make -C plugins/output_file $@
	make -C plugins/output_http $@
	make -C plugins/output_autofocus $@
	make -C plugins/input_gspcav1 $@
	make -C plugins/input_s3c2410 $@
	rm -f *.a *.o $(APP_BINARY) core *~ *.so *.lo test_jpeg

plugins: $(PLUGINS)

#input_testpicture.so output_autofocus.so input_gspcav1.so

application: $(APP_BINARY)

output_autofocus.so: mjpg_streamer.h utils.h
	make -C plugins/output_autofocus all CC=$(CC)
	cp plugins/output_autofocus/output_autofocus.so .

input_testpicture.so: mjpg_streamer.h utils.h
	make -C plugins/input_testpicture all CC=$(CC)
	cp plugins/input_testpicture/input_testpicture.so .

input_uvc.so: mjpg_streamer.h utils.h
	make -C plugins/input_uvc all CC=$(CC)
	cp plugins/input_uvc/input_uvc.so .

output_file.so: mjpg_streamer.h utils.h
	make -C plugins/output_file all CC=$(CC)
	cp plugins/output_file/output_file.so .

output_http.so: mjpg_streamer.h utils.h
	make -C plugins/output_http all CC=$(CC)
	cp plugins/output_http/output_http.so .

input_gspcav1.so: mjpg_streamer.h utils.h
	make -C plugins/input_gspcav1 all CC=$(CC)
	cp plugins/input_gspcav1/input_gspcav1.so .

input_s3c2410.so: mjpg_streamer.h utils.h
	make -C plugins/input_s3c2410 all CC=$(CC)
	cp plugins/input_s3c2410/input_s3c2410.so .
	
input_file.so: mjpg_streamer.h utils.h
	make -C plugins/input_file all
	cp plugins/input_file/input_file.so .	

input_control.so: mjpg_streamer.h utils.h
	make -C plugins/input_control all
	cp plugins/input_control/input_control.so .

$(APP_BINARY): mjpg_streamer.c mjpg_streamer.h mjpg_streamer.o utils.c utils.h utils.o
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTS) -o $(APP_BINARY)
	chmod 755 $(APP_BINARY)

package: application plugins
	tar czvf mjpg-streamer-mini2440-bin.tar.gz \
	--exclude www/.svn \
  mjpg_streamer \
  input_s3c2410.so \
	input_testpicture.so \
	input_uvc.so \
  output_file.so \
	output_http.so \
  start_s3c2410.sh \
	start_uvc.sh \
	start_uvc_yuv.sh \
  www \
  LICENSE
  
test_jpeg: test_jpeg.c simplified_jpeg_encoder.c simplified_jpeg_encoder.h
	gcc -O0 -g simplified_jpeg_encoder.c test_jpeg.c  -o test_jpeg
  
