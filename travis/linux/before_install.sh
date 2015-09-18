export DEBIAN_FRONTEND=noninteractive
sudo add-apt-repository ppa:apokluda/boost1.53 --yes
sudo apt-get update -qq
sudo apt-get install --force-yes \
    cmake libboost-chrono1.53-dev libboost-program-options1.53-dev libboost-filesystem1.53-dev libboost-timer1.53-dev \
    libboost-test1.53-dev libboost-date-time1.53-dev libboost-thread1.53-dev \
    libboost-system1.53-dev libboost-serialization1.53-dev \
    libmpfr-dev libgmp-dev
#CGAL
wget https://gforge.inria.fr/frs/download.php/file/32994/CGAL-4.3.tar.gz
tar xzf CGAL-4.3.tar.gz
cd CGAL-4.3 && cmake . && make && sudo make install && cd ..


cmake --version
clang --version
