build/platform/windows/utils/os/Resource.o: \
 platform/windows/utils/os/Resource.cpp utils/os/Resource.h define.h \
 lib/stb_image/stb_image.h lib/stb_image/stb_image_write.h res.h \
 utils/String.h utils/DataBuffer.h utils/os/Log.h utils/os/System.h
	g++ -c platform/windows/utils/os/Resource.cpp -o build/platform/windows/utils/os/Resource.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
