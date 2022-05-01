#include <base/Writable.h>

void IWritable::Write(char c){
    Write(&c, 1);
}

void IWritable::Write(short s){
    Write(&s, sizeof(short));
}

void IWritable::Write(int i){
    Write(&i, sizeof(int));
}

void IWritable::Write(long long l){
    Write(&l, sizeof(long long));
}

void IWritable::Write(float f){
    Write(&f, sizeof(float));
}

void IWritable::Write(double d){
    Write(&d, sizeof(double));
}