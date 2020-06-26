#include "Wstring.h"
#include "Helpers.h"

#include <Windows.h>

static const size_t npos = (-1);

Wstring::Wstring() {
    length = 0;
    data = nullptr;
}

Wstring::Wstring(const wchar_t* data) {
    length = (size_t) strLenW(data);
    this->data = (wchar_t*)hAlloc(sizeof(wchar_t)*this->length + 1);
    strCpyW(this->data, data);
}

Wstring::Wstring(const Wstring& s) {
    length = s.length;
    this->data = (wchar_t*)hAlloc(sizeof(wchar_t)*length + 1);
    strCpyW(this->data, s.data);
}

Wstring::~Wstring() {
    delete[] data;
}

const Wstring& Wstring::operator= (const wchar_t* data) {
    length = (size_t) strLenW(data);
    this->data = (wchar_t*)hAlloc(sizeof(wchar_t)*length + 1);
    strCpyW(this->data, data);
    return *this;
}

const Wstring& Wstring::operator= (const Wstring& s) {
    length = s.length;
    this->data = (wchar_t*)hAlloc(sizeof(wchar_t)*length + 1);
    strCpyW(this->data, s.data);
    return *this;
}

Wstring Wstring::operator+ (const Wstring& s) {
    Wstring temp;
    temp.length = length + s.length;
    temp.data = (wchar_t*)hAlloc(sizeof(wchar_t)*temp.length + 1);
    strCpyW(temp.data, data);
    strCatW(temp.data, s.data);
    return temp;
}

Wstring Wstring::operator+ (const wchar_t* d) {
    Wstring temp;
    temp.length = length + (int)strLenW(d);
    temp.data = (wchar_t*)hAlloc(sizeof(wchar_t)*temp.length + 1);
    strCpyW(temp.data, data);
    strCatW(temp.data, d);
    return temp;
}

Wstring& Wstring::operator+= (const wchar_t c) {
    PushBack(c);
    return *this;
}

Wstring& Wstring::operator+= (const wchar_t* c) {
    PushBack(c);
    return *this;
}

Wstring& Wstring::operator+= (const Wstring& s) {
    PushBack(s);
    return *this;
}

void Wstring::PushBack(const wchar_t c) {
    Resize(length + 1);
    data[length - 1] = c;
    data[length] = L'\0';
}

void Wstring::PushBack(const wchar_t* c) {
    Resize(length + strLenW(c));
    strCpyW(data, c);
}

void Wstring::PushBack(const Wstring& s) {
    Resize(length + strLenW(s.data));
    strCpyW(data, s.data);
}

void Wstring::Clear() {
    hFree(data);
    data = nullptr;
    length = 0;
}

void Wstring::Resize(const size_t n) {

    if (n == 0) {
        if (data != nullptr)
            delete data;
        data = nullptr;
        length = 0;
    }
    else if (n > length) {
        if (!IsEmpty()) {
            wchar_t* temp = (wchar_t*)hAlloc(sizeof(wchar_t)*(n + 3));
            strCpyW(temp, data);
            delete data;
            data = temp;
        }
        else {
            data = (wchar_t*)hAlloc(sizeof(wchar_t)*(n + 3));
            data[n] = '\0';
        }
        length = n;

    }
    else if (n < length) {

    }
}

bool Wstring::IsEmpty() const {
    return (data == nullptr) &&
        (length == 0);
}

bool Wstring::operator== (wchar_t* data) {
    
    wchar_t* str1 = this->data;
    wchar_t* str2 = data;

    if (strCmpW(str1, str2) == 0)
        return true;

  return false;
}

bool Wstring::operator== (const Wstring& s) {
   
    wchar_t* str1 = this->data;
    wchar_t* str2 = s.data;

    if (strCmpW(str1, str2) == 0)
        return true;

    return false;
}

bool Wstring::operator> (const Wstring& s) {
    
    wchar_t* str1 = this->data;
    wchar_t* str2 = s.data;

    if (strCmpW(str1, str2) > 0)
        return true;

    return false;
}

bool Wstring::operator< (const Wstring& s) {

    wchar_t* str1 = this->data;
    wchar_t* str2 = s.data;

    if (strCmpW(str1, str2) < 0)
        return true;

    return false;
}

wchar_t& Wstring::operator[] (const int i) {
    return *(data + i);
}

const wchar_t* Wstring::c_wstr() const {
    return data;
}

const char* Wstring::c_str() {
    size_t size_str_to_convert = length * sizeof(wchar_t);
    char* str_to_convert = (char*)hAlloc(size_str_to_convert);
    WideCharToMultiByte (CP_UTF8, 0, data, -1, str_to_convert, length, nullptr, nullptr);
    return str_to_convert;
}

size_t Wstring::size() const {
    return length;
}

size_t Wstring::Find(const Wstring& s, size_t pos) {
    return Find(s.data, pos);
}

size_t Wstring::Find(const wchar_t* data, size_t pos) {

    size_t found = npos;
    bool isFound = false;
    for (size_t i = pos; i < length && !isFound; i++) {
        if (data[0] == this->data[i]) {
            isFound = true;
            found = i;
            for (size_t j = 0; j < strLenW(data); j++) {
                if (data[j] != this->data[j + i]) {
                    isFound = false;
                    break;
                }
            }
        }
    }

    if (isFound) {
        return found;
    }
    return npos;
}

void Wstring::split(wchar_t* delim, Wstring* array_strings, size_t size_array_strings, size_t *count_splited) {
    size_t count = 0;
    wchar_t* p = strTokW(this->data, delim);
    while (p)
    {
        if (count >= size_array_strings) break;
        array_strings[count] = p;
        count++;
        p = strTokW(0, delim);
    }
    *count_splited = count;
}

Wstring Wstring::numeric_to_wstr(unsigned long num) {
    wchar_t converted_num[1024];
    return Wstring(itoaW(num, converted_num));
}

