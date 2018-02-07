./bootstrap && sed -i 's/-Werror //; s/gnu++98/gnu++11/; s/-pedantic-errors//' configure
make -f examples/Makefile-z1 clean
