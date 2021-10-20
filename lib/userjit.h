#ifndef USERJIT__H
#define USERJIT__H


class MiniJIT {
private:
public:
    // bool -> some kind of buffer
    virtual bool applyTo() = 0;
};

#endif