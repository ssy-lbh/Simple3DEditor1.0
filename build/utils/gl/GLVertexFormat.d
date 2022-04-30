build/utils/gl/GLVertexFormat.o: utils/gl/GLVertexFormat.cpp \
 utils/gl/GLVertexFormat.h define.h utils/List.h utils/os/Log.h
	g++ -c utils/gl/GLVertexFormat.cpp -o build/utils/gl/GLVertexFormat.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
