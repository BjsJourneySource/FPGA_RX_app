
#CROSS_COMPILE ?= arm-none-linux-gnueabi-
CROSS_COMPILE ?= arm-buildroot-linux-uclibcgnueabi-

SUB_DIRS := \
	udpx/src \
	libprotocol-yview/src 

SUB_DIRS += brvs_uvc_app
#SUB_DIRS += telescope/Sonix

CFLAGS := -Wall -g

LDFLAGS := -g

CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
AS=$(CROSS_COMPILE)as
TRIP=$(CROSS_COMPILE)strip

MAKE := make

ROOT := $(shell pwd)
OUT := $(ROOT)/out

export CC LD AS TRIP MAKE
export ROOT OUT
export CFLAGS LDFLAGS

# 
all: $(SUB_DIRS)
	@for dir in $^; do \
		$(MAKE) -C $$dir all; \
		echo ""; \
	done

# 
install: $(SUB_DIRS)
	@mkdir -p $(OUT)
	@for dir in $^; do \
		$(MAKE) -C $$dir install; \
		echo ""; \
	done

# 
uninstall: $(SUB_DIRS)
	@for dir in $^; do \
		$(MAKE) -C $$dir uninstall; \
		echo ""; \
	done
	@rm -rf $(OUT)

# 
.PHONY: clean
clean: $(SUB_DIRS)
	@for dir in $^; do \
		$(MAKE) -C $$dir clean; \
		echo ""; \
	done

