
#PREFIX := arm-none-linux-gnueabi-
#CC := $(PREFIX)gcc
#LD := $(PREFIX)ld

CFLAGS += -I ../include

LDFLAGS += 

SHARED := -shared -fPIC

SRC := udpx_misc.c \
	udpx_buffer.c \
	udpx_int_ring_buffer.c \
	udpx.c

OBJECT := libudpx.so


# 
all: $(OBJECT)

$(OBJECT) : ${SRC:.c=.o}
	$(CC) $(SHARED) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(SHARED) $(CFLAGS) -o $@ -c $^

# 
install: 
	cp $(OBJECT) $(OUT)

# 
uninstall: 
	rm $(OUT)/$(OBJECT)

# 
.PHONY: clean
clean: 
	@rm -rf *.o
	@rm -rf $(OBJECT)
	@echo "XXXXX clean."

