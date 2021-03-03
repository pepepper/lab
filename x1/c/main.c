void main() {
    char *str = "Hello World";
    int aa = 1234;
    int i;
    for (i=0; i<999; i++) {
        asm volatile(
            "mrc p15, 0, r10, c1, c0, 0\n"
            "bic r10, #1\n"
            "mcr p15, 0, r10, c1, c0, 0\n"
            : "=r" (aa)
        );
        aa += 1;
    }
}
