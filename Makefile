BUILD_DIR := build
PROGRAM = opencv_test

all:
	mkdir -p $(BUILD_DIR)
	${CXX} main.cpp -o $(BUILD_DIR)/$(PROGRAM) \
			-lopencv_core \
			-lopencv_imgproc \
			-lopencv_highgui \
			-lopencv_videoio \
			-lopencv_objdetect \
			-lopencv_tracking

# `pkg-config --cflags --libs opencv4`

clean:
	rm $(BUILD_DIR)/$(PROGRAM)
