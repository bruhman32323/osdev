#include <drivers/device/device.h>

int _device_index = 0;

struct device descriptors[64];

int push_device(struct device device) {
    int descriptor = _device_index;

    descriptors[descriptor] = device;
    _device_index++;

    return descriptor;
}

void* read(int descriptor, void* data) {
    if (descriptor >= _device_index) return -1;

    struct device device = descriptors[descriptor];

    return device.read(data);
}

void* write(int descriptor, void* data) {
    if (descriptor >= _device_index) return -1;

    struct device device = descriptors[descriptor];

    return device.write(data);
}