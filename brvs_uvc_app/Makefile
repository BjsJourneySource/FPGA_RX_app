#CROSS_COMPILE=arm-buildroot-linux-uclibcgnueabi-
#CC=$(CROSS_COMPILE)gcc
#AS=$(CROSS_COMPILE)as
#LD=$(CROSS_COMPILE)gcc
#STRIP=$(CROSS_COMPILE)strip


CFLAGS += -I/usr/src/linux-$(shell uname -r)/include/generated/uapi -I/usr/src/linux-$(shell uname -r)/include/uapi
CFLAGS += -I/usr/src/linux-$(shell uname -r)/include
CFLAGS += -I ../libprotocol-yview/include
CFLAGS += -O2 -ftree-vectorize -I./ -I./include -Iinc/ -std=gnu99 -mcpu=cortex-a9 -mfpu=neon

LDFLAGS += -lpthread \
		   -L ../udpx/src -ludpx \
		   -L ../libprotocol-yview/src -lprotocol-yview
LDFLAGS += -L./ -lm -lc


#OBJS = brvs_uvc_app.o \
	sonix_xu_ctrls.o

OBJS = bv_app.o lock.o vb_buffer.o bits.o wait.o \
	nalu.o

OBJECT := cfdm_rx_app

all: $(OBJECT)

$(OBJECT): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)


%o: %c
	$(CC) $(CFLAGS) -c -o $@ $^

# 
install: 
	cp $(OBJECT) $(OUT)

# 
uninstall: 
	rm $(OUT)/$(OBJECT)


.PHONY: clean
clean:
	-rm -f *.o $(OBJECT)


