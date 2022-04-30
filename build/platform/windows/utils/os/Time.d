build/platform/windows/utils/os/Time.o: \
 platform/windows/utils/os/Time.cpp utils/os/Time.h define.h main.h \
 utils/List.h utils/os/Log.h utils/os/AppFrame.h utils/String.h \
 utils/os/Font.h utils/math3d/LinearAlgebra.h utils/math3d/Math.h
	g++ -c platform/windows/utils/os/Time.cpp -o build/platform/windows/utils/os/Time.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
