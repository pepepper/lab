#include <libbrain.h>

int main(void *arg)
{
    typedef void (*FUNC_POINTER)();
    FUNC_POINTER func = (FUNC_POINTER)0x60198000;
    func();
    return 0;
}
