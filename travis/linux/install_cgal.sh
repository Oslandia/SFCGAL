#CGAL
if [ "$1" = "4.3" ]; then
    wget https://gforge.inria.fr/frs/download.php/file/32994/CGAL-4.3.tar.gz
    tar xzf CGAL-4.3.tar.gz
    cd CGAL-4.3 && cmake -DCMAKE_INSTALL_PREFIX=. . && make && make install && cd ..
elif [ "$1" = "4.7" ]; then
    wget https://github.com/CGAL/cgal/releases/download/releases%2FCGAL-4.7/CGAL-4.7.tar.gz
    tar xzf CGAL-4.7.tar.gz
    cd CGAL-4.7 && cmake -DCMAKE_INSTALL_PREFIX=. . && make && make install && cd ..
fi


cmake --version
clang --version
