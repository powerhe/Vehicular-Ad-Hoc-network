mjpg        文件夹是mipg-streamer网络摄像头视频流服务器源码；
webcam      文件夹是视频控制网页包，包括HTML文件；
control.c   需要交叉编译成cgi文件，是运行在ARM开发板linux系统下的脚本程序，用于              接收HTML发送的命令，传环境变量给cgi中程序执行；
vehicle_A.c A车单片机控制程序；
vehicle_B.c B车单片机控制程序；
server_A.c  A车ARM控制程序；
socket_B.c  B车ARM控制程序；
arm-linux-gcc-4.5.1-v6-vfp-20101103.tgz linux下交叉编译工具压缩包。
