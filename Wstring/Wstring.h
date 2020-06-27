#pragma once

class Wstring {

private:
    wchar_t* data;
    size_t length;

public:

    Wstring();
    Wstring(const wchar_t*);
    Wstring(const Wstring&);
    ~Wstring();

    const Wstring& operator= (const wchar_t*);
    const Wstring& operator= (const Wstring&);
    Wstring& operator+= (const wchar_t);
    Wstring& operator+= (const wchar_t*);
    Wstring& operator+= (const Wstring&);
    Wstring operator+ (const wchar_t*);
    Wstring operator+ (const Wstring&);
    bool operator== (wchar_t*);
    bool operator== (const Wstring&);
    bool operator> (const Wstring&);
    bool operator< (const Wstring&);
    wchar_t& operator[] (const int);


    void PushBack(const wchar_t c);
    void PushBack(const wchar_t* c);
    void PushBack(const Wstring& s);
    void split(wchar_t* delim, Wstring* array_strings, size_t size_array_strings, size_t* count_splited);
    Wstring NumericToWstr(unsigned long num);
    
    void Resize(const size_t n);
    void Clear();
    bool IsEmpty() const;

    size_t lenght() const;
    size_t size() const;

    const wchar_t* c_wstr() const;

    size_t Find(const Wstring&, size_t npos = 0);
    size_t Find(const wchar_t*, size_t npos = 0);

};