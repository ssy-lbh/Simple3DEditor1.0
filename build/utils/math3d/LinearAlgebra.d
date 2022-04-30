build/utils/math3d/LinearAlgebra.o: utils/math3d/LinearAlgebra.cpp \
 utils/math3d/LinearAlgebra.h define.h utils/math3d/Math.h
	g++ -c utils/math3d/LinearAlgebra.cpp -o build/utils/math3d/LinearAlgebra.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
