export DEBIAN_FRONTEND=noninteractive
sudo apt-get update -qq
sudo apt-get install --force-yes \
    cmake libboost-chrono1.58-dev libboost-program-options1.58-dev libboost-filesystem1.58-dev libboost-timer1.58-dev \
    libboost-test1.58-dev libboost-thread1.58-dev \
    libboost-system1.58-dev libboost-serialization1.58-dev \
    libmpfr-dev libgmp-dev \
    cmake
#CGAL

wget https://github.com/CGAL/cgal/archive/releases/CGAL-"$1".tar.gz
tar xzf CGAL-"$1".tar.gz
cd cgal-releases-CGAL-"$1" && cmake . && make && sudo make install && cd ..

cmake --version
clang --version
