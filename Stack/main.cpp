#include<iostream>
#include<string.h>
#include "Stack.h"
#include<math.h>
#define N_OPTR 9 //���������
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator; //���������
//�ӡ������ˡ������˷����׳ˡ������š������š���ʼ������ֹ��
using namespace std;
void convert(Stack<char> &s, int n,int base);//����ת��
float evaluate ( char* S, char* RPN );
bool paren ( const char exp[], int lo, int hi );//����ƥ��
void readNumber(char* & p,Stack<float> &opnd);//��ȡS�е����ֲ���ջ
char orderBetween(char &m,char&n);//�ж������˳��
void append(char* rpn,float opnd);//��ӵ�RPN
void append(char* rpn,char optr );
float calcu(float &pOpnd1,char &op,float &pOpnd2);//˫Ŀ���������
float calcu(char &op,float &pOpnd);//��Ŀ���������
Operator oper2rank(char op);
int main()
{
   Stack<char> s;
   //����ת�����Դ���
  /*  convert(s,89,2);
   while(!s.empty()) printf("%c",s.pop());
   cout<<endl; */
   //����ƥ����Դ���
   /* char str[]="(213[)]321)";
   bool value;
   value = paren(str,0,sizeof(str)-1);
   if (value==1) cout<<"ƥ��"<<endl;
   else cout<<"��ƥ��"<<endl; */
   //���������Դ���
   char str[]="(0!+1)*2^(3!+4)-(5!-67-(8+9))";
   char RPN[64]={'\0'};
   float sum;
   sum=evaluate(str,RPN);
   cout<<"�����:  "<<sum<<endl;
   system("pause");
} 


//����ת������:
void convert(Stack<char> &s, int n,int base){
   static char digit[]=//�½����µ���λ��,����Ϊbaseȡֵ��Χ���ʵ�����
                     {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
   while(n>0){//�ɵ͵���,��һ������½����µĸ���λ?
      s.push(digit[n % base]);//������ջ
      n/=base;//n����Ϊ���base����
   }
}

//����ƥ�����:����ж����ʽ�������Ƿ�ƥ��,const char:ָ������ݲ��ɸ���
bool paren ( const char exp[], int lo, int hi ) { //���ʽ����ƥ���飬�ɼ�����������
 Stack<char> S; //ʹ
 for ( int i = lo; i <= hi; i++ ) /* ��һ����ַ� */ 
 switch ( exp[i] ) { //
   case '(': case '[': case '{': S.push ( exp[i] ); 
            break;
   case ')': if ( ( S.empty() ) || ( '(' != S.pop() ) ) return false;//������������һ�������return false 
            break;
   case ']': if ( ( S.empty() ) || ( '[' != S.pop() ) ) return false; 
            break;
   case '}': if ( ( S.empty() ) || ( '{' != S.pop() ) ) return false; 
            break;
   default: break; //�������ַ�һ�ɺ���
 }
 return S.empty(); //�������ʽɨ�����ջ�����Բ����������ţ���ƥ�䣻����ջ�գ�ƥ��
}

//���ʽ��ֵ�㷨


const char pri[N_OPTR][N_OPTR] = { //��������ȵȼ� [ջ��] [��ǰ]
/* |-------------------- �� ǰ �� �� �� --------------------| */
/*          +    -    *    /    ^    !    (   )    \0 */
/* -- + */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
/* | - */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
/* ջ * */ '>', '>', '>', '>', '<', '<', '<', '>', '>',
/* �� / */ '>', '>', '>', '>', '<', '<', '<', '>', '>',
/* �� ^ */ '>', '>', '>', '>', '>', '<', '<', '>', '>',
/* �� ! */ '>', '>', '>', '>', '>', '>', ' ', '>', '>',
/* �� ( */ '<', '<', '<', '<', '<', '<', '<', '=', ' ',
/* | ) */ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
/* -- \0 */ '<', '<', '<', '<', '<', '<', '<', ' ', '='
};

float evaluate ( char* S, char *RPN ) { //�ԣ����޳��ո񣩱��ʽS��ֵ����ת��Ϊ�沨��ʽRPN
   Stack<float> opnd; Stack<char> optr; //������ջ�������ջ
   optr.push ( '\0' ); //β�ڱ�'\0'Ҳ��Ϊͷ�ڱ�������ջ
   while ( !optr.empty() ) {
      if ( isdigit ( *S ) ) { //����ǰ�ַ�Ϊ����������
      readNumber ( S, opnd ); //��ȡ������
      //sS++;
      append ( RPN, opnd.top() ); //��������������������RPNĩβ
      } else //����ǰ�ַ�Ϊ���������
      switch ( orderBetween ( optr.top(), *S ) ) { //���䲻ջ������������ȼ��ߵͷ�ɾ����
         case '<': //ջ����������ȼ�����ʱ
            optr.push ( *S ); S++; //�����Ƴ٣���ǰ�������ջ
         break;
         case '=': //���ȼ���ȣ���ǰ�����Ϊ�����Ž���β���ڱ�'\0'��ʱ
            optr.pop(); S++; //�����Ž�����һ���ַ�
         break;
         case '>': { //ջ����������ȼ�����ʱ����ʵʩ��Ӧ�ļ��㣬�������������ջ
            char op = optr.pop(); 
            append ( RPN, op ); //ջ���������ջ��������RPNĩβ
         if ( '!' == op ) { //������һԪ�����
            float pOpnd = opnd.pop(); //���赯��һ����������
            opnd.push ( calcu ( op, pOpnd ) ); //ʵʩһԪ���㣬�����ջ
         } else { //������������Ԫ�������
            float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop(); //������ǰ������
            opnd.push ( calcu ( pOpnd1, op, pOpnd2 ) ); //ʵʩ��Ԫ���㣬�����ջ 
         }
         break;
         }
         default : exit ( -1 ); //���﷨���󣬲�������ֱ���˳�
         }//switch
      }//while
   return opnd.pop(); //������ކ�����ļ�����
}

float calcu(float &pOpnd1,char &op,float &pOpnd2){//˫Ŀ����
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

float calcu(char &op,float &pOpnd){//��Ŀ����
   float sum=1;
   for(;pOpnd>0;pOpnd--){
      sum*=pOpnd;
   }
   return sum;
}

void append(char* rpn,float opnd){ //������������RPNĩβ
   int n=strlen(rpn);
   char buf[64];
   if (opnd !=(float)(int)opnd) sprintf(buf,"%0.2f ",opnd);//�����ʽ
   else sprintf(buf,"%d ",(int)opnd);//������ʽ
   strcat(rpn,buf);//rpn�ӳ�,Ĭ�Ͽռ����
}
void append(char* rpn,char optr ){//rpn�ӳ�,Ĭ�Ͽռ����
   int n=strlen(rpn);
   rpn[n+1] = optr;
   rpn[n+2] ='\0';
}

void readNumber(char* & p,Stack<float> &stk){//�����������Ӧ��,S������pһ��++
   stk.push((float)(*p-'0'));//��ǰ��λ��Ӧ��ֵ��ջ
   while(isdigit(*(++p)))//ֻҪ�������н��ڵ�����(����λ���������)����
      stk.push(stk.pop()*10+(*p-'0'));//����ԭ����׷����λ��,��ֵ������ջ
   if('.'!=*p) return;//û��С���㣬ֱ�ӷ���
   float fraction =1 ;//������ζ����С������
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