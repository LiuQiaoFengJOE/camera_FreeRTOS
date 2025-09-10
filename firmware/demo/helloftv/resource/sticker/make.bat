tcc yuv.c -o yuv.exe
if not exist bin (
   md bin
)
ffmpeg -i frame1.jpg -y -s 640x480 -pix_fmt nv12 4.yuv
yuv.exe 640 480 4.yuv bin\framelista1.bin
ffmpeg -i frame2.jpg -y -s 640x480 -pix_fmt nv12 4.yuv
yuv.exe 640 480 4.yuv bin\framelista2.bin
ffmpeg -i frame3.jpg -y -s 640x480 -pix_fmt nv12 4.yuv
yuv.exe 640 480 4.yuv bin\framelista3.bin
ffmpeg -i frame4.jpg -y -s 640x480 -pix_fmt nv12 4.yuv
yuv.exe 640 480 4.yuv bin\framelista4.bin
ffmpeg -i frame1.jpg -y -s 320x240 -pix_fmt nv12 4.yuv
yuv.exe 320 240 4.yuv bin\framelistb1.bin
ffmpeg -i frame2.jpg -y -s 320x240 -pix_fmt nv12 4.yuv
yuv.exe 320 240 4.yuv bin\framelistb2.bin
ffmpeg -i frame3.jpg -y -s 320x240 -pix_fmt nv12 4.yuv
yuv.exe 320 240 4.yuv bin\framelistb3.bin
ffmpeg -i frame4.jpg -y -s 320x240 -pix_fmt nv12 4.yuv
yuv.exe 320 240 4.yuv bin\framelistb4.bin
pause