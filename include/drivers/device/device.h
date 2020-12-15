#ifndef _DEVICE_H
#define _DEVICE_H 1

#ifdef __cplusplus
extern "C" {
#endif

struct device {
    void* (*write)(void* data);
    void* (*read)(void* data);
};

int push_device(struct device device);

void* read(int descriptor, void* data);
void* write(int descriptor, void* data);

#ifdef __cplusplus
}
#endif

#endif /* _DEVICE_H */