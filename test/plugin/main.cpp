#include <main.h>

#include <utils/String.h>
#include <manager/PluginManager.h>

int main(int argc, char** argv){
    PluginManager mgr;
    //String s(L"WString");
    mgr.LoadPlugin(WString(L"plugin"));
    Main::MainEntry(argc, argv);
}
