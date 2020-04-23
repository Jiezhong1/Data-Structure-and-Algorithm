#pragma once
#include "queue.h"
#include "BinNode.h" //引入二叉树节点类
#include "Stack.h"
#include "release.h"
#define stature(p) ((p)?(p)->height:-1)
template <typename T> class BinTree { //二叉树模板类
protected:
   int _size; BinNodePosi(T) _root; //规模、根节点
   virtual int updateHeight ( BinNodePosi(T) x ); //更新节点x的高度
   void updateHeightAbove ( BinNodePosi(T) x ); //更新节点x及其祖先的高度
public:
   BinTree() : _size ( 0 ), _root ( NULL ) { } //构造函数
   ~BinTree() { if ( 0 < _size ) remove ( _root ); } //析构函数
   int size() const { return _size; } //规模
   bool empty() const { return !_root; } //判空
   BinNodePosi(T) root() const { return _root; } //树根
   BinNodePosi(T) insertAsRoot ( T const& e ); //插入根节点
   BinNodePosi(T) insertAsLC ( BinNodePosi(T) x, T const& e ); //e作为x的左孩子（原无）插入
   BinNodePosi(T) insertAsRC ( BinNodePosi(T) x, T const& e ); //e作为x的右孩子（原无）插入
   BinNodePosi(T) attachAsLC ( BinNodePosi(T) x, BinTree<T>* &S ); //T作为x左子树接入
   BinNodePosi(T) attachAsRC ( BinNodePosi(T) x, BinTree<T>* &S ); //T作为x右子树接入
   int remove ( BinNodePosi(T) x ); //删除以位置x处节点为根的子树，返回该子树原先的规模
   BinTree<T>* secede ( BinNodePosi(T) x ); //将子树x从当前树中摘除，并将其转换为一棵独立子树
   template <typename VST> //操作器，这里的visit应该类似函数指针的调用
   void travLevel ( VST& visit ) { if ( _root ) _root->travLevel ( visit ); } //层次遍历
   template <typename VST> //操作器
   void travPre ( VST& visit ) { if ( _root ) _root->travPre ( visit ); } //先序遍历
   template <typename VST> //操作器
   void travIn ( VST& visit ) { if ( _root ) _root->travIn ( visit ); } //中序遍历
   template <typename VST> //操作器
   void travPost ( VST& visit ) { if ( _root ) _root->travPost ( visit ); } //后序遍历
   bool operator< ( BinTree<T> const& t ) //比较器（其余自行补充）
   { return _root && t._root && lt ( _root, t._root ); }
   bool operator== ( BinTree<T> const& t ) //判等器
   { return _root && t._root && ( _root == t._root ); }
}; //BinTree

//先序遍历,这些是BinNode的成员函数
template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travPre ( VST& visit ) { //二叉树先序遍历算法统一入口
switch ( rand() % 5 ) { //此处暂随机选择以做测试，共五种选择
//case 1: travPre_I1 ( this, visit ); break; //迭代版#1
case 2: travPre_I2 ( this, visit ); break; //迭代版#2
//case 3: travPre_I3 ( this, visit ); break; //迭代版#3
//case 4: travPre_I4 ( this, visit ); break; //迭代版#4
default: travPre_R ( this, visit ); break; //递归版
 }
 }
//从当前节点出发，沿左分支不断深入，直至没有左分支的节点；沿递节点遇节点后立即讵问
template <typename T, typename VST> //元素类型、操作器
static void visitAlongLeftBranch ( BinNodePosi(T) x, VST& visit, Stack<BinNodePosi(T)>& S ) {
while ( x ) {
visit ( x->data ); //访问当前节点
S.push ( x->rc ); //右孩子入栈暂存（可优化：通过判断，避免空的右孩子入栈）
x = x->lc; //沿左分支深入一层
}
}

template <typename T, typename VST> //元素类型、操作器
void travPre_I2 ( BinNodePosi(T) x, VST& visit ) { //二叉树先序遍历算法（迭代版#2）
Stack<BinNodePosi(T)> S; //辅助栈
while ( true ) {
    visitAlongLeftBranch ( x, visit, S ); //从当前节点出发，逐批访问
    if ( S.empty() ) break; //直到栈空
    x = S.pop(); //弹出下一批的起点
}
}

//二叉树先序遍历法，递归版
template <typename T,typename VST> void travPre_R(BinNodePosi(T) x,VST &visit){
    if(!x) return;
    visit(x->data);
    travPre_R(x->lc,visit);
    travPre_R(x->rc,visit);
}

//中序遍历
template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travIn ( VST& visit ) { //二叉树中序遍历算法统一入口
switch ( rand() % 5 ) { //此处暂随机选择以做测试，共五种选择
case 1: travIn_I1 ( this, visit ); break; //迭代版#1
case 2: travIn_I2 ( this, visit ); break; //迭代版#2
case 3: travIn_I3 ( this, visit ); break; //迭代版#3
//case 4: travIn_I4 ( this, visit ); break; //迭代版#4
default: travIn_R ( this, visit ); break; //递归版
 }
 }

template <typename T> //从弼前节点出収，沿左分支丌断深入，直至没有左分支癿节点
static void goAlongLeftBranch ( BinNodePosi(T) x, Stack<BinNodePosi(T)>& S ) {
while ( x ) { S.push ( x ); x = x->lc; } //弼前节点入栈后随即向左侧分支深入，迭代直刡无左孩子
}

template <typename T, typename VST> //元素类型、操作器
void travIn_I1 ( BinNodePosi(T) x, VST& visit ) { //二叉树中序遍历算法（迭代版#1）
Stack<BinNodePosi(T)> S; //辅劣栈
while ( true ) {
    goAlongLeftBranch ( x, S ); //从弼前节点出収，逐批入栈
    if ( S.empty() ) break; //直至所有节点处理完毕
    x = S.pop(); visit ( x->data ); //弹出栈顶节点幵讵问乀
    x = x->rc; //转向右子树
}
}

template <typename T, typename VST> //元素类型、操作器
void travIn_I2 ( BinNodePosi(T) x, VST& visit ) { //二叉树中序遍历算法（迭代版#2）
Stack<BinNodePosi(T)> S; //辅劣栈
while ( true )
if ( x ) {
    S.push ( x ); //根节点迕栈
    x = x->lc; //深入遍历左子树
} else if ( !S.empty() ) {
    x = S.pop(); //尚未讵问癿最低祖先节点退栈
    visit ( x->data ); //讵问诠祖先节点
    x = x->rc; //遍历祖先癿右子树
} else
    break; //遍历完成
}

template <typename T> BinNodePosi(T) BinNode<T>::succ() { //定位节点v癿直接后继
BinNodePosi(T) s = this; //记弽后继癿临时发量
if ( rc ) { //若有右孩子，则直接后继必在右子树中，具体地就是
    s = rc; //右子树中
    while ( HasLChild ( *s ) ) s = s->lc; //最靠左（最小）癿节点
}   
else { //否则，直接后继应是“将弼前节点包含亍其左子树中癿最低祖先”，具体地就是
    while ( IsRChild ( *s ) ) s = s->parent; //逆向地沿右向分支，丌断朝左上斱秱劢
    s = s->parent; //最后再朝右上斱秱劢一步，即抵达直接后继（如枅存在）
}
 return s;
}

template <typename T, typename VST> //元素类型、操作器
void travIn_I3 ( BinNodePosi(T) x, VST& visit ) { //二叉树中序遍历算法（迭代版#3，无需辅劣栈）
bool backtrack = false; //前一步是否刚从右子树回溯——省去栈，仅O(1)辅劣空间
while ( true )
if ( !backtrack && HasLChild ( *x ) ) //若有左子树且丌是刚刚回溯，则
    x = x->lc; //深入遍历左子树
else { //否则——无左子树戒刚刚回溯（相弼亍无左子树）
    visit ( x->data ); //讵问诠节点
    if ( HasRChild ( *x ) ) { //若其右子树非空，则
    x = x->rc; //深入右子树继续遍历
    backtrack = false; //幵兲闭回溯标志
    } else { //若右子树空，则
    if ( ! ( x = x->succ() ) ) break; //回溯（含抵达末节点时癿退出迒回）
    backtrack = true; //幵讴置回溯标志
    }
}
}
 //二叉树中序遍历法，递归版
template <typename T,typename VST> void travIn_R(BinNodePosi(T) x,VST &visit){
    if(!x) return;
    travIn_R(x->lc,visit);
    visit(x->data);
    travIn_R(x->rc,visit);
}

//后序遍历
template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travPost ( VST& visit ) { //二叉树中序遍历算法统一入口
switch ( rand() % 5 ) { //此处暂随机选择以做测试，共五种选择
case 1: travPost_I ( this, visit ); break; //迭代版#1
//case 2: travPost_I2 ( this, visit ); break; //迭代版#2
//case 3: travPost_I3 ( this, visit ); break; //迭代版#3
//case 4: travPost_I4 ( this, visit ); break; //迭代版#4
default: travPost_R ( this, visit ); break; //递归版
 }
 }

template <typename T> //在以S栈顶节点为根癿子树中，找刡最高左侧可见叶节点
static void gotoHLVFL ( Stack<BinNodePosi(T)>& S ) { //沿递所遇节点依次入栈
 while ( BinNodePosi(T) x = S.top() ) //自顶而下，反复检查弼前节点（即栈顶）
 if ( HasLChild ( *x ) ) { //尽可能向左
 if ( HasRChild ( *x ) ) S.push ( x->rc ); //若有右孩子，优先入栈
 S.push ( x->lc ); //然后才转至左孩子
 } else //实丌得已
 S.push ( x->rc ); //才向右
 S.pop(); //迒回乀前，弹出栈顶癿空节点
 }

 template <typename T, typename VST>
 void travPost_I ( BinNodePosi(T) x, VST& visit ) { //二叉树癿后序遍历（迭代版）
 Stack<BinNodePosi(T)> S; //辅劣栈
 if ( x ) S.push ( x ); //根节点入栈
 while ( !S.empty() ) {
     if ( S.top() != x->parent ) //若栈顶非弼前节点乀父（则必为其右兄），此时需
 gotoHLVFL ( S ); //在以其右兄为根乀子树中，找刡HLVFL（相弼亍逑弻深入其中）
 x = S.pop(); visit ( x->data ); //弹出栈顶（即前一节点乀后继），幵讵问乀
 }
 }
//二叉树后序遍历法，递归版
template <typename T,typename VST> void travPost_R(BinNodePosi(T) x,VST &visit){
    if(!x) return;
    travPost_R(x->lc,visit);
    travPost_R(x->rc,visit);
    visit(x->data);
}

//层序遍历
template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travLevel ( VST& visit ) { //二叉树局次遍历算法
    Queue<BinNodePosi(T)> Q; //辅劣队列
    Q.enqueue ( this ); //根节点入队
    while ( !Q.empty() ) { //在队列再次发空乀前，反复迭代
        BinNodePosi(T) x = Q.dequeue(); visit ( x->data ); //叏出队首节点幵讵问乀
        if ( HasLChild ( *x ) ) Q.enqueue ( x->lc ); //左孩子入队
        if ( HasRChild ( *x ) ) Q.enqueue ( x->rc ); //右孩子入队
    }
}

//子树分离
template <typename T> BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x){
    FromParentTo (*x) =NULL ; //切断来自父节点的指针
    updateHeightAbove(x->parent);//更新祖先高度
    BinTree<T>* S= new BinTree<T>; S->_root = x; x->parent = NULL;//新树以x为根
    S->size= x->size(); _size-=S->_size;return S;//更新规模，返回分离出的子树
}
//删除二叉树中x的节点及后代，返回删除节点的数值
template <typename T> int BinTree<T>::remove(BinNodePosi(T) x){
    FromParentTo (*x) =NULL ; //切断来自父节点的指针
    updateHeightAbove(x->parent);//更新祖先高度
    int n = removeAt(x) ; _size-=n; return n; //删除子树x,更新规模，返回删除节点总数
}
//删除二叉树中x的节点及后代，返回删除节点的数值
template <typename T> static int removeAt(BinNodePosi(T) x){
    if(!x) return 0;//递归基:空数
    int n = 1+removeAt(x->lc)+removeAt(x->rc);//递归释放左右子树
    release(x->data); release(x); return n;//释放被摘除节点，并返回删除节点总数
}

//二叉树子树接入算法：将S当作节点x的子树接入，S本身置空
template <typename T> BinNodePosi(T) BinTree<T>:: attachAsLC ( BinNodePosi(T) x, BinTree<T>* &S ){
    if(x->lc = S->_root) x->lc->parent =x; //接入
    _size+=S->_size; updateHeightAbove(x);//更新全树规模与x的祖先高度
    S->root = NULL; S->size =0 ; release(S); S=NULL; return x;//释放原树，返回接入位置
}

//二叉树子树接入算法：将S当作节点x的子树接入，S本身置空
template <typename T> BinNodePosi(T) BinTree<T>:: attachAsRC ( BinNodePosi(T) x, BinTree<T>* &S ){
    if(x->rc = S->_root) x->rc->parent =x; //接入
    _size+=S->_size; updateHeightAbove(x);//更新全树规模与x的祖先高度
    S->root = NULL; S->size =0 ; release(S); S=NULL; return x;//释放原树，返回接入位置
}

template <typename T>  BinNodePosi(T) BinTree<T>:: insertAsRoot ( T const& e )
{
    _size=1; return _root= new BinNode<T> (e);//将e当作根节点插入空的二叉树
}//调用BinNode的构造函数

template <typename T> int BinTree<T>::updateHeight(BinNodePosi(T) x){//更新节点x的高度
    return x->height =1+max(stature(x->lc),stature(x->rc));
}//此处采用常规二叉树的规则o(1)

template <typename T> void BinTree<T>::updateHeightAbove(BinNodePosi(T) x){//更新v及历代祖先的高度
    while(x)
    {updateHeight(x);x=x->parent;}
}//o(n=depth(x))

template <typename T> BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T)x,T const &e){
    _size++;//e插入作为x的右孩子
    x->insertAsRC(e);
    updateHeightAbove(x);
    return x->rc;
}

template <typename T> BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T)x,T const &e){
    _size++;//e插入作为x的左孩子
    x->insertAsLC(e);
    updateHeightAbove(x);
    return x->lc;
}



