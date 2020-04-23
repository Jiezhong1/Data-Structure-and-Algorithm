typedef int Rank;//秩
#define ListNodePosi(T) ListNode<T>* //列表节点位置
template <typename T> struct ListNode{//列表节点模板类（双向链表）
 //成员
  T data; ListNodePosi(T) pred; ListNodePosi(T) succ;//数值，前驱，后继
//构造函数
  ListNode() {}//针对header和trailer的构造
  ListNode (T e, ListNodePosi(T) p, ListNodePosi(T) s)//形参
            :data(e),pred(p),succ(s){}//默认构造器,赋值
//操作接口
  ListNodePosi(T) insertAsPred(T const&e);//紧靠当前节点之前插入新节点
  ListNodePosi(T) insertAsSucc(T const&e);//紧靠当前节点之后插入新节点
};
//pred和succ是两个结构指针
//定义列表节点结构体