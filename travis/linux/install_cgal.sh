#CGAL

wget https://github.com/CGAL/cgal/releases/download/releases/CGAL-"$1"/CGAL-"$1".tar.xz
tar xJf CGAL-"$1".tar.xz
cd CGAL-"$1" && mkdir build && cd build && cmake -DCMAKE_INSTALL_PREFIX=$HOME/CGAL-"$1" .. && make && make install && cd ../..

clang --version
