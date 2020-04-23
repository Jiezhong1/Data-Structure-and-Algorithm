#include<iostream>
#include<string.h>
#include "Stack.h"
#include<math.h>
#define N_OPTR 9 //运算符总数
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator; //运算符集合
//加、减、乘、除、乘方、阶乘、左括号、右括号、起始符不终止符
using namespace std;
void convert(Stack<char> &s, int n,int base);//进制转换
float evaluate ( char* S, char* RPN );
bool paren ( const char exp[], int lo, int hi );//括号匹配
void readNumber(char* & p,Stack<float> &opnd);//读取S中的数字并入栈
char orderBetween(char &m,char&n);//判断运算符顺序
void append(char* rpn,float opnd);//添加到RPN
void append(char* rpn,char optr );
float calcu(float &pOpnd1,char &op,float &pOpnd2);//双目运算符计算
float calcu(char &op,float &pOpnd);//单目运算符计算
Operator oper2rank(char op);
int main()
{
   Stack<char> s;
   //进制转换测试代码
  /*  convert(s,89,2);
   while(!s.empty()) printf("%c",s.pop());
   cout<<endl; */
   //括号匹配测试代码
   /* char str[]="(213[)]321)";
   bool value;
   value = paren(str,0,sizeof(str)-1);
   if (value==1) cout<<"匹配"<<endl;
   else cout<<"不匹配"<<endl; */
   //计算器测试代码
   char str[]="(0!+1)*2^(3!+4)-(5!-67-(8+9))";
   char RPN[64]={'\0'};
   float sum;
   sum=evaluate(str,RPN);
   cout<<"结果是:  "<<sum<<endl;
   system("pause");
} 


//进制转换代码:
void convert(Stack<char> &s, int n,int base){
   static char digit[]=//新进制下的数位号,可视为base取值范围的适当扩充
                     {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
   while(n>0){//由低到高,逐一计算出新进制下的各数位?
      s.push(digit[n % base]);//余数入栈
      n/=base;//n更新为其对base的商
   }
}

//括号匹配代码:如何判定表达式中括号是否匹配,const char:指向的内容不可更改
bool paren ( const char exp[], int lo, int hi ) { //表达式括号匹配检查，可兼顼三种括号
 Stack<char> S; //使
 for ( int i = lo; i <= hi; i++ ) /* 逐一检查字符 */ 
 switch ( exp[i] ) { //
   case '(': case '[': case '{': S.push ( exp[i] ); 
            break;
   case ')': if ( ( S.empty() ) || ( '(' != S.pop() ) ) return false;//这两个条件有一个满足就return false 
            break;
   case ']': if ( ( S.empty() ) || ( '[' != S.pop() ) ) return false; 
            break;
   case '}': if ( ( S.empty() ) || ( '{' != S.pop() ) ) return false; 
            break;
   default: break; //非括号字符一律忽略
 }
 return S.empty(); //整个表达式扫描过后，栈中若仍残留（左）括号，则不匹配；否则（栈空）匹配
}

//表达式求值算法


const char pri[N_OPTR][N_OPTR] = { //运算符优先等级 [栈顶] [当前]
/* |-------------------- 当 前 运 算 符 --------------------| */
/*          +    -    *    /    ^    !    (   )    \0 */
/* -- + */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
/* | - */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
/* 栈 * */ '>', '>', '>', '>', '<', '<', '<', '>', '>',
/* 顶 / */ '>', '>', '>', '>', '<', '<', '<', '>', '>',
/* 运 ^ */ '>', '>', '>', '>', '>', '<', '<', '>', '>',
/* 算 ! */ '>', '>', '>', '>', '>', '>', ' ', '>', '>',
/* 符 ( */ '<', '<', '<', '<', '<', '<', '<', '=', ' ',
/* | ) */ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
/* -- \0 */ '<', '<', '<', '<', '<', '<', '<', ' ', '='
};

float evaluate ( char* S, char *RPN ) { //对（已剔除空格）表达式S求值，并转换为逆波兰式RPN
   Stack<float> opnd; Stack<char> optr; //运算数栈、运算符栈
   optr.push ( '\0' ); //尾哨兵'\0'也作为头哨兵首先入栈
   while ( !optr.empty() ) {
      if ( isdigit ( *S ) ) { //若当前字符为操作数，则
      readNumber ( S, opnd ); //读取操作数
      //sS++;
      append ( RPN, opnd.top() ); //读入操作数，并将其接至RPN末尾
      } else //若当前字符为运算符，则
      switch ( orderBetween ( optr.top(), *S ) ) { //规其不栈顶运算符间优先级高低分删处理
         case '<': //栈顶运算符优先级更低时
            optr.push ( *S ); S++; //计算推迟，当前运算符进栈
         break;
         case '=': //优先级相等（当前运算符为右括号戒者尾部哨兵'\0'）时
            optr.pop(); S++; //脱括号接收下一个字符
         break;
         case '>': { //栈顶运算符优先级更高时，可实施相应的计算，并将结果重新入栈
            char op = optr.pop(); 
            append ( RPN, op ); //栈顶运算符出栈并续接至RPN末尾
         if ( '!' == op ) { //若属于一元运算符
            float pOpnd = opnd.pop(); //叧需弹出一个操作数，
            opnd.push ( calcu ( op, pOpnd ) ); //实施一元计算，结果入栈
         } else { //对于其它（二元）运算符
            float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop(); //弹出后、前操作数
            opnd.push ( calcu ( pOpnd1, op, pOpnd2 ) ); //实施二元计算，结果入栈 
         }
         break;
         }
         default : exit ( -1 ); //逢语法错误，不做处理直接退出
         }//switch
      }//while
   return opnd.pop(); //弹出幵迒回最后的计算结果
}

float calcu(float &pOpnd1,char &op,float &pOpnd2){//双目运算
   float sum;
   switch (op)
   {
      case '+': sum=pOpnd1+pOpnd2;
         break;
      case '-': sum=pOpnd1-pOpnd2;
         break;
      case '*': sum=pOpnd1*pOpnd2;
         break;
      case '/': sum=pOpnd1/pOpnd2;
         break;
      case '^': sum=pow(pOpnd1,pOpnd2);
         break;
      default:
         break;
   }
   return sum;
}

float calcu(char &op,float &pOpnd){//单目运算
   float sum=1;
   for(;pOpnd>0;pOpnd--){
      sum*=pOpnd;
   }
   return sum;
}

void append(char* rpn,float opnd){ //将操作数接至RPN末尾
   int n=strlen(rpn);
   char buf[64];
   if (opnd !=(float)(int)opnd) sprintf(buf,"%0.2f ",opnd);//浮点格式
   else sprintf(buf,"%d ",(int)opnd);//整数格式
   strcat(rpn,buf);//rpn加长,默认空间充足
}
void append(char* rpn,char optr ){//rpn加长,默认空间充足
   int n=strlen(rpn);
   rpn[n+1] = optr;
   rpn[n+2] ='\0';
}

void readNumber(char* & p,Stack<float> &stk){//就是由于这个应用,S会随着p一起++
   stk.push((float)(*p-'0'));//当前数位对应数值进栈
   while(isdigit(*(++p)))//只要后续还有紧邻的数字(即多位整数的情况)，则
      stk.push(stk.pop()*10+(*p-'0'));//弹出原数并追加新位后,数值重新入栈
   if('.'!=*p) return;//没有小数点，直接返回
   float fraction =1 ;//否则，意味着有小数部分
   while(isdigit(*(++p))){
      fraction/=10;
      stk.push(stk.pop()+(*p-'0')*fraction);
   }
}

char orderBetween(char &m,char&n){
   return pri[oper2rank(m)][oper2rank(n)];
}

Operator oper2rank(char op){
   Operator operator_p;
   switch(op){
      case '+': operator_p=ADD;
         break;
      case '-': operator_p=SUB;
         break;
      case '*': operator_p=MUL;
         break;
      case '/': operator_p=DIV;
         break;
      case '^': operator_p=POW;
         break;
      case '!': operator_p=FAC;
         break;
      case '(': operator_p=L_P;
         break;
      case ')': operator_p=R_P;
         break;
      case '\0': operator_p=EOE;
         break;
      default:
         break;
   }
   return operator_p;
}