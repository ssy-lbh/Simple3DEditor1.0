#include <main.h>

int main(int argc, char** argv){
    Main::inst = new Main();
    int ret = Main::inst->MainEntry(argc, argv);
    delete Main::inst;
    return ret;
}