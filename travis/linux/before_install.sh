export DEBIAN_FRONTEND=noninteractive
sudo add-apt-repository ppa:apokluda/boost1.53 --yes
sudo apt-get update -qq
sudo apt-get install --force-yes \
    cmake libboost-chrono1.53-dev libboost-program-options1.53-dev libboost-filesystem1.53-dev libboost-timer1.53-dev \
    libboost-test1.53-dev libboost-date-time1.53-dev libboost-thread1.53-dev \
    libboost-system1.53-dev libboost-serialization1.53-dev \
    libmpfr-dev libgmp-dev
#CGAL
if [ "$1" = "4.3" ]; then
    wget https://gforge.inria.fr/frs/download.php/file/32994/CGAL-4.3.tar.gz
    tar xzf CGAL-4.3.tar.gz
    cd CGAL-4.3 && cmake . && make && sudo make install && cd ..
elif [ "$1" = "4.7" ]; then
    wget https://github.com/CGAL/cgal/releases/download/releases%2FCGAL-4.7/CGAL-4.7.tar.gz
    tar xzf CGAL-4.7.tar.gz
    cd CGAL-4.7 && cmake . && make && sudo make install && cd ..
fi


cmake --version
clang --version
