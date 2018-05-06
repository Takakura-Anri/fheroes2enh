#pragma once

#include "types.h"
#include <vector>
#include "ByteVectorReader.h"
#include <map>
#include <list>

class ByteVectorWriter
{
    std::vector<u8> _data;
    bool _isBigEndian;

    void put8(u8 val)
    {
        _data.push_back(val);
    }

public:
    explicit ByteVectorWriter(int sz = 0);
    void putLE16(u16 v);
    void putBE16(u16 v);
    void putLE32(uint32_t v);
    void putBE32(uint32_t v);

    const std::vector<u8> data() const
    {
        return _data;
    }

    void SetBigEndian(bool value);

    ByteVectorWriter &operator<<(const char &);

    void put32(uint32_t sz);
    void put16(u16 u16);


    ByteVectorWriter &operator<<(const bool &);
    ByteVectorWriter &operator<<(const u8 &v);
    ByteVectorWriter &operator<<(const u16 &v);
    ByteVectorWriter &operator<<(const u32 &v);
    ByteVectorWriter &operator<<(const s32 &);
    ByteVectorWriter &operator<<(const Size &);

    ByteVectorWriter &operator<<(const std::string &);


    template<class Type1, class Type2>
    ByteVectorWriter &operator>>(std::pair<Type1, Type2> &p)
    {
        return *this >> p.first >> p.second;
    }

    template<class Type>
    ByteVectorWriter &operator>>(std::vector<Type> &v)
    {
        const uint32_t size = get32();
        v.resize(size);
        for (auto it = v.begin(); it != v.end(); ++it)
            *this >> *it;
        return *this;
    }

    template<class Type>
    ByteVectorWriter &operator>>(std::list <Type> &v)
    {
        const uint32_t size = get32();
        v.resize(size);
        for (auto it = v.begin(); it != v.end(); ++it)
            *this >> *it;
        return *this;
    }

    template<class Type1, class Type2>
    ByteVectorWriter &operator>>(std::map<Type1, Type2> &v)
    {
        const uint32_t size = get32();
        v.clear();
        for (uint32_t ii = 0; ii < size; ++ii)
        {
            pair<Type1, Type2> pr;
            *this >> pr;
            v.insert(pr);
        }
        return *this;
    }

    template<class Type1, class Type2>
    ByteVectorWriter &operator<<(const std::pair<Type1, Type2> &p)
    {
        return *this << p.first << p.second;
    }

    template<class Type>
    ByteVectorWriter &operator<<(const std::vector<Type> &v)
    {
        put32(static_cast<uint32_t>(v.size()));
        for (auto it : v)
            *this << it;
        return *this;
    }

    template<class Type>
    ByteVectorWriter &operator<<(const std::list <Type> &v)
    {
        put32(static_cast<uint32_t>(v.size()));
        for (auto it : v)
            *this << it;
        return *this;
    }

    template<class Type1, class Type2>
    ByteVectorWriter &operator<<(const std::map<Type1, Type2> &v)
    {
        put32(static_cast<uint32_t>(v.size()));
        for (typename map<Type1, Type2>::const_iterator
            it = v.begin(); it != v.end(); ++it)
            *this << *it;
        return *this;
    }


    void clear()
    {
        _data.clear();
    };
};

