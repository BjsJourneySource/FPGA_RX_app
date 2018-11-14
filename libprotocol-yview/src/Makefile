
#PREFIX := arm-none-linux-gnueabi-
#CC := $(PREFIX)gcc
#LD := $(PREFIX)ld

CFLAGS += -I ../include \
		  -I $(ROOT)/udpx/include

LDFLAGS += -L $(ROOT)/udpx/src -ludpx

SHARED := -shared -fPIC

SRC := protocol.c

OBJECT := libprotocol-yview.so


# 
all: $(OBJECT)

$(OBJECT): ${SRC:.c=.o}
	$(CC) $(SHARED) $(CFLAGS) $(LDFLAGS) -o $@ $^

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

