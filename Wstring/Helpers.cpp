#include <Windows.h>

#define WCHAR_MIN 0x0000
#define WCHAR_MAX 0xffff
#define BITOP(a,b,op) \
 ((a)[(size_t)(b)/(8*sizeof *(a))] op (size_t)1<<((size_t)(b)%(8*sizeof *(a))))

#define ALIGN (sizeof(size_t))
#define ONES ((size_t)-1/WCHAR_MAX)
#define HIGHS (ONES * (WCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

size_t strLenW(const wchar_t* str)
{
    size_t len = 0;
    while (str[len] != L'\0')
    {
        len++;
    }

    return len;
}

wchar_t* strCpyW(wchar_t* dest, const wchar_t* src)
{
    wchar_t* wcp = dest - 1;
    wint_t c;
    const ptrdiff_t off = src - dest + 1;
    do
    {
        c = wcp[off];
        *++wcp = c;
    } while (c != L'\0');

    return wcp;
}

int strCmpW(wchar_t* s1, wchar_t* s2)
{
    wchar_t c1, c2;
    do
    {
        c1 = *s1++;
        c2 = *s2++;
        if (c2 == L'\0')
            return c1 - c2;
    } while (c1 == c2);

    return c1 < c2 ? -1 : 1;
}

wchar_t* strCatW(wchar_t* dest, const wchar_t* src)
{
    wchar_t* s1 = dest;
    const wchar_t* s2 = src;
    wchar_t c;
    do
        c = *s1++;
    while (c != L'\0');

    s1 -= 2;

    do
    {
        c = *s2++;
        *++s1 = c;
    } while (c != L'\0');

    return dest;
}

wchar_t* strchrnulW(const wchar_t* s, int c)
{
    size_t* w, k;

    c = (wchar_t)c;
    if (!c) return (wchar_t*)s + strLenW(s);

    for (; (uintptr_t)s % ALIGN; s++)
        if (!*s || *(wchar_t*)s == c) return (wchar_t*)s;
    k = ONES * c;
    for (w = (size_t*)s; !HASZERO(*w) && !HASZERO(*w ^ k); w++);
    for (s = (wchar_t*)w; *s && *(wchar_t*)s != c; s++);
    return (wchar_t*)s;
}

size_t strspnW(const wchar_t* s, const wchar_t* c)
{
    const wchar_t* a = s;
    size_t byteset[32 / sizeof(size_t)] = { 0 };

    if (!c[0]) return 0;
    if (!c[1]) {
        for (; *s == *c; s++);
        return s - a;
    }

    for (; *c && BITOP(byteset, *(wchar_t*)c, |=); c++);
    for (; *s && BITOP(byteset, *(wchar_t*)s, &); s++);
    return s - a;
}

static inline void* memset(void* buf, char z, size_t bytes)
{
    if (buf)
    {
        char* tmp_mem = (char*)buf;
        while (bytes--) *tmp_mem++ = z;
    }

    return buf;
}

size_t strcspnW(const wchar_t* s, const wchar_t* c)
{
    const wchar_t* a = s;
    size_t byteset[32 / sizeof(size_t)];

    if (!c[0] || !c[1]) return strchrnulW(s, *c) - a;

    memset(byteset, 0, sizeof byteset);
    for (; *c && BITOP(byteset, *(wchar_t*)c, |=); c++);
    for (; *s && !BITOP(byteset, *(wchar_t*)s, &); s++);
    return s - a;
}

wchar_t* strTokW(wchar_t* s, const wchar_t* sep)
{
    static wchar_t* p;
    if (!s && !(s = p)) return nullptr;
    s += strspnW(s, sep);
    if (!*s) return p = 0;
    p = s + strcspnW(s, sep);
    if (*p) *p++ = 0;
    else p = 0;
    return s;
}

wchar_t* itoaW(unsigned long i, wchar_t b[])
{
    wchar_t digit[] = { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'\0' };
    wchar_t* p = b;
    if (i < 0)
    {
        *p++ = '-';
        i *= -1;
    }

    int shifter = i;
    do
    {
        ++p;
        shifter = shifter / 10;
    } while (shifter);

    *p = '\0';

    do
    {
        *--p = digit[i % 10];
        i = i / 10;
    } while (i);
    return b;
}