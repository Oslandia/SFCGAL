export DEBIAN_FRONTEND=noninteractive
sudo apt-get update -qq
sudo apt-get install --force-yes \
    cmake libboost-chrono1.58-dev libboost-program-options1.58-dev libboost-filesystem1.58-dev libboost-timer1.58-dev \
    libboost-test1.58-dev libboost-thread1.58-dev \
    libboost-system1.58-dev libboost-serialization1.58-dev \
    libmpfr-dev libgmp-dev \
    cmake
#CGAL

wget https://github.com/CGAL/cgal/releases/download/releases/CGAL-"$1"/CGAL-"$1".tar.xz
tar xJf CGAL-"$1".tar.xz
cd CGAL-"$1" && mkdir build && cd build && cmake -DCMAKE_INSTALL_PREFIX=$HOME/CGAL-"$1" .. && make && make install && cd ../..

cmake --version
clang --version
