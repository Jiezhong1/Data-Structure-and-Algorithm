#pragma once
#include<algorithm>
using namespace std;
typedef int Rank;//秩
#define DEFAULT_CAPACITY 3//默认的初始容量
template <typename T> class Vector{//向量模板类
protected://pritected成员只有本类成员才能访问
    Rank _size; int _capacity; T* _elem;//规模、容量、数据区
    void copyFrom(T const *A,Rank lo,Rank hi);//复制数组区间A[lo,hi)
    void expand(); //空间不足时扩容
public:
//构造函数:创建时自动调用
    Vector(int c = DEFAULT_CAPACITY, int s=0,T v=0){//容量为c,规模为s,所有元素初始为v;
        _elem=new T[_capacity=c]; for(_size=0;_size<s;_elem[_size++]=v);
    }
    Vector ( T const* A, Rank n ) { copyFrom ( A, 0, n ); } //数组整体复制
//析构函数:对象消失前自动调用
    ~Vector(){
        delete []_elem;//释放内存
    }
//只读接口
     Rank size() const { return _size; } //规模
      bool empty() const { return !_size; } //判空
//可访问接口
     Rank find ( T const& e ) const { return find ( e, 0, _size ); } //无序向量整体查找
     Rank find ( T const& e, Rank lo, Rank hi ) const; //无序向量区间查找
     Rank search ( T const& e, Rank lo, Rank hi ) const; //有序向量区间查找
     Rank search ( T const& e ) const //有序向量整体查找
   { return ( 0 >= _size ) ? -1 : search ( e, 0, _size ); }
     T & operator[] ( Rank r ); //重载下标操作符，可以类似于数组形式引用各元素
     Vector<T> & operator= ( Vector<T> const& ); //重载赋值操作符，以便直接克隆向量
     Rank insert ( Rank r, T const& e ); //插入元素
     Rank insert ( T const& e ) { return insert ( _size, e ); } //默认作为末元素插入
     T remove ( Rank r ); //删除秩为r的元素
     int remove ( Rank lo, Rank hi ); //删除秩在区间[lo, hi)之内的元素
     int deduplicate(); //无序去重
     int uniquify(); //有序去重
     void sort() { sort ( 0, _size ); } //整体排序
     void sort ( Rank lo, Rank hi ); //对[lo, hi)排序
};//vector



template <typename T> 
void Vector<T>::copyFrom(T const *A, Rank lo,Rank hi){//T *cons表示指针指向不能改变，T为任意数据类型
    _elem=new T[_capacity=2*(hi-lo)];
    _size=0;
    while(lo<hi)//A[lo,hi]内的元素逐一
      _elem[_size++]=A[lo++];
}

template<typename T>//倍增扩容,效率更高
void Vector<T>::expand(){
    if(_size<_capacity)  return;
    _capacity = max(_capacity, DEFAULT_CAPACITY);
    T* oldElem=_elem; _elem=new T[_capacity<<=1];
    for(int i=0;i<_size;i++) _elem[i]=oldElem[i];
     delete [] oldElem;
}
 

template <typename T> 
T & Vector<T>::operator[] ( Rank r ) //重载下标操作符
{ return _elem[r]; } // assert: 0 <= r < _size

template <typename T>
Rank Vector<T>::insert(Rank r,T const & e){
    expand();
    for(int i=_size;i>r;i--) _elem[i]=_elem[i-1];//自后向前
     _elem[r]=e; 
     _size++;
     return r;
}

template <typename T> 
int Vector<T>::remove ( Rank lo, Rank hi ) { //删除区间[lo, hi)
   if ( lo == hi ) return 0; //出于效率考虑，单独处理退化情况，比如remove(0, 0)
   while ( hi < _size ) _elem[lo++] = _elem[hi++]; //[hi, _size)顺次前移hi - lo个单元
   _size = lo; //更新规模，直接丢弃尾部[lo, _size = hi)区间
   return hi - lo; //返回被删除元素的数目
}

template <typename T> 
T Vector<T>::remove ( Rank r ) { //删除向量中秩为r的元素，0 <= r < size
   T e = _elem[r]; //备份被删除元素
   remove ( r, r + 1 ); //调用区间删除算法，等效于对区间[r, r + 1)的删除
   return e; //返回被删除元素
}

template <typename T> //无序向量的顺序查找：返回最后一个元素e的位置；失败时，返回lo - 1
Rank Vector<T>::find ( T const& e, Rank lo, Rank hi ) const { //assert: 0 <= lo < hi <= _size
   while ( ( lo < hi-- ) && ( e != _elem[hi] ) ); //从后向前，顺序查找
   return hi; //若hi < lo，则意味着失败；否则hi即命中元素的秩
}

template <typename T> //在有序向量的区间[lo, hi)内，确定不大于e的最后一个节点的秩
Rank Vector<T>::search ( T const& e, Rank lo, Rank hi ) const { //assert: 0 <= lo < hi <= _size
   return ( rand() % 2 ) ? //按各50%的概率随机使用二分查找或Fibonacci查找
          binSearch ( _elem, e, lo, hi ) : fibSearch ( _elem, e, lo, hi );
}

template <typename T> 
int Vector<T>::deduplicate() { //删除无序向量中重复元素（高效版）
   int oldSize = _size; //记录原规模
   Rank i = 1; //从_elem[1]开始
   while ( i < _size ) //自前向后逐一考查各元素_elem[i]
      ( find ( _elem[i], 0, i ) < 0 ) ? //在其前缀中寻找与之雷同者（至多一个）
      i++ : remove ( i ); //若无雷同则继续考查其后继，否则删除雷同者
   return oldSize - _size; //向量规模变化量，即被删除元素总数
}

template <typename T> 
int Vector<T>::uniquify() { //有序向量重复元素剔除算法（高效版）
   Rank i = 0, j = 0; //各对互异“相邻”元素的秩
   while ( ++j < _size ) //逐一扫描，直至末元素
      if ( _elem[i] != _elem[j] ) //跳过雷同者
         _elem[++i] = _elem[j]; //发现不同元素时，向前移至紧邻于前者右侧
   _size = ++i; //直接截除尾部多余元素
   return j - i; //向量规模变化量，即被删除元素总数
}

template<typename T> void Vector<T>::sort(Rank lo,Rank hi){//非常巧妙的起泡排序
    for( Rank last = --hi; lo < hi; hi = last )
      for( Rank i = last = lo; i < hi; i++ )
         if( _elem[i] > _elem[i + 1] ) //若逆序，则
            swap( _elem[ last = i ], _elem[ i + 1 ] ); //经交换使局部有序
}