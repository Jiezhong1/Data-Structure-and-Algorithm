#include "BinTree.h"
#include "Vector.h"
#include "Bitmap.h"
#include "skiplist.h"
typedef BinTree<char > PFCTree;//PFC树
typedef Vector<PFCTree *> PFCForest;//PFC森林
typedef SkipList<char,char*> PFCTable;//PFC编码表，词条格式为:(key = 字符,value = 编码串)
#define N_CHAR (0x80 - 0x20) //只考虑可以打印字符

PFCForest * initForest(){//PCF森林初始化
    PFCForest * forest = new PFCForest;//首先创建空森林
    for(int i=0;i<N_CHAR;i++){//对每一个可打印字符[0x20,0x80)
        forest->insert(i,new PFCTree());//创建一颗对应的PFC编码数,初始时其中
        (* forest)[i]->insertAsRoot(0x20+i);//只包含对应的一个(叶、根)节点
    }
    return forest;//返回包含N_CHAR棵树的森林，其中每棵树各包含一个字符
}

PFCTree * generateTree (PFCForest *forest){//构造PFC树
    srand((unsigned int )time(NULL));//这里将随机取树合并，故先设置随机种子
    while(1<forest->size()){//共做|forest|-1次合并
        PFCTree * s=new PFCTree; s->insertAsRoot('^');//创建新树(根标记为"^")
        Rank r1 = rand()%forest->size();//随机选取r1,且
        s->attachAsLC(s->root(),(*forest)[r1]);//作为左子树接入后
        forest->remove(r1);//随机剔除
        Rank r2 = rand()%forest->size();//随机选取r2,且
        s->attachAsRC(s->root(),(*forest)[r2]);//作为右子树接入后
        forest->remove(r2);//随机剔除
        forest->insert(forest->size(),s);//合并后的PFC重新植入PFC森林
    }
    return (*forest)[0];//至此，森林中尚存的最后一棵树，即全局PFC编码树
}

void generateCT//通过遍历获取各字符的编码
(Bitmap *code, int length, PFCTable* table,BinNodePosi(char) v){
    if(HasChild(*v))//若是叶节点
    {table->put(v->data,code->bits2string(length));return;}
    if(HasLChild(*v))//left=1
    {code->clear(length);generateCT(code,length+1,table,v->lc);}
    if(HasRChild(*v))//left=1
    {code->set(length);generateCT(code,length+1,table,v->rc);}
}

PFCTable *generateTable(PFCTree * tree){//构造PFC编码表
    PFCTable *table = new PFCTable;//创建以Skiplist实现的编码表
    Bitmap *code = new Bitmap;//用于记录RPS的位图
    generateCT(code,0,table,tree->root());//遍历以获取各字符(叶节点)的RPS
    release(code); return table;//释放编码位图，返回编码表
}

/* int encode ( PFCTable* table, Bitmap& codeString, char* s ) { //PFC编码算法
    int n = 0;
    for ( size_t m = strlen ( s ), i = 0; i < m; i++ ) { //对亍明文s[]中癿殏个字符
        char ** pCharCode = table->get ( s[i] ); //取出对应编码串
        if ( !pCharCode ) pCharCode = table->get ( s[i] + 'A' - 'a' ); //小写字殎转为大写
        if ( !pCharCode ) pCharCode = table->get ( ' ' ); //无法识删癿字符统一规作空格
        printf ( "%s", *pCharCode ); //输出弼前字符癿编码
        for ( size_t m = strlen ( *pCharCode ), j = 0; j < m; j++ ) //将弼前字符癿编码接入编码串
            '1' == * ( *pCharCode + j ) ? codeString.set ( n++ ) : codeString.clear ( n++ );
    }
    return n; //二迕刢编码串记弽亍codeString中，迒回编码串总长
} */