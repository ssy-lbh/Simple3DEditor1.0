build/platform/windows/manager/PluginManager.o: \
 platform/windows/manager/PluginManager.cpp manager/PluginManager.h \
 define.h utils/List.h utils/os/Log.h utils/String.h \
 utils/StringBuilder.h utils/File.h
	g++ -c platform/windows/manager/PluginManager.cpp -o build/platform/windows/manager/PluginManager.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
