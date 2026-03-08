BUILD_DIR := build
PROGRAM = opencv_test

all:
	${CXX} main.c -o $(BUILD_DIR)/$(PROGRAM) `pkg-config --cflags --libs opencv4`

clean:
	rm $(BUILD_DIR)/$(PROGRAM)
