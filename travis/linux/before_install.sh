export DEBIAN_FRONTEND=noninteractive
sudo -E apt-add-repository -y "ppa:ppsspp/cmake"
sudo apt-get update -qq
sudo apt-get install --force-yes \
    cmake libboost-chrono1.55-dev libboost-program-options1.55-dev libboost-filesystem1.55-dev libboost-timer1.55-dev \
    libboost-test1.55-dev libboost-date-time1.55-dev libboost-thread1.55-dev \
    libboost-system1.55-dev libboost-serialization1.55-dev \
    libmpfr-dev libgmp-dev \
    cmake
#CGAL

wget https://github.com/CGAL/cgal/archive/releases/CGAL-"$1".tar.gz
tar xzf CGAL-"$1".tar.gz
cd cgal-releases-CGAL-"$1" && cmake . && make && sudo make install && cd ..

cmake --version
clang --version
