#include<iostream>
#include "BinNode.h"
#include "BinTree.h"
using namespace std;
template <typename T> void visit(T t);
template <typename T>void CreateBinTree(BinNode<T> * TreeNode);
/*建立一个二叉树并先序输出
               i
         d           l
      c     h      k    n
   a      f       j    m   p

*/
int main()
{
   BinTree<char> Tree;
   BinNode<char>* _root;
   _root = Tree.insertAsRoot('i');
   CreateBinTree(_root);
   cout<<"先序遍历"<<endl;
   Tree.travPre(visit<char>);
   cout<<"中序遍历"<<endl;
   Tree.travIn(visit<char>);
   cout<<"后序遍历"<<endl;
   Tree.travPost(visit<char>);
   cout<<"层序遍历"<<endl;
   Tree.travLevel(visit<char>);
   system("pause");
} 

template <typename T>void CreateBinTree(BinNode<T> * TreeNode){//先序创建二叉树
   T _data;
   cout<<"请输入"<<TreeNode->data<<"的左孩子节点："<<endl;
   cin>>_data;
   if(_data == '0') TreeNode->lc = NULL;    //'0'代表终端节点值
   else {
      TreeNode->lc = new BinNode<T>(_data);  
      CreateBinTree(TreeNode->lc);
   }
   cout<<"请输入"<<TreeNode->data<<"的右孩子节点："<<endl;
   cin>>_data;
   if(_data == '0') TreeNode->rc = NULL;    //'0'代表终端节点值
   else{
      TreeNode->rc = new BinNode<T>(_data);  
      CreateBinTree(TreeNode->rc);
   }
}

template <typename T>
void visit(T t)
{
	cout<<t<<" "<<endl;
}
