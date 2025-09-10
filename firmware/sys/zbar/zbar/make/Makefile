GCCPATH = C:\MinGW-4.7.1\bin

LIB     = .\libzbar.a

CC         = $(GCCPATH)/or1k-elf-gcc.exe  -DOR1K_NEWCPU -mno-delay
AS         = $(GCCPATH)/or1k-elf-as.exe
AR         = $(GCCPATH)/or1k-elf-ar.exe



C_FLAGS    = -I./ -I../inc -O2 -Wall -mhard-div -c 
CAS_FLAGS  = -I./ -I../inc -c 
AR_FLAGS   = ru


# 取得当前目录所有程序文件
#files := $(wildcard ../src/*.c ../src/*.S )
files := $(wildcard ../src/*.c ../src/decoder/*.c ../src/qrcode/*.c ../iconv/src/iconv.c ../iconv/src/localcharset.c ../iconv/src/relocatable.c)


files := $(basename $(files))

objs  := $(addsuffix .o, $(files))

OBJECTS := $(objs)

%.a : $(OBJECTS)
	$(AR) $(AR_FLAGS) $@ $^

%.o : %.c
	$(CC) $(C_FLAGS) $< -o $@

%.o : %.s
	$(AS) $(AS_FLAGS) $< -o $@

%.o : %.S
	$(CC) $(CAS_FLAGS) $< -o $@


all : $(LIB)
	copy .\$(LIB) ..\..\$(LIB)

clean :
	del *.bak
	del *.a
	del /Q objs
	del $(LIB)

