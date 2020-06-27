#include "Wstring.h"
#include "Helpers.h"
#include "CustomAlloc.h"

static const size_t npos = (-1);

Wstring::Wstring() {
    length = 0;
    data = nullptr;
    tmp_str = nullptr;
}

Wstring::Wstring(const wchar_t* data) {
    length = strLenW(data);
    this->data = (wchar_t*)_malloc(sizeof(wchar_t)*this->length + sizeof(wchar_t));
    if ((this->data != nullptr) && (data != nullptr)) {
        strCpyW(this->data, data);
    }
}

Wstring::Wstring(const Wstring& s) {
    length = s.length;
    this->data = (wchar_t*)_malloc(sizeof(wchar_t)*length + sizeof(wchar_t));
    if ((this->data != nullptr) && (s.data != nullptr)) {
        strCpyW(this->data, s.data);
    }
}

Wstring::~Wstring() {
    _free(data);
}

const Wstring& Wstring::operator= (const wchar_t* data) {
    length = strLenW(data);
    if (this->data != nullptr) {
        _free(this->data);
        this->data = nullptr;
    }
    this->data = (wchar_t*)_malloc(sizeof(wchar_t)*length + sizeof(wchar_t));
    if ((this->data != nullptr) && (data != nullptr)) {
        strCpyW(this->data, data);
    }
    return *this;
}

const Wstring& Wstring::operator= (const Wstring& s) {
    length = s.length;
    if (this->data != nullptr) {
        _free(this->data);
        this->data = nullptr;
    }
    this->data = (wchar_t*)_malloc(sizeof(wchar_t)*length + sizeof(wchar_t));
    if ((this->data != nullptr) && (s.data != nullptr)) {
        strCpyW(this->data, s.data);
    }
    return *this;
}

Wstring Wstring::operator+ (const Wstring& s) {
    Wstring temp;
    temp.length = length + s.length;
    temp.data = (wchar_t*)_malloc(sizeof(wchar_t)*temp.length + sizeof(wchar_t));
    if ((temp.data != nullptr) && (data != nullptr) && (s.data != nullptr)) {
        strCpyW(temp.data, data);
        strCatW(temp.data, s.data);
    }
    return temp;
}

Wstring Wstring::operator+ (const wchar_t* d) {
    Wstring temp;
    temp.length = length + (int)strLenW(d);
    temp.data = (wchar_t*)_malloc(sizeof(wchar_t)*temp.length + sizeof(wchar_t));
    if ((temp.data != nullptr) && (data != nullptr) && (d != nullptr)) {
        strCpyW(temp.data, data);
        strCatW(temp.data, d);
    }
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
    _free(data);
    data = nullptr;
    length = 0;
}

void Wstring::Resize(const size_t n) {

    if (n == 0) {
        if (data != nullptr)
            _free(data);
        data = nullptr;
        length = 0;
    }
    else if (n > length) {
        if (!IsEmpty()) {
            wchar_t* temp = (wchar_t*)_malloc(sizeof(wchar_t)*(n + 3));
            if (temp != nullptr) {
                strCpyW(temp, data);
                _free(data);
            }
            data = temp;
        }
        else {
            data = (wchar_t*)_malloc(sizeof(wchar_t)*(n + 3));
            if (data != nullptr) {
                data[n] = L'\0';
            }
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

size_t Wstring::LenStr() const {
    return (length - 1);
}

size_t Wstring::Size() const {
    return (length*(sizeof(wchar_t)));
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

void Wstring::Split(wchar_t* delim, Wstring* array_strings, size_t size_array_strings, size_t *count_splited) {
    
    size_t count = 0;

    //Т.к. strtok меняет исходную строку, то будем работать с временной строкой
    if (tmp_str != nullptr) {
        _free(tmp_str);
        tmp_str = nullptr;
    }

    tmp_str = (wchar_t*)_malloc(sizeof(wchar_t)*length + sizeof(wchar_t));
    if ((tmp_str != nullptr) && (this->data != nullptr)) {
        strCpyW(tmp_str, this->data);
        wchar_t* p = strTokW(tmp_str, delim);
        while (p)
        {
            if (count >= size_array_strings) break;
            array_strings[count] = p;
            count++;
            p = strTokW(0, delim);
        }
    }
    *count_splited = count;
}

Wstring Wstring::NumericToWstr(unsigned long num) {
    wchar_t converted_num[1024];
    return Wstring(itoaW(num, converted_num));
}

