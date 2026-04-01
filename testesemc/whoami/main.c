#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <cpuid.h>
#include <string.h>

#define C_SMALL_STRING_LEN 32
#define C_MEDIUM_STRING_LEN 64
#define C_BIG_STRING_LEN 128

typedef struct {
	char		Name[C_MEDIUM_STRING_LEN];
	int			Cores;
	int			Threads;
	char		Arch[C_SMALL_STRING_LEN];
	int			Clock;
	int			Cache;

} cpu_data;

typedef struct {
	char		Name[C_BIG_STRING_LEN];
	int			Active;

} av_data;

typedef struct {
	char		Name[C_BIG_STRING_LEN];
	char		Version[C_BIG_STRING_LEN];
	char		Arch[C_SMALL_STRING_LEN];
	char		Hostname[C_BIG_STRING_LEN];
	char		Username[C_BIG_STRING_LEN];
	char		Domain[C_BIG_STRING_LEN];
	int			Uptime;
	av_data		AV;

} os_data;

typedef struct {
	char		IP[C_SMALL_STRING_LEN];
	cpu_data	CPU;
	os_data		OS;

} C_Scroll;

C_Scroll fetchTargetInfo() {
    /*
        NOTES:
            - As it is a basic spell, i'll not set smts like av_data

    */

    C_Scroll botData;
    memset(&botData, 0, sizeof(botData));
    char cpuName[49];
    unsigned int cores = 0, threads = 0, cache = 0;
    double cpuClock = 0.0;
    char _cpuinfoLine[256];

    unsigned int eax, ebx, ecx, edx;
    
    cpuName[0] = '\0';
    for (int i = 0x80000002; i <= 0x80000004; i++) {
        if (__get_cpuid(i, &eax, &ebx, &ecx, &edx)) {
            unsigned int *_p = (unsigned int*)(cpuName + (i - 0x80000002) * 16);
            _p[0] = eax; _p[1] = ebx; _p[2] = ecx; _p[3] = edx;

        }

    }

    cpuName[48] = '\0';
    unsigned int _maxLeaf = __get_cpuid_max(0, NULL);
    if (_maxLeaf >= 4) {
        __cpuid_count(4, 0, eax, ebx, ecx, edx);
        threads = ((eax >> 26) & 0x3F) + 1;

        __cpuid_count(0xB, 0, eax, ebx, ecx, edx);
        cores = threads / (ebx & 0xFFF);

        int _i = 0;
        while (1) {
            __cpuid_count(4, _i, eax, ebx, ecx, edx);
            unsigned int cacheType = eax & 0x1F;
            if (cacheType == 0) break; // End of count

            unsigned int _size = (((ebx >> 22) & 0x3FF) + 1) * (((ebx >> 12) & 0x3FF) + 1) * ((ebx & 0xFFF) + 1) * (ecx + 1) / 1024;
            cache += _size;
            _i++;

        }

    }

    char *_tmp_procCpuinfoPath = "/proc/cpuinfo";
    FILE *_cpuinfo = fopen(_tmp_procCpuinfoPath, "r");
    if (_cpuinfo != NULL) {
        while (fgets(_cpuinfoLine, 256, _cpuinfo)) {
            if (strstr(_cpuinfoLine, "cpu MHz")) {
                sscanf(_cpuinfoLine, "cpu MHz: %lf", &cpuClock);
                break;

            }

        }

        fclose(_cpuinfo);

    }

    strcpy(botData.CPU.Name, cpuName);
    botData.CPU.Cores = cores;
    botData.CPU.Threads = threads;
    botData.CPU.Cache = cache;
    botData.CPU.Clock = (int)cpuClock;
    strcpy(botData.CPU.Arch, "x86_64");

    char osHostname[128];
    char osName[128];
    char osVersion[128];
    strcpy(botData.OS.Arch, "x86");
    char *osUsername;
    double osUptime;

    char *_tmp_hostnamePath = "/etc/hostname";
    FILE *_hostname = fopen(_tmp_hostnamePath, "r");
    char _hostnameLine[128];
    if (_hostname != NULL) {
        while(fgets(_hostnameLine, 128, _hostname)) {
            sscanf(_hostnameLine, "%s", osHostname);
            break;

        }

        fclose(_hostname);

    }

    char *_tmp_osReleasePath = "/etc/os-release";
    FILE *_osRealease = fopen(_tmp_osReleasePath, "r");
    char _osRealeaseLine[128];
    if (_osRealease != NULL) {
        while (fgets(_osRealeaseLine, 128, _osRealease) != NULL) {
            _osRealeaseLine[strcspn(_osRealeaseLine, "\n")] = '\0';
            sscanf(_osRealeaseLine, "NAME=\"%s", osName);
            sscanf(_osRealeaseLine, "VERSION=\"%s", osVersion);
            
        }

        fclose(_osRealease);

    }

    long _limit = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (_limit == -1) _limit = 1024;

    char *_username = malloc(_limit);
    if (_username) {
        if (getlogin_r(_username, _limit) == 0) {
            osUsername = malloc(strlen(_username) + 1);
            strcpy(osUsername, _username);

        }

        free(_username);

    }

    char *_tmp_uptimePath = "/proc/uptime";
    FILE *_uptime = fopen(_tmp_uptimePath, "r");
    char _uptimeLine[512];
    if (_uptime != NULL) {
        while (fgets(_uptimeLine, 512, _uptime) != NULL) {
            sscanf(_uptimeLine, "%lf", &osUptime);

        }

    }

    strcpy(botData.OS.Name, osName);
    strcpy(botData.OS.Version, osVersion);
    strcpy(botData.OS.Hostname, osHostname);
    strcpy(botData.OS.Username, osUsername);
    botData.OS.Uptime = (int)osUptime;

    free(osUsername);

    return botData;

};

void printScroll(C_Scroll scroll) {
    printf("IP: %s,\nCPU: {\n    Name: %s,\n    Cores: %d,\n    Threads: %d,\n    Arch: %s,\n    Clock: %d,\n    Cache: %d MB\n},\nOS: {\n    Name: %s,\n    Version: %s,\n    Arch: %s,\n    Hostname: %s,\n    Domain: %s,\n    Uptime: %d seconds,\n    AV: {\n        Name: %s,\n        Active: %s\n    }\n}\n", scroll.IP, scroll.CPU.Name, scroll.CPU.Cores, scroll.CPU.Threads, scroll.CPU.Arch, scroll.CPU.Clock, scroll.CPU.Cache, scroll.OS.Name, scroll.OS.Version, scroll.OS.Arch, scroll.OS.Hostname, scroll.OS.Domain, scroll.OS.Uptime, scroll.OS.AV.Name, scroll.OS.AV.Active == 1 ? "Yes" : "No");
}

int main() {
    printScroll(fetchTargetInfo());
    return 0;

}
