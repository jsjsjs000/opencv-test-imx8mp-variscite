# BUILD = debug
BUILD = release


BUILD_DIR := build
PROGRAM = opencv_test

PKG_CONFIG += -lopencv_core
PKG_CONFIG += -lopencv_imgproc
PKG_CONFIG += -lopencv_highgui
PKG_CONFIG += -lopencv_videoio
PKG_CONFIG += -lopencv_objdetect
PKG_CONFIG += -lopencv_tracking
# PKG_CONFIG += `pkg-config --cflags --libs opencv4`
LDLIBS += -I/opt/fslc-xwayland/4.0/sysroots/armv8a-fslc-linux/usr/include/opencv4

CFLAGS += -Wall
LDFLAGS += -Wl,--no-undefined

ifeq ($(BUILD),debug)
	LDFLAGS += -g
endif

ifeq ($(BUILD),release)
	CFLAGS += -O2
endif

LDLIBS += -lm  # math library

all:
	$(info Build type: $(BUILD))$(info )
	mkdir -p $(BUILD_DIR)
	${CXX} $(CFLAGS) $(LDFLAGS) \
			main.cpp -o \
			$(BUILD_DIR)/$(PROGRAM) \
			$(PKG_CONFIG) $(LDLIBS)

clean:
	rm $(BUILD_DIR)/$(PROGRAM)
