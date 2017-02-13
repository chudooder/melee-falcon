# Install essentials
apt-get install build-essential
apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev

# Get OpenCV
wget https://github.com/opencv/opencv/archive/3.2.0.zip -O opencv-3.2.0.zip
tar xvf opencv-3.2.0.zip

# Build OpenCV
cd opencv-3.2.0/opencv-3.2.0
mkdir build
cd build
cmake .. #TODO: Fix with FFMPEG support
cd ..
make install
