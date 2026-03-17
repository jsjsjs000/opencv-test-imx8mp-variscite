  # Build OpenCV 4.12.0 from github
source /opt/fslc-xwayland/4.0/environment-setup-armv8a-fslc-linux; export LDFLAGS=

opencv=$HOME/opencv_4_12_imx8mp_variscite/

git clone -b 4.12.0 https://github.com/opencv/opencv
mv opencv/ ${opencv}
cd ${opencv}/

git clone -b 4.12.0 https://github.com/opencv/opencv_contrib

mkdir build
cd build
cmake \
	-DWITH_OPENJPEG=OFF \
	-DBUILD_TESTS=OFF \
	-DBUILD_PERF_TESTS=OFF \
  -DBUILD_EXAMPLES=ON \
	-DCMAKE_INSTALL_PREFIX=/usr \
	-DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules/ \
	..
# -D CMAKE_BUILD_TYPE=Release

make -j$(nproc)
make install DESTDIR=install/

# test output libraries
ls ${opencv}/build/install/usr/include/opencv4/opencv2/
ls ${opencv}/build/install/usr/lib/

ls /opt/fslc-xwayland/4.0/sysroots/armv8a-fslc-linux/usr/include/opencv4/opencv2/
ls /opt/fslc-xwayland/4.0/sysroots/armv8a-fslc-linux/usr/lib/

ls lib/

file lib/libopencv_core.so.4.12.0
#> lib/libopencv_core.so.4.12.0: ELF 64-bit LSB shared object, ARM aarch64, version 1 (GNU/Linux), dynamically linked, BuildID[sha1]=99b32a36e1a599fe8047591affe828b874b09a8f, with debug_info, not stripped

# ------------------------------------------------------------------------------

  # Copy compiled files
# backup OpenCV SDK toolchain
cd /opt/fslc-xwayland/4.0/sysroots/armv8a-fslc-linux/usr/
cd lib/
sudo mkdir opencv_4.6
sudo mv libopencv* opencv_4.6/
cd ..

cd bin/
sudo mkdir _opencv_4.6
sudo mv opencv* _opencv_4.6/
cd ..

cd include/
sudo mv opencv4/ opencv4.6/
cd ..

# copy new OpenCV 4.12 to SDK toolchain
cd ${opencv}/build/install/usr/
cd lib/
sudo cp * /opt/fslc-xwayland/4.0/sysroots/armv8a-fslc-linux/usr/lib/
cd ..

cd bin/
sudo cp * /opt/fslc-xwayland/4.0/sysroots/armv8a-fslc-linux/usr/bin/
cd ..

cd include/
sudo cp -r opencv4/ /opt/fslc-xwayland/4.0/sysroots/armv8a-fslc-linux/usr/include/
cd ..

  # copy new OpenCV 4.12 to i.MX
cd ${opencv}/build/install/usr/
scp bin/* root@192.168.3.11:/usr/bin/
scp lib/* root@192.168.3.11:/usr/lib/

  # upload xml file
scp haarcascade_frontalface_default.xml root@192.168.3.11:/home/root/
# downloaded from https://github.com/opencv/opencv/blob/master/data/haarcascades/haarcascade_frontalface_default.xml

# ------------------------------------------------------------------------------

  # restore backuped OpenCV 4.6 (orginal in Yocto)
cd /opt/fslc-xwayland/4.0/sysroots/armv8a-fslc-linux/usr/
cd lib/
sudo cp opencv_4.6/* .
cd ..

cd bin/
sudo cp _opencv_4.6/* .
cd ..

cd include/
sudo cp -r opencv4.6/ opencv4/
cd ..

  # copy OpenCV 4.6 to i.MX
scp bin/opencv* root@192.168.3.11:/usr/bin/
scp lib/libopencv* root@192.168.3.11:/usr/lib/

# ------------------------------------------------------------------------------

  # PC Ubuntu
sudo apt install libopencv-dev

# ------------------------------------------------------------------------------

  # Check library
readelf -d build/opencv_test

ldd build/opencv_test | grep libopencv*
objdump -p build/opencv_test | grep NEEDED

ls /usr/lib/libopencv_*.so*

# libopencv_core.so -> libopencv_core.so.406
# libopencv_core.so.406 -> libopencv_core.so.4.6.0
# libopencv_core.so.4.6.0

# linker używa        libopencv_core.so
# runtime używa       libopencv_core.so.406
# faktyczny plik      libopencv_core.so.4.6.0

pkg-config --modversion opencv4

pkg-config --cflags --libs opencv4

# ------------------------------------------------------------------------------

# run on i.MX
./opencv_test

# test
gst-launch-1.0 videotestsrc ! video/x-raw,format=BGR,width=1280,height=720,framerate=30/1 ! videoconvert ! \
    waylandsink sync=true window-width=1280 window-height=720

gst-launch-1.0 videotestsrc ! video/x-raw,format=BGR,width=1280,height=720,framerate=30/1 ! videoconvert ! \
    fpsdisplaysink sync=true video-sink="waylandsink sync=true window-width=1280 window-height=720"
