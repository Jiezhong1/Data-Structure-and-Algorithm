#include<algorithm>
#include "listNode.h"
using namespace std;
template <typename T>class List{//列表模板类
private:
     int _size; ListNodePosi(T) header; ListNodePosi(T) trailer;//规模，头哨兵，尾哨兵
protected:
     void init();//列表创建时的初始化
     int clear(); //清除所有节点
     void copyNodes ( ListNodePosi(T), int ); //复制列表中自位置p起的n项
    //要用这个copy前提是必须要有一个列表
public:
//构造函数
    List(){init();}//默认
//析构函数
    ~List();//释放所有节点
//只读访问接口:
     bool empty() const { return _size <= 0; } //判空
     T& operator[] ( Rank r ) const; //重载，支持循秩访问（效率低）
     ListNodePosi(T) find ( T const& e, int n, ListNodePosi(T) p ) const; //无序区间查找
     ListNodePosi(T) first() ;//首节点位置
     ListNodePosi(T) last() const { return trailer->pred; } //末节点位置
    
//可写接口
     ListNodePosi(T) insertA ( ListNodePosi(T) p, T const& e ); //将e当作p的后继插入（After）
     ListNodePosi(T) insertB ( ListNodePosi(T) p, T const& e ); //将e当作p的前驱插入（Before）
     ListNodePosi(T) insertAsFirst ( T const& e ); //将e当作首节点插入
     ListNodePosi(T) insertAsLast ( T const& e ); //将e当作末节点插入
     T remove ( ListNodePosi(T) p ); //删除合法位置p处的节点,返回被删除节点
//遍历
    void traverse ( void (* ) ( T& ) ); //遍历，依次实施visit操作（函数指针，只读或局部性修改）
};

template <typename T> ListNodePosi(T) List<T>::first(){
   return header->succ;
}
//header和trailer是两个结构指针,可见列表部分还是空的
template <typename T> void List<T>::init() { //列表初始化，在创建列表对象时统一调用
   header = new ListNode<T>; //创建头哨兵节点
   trailer = new ListNode<T>; //创建尾哨兵节点
   header->succ = trailer; header->pred = NULL;
   trailer->pred = header; trailer->succ = NULL;
   _size = 0; //记录规模
}


template <typename T> //重载下标操作符，以通过秩直接访问列表节点（虽方便，效率低，需慎用）
T& List<T>::operator[] ( Rank r ) const { //assert: 0 <= r < size
   ListNodePosi(T) p = first(); //从首节点出发
   while ( 0 < r-- ) p = p->succ; //顺数第r个节点即是
   return p->data; //目标节点，返回其中所存元素
}

template <typename T> //在无序列表内节点p（可能是trailer）的n个（真）前驱中，找到等于e的最后者
ListNodePosi(T) List<T>::find ( T const& e, int n, ListNodePosi(T) p ) const {
   while ( 0 < n-- ) //（0 <= n <= rank(p) < _size）对于p的最近的n个前驱，从右向左
      if ( e == ( p = p->pred )->data ) return p; //逐个比对，直至命中或范围越界
   return NULL; //p越出左边界意味着区间内不含e，查找失败
} //失败时，返回NULL

template <typename T> ListNodePosi(T) List<T>::insertAsFirst ( T const& e )
{  _size++; return header->insertAsSucc ( e );  } //e当作首节点插入

template <typename T> ListNodePosi(T) List<T>::insertAsLast ( T const& e )
{  _size++; return trailer->insertAsPred ( e );  } //e当作末节点插入

template <typename T> ListNodePosi(T) List<T>::insertA ( ListNodePosi(T) p, T const& e )
{  _size++; return p->insertAsSucc ( e );  } //e当作p的后继插入（After）

template <typename T> ListNodePosi(T) List<T>::insertB ( ListNodePosi(T) p, T const& e )
{  _size++; return p->insertAsPred ( e );  } //e当作p的前驱插入（Before）

template <typename T> //将e紧随当前节点之后插入于当前节点所属列表（设有哨兵尾节点trailer）
ListNodePosi(T) ListNode<T>::insertAsSucc ( T const& e ) {
   ListNodePosi(T) x = new ListNode ( e, this, succ ); //创建新节点
   succ->pred = x; succ = x; //设置逆向链接
   return x; //返回新节点的位置
}

template <typename T> //将e紧靠当前节点之前插入于当前节点所属列表（设有哨兵头节点header）
ListNodePosi(T) ListNode<T>::insertAsPred ( T const& e ) {
   ListNodePosi(T) x = new ListNode ( e, pred, this ); //创建新节点
   pred->succ = x; pred = x; //设置正向链接
   return x; //返回新节点的位置
}

template <typename T> List<T>::~List() //列表析构器
{ clear(); delete header; delete trailer; } //清空列表，释放头、尾哨兵节点

template <typename T> T List<T>::remove ( ListNodePosi(T) p ) { //删除合法节点p，返回其数值
   T e = p->data; //备份待删除节点的数值（假定T类型可直接赋值）
   p->pred->succ = p->succ; p->succ->pred = p->pred; //后继、前驱
   delete p; _size--; //释放节点，更新规模
   return e; //返回备份的数值
}

template <typename T> int List<T>::clear() { //清空列表
   int oldSize = _size;
   while ( 0 < _size ) remove ( header->succ ); //反复删除首节点，直至列表变空
   return oldSize;
}

template <typename T> //列表内部方法：复制列表中自位置p起的n项
void List<T>::copyNodes ( ListNodePosi(T) p, int n ) { //p合法，且至少有n-1个真后继节点
   init(); //创建头、尾哨兵节点并做初始化
   while ( n-- ) { insertAsLast ( p->data ); p = p->succ; } //将起自p的n项依次作为末节点插入
}

template <typename T> void List<T>::traverse ( void ( *visit ) ( T& ) ) //借助函数指针机制遍历
{  for ( ListNodePosi(T) p = header->succ; p != trailer; p = p->succ ) visit ( p->data );  }