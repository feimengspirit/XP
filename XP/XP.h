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
#include <mutex>

#define SAFE_DELETE(p)  \
do {                    \
    if (p) delete p;    \
    p = nullptr;        \
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

    XP(const XP<T>& other);
    
    template <typename Y>
    XP(const XP<Y>& other);
    
    XP<T>& operator=(const XP<T>& other);
    
    template <typename Y>
    XP<T>& operator=(const XP<Y>& other);
    
    ElementType* operator->();
    ElementType operator*();
    const ElementType& operator*() const;
    
private:
    void release();
    void increment();
    
public:
    CountType *_count;
    ElementType *_pointer;
    Deletor _deletor;
    std::mutex *_mutex;
};
  
template <typename T>
XP<T>::XP(ElementType *other, Deletor d)
: _count(new CountType(1)),
  _pointer(other),
  _deletor(d),
  _mutex(new std::mutex)
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
    {
    std::lock_guard<std::mutex> lck(*_mutex);
    --*_count;
    }
    if (0 ==*_count) {
        _deletor(_pointer);
        _pointer = nullptr;
        SAFE_DELETE(_count);
        SAFE_DELETE(_mutex);
    }
}
    
template <typename T>
void XP<T>::increment()
{
    std::lock_guard<std::mutex> lck(*_mutex);
    ++*_count;
}

template <typename T>
XP<T>::XP(const XP<T>& other)
: _count(other._count), _pointer(static_cast<ElementType*>(other._pointer)),
  _deletor(other._deletor),
  _mutex(other._mutex)
{
    increment();
}

template <typename T>
template <typename Y>
XP<T>::XP(const XP<Y>& other)
: _count(other._count), _pointer(static_cast<ElementType*>(other._pointer)),
  _deletor([&](T *p){other._deletor(static_cast<Y*>(p));}),
  _mutex(other._mutex)
{
    increment();
}
    
template <typename T>
XP<T>& XP<T>::operator=(const XP<T>& other)
{
    return this->operator=<T>(other);
}

template <typename T>
template <typename Y>
XP<T>& XP<T>::operator=(const XP<Y>& other)
{
    this->release();
    this->_pointer = static_cast<ElementType*>(other._pointer);
    this->_count = other._count;
    this->_deletor = [&](T *p){ other._deletor(static_cast<Y*>(p)); };
    this->_mutex = other._mutex;
    increment();
    return *this;
}

template <typename T>
typename XP<T>::ElementType* XP<T>::operator->()
{
    return _pointer;
}

template <typename T>
typename XP<T>::ElementType XP<T>::operator*()
{
    return *_pointer;
}

template <typename T>
const typename XP<T>::ElementType& XP<T>::operator*() const
{
    return *_pointer;
}
}


#endif
