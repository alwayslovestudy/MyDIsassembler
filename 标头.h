#pragma once
#include<Windows.h>
#include<string>
#include<stdlib.h>
#include<iostream>
using namespace std;

#define INVALID  0xff                                //表示无效数据
//属性定义，用于填充表格
#define     _prefix      0x00000001                //Prefix
#define     _modr_m      0x00000002                //ModR_M	
#define     _sib         0x00000004                //SIB
#define     _disp8       0x00000008                //Disp8
#define     _disp16      0x00000010                //disp16
#define     _disp32      0x00000020                //Disp32
#define     _imme8       0x00000040                //imme8
#define     _imme16      0x00000080                //imme16
#define     _imme32      0x00000100                //imme32
#define     _group       0x00000200                //group table
#define     _onebyte     0x00000400                //指令只有一个字节
#define     _nomore      0x00000800                //后续没有更多操作数
#define     _double_opcode 0x10000000              //双字节指令


typedef  struct  _INSTRUCTION                      //存放指令的结构体
{
	BYTE Prefix = INVALID;
	BYTE Opecode = INVALID;
	BYTE Double_Opcode = INVALID;
	BYTE ModR_M = INVALID;
	BYTE ModR_M345 = 1;                    //ModR_M的3，4，5位，在Group表中使用
	BYTE MODR_M12 = INVALID;                     //modR_M的1，2位，在sib表中使用
	BYTE SIB = INVALID;
	BYTE SIB_Base = INVALID;                     //记录sib的末三位，sib表使用
	union                                        //   Displacement
	{
		BYTE  Disp8 ;                    //一个字节
		WORD  Disp16;                            //两个字节
		DWORD Disp32 = INVALID;                            //四个字节
	}Displacement;
	union                                        //Immediate
	{
		BYTE  Imme8 ;
		WORD  Imme16;
		DWORD Imme32 = INVALID;

	}Immediate;
	DWORD InstructionLength = 0;             //存储指令长度




}INSTRUCTION, *PINSTRUCTION;
//extern INSTRUCTION Instruction;
bool Print_Instrution(INSTRUCTION  instruction)
{
	if (instruction.Prefix != INVALID)
		printf("Prefix:0x%x\n", instruction.Prefix);
	if (instruction.Opecode != INVALID)
		printf("Opecode:0x%x\n", instruction.Opecode);
	if (instruction.Double_Opcode != INVALID)
		printf("Double_Opecode:0x0f%x\n", instruction.Double_Opcode);
	if (instruction.ModR_M != INVALID)
		printf("ModR_M:0x%x\n", instruction.ModR_M);
	if (instruction.SIB != INVALID)
		printf("SIB:0x%x\n", instruction.SIB);
	if (instruction.Displacement.Disp32 != INVALID)
		printf("Displacement:0x%x\n", instruction.Displacement.Disp32);
	else if (instruction.Displacement.Disp16 != INVALID)
		printf("Displacement:0x%x\n", instruction.Displacement.Disp16);
	else if (instruction.Displacement.Disp8 != INVALID)
		printf("Displacement:0x%x\n", instruction.Displacement.Disp8);
	if (instruction.Immediate.Imme32 != INVALID)
		printf("immediate:0x%x\n", instruction.Immediate.Imme32);
	else if (instruction.Immediate.Imme16 != INVALID)
		printf("immediate:0x%x\n", instruction.Immediate.Imme16);
	else if (instruction.Immediate.Imme8 != INVALID)
		printf("immediate:0x%x\n", instruction.Immediate.Imme8);
	printf("InstructionLength:0x%x\n", instruction.InstructionLength);
	return TRUE;

}
string Str(BYTE * P)//将相应数字转化为字符串
{
	int i, j, k;
	char str1;
	string str;
	
		j = *P;
		k = j & 0xF;//取低位数字
		j = j>>4;//取高位数字
		if (j <= 9)
		{
			str1 = j + 0x30;
			str = str1;
			str1 = k + 0x30;
			str += str1;
		}
		else
		{
			str1 = j + 0x37;
			str += str1;
			str1 = k + 0x37;
			str += str1;

		}
		
	
	return str;
	
}
string Str(DWORD * P1)//将相应数字转化为字符串
{
	BYTE* P = (BYTE *)P1;
	int i, j, k;
	char str1;
	string str;
	P+= 4;
	if (*P == 0xFF)
		P--;
	for (int x = 0; x < 4; x++)
	{
		    
			j = *P;
			k = j & 0xF;//取低位数字
			j = j >> 4;//取高位数字
			if (j <= 9)
			{
				str1 = j + 0x30;
				str = str1;
				
			}
			else
			{
				str1 = j + 0x37;
				str = str1;
			

			}	
			if (k <= 9)
			{
				str1 = k + 0x30;
				str += str1;

			}
			else
			{
				str1 = k+ 0x37;
				str += str1;


			}
			P--;
	}
	return str;


}
string Str(WORD * P1)//将相应数字转化为字符串
{
	BYTE* P = (BYTE *)P1;
	int i, j, k;
	char str1;
	string str; 
	P++;
	if (*P == 0xFF)
		P--;
	for (int x = 0; x < 2; x++)
	{

		j = *P;
		k = j & 0xF;//取低位数字
		j = j >> 4;//取高位数字
		if (j <= 9)
		{
			str1 = j + 0x30;
			str = str1;

		}
		else
		{
			str1 = j + 0x37;
			str = str1;


		}
		if (k <= 9)
		{
			str1 = k + 0x30;
			str += str1;

		}
		else
		{
			str1 = k + 0x37;
			str += str1;


		}
		P--;
	}
	return str;


}
typedef  struct INSTRUCTION_string
{

	string  Operand_string =nullptr;                           
	

}INSTRUCTION_string, *PINSTRUCTION_string;

DWORD  OpCode_Table[0x100] =                         //操作码表
{ //          0        1       2       3         4          5         6         7        8        9               A         B                C          D         E          F
	/*0*/	_modr_m, _modr_m, _modr_m, _modr_m, _imme8,   _imme32,  _onebyte, _onebyte, _modr_m, _modr_m,       _modr_m,  _modr_m,        _imme8,   _imme32,  _onebyte, _double_opcode,
	/*1*/	_modr_m, _modr_m, _modr_m, _modr_m, _imme8,   _imme32,  _onebyte, _onebyte, _modr_m, _modr_m,       _modr_m,  _modr_m,        _imme8,   _imme32,  _onebyte, _onebyte,
	/*2*/   _modr_m, _modr_m, _modr_m, _modr_m, _imme8,   _imme32,  _prefix,  _onebyte, _modr_m, _modr_m,       _modr_m,  _modr_m,        _imme8,   _imme32,  _prefix,  _onebyte,
	/*3*/   _modr_m, _modr_m, _modr_m, _modr_m, _imme8,   _imme32,  _prefix,  _onebyte, _modr_m, _modr_m,       _modr_m,  _modr_m,        _imme8,   _imme32,  _prefix,  _onebyte,
	/*4*/   _onebyte,_onebyte,_onebyte,_onebyte,_onebyte, _onebyte,_onebyte,  _onebyte, _onebyte,_onebyte,      _onebyte, _onebyte,       _onebyte, _onebyte, _onebyte, _onebyte,
	/*5*/   _onebyte,_onebyte,_onebyte,_onebyte,_onebyte, _onebyte,_onebyte,  _onebyte, _onebyte,_onebyte,      _onebyte, _onebyte,       _onebyte, _onebyte, _onebyte, _onebyte,
	/*6*/   _onebyte,_onebyte,_modr_m,_modr_m,  _prefix,_prefix,   _prefix,     _prefix,/*8*/  _imme32, _modr_m | _imme32,_imme8,  _modr_m | _imme8 ,_onebyte, _onebyte, _onebyte, _onebyte,
	/*7*/   _imme8,   _imme8, _imme8, _imme8,   _imme8,    _imme8,   _imme8,  _imme8,   _imme8,  _imme8,        _imme8,   _imme8,         _imme8,   _imme8,   _imme8,   _imme8,
	/*8*/   _modr_m | _group | _imme8,_modr_m | _group | _imme32,  _modr_m | _group | _imme8,   _modr_m | _group | _imme8,_modr_m,_modr_m,_modr_m,_modr_m,/*8*/_modr_m,_modr_m,_modr_m,_modr_m,_modr_m,_modr_m,_modr_m,_modr_m | _group,
	/*9*/   _onebyte, _onebyte,_onebyte,_onebyte,_onebyte, _onebyte,_onebyte, _onebyte, _onebyte,_onebyte,     _imme32,   _onebyte,       _onebyte, _onebyte, _onebyte, _onebyte,
	/*A*/   _imme8,  _imme32, _imme8, _imme32,   _onebyte, _onebyte,_onebyte, _onebyte, _imme8,  _imme32,       _onebyte, _onebyte,       _onebyte, _onebyte, _onebyte, _onebyte,
	/*B*/   _imme8,  _imme8, _imme8,   _imme8,  _imme8,  _imme8,   _imme8,   _imme8,   _imme32, _imme32,      _imme32,   _imme32,        _imme32,  _imme32,  _imme32,  _imme32,
	/*C*/   _modr_m | _group | _imme8,_modr_m | _group | _imme8,_imme16,_onebyte,_modr_m,_modr_m,_modr_m | _group | _imme8,_modr_m | _group | _imme32,/*8*/_imme16 | _imme8,_onebyte,_imme16,_onebyte,_onebyte,_imme8,_onebyte,_onebyte,
	/*D*/   _modr_m | _group,_modr_m | _group,_modr_m | _group,_modr_m | _group,_imme8,_imme8,INVALID,_onebyte,/*8*/INVALID,INVALID,INVALID,INVALID,INVALID,INVALID,INVALID,INVALID,
	/*E*/   _imme8,  _imme8,   _imme8, _imme8,  _imme8,   _imme8,  _imme8,    _imme8,   _imme32, _imme32,      _imme32 | _imme16,  _imme8,        _onebyte, _onebyte,_onebyte,  _onebyte,
	/*F*/   _prefix, _onebyte,_prefix,_prefix, _onebyte,  _onebyte,_modr_m | _group,_modr_m | _group,_onebyte,_onebyte,_onebyte,_onebyte,_onebyte,_onebyte,_group,_group
};

extern DWORD _16Bit_ModRM[0x100] =                     //16位的ModR/M表
{    //      0        1        2         3        4       5        6        7       
	/*0*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _disp16, _nomore,
	/*1*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _disp16, _nomore,
	/*2*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _disp16, _nomore,
	/*3*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _disp16, _nomore,
	/*4*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _disp16, _nomore,
	/*5*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _disp16, _nomore,
	/*6*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _disp16, _nomore,
	/*7*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _disp16, _nomore,
	/*0*/  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,
	/*1*/  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,
	/*2*/  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,
	/*3*/  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,
	/*4*/  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,
	/*5*/  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,
	/*6*/  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,
	/*7*/  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,  _disp8,
	/*0*/  _disp16, _disp16, _disp16, _disp16, _disp16, _disp16, _disp16, _disp16,
	/*1*/  _disp16, _disp16, _disp16, _disp16, _disp16, _disp16, _disp16, _disp16,
	/*2*/  _disp16, _disp16, _disp16, _disp16, _disp16, _disp16, _disp16, _disp16,
	/*3*/  _disp16, _disp16, _disp16, _disp16, _disp16, _disp16, _disp16, _disp16,
	/*4*/  _disp16, _disp16, _disp16, _disp16, _disp16, _disp16, _disp16, _disp16,
	/*5*/  _disp16, _disp16, _disp16, _disp16, _disp16, _disp16, _disp16, _disp16,
	/*6*/  _disp16, _disp16, _disp16, _disp16, _disp16, _disp16, _disp16, _disp16,
	/*7*/  _disp16, _disp16, _disp16, _disp16, _disp16, _disp16, _disp16, _disp16,
	/*0*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore,
	/*1*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore,
	/*2*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore,
	/*3*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore,
	/*4*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore,
	/*5*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore,
	/*6*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore,
	/*7*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore
};
 DWORD _32Bit_ModRM[0x100] =                     //32位的ModR/M表
{ //          0        1        2         3       4       5        6        7       
	/*0*/  _nomore, _nomore, _nomore, _nomore,  _sib, _disp32,  _nomore, _nomore,
	/*1*/  _nomore, _nomore, _nomore, _nomore,  _sib, _disp32,  _nomore, _nomore,
	/*2*/  _nomore, _nomore, _nomore, _nomore,  _sib, _disp32,  _nomore, _nomore,
	/*3*/  _nomore, _nomore, _nomore, _nomore,  _sib, _disp32,  _nomore, _nomore,
	/*4*/  _nomore, _nomore, _nomore, _nomore,  _sib, _disp32,  _nomore, _nomore,
	/*5*/  _nomore, _nomore, _nomore, _nomore,  _sib, _disp32,  _nomore, _nomore,
	/*6*/  _nomore, _nomore, _nomore, _nomore,  _sib, _disp32,  _nomore, _nomore,
	/*7*/  _nomore, _nomore, _nomore, _nomore,  _sib, _disp32,  _nomore, _nomore,
	/*0*/  _disp8,  _disp8,  _disp8,  _disp8,   _sib | _disp8,  _disp8,  _disp8,  _disp8,
	/*1*/  _disp8,  _disp8,  _disp8,  _disp8,   _sib | _disp8,  _disp8,  _disp8,  _disp8,
	/*2*/  _disp8,  _disp8,  _disp8,  _disp8,   _sib | _disp8,  _disp8,  _disp8,  _disp8,
	/*3*/  _disp8,  _disp8,  _disp8,  _disp8,   _sib | _disp8,  _disp8,  _disp8,  _disp8,
	/*4*/  _disp8,  _disp8,  _disp8,  _disp8,   _sib | _disp8,  _disp8,  _disp8,  _disp8,
	/*5*/  _disp8,  _disp8,  _disp8,  _disp8,   _sib | _disp8,  _disp8,  _disp8,  _disp8,
	/*6*/  _disp8,  _disp8,  _disp8,  _disp8,   _sib | _disp8,  _disp8,  _disp8,  _disp8,
	/*7*/  _disp8,  _disp8,  _disp8,  _disp8,   _sib | _disp8,  _disp8,  _disp8,  _disp8,
	/*0*/  _disp32, _disp32, _disp32, _disp32,  _sib | _disp32, _disp32, _disp32, _disp32,
	/*1*/  _disp32, _disp32, _disp32, _disp32,  _sib | _disp32, _disp32, _disp32, _disp32,
	/*2*/  _disp32, _disp32, _disp32, _disp32,  _sib | _disp32, _disp32, _disp32, _disp32,
	/*3*/  _disp32, _disp32, _disp32, _disp32,  _sib | _disp32, _disp32, _disp32, _disp32,
	/*4*/  _disp32, _disp32, _disp32, _disp32,  _sib | _disp32, _disp32, _disp32, _disp32,
	/*5*/  _disp32, _disp32, _disp32, _disp32,  _sib | _disp32, _disp32, _disp32, _disp32,
	/*6*/  _disp32, _disp32, _disp32, _disp32,  _sib | _disp32, _disp32, _disp32, _disp32,
	/*7*/  _disp32, _disp32, _disp32, _disp32,  _sib | _disp32, _disp32, _disp32, _disp32,
	/*0*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore,
	/*1*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore,
	/*2*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore,
	/*3*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore,
	/*4*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore,
	/*5*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore,
	/*6*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore,
	/*7*/  _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore, _nomore
};
extern DWORD  Group_table[8][0x8] =
{//          0       1       2       3       4       5       6       7
	/*0*/ _nomore,_nomore,_nomore,_nomore,_nomore,_nomore,_nomore,_nomore,
	/*1*/ _nomore,INVALID,INVALID,INVALID,INVALID,INVALID,INVALID,INVALID,
	/*2*/ _nomore,_nomore,_nomore,_nomore,_nomore,_nomore,INVALID,_nomore,
	/*3*/ _imme8|_imme32,INVALID,_nomore,_nomore,_nomore,_nomore,_nomore,_nomore,
	/*4*/ _nomore,_nomore,INVALID,INVALID,INVALID,INVALID,INVALID,INVALID,
	/*5*/ _nomore,_nomore,_nomore,_nomore,_nomore,_nomore,_nomore,INVALID,
	/*6*/ _nomore,INVALID,INVALID,INVALID,INVALID,INVALID,INVALID,INVALID,
	/*7*/ _nomore,INVALID,INVALID,INVALID,INVALID,INVALID,INVALID,INVALID,


};
void InitStringTable(INSTRUCTION &Instruction)
{
	string SIBstring_Table[0x100] =//SIB字符B表
	{//     0           1             2           3             4          5          6            7
		/*0*/"[eax*2" , "[eax+ecx",  "[eax+edx",  "[eax+ebx",   "[eax+esp",  "[eax",  "[eax+esi",  "[eax+edi",
		/*1*/"[ecx+eax",  "[ecx*2",  "[ecx+edx",  "[ecx+ebx",   "[ecx+esp",  "[ecx",  "[ecx+esi",  "[ecx+edi",
		/*2*/"[edx+eax",  "[edx+ecx",  "[edx*2",  "[edx+ebx",   "[edx+esp",  "[edx" , "[edx+esi",  "[edx+edi",
		/*3*/"[ebx+eax",  "[ebx+ecx",  "[ebx+edx",  "[ebx*2",   "[ebxesp",  "[ebx " ,"[ebx+esi",  "[ebx+edi",
		/*4*/  "",      "",      "",       "",        "",        "",      "",       "",
		/*5*/"[ebp+eax",  "[ebp+ecx",  "[ebp+edx",  "[ebp+ebx",   "[ebp+esp",  "[ebp" ,    "[ebp+esi",  "[ebp+edi",
		/*6*/"[esi+eax",  "[esi+ecx",  "[esi+edx",  "[esi+ebx",   "[esi+esp",  "[esi" ,    "[esi+esi",  "[esi+edi",
		/*7*/"[edi+eax",  "[edi+ecx",  "[edi+edx",  "[edi+ebx",   "[edi+esp",  "[edi" ,    "[edi+esi",  "[edi*2",
		/*8*/"[eax*3+","[eax*2+ecx","[eax*2+edx","[eax*2+ebx", "[eax*2+esp", "[eax*2",  "[eax*2+esi","[eax*2+edi",
		/*9*/"[ecx*2+eax","[ecx*3","[ecx*2+edx","[ecx*2+ebx", "[ecx*2+esp",   "[ecx*2",   "[ecx*2+esi","[ecx*2+edi",
		/*A*/"[edx*2+eax","[edx*2+ecx","[edx*3","[edx*2+ebx", "[edx*2+esp",   "[edx*2",   "[edx*2+esi","[edx*2+esi",
		/*B*/"[ebx*2+","[ebx*2+","[ebx*2+","[ebx*2+", "[ebx*2+",   "[ebx*2+",   "[ebx*2+","[ebx*2+",
		/*C*/ "",      "",      "",       "",        "",        "",      "",       "",
		/*D*/"[ebp*2+eax","[ebp*2+ecx","[ebp*2+edx","[ebp*2+ebx", "[ebp*2+esp",   "[ebp*2",   "[ebp*2+esi","[ebp*2+edi",
		/*E*/"[esi*2+eax","[esi*2+ecx","[esi*2+edx","[esi*2+ebx", "[esi*2+esp",   "[esi*2",   "[esi*3","[esi*2+edi",
		/*F*/"[edi*2+eax","[edi*2+ecx","[edi*2+edx","[edi*2+ebx", "[edi*2+esp",   "[edi*2",   "[edi*2+esi","[edi*3",
		/*0*/"[eax*5","[eax*4+ecx","[eax*4+edx","[eax*4+ebx", "[eax*4+esp",   "[eax*4",   "[eax*4+esi","[eax*4+edi",
		/*1*/"[ecx*4+eax","[ecx*5+","[ecx*4+edx","[ecx*4+ebx", "[ecx*4+esp",   "[ecx*4",   "[ecx*4+esi","[ecx*4+edi",
		/*2*/"[edx*4+eax","[edx*4+ecx","[edx*5","[edx*4+ebx", "[edx*4+esp",   "[edx*4",   "[edx*4+esi","[edx*4+edi",
		/*3*/"[ebx*4+eax","[ebx*4+ecx","[ebx*4+edx","[ebx*5", "[ebx*4+esp",   "[ebx*4",   "[ebx*4+esi","[ebx*4+edi",
		/*4*/ "",      "",      "",       "",        "",        "",      "",       "",
		/*5*/"[ebp*4+eax","[ebp*4+ecx","[ebp*4+edx","[ebp*4+ebx", "[ebp*4+esp",   "[ebp*4",   "[ebp*4+esi","[ebp*4+edi",
		/*6*/"[esi*4+eax","[esi*4+ecx","[esi*4+edx","[esi*4+ebx", "[esi*4+esp",   "[esi*4",   "[esi*5","[esi*4+edi",
		/*7*/"[edi*4+eax","[edi*4+ecx","[edi*4+edx","[edi*4+ebx", "[edi*4+esp",   "[edi*4",   "[edi*4+esi","[edi*5",
		/*8*/"[eax*9","[eax*8+ecx","[eax*8+edx","[eax*8+ebx","[eax*8+esp",   "[eax*8",   "[eax*8+esi","[eax*8+edi",
		/*9*/"[ecx*8+eax","[ecx*9","[ecx*8+edx","[ecx*8+ebx", "[ecx*8+esp",   "[ecx*8",  "[ecx*8+esi","[ecx*8+edi",
		/*A*/"[edx*8+eax","[edx*8+ecx","[edx*9","[edx*8+ebx", "[edx*8+esp",   "[edx*8",   "[edx*8+esi","[edx*8+edi",
		/*B*/"[ebx*8+eax","[ebx*8+ecx","[ebx*8+edx","[ebx*9", "[ebx*8+esp",   "[ebx*8",   "[ebx*8+esi","[ebx*8 +edi",
		/*C*/ "",      "",      "",       "",        "",        "",      "",       "",
		/*D*/"[ebp*8+eax","[ebp*8+ecx","[ebp*8+edx","[ebp*8+ebx",  "[ebp*8+esp", "[ebp*8","[ebp*8+esi","[ebp*8+edi",
		/*E*/"[esi*8+eax","[esi*8+ecx","[esi*8+edx","[esi*8+ebx","[esi*8+esp", "[esi*8","[esi*9","[esi*8+edi",
		/*F*/"[edi*8+eax","[edi*8+ecx","[edi*8+edx","[edi*8+ebx", "[edi*8+esp", "[edi*8","[edi*8+esi","[edi*9"
	};

	string ModR_StringE32[0x100] =
	{//         0    1   2   3    4    5    6   7
		/*0*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*1*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]", "[esi]","[edi]",
		/*2*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*3*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*4*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*5*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*6*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*7*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*0*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*1*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*2*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*3*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*4*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*5*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*6*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*7*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*0*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*1*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*2*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*3*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*4*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*5*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*6*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*7*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*0*/ "eax","ecx","edx","ebx","esp","ebp","esi","edi",
		/*1*/ "eax","ecx","edx","ebx","esp","ebp","esi","edi",
		/*2*/ "eax","ecx","edx","ebx","esp","ebp","esi","edi",
		/*3*/ "eax","ecx","edx","ebx","esp","ebp","esi","edi",
		/*4*/ "eax","ecx","edx","ebx","esp","ebp","esi","edi",
		/*5*/ "eax","ecx","edx","ebx","esp","ebp","esi","edi",
		/*6*/ "eax","ecx","edx","ebx","esp","ebp","esi","edi",
		/*7*/ "eax","ecx","edx","ebx","esp","ebp","esi","edi"
	};


	string ModR_StringE16[0x100] =
	{//    0    1   2   3    4    5    6   7
		/*0*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*1*/ "[eax]","[ecx]""[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*2*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*3*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*4*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*5*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*6*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*7*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*0*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*1*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*2*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*3*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*4*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*5*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*6*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*7*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*0*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*1*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*2*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*3*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*4*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*5*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*6*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*7*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*0*/ "ax","cx","dx","bx","sp","bp","si","di",
		/*1*/ "ax","cx","dx","bx","sp","bp","si","di",
		/*2*/ "ax","cx","dx","bx","sp","bp","si","di",
		/*3*/ "ax","cx","dx","bx","sp","bp","si","di",
		/*4*/ "ax","cx","dx","bx","sp","bp","si","di",
		/*5*/ "ax","cx","dx","bx","sp","bp","si","di",
		/*6*/ "ax","cx","dx","bx","sp","bp","si","di",
		/*7*/ "ax","cx","dx","bx","sp","bp","si","di"
	};
	string ModR_StringE8[0x100] =
	{//    0    1   2   3    4    5    6   7
		/*0*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*1*/ "[eax]","[ecx]""[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*2*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*3*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*4*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*5*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*6*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*7*/ "[eax]","[ecx]","[edx]","[ebx]",SIBstring_Table[Instruction.SIB] + "]","[" + Str(&Instruction.Displacement.Disp32) + "]","[esi]","[edi]",
		/*0*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*1*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*2*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*3*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*4*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*5*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]","[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*6*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]", "[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*7*/ "[eax+" + Str(&Instruction.Displacement.Disp8) + "]","[ecx+" + Str(&Instruction.Displacement.Disp8) + "]",  "[edx+" + Str(&Instruction.Displacement.Disp8) + "]","[ebx+" + Str(&Instruction.Displacement.Disp8) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp8) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp8) + "]", "[esi+" + Str(&Instruction.Displacement.Disp8) + "]", "[edi+" + Str(&Instruction.Displacement.Disp8) + "]",
		/*0*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*1*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*2*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*3*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*4*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*5*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*6*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
		/*7*/ "[eax+" + Str(&Instruction.Displacement.Disp32) + "]","[ecx+" + Str(&Instruction.Displacement.Disp32) + "]","[edx+" + Str(&Instruction.Displacement.Disp32) + "]","[ebx+" + Str(&Instruction.Displacement.Disp32) + "]",SIBstring_Table[Instruction.SIB] + Str(&Instruction.Displacement.Disp32) + "]", "[ebp+" + Str(&Instruction.Displacement.Disp32) + "]", "[esi+" + Str(&Instruction.Displacement.Disp32) + "]", "[edi+" + Str(&Instruction.Displacement.Disp32) + "]",
	    /*0*/ "al","cl","dl","bl","ah","ch","dh","bh",
	    /*1*/ "al","cl","dl","bl","ah","ch","dh","bh",
	    /*2*/ "al","cl","dl","bl","ah","ch","dh","bh",
	    /*3*/ "al","cl","dl","bl","ah","ch","dh","bh",
	    /*4*/ "al","cl","dl","bl","ah","ch","dh","bh",
	    /*5*/ "al","cl","dl","bl","ah","ch","dh","bh",
	    /*6*/ "al","cl","dl","bl","ah","ch","dh","bh",
	    /*7*/ "al","cl","dl","bl","ah","ch","dh","bh",
	};
	string ModR_StringG32[0x100] =
	{ //  0  1  2   3   4   5  6   7
   /*0*/ "eax","eax","eax","eax","eax","eax","eax","eax",
   /*1*/ "ecx","ecx","ecx","ecx","ecx","ecx","ecx","ecx",
   /*2*/ "edx","edx","edx","edx","edx","edx","edx","edx",
   /*3*/ "ebx","ebx","ebx","ebx","ebx","ebx","ebx","ebx",
   /*4*/ "esp","esp","esp","esp","esp","esp","esp","esp",
   /*5*/ "ebp","ebp","ebp","ebp","ebp","ebp","ebp","ebp",
   /*6*/ "esi","esi","esi","esi","esi","esi","esi","esi",
   /*7*/ "edi","edi","edi","edi","edi","edi","edi","edi",
   /*0*/ "eax","eax","eax","eax","eax","eax","eax","eax",
   /*1*/ "ecx","ecx","ecx","ecx","ecx","ecx","ecx","ecx",
   /*2*/ "edx","edx","edx","edx","edx","edx","edx","edx",
   /*3*/ "ebx","ebx","ebx","ebx","ebx","ebx","ebx","ebx",
   /*4*/ "esp","esp","esp","esp","esp","esp","esp","esp",
   /*5*/ "ebp","ebp","ebp","ebp","ebp","ebp","ebp","ebp",
   /*6*/ "esi","esi","esi","esi","esi","esi","esi","esi",
   /*7*/ "edi","edi","edi","edi","edi","edi","edi","edi",
   /*0*/ "eax","eax","eax","eax","eax","eax","eax","eax",
   /*1*/ "ecx","ecx","ecx","ecx","ecx","ecx","ecx","ecx",
   /*2*/ "edx","edx","edx","edx","edx","edx","edx","edx",
   /*3*/ "ebx","ebx","ebx","ebx","ebx","ebx","ebx","ebx",
   /*4*/ "esp","esp","esp","esp","esp","esp","esp","esp",
   /*5*/ "ebp","ebp","ebp","ebp","ebp","ebp","ebp","ebp",
   /*6*/ "esi","esi","esi","esi","esi","esi","esi","esi",
   /*7*/ "edi","edi","edi","edi","edi","edi","edi","edi",
   /*0*/ "eax","eax","eax","eax","eax","eax","eax","eax",
   /*1*/ "ecx","ecx","ecx","ecx","ecx","ecx","ecx","ecx",
   /*2*/ "edx","edx","edx","edx","edx","edx","edx","edx",
   /*3*/ "ebx","ebx","ebx","ebx","ebx","ebx","ebx","ebx",
   /*4*/ "esp","esp","esp","esp","esp","esp","esp","esp",
   /*5*/ "ebp","ebp","ebp","ebp","ebp","ebp","ebp","ebp",
   /*6*/ "esi","esi","esi","esi","esi","esi","esi","esi",
   /*7*/ "edi","edi","edi","edi","edi","edi","edi","edi",
	};
string ModR_StringG16[0x100] =
{//     0   1    2    3    4    5     6    7
/*0*/ "ax","ax","ax","ax","ax","ax","ax","ax",
/*1*/ "cx","cx","cx","cx","cx","cx","cx","cx",
/*2*/ "dx","dx","dx","dx","dx","dx","dx","dx",
/*3*/ "bx","bx","bx","bx","bx","bx","bx","bx",
/*4*/ "sp","sp","sp","sp","sp","sp","sp","sp",
/*5*/ "bp","bp","bp","bp","bp","bp","bp","bp",
/*6*/ "si","si","si","si","si","si","si","si",
/*7*/ "di","di","di","di","di","di","di","di",
/*0*/ "ax","ax","ax","ax","ax","ax","ax","ax",
/*1*/ "cx","cx","cx","cx","cx","cx","cx","cx",
/*2*/ "dx","dx","dx","dx","dx","dx","dx","dx",
/*3*/ "bx","bx","bx","bx","bx","bx","bx","bx",
/*4*/ "sp","sp","sp","sp","sp","sp","sp","sp",
/*5*/ "bp","bp","bp","bp","bp","bp","bp","bp",
/*6*/ "si","si","si","si","si","si","si","si",
/*7*/ "di","di","di","di","di","di","di","di",
/*0*/ "ax","ax","ax","ax","ax","ax","ax","ax",
/*1*/ "cx","cx","cx","cx","cx","cx","cx","cx",
/*2*/ "dx","dx","dx","dx","dx","dx","dx","dx",
/*3*/ "bx","bx","bx","bx","bx","bx","bx","bx",
/*4*/ "sp","sp","sp","sp","sp","sp","sp","sp",
/*5*/ "bp","bp","bp","bp","bp","bp","bp","bp",
/*6*/ "si","si","si","si","si","si","si","si",
/*7*/ "di","di","di","di","di","di","di","di",
/*0*/ "ax","ax","ax","ax","ax","ax","ax","ax",
/*1*/ "cx","cx","cx","cx","cx","cx","cx","cx",
/*2*/ "dx","dx","dx","dx","dx","dx","dx","dx",
/*3*/ "bx","bx","bx","bx","bx","bx","bx","bx",
/*4*/ "sp","sp","sp","sp","sp","sp","sp","sp",
/*5*/ "bp","bp","bp","bp","bp","bp","bp","bp",
/*6*/ "si","si","si","si","si","si","si","si",
/*7*/ "di","di","di","di","di","di","di","di"

};
string ModR_StringG8[0x100] =
{//   0    1    2    3    4    5    6    7
/*0*/"al","al","al","al","al","al","al","al",
/*1*/"cl","cl","cl","cl","cl","cl","cl","cl",
/*2*/"dl","dl","dl","dl","dl","dl","dl","dl",
/*3*/"bl","bl","bl","bl","bl","bl","bl","bl",
/*4*/"ah","ah","ah","ah","ah","ah","ah","ah",
/*5*/"ch","ch","ch","ch","ch","ch","ch","ch",
/*6*/"dh","dh","dh","dh","dh","dh","dh","dh",
/*7*/"bh","bh","bh","bh","bh","bh","bh","bh",
/*0*/"al","al","al","al","al","al","al","al",
/*1*/"cl","cl","cl","cl","cl","cl","cl","cl",
/*2*/"dl","dl","dl","dl","dl","dl","dl","dl",
/*3*/"bl","bl","bl","bl","bl","bl","bl","bl",
/*4*/"ah","ah","ah","ah","ah","ah","ah","ah",
/*5*/"ch","ch","ch","ch","ch","ch","ch","ch",
/*6*/"dh","dh","dh","dh","dh","dh","dh","dh",
/*7*/"bh","bh","bh","bh","bh","bh","bh","bh",
/*0*/"al","al","al","al","al","al","al","al",
/*1*/"cl","cl","cl","cl","cl","cl","cl","cl",
/*2*/"dl","dl","dl","dl","dl","dl","dl","dl",
/*3*/"bl","bl","bl","bl","bl","bl","bl","bl",
/*4*/"ah","ah","ah","ah","ah","ah","ah","ah",
/*5*/"ch","ch","ch","ch","ch","ch","ch","ch",
/*6*/"dh","dh","dh","dh","dh","dh","dh","dh",
/*7*/"bh","bh","bh","bh","bh","bh","bh","bh",
/*0*/"al","al","al","al","al","al","al","al",
/*1*/"cl","cl","cl","cl","cl","cl","cl","cl",
/*2*/"dl","dl","dl","dl","dl","dl","dl","dl",
/*3*/"bl","bl","bl","bl","bl","bl","bl","bl",
/*4*/"ah","ah","ah","ah","ah","ah","ah","ah",
/*5*/"ch","ch","ch","ch","ch","ch","ch","ch",
/*6*/"dh","dh","dh","dh","dh","dh","dh","dh",
/*7*/"bh","bh","bh","bh","bh","bh","bh","bh"
};
string Group_String[64]=
{//    0      1    2     3     4     5     6     7
/*0*/ "ADD ","OR ","ADC ","SBB ","AND ","SUB ","XOR ","CMP ",
/*1*/ "POP "," ",  " ",   " ",   " ",   " ",   " ",   " ",
/*2*/ "ROL ","ROR ","RCL ","RCR ","SHL ","SHR "," ","SAR " ,
/*3*/ "TEST "+Str(&Instruction.Immediate.Imme8)," ","NOT ","NEG ","MUL al/eax ","IMUL al/rax ","DIV al/eax ","IDIV al/eax",
/*4*/ "INC ","DEC "," "," "," "," "," "," ",
/*5*/"INC ","DEC ","CALL ","CALL","JMP ","JMP","PUSH "," ",
/*6*/"MOV "," "," "," "," "," "," ","XABORT ",
/*7*/"MOV"," "," "," "," "," "," ","XBEGIN"
};
string OpCode_String[0x100] =
{//    0                                                                             1                                                                                2                                                                             3                                                                                 4       5       6      7     
	/*0*/ "ADD " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M],"ADD " + ModR_StringE32[Instruction.ModR_M] + "," + ModR_StringG32[Instruction.ModR_M],"ADD " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M],"ADD " + ModR_StringG32[Instruction.ModR_M] + "," + ModR_StringE32[Instruction.ModR_M],"ADD al," + Str(&Instruction.Immediate.Imme8),"ADD eax," + Str(&Instruction.Immediate.Imme32),"PUSH es","POP es",
	 "OR " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M],"OR " + ModR_StringE32[Instruction.ModR_M] + "," + ModR_StringG32[Instruction.ModR_M],"OR " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M],"OR " + ModR_StringG32[Instruction.ModR_M] + "," + ModR_StringE32[Instruction.ModR_M],"OR al," + Str(&Instruction.Immediate.Imme8),"OR eax," + Str(&Instruction.Immediate.Imme32),"PUSH cs","  ",//双字节前缀
	/*1*/ "ADC " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M],"ADC " + ModR_StringE32[Instruction.ModR_M] + "," + ModR_StringG32[Instruction.ModR_M],"ADC " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M],"ADC " + ModR_StringG32[Instruction.ModR_M] + "," + ModR_StringE32[Instruction.ModR_M],"ADC al," + Str(&Instruction.Immediate.Imme8),"ADC eax," + Str(&Instruction.Immediate.Imme32),"PUSH ss","POP ss",
		  "SBB " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M],"SBB " + ModR_StringE32[Instruction.ModR_M] + "," + ModR_StringG32[Instruction.ModR_M],"SBB " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M],"SBB " + ModR_StringG32[Instruction.ModR_M] + "," + ModR_StringE32[Instruction.ModR_M],"SBB al," + Str(&Instruction.Immediate.Imme8),"SBB eax," + Str(&Instruction.Immediate.Imme32),"PUSH ds","POP ds",
	/*2*/ "AND " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M], "AND " + ModR_StringE32[Instruction.ModR_M] + "," + ModR_StringG32[Instruction.ModR_M], "AND " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M], "AND " + ModR_StringG32[Instruction.ModR_M] + "," + ModR_StringE32[Instruction.ModR_M], "AND al," + Str(&Instruction.Immediate.Imme8), "AND eax," + Str(&Instruction.Immediate.Imme32), "  ", "DAA",
		 "SUB " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M], "SUB " + ModR_StringE32[Instruction.ModR_M] + "," + ModR_StringG32[Instruction.ModR_M], "SUB " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M], "SUB " + ModR_StringG32[Instruction.ModR_M] + "," + ModR_StringE32[Instruction.ModR_M], "SUB al," + Str(&Instruction.Immediate.Imme8), "SUB eax," + Str(&Instruction.Immediate.Imme32), "  ", "DAS",
	/*3*/ "XOR " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M], "XOR " + ModR_StringE32[Instruction.ModR_M] + "," + ModR_StringG32[Instruction.ModR_M], "XOR " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M], "XOR " + ModR_StringG32[Instruction.ModR_M] + "," + ModR_StringE32[Instruction.ModR_M], "XOR al," + Str(&Instruction.Immediate.Imme8), "XOR eax," + Str(&Instruction.Immediate.Imme32), "  ", "AAA",
		  "CMP " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M], "CMP " + ModR_StringE32[Instruction.ModR_M] + "," + ModR_StringG32[Instruction.ModR_M], "CMP " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M], "CMP " + ModR_StringG32[Instruction.ModR_M] + "," + ModR_StringE32[Instruction.ModR_M], "CMP al," + Str(&Instruction.Immediate.Imme8), "CMP eax," + Str(&Instruction.Immediate.Imme32), "  ", "AAS",
	/*4*/ "INC eax","INC ecx","INC edx","INC ebx","INC esp","INC ebp","INC esi","INC edi","DEC eax", "DEC ecx", "DEC edx", "DEC ebx", "DEC esp", "DEC ebp", "DEC esi", "DEC edi",
    /*5*/ "PUSH eax", "PUSH ecx", "PUSH edx", "PUSH ebx", "PUSH esp", "PUSH ebp", "PUSH esi", "PUSH edi","POP eax", "POP ecx", "POP edx", "POP ebx", "POP esp", "POP ebp", "POP esi", "POP edi",
	/*6*/ "PUSHAD","POPAD","BOUND " + ModR_StringG32[Instruction.ModR_M] + "," + "两个地址","ARPL " + ModR_StringE16[Instruction.ModR_M] + "," + ModR_StringG16[Instruction.ModR_M]," "," "," "," ","PUSH " + Str(&Instruction.Immediate.Imme32),"IMUL " + ModR_StringG32[Instruction.ModR_M] + "," + ModR_StringE32[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme32),"PUSH "+Str(&Instruction.Immediate.Imme8),"IMUL " + ModR_StringG32[Instruction.ModR_M] + "," + ModR_StringE32[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme32),"INS byte ptr es:edi,dx","INS word ptr es:edi,dx","OUTS DX,byte ptr ds:esi","OUTS DX,word ptr ds:esi",
	/*7*/ "JO " + Str(&Instruction.Immediate.Imme8),"JNO " + Str(&Instruction.Immediate.Imme8),"JB " + Str(&Instruction.Immediate.Imme8),"JNB " + Str(&Instruction.Immediate.Imme8),"JZ " + Str(&Instruction.Immediate.Imme8),"JNZ " + Str(&Instruction.Immediate.Imme8),"JBE " + Str(&Instruction.Immediate.Imme8),"JNBE " + Str(&Instruction.Immediate.Imme8),"JS " + Str(&Instruction.Immediate.Imme8),"JNS " + Str(&Instruction.Immediate.Imme8),"JP " + Str(&Instruction.Immediate.Imme8),"JNP " + Str(&Instruction.Immediate.Imme8),"JL " + Str(&Instruction.Immediate.Imme8),"JNL " + Str(&Instruction.Immediate.Imme8),"JLE " + Str(&Instruction.Immediate.Imme8),"JNLE " + Str(&Instruction.Immediate.Imme8),
	/*8*/(string)Group_String[Instruction.ModR_M345] + ModR_StringE8[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme8),(string)Group_String[Instruction.ModR_M345] + ModR_StringE32[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme32),(string)Group_String[Instruction.ModR_M345] + ModR_StringE8[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme8),(string)Group_String[Instruction.ModR_M345] + ModR_StringE32[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme8),"TEST " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M],"TEST " + ModR_StringE32[Instruction.ModR_M] + "," + ModR_StringG32[Instruction.ModR_M],"XCHG " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M], "XCHG " + ModR_StringE32[Instruction.ModR_M] + "," + ModR_StringG32[Instruction.ModR_M],
		 "MOV " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M], "MOV " + ModR_StringE32[Instruction.ModR_M] + "," + ModR_StringG32[Instruction.ModR_M],"MOV " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M], "MOV " + ModR_StringG32[Instruction.ModR_M] + "," + ModR_StringE32[Instruction.ModR_M],"MOV " + ModR_StringE16[Instruction.ModR_M] + "," + "段寄存器","LEA " + ModR_StringG32[Instruction.ModR_M] + "," + ModR_StringE32[Instruction.ModR_M],"MOV 段寄存器，" + ModR_StringE16[Instruction.ModR_M],Group_String[8+Instruction.ModR_M345] + ModR_StringE32[Instruction.ModR_M],
	/*9*/ "NOP","XCHG ecx,eax","XCHG edx,eax","XCHG ebx,eax","XCHG esp,eax","XCHG ebp,eax","XCHG esi,eax","XCHG edi,eax","CBW","CWD","CALL " + Str(&Instruction.Immediate.Imme32),"FWAIT","PUSHF","POPF","SAHF","LAHF",
    /*A*/"MOV al,"+Str(&Instruction.Immediate.Imme8),"MOV eax," + Str(&Instruction.Immediate.Imme32),"MOV " + Str(&Instruction.Immediate.Imme8)+",al","MOV " + Str(&Instruction.Immediate.Imme32) + ",eax","MOVS byte ptr es:edi,ds:esi","MOVS dword ptr ptr es:edi,ds:esi","CMPS byte ptr ds:esi,es:edi","CMPS dword ptr ds:esi,es:edi","TEST al," + Str(&Instruction.Immediate.Imme8),"TEST eax," + Str(&Instruction.Immediate.Imme32),"STOS byte ptr es:edi,al","STOS dword ptr es:edi,eax","LODS al,byte ptr ds:esi","LODS eax,dword ptr ds:esi","SCAS al,byte ptr es:edi","SCAS eax,dword ptr es:edi",
	/*B*/ "MOV al," + Str(&Instruction.Immediate.Imme8),"MOV cl," + Str(&Instruction.Immediate.Imme8),"MOV dl," + Str(&Instruction.Immediate.Imme8),"MOV bl," + Str(&Instruction.Immediate.Imme8),"MOV ah," + Str(&Instruction.Immediate.Imme8),"MOV ch," + Str(&Instruction.Immediate.Imme8),"MOV dh," + Str(&Instruction.Immediate.Imme8),"MOV bh," + Str(&Instruction.Immediate.Imme8),"MOV eax," + Str(&Instruction.Immediate.Imme32),"MOV ecx," + Str(&Instruction.Immediate.Imme32),"MOV edx," + Str(&Instruction.Immediate.Imme32),"MOV ebx," + Str(&Instruction.Immediate.Imme32),"MOV esp," + Str(&Instruction.Immediate.Imme32),"MOV ebp," + Str(&Instruction.Immediate.Imme32),"MOV esi," + Str(&Instruction.Immediate.Imme32),"MOV edi," + Str(&Instruction.Immediate.Imme32),
    /*C*/ (string)Group_String[16+Instruction.ModR_M345] + ModR_StringE8[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme8),(string)Group_String[16+Instruction.ModR_M345] + ModR_StringE32[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme8),"RET " + Str(&Instruction.Immediate.Imme16),"RET","LES " + ModR_StringG32[Instruction.ModR_M] + "," + ModR_StringE32[Instruction.ModR_M],"LDS " + ModR_StringG32[Instruction.ModR_M] + "," + ModR_StringE32[Instruction.ModR_M],Group_String[7*8+Instruction.ModR_M345] + ModR_StringE8[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme8),Group_String[7*8+Instruction.ModR_M345] + ModR_StringE32[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme32),"ENTER " + Str(&Instruction.Immediate.Imme16) + "," + Str(&Instruction.Displacement.Disp8),"LEAVE","RET " + Str(&Instruction.Immediate.Imme16),"RET","INT 3","INT " + Str(&Instruction.Immediate.Imme8),"INTO","IRET",
    /*D*/(string)Group_String[16+Instruction.ModR_M345] + ModR_StringE8[Instruction.ModR_M] + ", 1",(string)Group_String[16+Instruction.ModR_M345] + ModR_StringE32[Instruction.ModR_M] + ", 1",(string)Group_String[16+Instruction.ModR_M345] + ModR_StringE8[Instruction.ModR_M] + ", cl",(string)Group_String[16+Instruction.ModR_M345] + ModR_StringE32[Instruction.ModR_M] + ", cl","AAM " + Str(&Instruction.Immediate.Imme8),"AAD " + Str(&Instruction.Immediate.Imme8)," ","XLAT"," "," "," "," "," "," "," "," ",
	/*E*/"LOOPNE" + Str(&Instruction.Immediate.Imme8),"LOOPE" + Str(&Instruction.Immediate.Imme8),"LOOP" + Str(&Instruction.Immediate.Imme8),"JrCXZ" + Str(&Instruction.Immediate.Imme8),"IN al" + Str(&Instruction.Immediate.Imme8),"IN eax" + Str(&Instruction.Immediate.Imme8),"OUT " + Str(&Instruction.Immediate.Imme8) + ", al","OUT " + Str(&Instruction.Immediate.Imme8) + ", eax","CALL " + Str(&Instruction.Immediate.Imme32),"JMP " + Str(&Instruction.Immediate.Imme32),"JMP " + Str(&Instruction.Displacement.Disp16) + ":" + Str(&Instruction.Immediate.Imme32),"JMP " + Str(&Instruction.Immediate.Imme8),"IN al,dx","IN eax,dx","OUT dx,al","OUT dx,eax",
	/*F*/" ","INT1"," "," ","HLT","CMC",Group_String[3*8+Instruction.ModR_M345] + ModR_StringE8[Instruction.ModR_M],Group_String[3*8+Instruction.ModR_M345] + ModR_StringE32[Instruction.ModR_M],"CLC","STC","CLI","STI","CLD","STD",Group_String[8*5+Instruction.ModR_M345],Group_String[8*5+Instruction.ModR_M345]
};
string OpCode66_String[0x100] =
{   //      0                                                                                    1                                                                                      2                                                                                    3                                                                                     4                                              5                                               6         7     
    /*0*/ "ADD " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M],"ADD " + ModR_StringE16[Instruction.ModR_M] + "," + ModR_StringG16[Instruction.ModR_M],"ADD " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M],"ADD " + ModR_StringG16[Instruction.ModR_M] + "," + ModR_StringE16[Instruction.ModR_M],"ADD al," + Str(&Instruction.Immediate.Imme8),"ADD ax," + Str(&Instruction.Immediate.Imme16),"PUSH es","POP es",
		  "OR " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M], "OR " + ModR_StringE16[Instruction.ModR_M] + "," + ModR_StringG16[Instruction.ModR_M],  "OR " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M],   "OR " + ModR_StringG16[Instruction.ModR_M] + "," + ModR_StringE16[Instruction.ModR_M],    "OR al," + Str(&Instruction.Immediate.Imme8),"OR ax," + Str(&Instruction.Immediate.Imme16),  "PUSH cs","  ",//双字节前缀
	/*1*/ "ADC " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M],"ADC " + ModR_StringE16[Instruction.ModR_M] + "," + ModR_StringG16[Instruction.ModR_M],"ADC " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M],"ADC" + ModR_StringG16[Instruction.ModR_M] + "," + ModR_StringE16[Instruction.ModR_M],"ADC al," + Str(&Instruction.Immediate.Imme8),"ADC ax," + Str(&Instruction.Immediate.Imme16),"PUSH ss","POP ss",
		  "SBB " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M],"SBB " + ModR_StringE16[Instruction.ModR_M] + "," + ModR_StringG16[Instruction.ModR_M],"SBB " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M],"SBB " + ModR_StringG16[Instruction.ModR_M] + "," + ModR_StringE16[Instruction.ModR_M],"SBB al," + Str(&Instruction.Immediate.Imme8),"SBB ax," + Str(&Instruction.Immediate.Imme16),"PUSH ds","POP ds",
	/*2*/ "AND " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M], "AND " + ModR_StringE16[Instruction.ModR_M] + "," + ModR_StringG16[Instruction.ModR_M], "AND " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M], "AND " + ModR_StringG16[Instruction.ModR_M] + "," + ModR_StringE16[Instruction.ModR_M], "AND al," + Str(&Instruction.Immediate.Imme8), "AND ax," + Str(&Instruction.Immediate.Imme16), "  ", "DAA",
          "SUB " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M], "SUB " + ModR_StringE16[Instruction.ModR_M] + "," + ModR_StringG16[Instruction.ModR_M], "SUB " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M], "SUB " + ModR_StringG16[Instruction.ModR_M] + "," + ModR_StringE16[Instruction.ModR_M], "SUB al," + Str(&Instruction.Immediate.Imme8), "SUB ax," + Str(&Instruction.Immediate.Imme16), "  ", "DAS",
	/*3*/ "XOR " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M], "XOR " + ModR_StringE16[Instruction.ModR_M] + "," + ModR_StringG16[Instruction.ModR_M], "XOR " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M], "XOR " + ModR_StringG16[Instruction.ModR_M] + "," + ModR_StringE16[Instruction.ModR_M], "XOR al," + Str(&Instruction.Immediate.Imme8), "XOR ax," + Str(&Instruction.Immediate.Imme16), "  ", "AAA",
	      "CMP " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M], "CMP " + ModR_StringE16[Instruction.ModR_M] + "," + ModR_StringG16[Instruction.ModR_M], "CMP " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M], "CMP " + ModR_StringG16[Instruction.ModR_M] + "," + ModR_StringE16[Instruction.ModR_M], "CMP al," + Str(&Instruction.Immediate.Imme8), "CMP ax," + Str(&Instruction.Immediate.Imme16), "  ", "AAS",
	/*4*/ "INC ax", "INC cx", "INC dx", "INC bx", "INC sp", "INC bp", "INC si", "INC di", "DEC ax", "DEC cx", "DEC dx", "DEC bx", "DEC sp", "DEC bp", "DEC si", "DEC di",
	/*5*/ "PUSH ax", "PUSH cx", "PUSH dx", "PUSH bx", "PUSH sp", "PUSH bp", "PUSH si", "PUSH di", "POP ax", "POP cx", "POP dx", "POP bx", "POP sp", "POP bp", "POP si", "POP di",
	/*6*/ "PUSHAD","POPAD","BOUND " + ModR_StringG16[Instruction.ModR_M] + "," + "两个地址","ARPL " + ModR_StringE16[Instruction.ModR_M] + "," + ModR_StringG16[Instruction.ModR_M]," "," "," "," ","PUSH " + Str(&Instruction.Immediate.Imme16),"IMUL " + ModR_StringG16[Instruction.ModR_M] + "," + ModR_StringE16[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme16),"PUSH " + Str(&Instruction.Immediate.Imme8),"IMUL " + ModR_StringG16[Instruction.ModR_M] + "," + ModR_StringE16[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme16),"INS byte ptr es:edi,dx","INS word ptr es:edi,dx","OUTS DX,byte ptr ds:esi","OUTS DX,word ptr ds:esi", 
	/*7*/"JO " + Str(&Instruction.Displacement.Disp8),"JNO " + Str(&Instruction.Displacement.Disp8),"JB " + Str(&Instruction.Displacement.Disp8),"JNB " + Str(&Instruction.Displacement.Disp8),"JZ " + Str(&Instruction.Displacement.Disp8),"JNZ " + Str(&Instruction.Displacement.Disp8),"JBE " + Str(&Instruction.Displacement.Disp8),"JNBE " + Str(&Instruction.Displacement.Disp8),"JS " + Str(&Instruction.Displacement.Disp8),"JNS " + Str(&Instruction.Displacement.Disp8),"JP " + Str(&Instruction.Displacement.Disp8),"JNP " + Str(&Instruction.Displacement.Disp8),"JL " + Str(&Instruction.Displacement.Disp8),"JNL " + Str(&Instruction.Displacement.Disp8),"JLE " + Str(&Instruction.Displacement.Disp8),"JNLE " + Str(&Instruction.Displacement.Disp8),
	/*8*/Group_String[Instruction.ModR_M345] + ModR_StringE8[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme8),Group_String[Instruction.ModR_M345] + ModR_StringE16[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme16),Group_String[Instruction.ModR_M345] + ModR_StringE8[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme8),Group_String[Instruction.ModR_M345] + ModR_StringE16[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme8),"TEST " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M],"TEST " + ModR_StringE16[Instruction.ModR_M] + "," + ModR_StringG16[Instruction.ModR_M],"XCHG " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M], "XCHG " + ModR_StringE16[Instruction.ModR_M] + "," + ModR_StringG16[Instruction.ModR_M],
		 "MOV " + ModR_StringE8[Instruction.ModR_M] + "," + ModR_StringG8[Instruction.ModR_M], "MOV " + ModR_StringE16[Instruction.ModR_M] + "," + ModR_StringG16[Instruction.ModR_M],"MOV " + ModR_StringG8[Instruction.ModR_M] + "," + ModR_StringE8[Instruction.ModR_M], "MOV " + ModR_StringG16[Instruction.ModR_M] + "," + ModR_StringE16[Instruction.ModR_M],"MOV " + ModR_StringE16[Instruction.ModR_M] + "," + "段寄存器","LEA " + ModR_StringG16[Instruction.ModR_M] + "," + ModR_StringE16[Instruction.ModR_M],"MOV 段寄存器，" + ModR_StringE16[Instruction.ModR_M],Group_String[8+Instruction.ModR_M345] + ModR_StringE16[Instruction.ModR_M],
	/*9*/ "NOP","XCHG ecx,eax","XCHG edx,eax","XCHG ebx,eax","XCHG esp,eax","XCHG ebp,eax","XCHG esi,eax","XCHG edi,eax","CBW","CWD","CALL " + Str(&Instruction.Immediate.Imme32),"FWAIT","PUSHF","POPF","SAHF","LAHF",
	/*A*/"MOV al," + Str(&Instruction.Immediate.Imme8),"MOV ax," + Str(&Instruction.Immediate.Imme16),"MOV " + Str(&Instruction.Immediate.Imme8) + ",al","MOV " + Str(&Instruction.Immediate.Imme16) + ",ax","MOVS byte ptr es:edi,ds:esi","MOVS word ptr ptr es:edi,ds:esi","CMPS byte ptr ds:esi,es:edi","CMPS word ptr ds:esi,es:edi", "TEST al," + Str(&Instruction.Immediate.Imme8),"TEST ax," + Str(&Instruction.Immediate.Imme16),"STOS byte ptr es:edi,al","STOS word ptr es:edi,ax","LODS al,byte ptr ds:esi","LODS ax,word ptr ds:esi","SCAS al,byte ptr es:edi","SCAS ax,word ptr es:edi",
	/*B*/ "MOV al," + Str(&Instruction.Immediate.Imme8), "MOV cl," + Str(&Instruction.Immediate.Imme8), "MOV dl," + Str(&Instruction.Immediate.Imme8), "MOV bl," + Str(&Instruction.Immediate.Imme8), "MOV ah," + Str(&Instruction.Immediate.Imme8), "MOV ch," + Str(&Instruction.Immediate.Imme8), "MOV dh," + Str(&Instruction.Immediate.Imme8), "MOV bh," + Str(&Instruction.Immediate.Imme8), "MOV ax," + Str(&Instruction.Immediate.Imme16), "MOV cx," + Str(&Instruction.Immediate.Imme16), "MOV dx," + Str(&Instruction.Immediate.Imme16), "MOV bx," + Str(&Instruction.Immediate.Imme16), "MOV sp," + Str(&Instruction.Immediate.Imme16), "MOV bp," + Str(&Instruction.Immediate.Imme16), "MOV si," + Str(&Instruction.Immediate.Imme16), "MOV di," + Str(&Instruction.Immediate.Imme16),
	/*C*/ Group_String[16+Instruction.ModR_M345] + ModR_StringE8[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme8),Group_String[16+Instruction.ModR_M345] + ModR_StringE32[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme8),"RET " + Str(&Instruction.Immediate.Imme16),"RET","LES " + ModR_StringG16[Instruction.ModR_M] + "," + ModR_StringE16[Instruction.ModR_M],"LDS " + ModR_StringG16[Instruction.ModR_M] + "," + ModR_StringE16[Instruction.ModR_M],Group_String[7*8+Instruction.ModR_M345] + ModR_StringE8[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme8),Group_String[7*8+Instruction.ModR_M345] + ModR_StringE16[Instruction.ModR_M] + "," + Str(&Instruction.Immediate.Imme16),"ENTER " + Str(&Instruction.Immediate.Imme16) + "," + Str(&Instruction.Displacement.Disp8),"LEAVE","RET " + Str(&Instruction.Immediate.Imme16),"RET","INT 3","INT " + Str(&Instruction.Immediate.Imme8),"INTO","IRET",
	/*D*/Group_String[16+Instruction.ModR_M345] + ModR_StringE8[Instruction.ModR_M] + ", 1",Group_String[16+Instruction.ModR_M345] + ModR_StringE32[Instruction.ModR_M] + ", 1",Group_String[2*8+Instruction.ModR_M345] + ModR_StringE8[Instruction.ModR_M] + ", cl",Group_String[2*8+Instruction.ModR_M345] + ModR_StringE32[Instruction.ModR_M] + ", cl","AAM " + Str(&Instruction.Immediate.Imme8),"AAD " + Str(&Instruction.Immediate.Imme8)," ","XLAT"," "," "," "," "," "," "," "," ",
	/*E*/"LOOPNE" + Str(&Instruction.Immediate.Imme8),"LOOPE" + Str(&Instruction.Immediate.Imme8),"LOOP" + Str(&Instruction.Immediate.Imme8),"JrCXZ" + Str(&Instruction.Immediate.Imme8),"IN al" + Str(&Instruction.Immediate.Imme8),"IN eax" + Str(&Instruction.Immediate.Imme8),"OUT " + Str(&Instruction.Immediate.Imme8) + ", al","OUT " + Str(&Instruction.Immediate.Imme8) + ", eax","CALL " + Str(&Instruction.Immediate.Imme16),"JMP " + Str(&Instruction.Immediate.Imme16),"JMP " + Str(&Instruction.Displacement.Disp16) + ":" + Str(&Instruction.Immediate.Imme16),"JMP " + Str(&Instruction.Immediate.Imme8),"IN al,dx","IN eax,dx","OUT dx,al","OUT dx,eax",
	/*F*/" ","INT1"," "," ","HLT","CMC",Group_String[3*8+Instruction.ModR_M345] + ModR_StringE8[Instruction.ModR_M],Group_String[3*8+Instruction.ModR_M345] + ModR_StringE32[Instruction.ModR_M],"CLC","STC","CLI","STI","CLD","STD",Group_String[5*8+Instruction.ModR_M345],Group_String[5*8+Instruction.ModR_M345]
};
if(Instruction.Prefix==0x66)
cout <<Instruction.Prefix<<":"<<OpCode66_String[Instruction.Opecode] << endl;//有0x66prefix
else if(Instruction.Prefix!=INVALID)
cout << Instruction.Prefix << ":" << OpCode_String[Instruction.Opecode] << endl;
else 
cout<< OpCode_String[Instruction.Opecode] << endl;

} //函数末尾