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


  # PC Ubuntu
sudo apt install libopencv-dev


  # OpenCV 4.12.0 from github
git clone https://github.com/opencv/opencv -b 4.12.0
cd opencv/
mkdir build
cd build
cmake -DWITH_OPENJPEG=OFF ..
make -j$(nproc)
make install DESTDIR=install/

ls $HOME/opencv/build/install/$HOME/opencv/build/install/include/opencv4/opencv2/
ls $HOME/opencv/build/install/$HOME/opencv/build/install/lib/

ls /opt/fslc-xwayland/4.0/sysroots/armv8a-fslc-linux/usr/include/opencv4/opencv2/
/opt/fslc-xwayland/4.0/sysroots/armv8a-fslc-linux/usr/lib/

ls lib/
file lib/libopencv_core.so.4.12.0
#> lib/libopencv_core.so.4.12.0: ELF 64-bit LSB shared object, ARM aarch64, version 1 (GNU/Linux), dynamically linked, BuildID[sha1]=99b32a36e1a599fe8047591affe828b874b09a8f, with debug_info, not stripped


# backup OpenCV SDK toolchain
cd /opt/fslc-xwayland/4.0/sysroots/armv8a-fslc-linux/usr/
cd lib/
sudo mkdir _opencv_4.6
sudo mv libopencv* _opencv_4.6/
cd ..

cd bin/
sudo mkdir _opencv_4.6
sudo mv opencv* _opencv_4.6/
cd ..

cd include/
sudo mv opencv4/ opencv4.6/
cd ..

# copy new OpenCV 4.12 to SDK toolchain
cd ~/opencv/build/install/$HOME/opencv/build/install/
cd lib/
sudo cp * /opt/fslc-xwayland/4.0/sysroots/armv8a-fslc-linux/usr/lib/
cd ..

cd bin/
sudo cp * /opt/fslc-xwayland/4.0/sysroots/armv8a-fslc-linux/usr/bin/
cd ..

cd include/
sudo cp -r opencv4/ /opt/fslc-xwayland/4.0/sysroots/armv8a-fslc-linux/usr/include/
cd ..

  # i.MX
