#pragma once
#include <Windows.h>
void* hAlloc(size_t size);
void hFree(void* mem);
wchar_t* itoaW(unsigned long i, wchar_t b[]);
wchar_t* strTokW(wchar_t* s, const wchar_t* sep);
size_t strcspnW(const wchar_t* s, const wchar_t* c);
void* my_memset(void* buf, char z, size_t bytes);
size_t strspnW(const wchar_t* s, const wchar_t* c);
wchar_t* strchrnulW(const wchar_t* s, int c);
wchar_t* strCatW(wchar_t* dest, const wchar_t* src);
int strCmpW(wchar_t* s1, wchar_t* s2);
wchar_t* strCpyW(wchar_t* dest, const wchar_t* src);
size_t strLenW(const wchar_t* str);