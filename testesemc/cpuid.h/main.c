#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cpuid.h>

int main() {
    unsigned int eax, ebx, ecx, edx;
    char vendor[13];
    char brand[49];

    if (__get_cpuid(0, &eax, &ebx, &ecx, &edx)) {
        ((uint*)vendor)[0] = ebx;
        ((uint*)vendor)[1] = edx;
        ((uint*)vendor)[2] = ecx;
        vendor[12] = '\0';
        printf("Vendor: %s\n", vendor);

    }

    brand[0] = '\0';
    for (int i = 0x80000002; i <= 0x80000004; i++) {
        if (__get_cpuid(i, &eax, &ebx, &ecx, &edx)) {
            uint *p = (uint*)(brand + (i - 0x80000002) * 16);
            p[0] = eax; p[1] = ebx; p[2] = ecx; p[3] = edx;

        }

    }
    brand[48] = '\0';
    printf("CPU Name: %s\n", brand);

    uint maxLeaf = __get_cpuid_max(0, NULL);
    if (maxLeaf >= 4) {
        __cpuid_count(4, 0, eax, ebx, ecx, edx);
        uint threads = ((eax >> 26) & 0x3F) + 1;

        __cpuid_count(0xB, 0, eax, ebx, ecx, edx);
        uint threadsPerCore = ebx & 0xFFF;
        printf("Cores: %u\nThreads: %u\n", threads / threadsPerCore, threads);

    }

    if (maxLeaf >= 1) {
        __get_cpuid(1, &eax, &ebx, &ecx, &edx);
        uint baseMhz = eax & 0xFFFF;
        uint maxMhz = ebx & 0xFFFF;
        printf("Base clock: %u MHz\nMax clock: %u MHz\n", baseMhz, maxMhz);

    }

    if (maxLeaf >= 4) {
        int i = 0;
        while (1) {
            __cpuid_count(4, i, eax, ebx, ecx, edx);
            uint cacheType = eax & 0x1F;
            if (cacheType == 0) break;
            
            uint level = (eax >> 5) & 0x7;
            uint ways = ((ebx >> 22) & 0x3FF) + 1;
            uint partitions = ((ebx >> 12) & 0x3FF) + 1;
            uint lineSize = (ebx & 0xFFF) + 1;
            uint sets = ecx + 1;
            uint size = ways * partitions * lineSize * sets / 1024;

            printf("L%u cache size: %u KB\n", level, size);
            i++;

        }

    }

    return 0;

}
