#ifndef __SHELL__
#define __SHELL__

struct InputAttribute {
    enum Type {
        INT,
        FLOAT,
        STRING
    } type;
    union {
        int intValue;
        float floatValue;
        const char* strValue;// Flush After Use
    };
};

bool ShellEInputWindow(InputAttribute* attrs, int cnt);

#endif