#CGAL

wget https://github.com/CGAL/cgal/archive/releases/CGAL-"$1".tar.gz
tar xzf CGAL-"$1".tar.gz
cd CGAL-"$1" && mkdir build && cd build && cmake -DCMAKE_INSTALL_PREFIX=$HOME/CGAL-"$1" .. && make && make install && cd ../..

clang --version
