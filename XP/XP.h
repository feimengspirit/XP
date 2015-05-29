//
//  XP.h
//  XP
//
//  Created by mengfei.mf on 5/28/15.
//  Copyright (c) 2015 mengfei.mf. All rights reserved.
//

#ifndef XP_XP_h
#define XP_XP_h

#include <functional>

#define SAFE_DELETE(p)  \
do {                    \
    if (p) delete p;    \
} while (0)

namespace feimengspirit
{
template <typename T>
class XP
{
public:
    typedef T ElementType;
    typedef long CountType;
    typedef std::function<void(T*)> Deletor;

public:
    XP(ElementType *other, Deletor d = [](T *p) { if (p) delete p; });
    ~XP();

    template <typename Y>
    XP(Y *other, Deletor d = [](T *p) { if (p) delete p; });
    
    XP(const XP<T>& other);
    
    template <typename Y>
    XP(const XP<Y>& other);
    
    XP<T>& operator=(const XP<T>& other);
    
    template <typename Y>
    XP<T>& operator=(const XP<Y>& other);
    
    ElementType* operator->();
    
private:
    void release();
    
public:
    CountType * _count;
    ElementType *_pointer;
    Deletor _deletor;
};
  
template <typename T>
XP<T>::XP(ElementType *other, Deletor d)
: _count(new CountType(0)), _pointer(nullptr), _deletor(d)
{
}

template <typename T>
XP<T>::~XP()
{
    this->release();
}
    
template <typename T>
void XP<T>::release()
{
    if (0 == --*_count) {
        _deletor(_pointer);
        SAFE_DELETE(_count);
    }
}

template <typename T>
template <typename Y>
XP<T>::XP(Y *other, Deletor d)
: _count(new CountType(1)), _pointer(other), _deletor(d)
{
}

template <typename T>
XP<T>::XP(const XP<T>& other)
:_count(other._count), _pointer(other._pointer), _deletor(other._deletor)
{
    ++*_count;
}

template <typename T>
template <typename Y>
XP<T>::XP(const XP<Y>& other)
: _count(other._count), _pointer(static_cast<ElementType*>(other._pointer)),
    _deletor(other._deletor)
{
}
    
template <typename T>
XP<T>& XP<T>::operator=(const XP<T>& other)
{
    this->release();
    this->_pointer = other._pointer;
    this->_count = other._count;
    this->_deletor = other._deletor;
    ++*_count;
    return *this;
}
    
template <typename T>
template <typename Y>
XP<T>& XP<T>::operator=(const XP<Y>& other)
{
    this->release();
    this->_pointer = static_cast<ElementType*>(other._pointer);
    this->_count = other._count;
    this->_deletor = other._deletor;
    ++*_count;
    return *this;
}

template <typename T>
typename XP<T>::ElementType* XP<T>::operator->()
{
    return _pointer;
}
}

#endif
