#pragma once
#include "Vector.h"//以向量为基础,派生出栈模板类
template<typename T>class Stack:public Vector<T>{//将向量的首/末端作为栈底/顶
    public://size(),empty()及其开放接口,均可直接沿用
        void push(T const& e){this->insert (e);}//入栈:等效为将新元素作为向量的末元素插入
        T pop(){return this->remove(this->size()-1);}//出栈:等效于删除向量的末元素 o(1)复杂度
        T& top(){return (*this)[this->size()-1];}//取顶:直接返回向量的末元素
};
//C++中,若class的成员函数在其它地方倍调用了,需要在前面加一个this