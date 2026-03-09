BUILD_DIR := build
PROGRAM = opencv_test

all:
	${CXX} main.cpp -o $(BUILD_DIR)/$(PROGRAM) \
			-lopencv_core \
			-lopencv_imgproc \
			-lopencv_highgui \
			-lopencv_videoio \
			-lopencv_objdetect

# `pkg-config --cflags --libs opencv4`

clean:
	rm $(BUILD_DIR)/$(PROGRAM)
