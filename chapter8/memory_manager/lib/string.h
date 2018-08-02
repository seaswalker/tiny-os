# include "global.h"
# include "debug.h"

void memset(void* address, uint8_t value, uint32_t size);
void memcpy(void* dst, const void* src, uint32_t size);
int memcmp(const void* left, const void* right, uint32_t size);
char* strcpy(char* dst, const char* src);
uint32_t strlen(const char* str);
int8_t strcmp(const char* left, const char* right);
char* strchr(const char* str, const uint8_t c);
char* strrchr(const char* str, const uint8_t c);
char* strcat(char* dst, const char* src);
uint32_t strchrs(const char* str, const uint8_t c);