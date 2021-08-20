#ifndef PACK_H
#define PACK_H

typedef struct
{
    int width;
    int height;
    int bytesPerline;
    bool isLastpack;
    int packTaken;
    char data[4096];
} ImagePackage;

struct AudioPackage
{
    char data[1024];
    int datalen;
};

#endif // PACK_H
