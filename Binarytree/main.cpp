#include<iostream>
#include "BinNode.h"
#include "BinTree.h"
using namespace std;
template <typename T> void visit(T t);
template <typename T>void CreateBinTree(BinNode<T> * TreeNode);
/*����һ�����������������
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
   cout<<"�������"<<endl;
   Tree.travPre(visit<char>);
   cout<<"�������"<<endl;
   Tree.travIn(visit<char>);
   cout<<"�������"<<endl;
   Tree.travPost(visit<char>);
   cout<<"�������"<<endl;
   Tree.travLevel(visit<char>);
   system("pause");
} 

template <typename T>void CreateBinTree(BinNode<T> * TreeNode){//���򴴽�������
   T _data;
   cout<<"������"<<TreeNode->data<<"�����ӽڵ㣺"<<endl;
   cin>>_data;
   if(_data == '0') TreeNode->lc = NULL;    //'0'�����ն˽ڵ�ֵ
   else {
      TreeNode->lc = new BinNode<T>(_data);  
      CreateBinTree(TreeNode->lc);
   }
   cout<<"������"<<TreeNode->data<<"���Һ��ӽڵ㣺"<<endl;
   cin>>_data;
   if(_data == '0') TreeNode->rc = NULL;    //'0'�����ն˽ڵ�ֵ
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
