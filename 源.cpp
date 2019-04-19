#include"标头.h"
#include<string>
using namespace std;
INSTRUCTION  Instruction;

bool FillMODR_M12345(INSTRUCTION &instruction)
{
	instruction.MODR_M12 = instruction.ModR_M >> 6;
	instruction.ModR_M345 = (instruction.ModR_M & 0x38) >> 3;
	return TRUE;
}
bool FillSIB_Base(INSTRUCTION &instruction)
{
	instruction.SIB_Base = instruction.SIB & 7;
	return TRUE;
}

bool FillInstruction(byte * ArrayofData, INSTRUCTION &Instruction)       //指令填充函数
{
	byte * P_index = ArrayofData;                                        //记录数已取到的位置
	switch (OpCode_Table[*P_index])
	{// 有前缀
	case _prefix: //第一个字节具有prefix属性                                            
		Instruction.Prefix = *P_index;
		P_index++;
	case _modr_m: 
	case _imme8:
	case _imme16:
	case _imme32:
	case _onebyte:
	case _double_opcode:
	case _modr_m | _imme8:
	case _modr_m|_imme32:
	case _modr_m|_group|_imme8:
	case _modr_m |_group|_imme32:
	case _modr_m|_group:
	case _group:
	case _imme16|_imme32:
			switch (OpCode_Table[*P_index])
	{
		case _modr_m:  //第二个字节opcode具有modr属性 
			Instruction.Opecode = *P_index;
			P_index++;
			Instruction.ModR_M = *P_index;
			P_index++;
			if (Instruction.Prefix ==0x67)                                 //具有0x67前缀,使用16位地址表
			{
				switch (_16Bit_ModRM[Instruction.ModR_M])
				{
				case _nomore:  //第三个字节modrm具有nomore属性 
					Instruction.InstructionLength = P_index - ArrayofData;         //计算指令占的字节数
					return TRUE;

				case _disp8:   //第三个字节modrm具有disp8属性
					Instruction.Displacement.Disp8 = *P_index;
					P_index++;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;

				case _disp16: //第三个字节modrm具有disp16属性

					Instruction.Displacement.Disp16 = *(WORD *)P_index;
					P_index += 2;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;

				}
			}
			
				switch (_32Bit_ModRM[Instruction.ModR_M])
				{
				case _nomore:  //第三个字节modrm具有nomore属性 
					Instruction.InstructionLength = P_index - ArrayofData;         //计算指令占的字节数
					return TRUE;

				case _disp8:   //第三个字节modrm具有disp8属性
					Instruction.Displacement.Disp8 = *P_index;
					P_index++;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;

				case _disp32: //第三个字节modrm具有disp32属性

					Instruction.Displacement.Disp32 = *(DWORD *)P_index;
					P_index += 4;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;
				case _sib:  //第三个字节modrm具有sib属性

					Instruction.SIB = *P_index;
					P_index++;
					FillMODR_M12345(Instruction);
					if (Instruction.MODR_M12 != 0 && Instruction.MODR_M12 != 1 && Instruction.MODR_M12 != 2)   //第四个字节sib没有displacement
					{
						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;

					}
					else                                //第四个字节sib有displacement
					{
						FillSIB_Base(Instruction);     //填充SIB_Base
						if (Instruction.MODR_M12 == 0 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
						}
						else if (Instruction.MODR_M12 == 1 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp8 = *P_index;
							P_index++;
						}
						else if (Instruction.MODR_M12 = 2 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
						}
						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;
					}
				case _sib | _disp8:    //第三个字节modrm具有sib|disp8属性 
					Instruction.SIB = *P_index;
					P_index++;
					FillMODR_M12345(Instruction);
					if (Instruction.MODR_M12 != 0 && Instruction.MODR_M12 != 1 && Instruction.MODR_M12 != 2)   //第四个字节sib没有displacement
					{  
						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;

					}
					else
					{
						FillSIB_Base(Instruction);
						//第四个字节sib有displacement
						if (Instruction.MODR_M12 == 0 &&Instruction.SIB_Base == 5)//modrm和sib都有displacement
						{
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
							Instruction.Displacement.Disp32 += *P_index;
							P_index++;
						}
						else if (Instruction.MODR_M12 == 1 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp16 = *P_index;
							P_index++;
							Instruction.Displacement.Disp16 += *P_index;
							P_index++;
						}
						else if (Instruction.MODR_M12 = 2 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
							Instruction.Displacement.Disp32 += *P_index;
							P_index++;
						}

						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;
					}
				case _sib|_disp32:  //第三个字节modrm具有sib|disp32属性
					Instruction.SIB = *P_index;
					P_index++;
					FillMODR_M12345(Instruction);
					if (Instruction.MODR_M12 != 0 && Instruction.MODR_M12 != 1 && Instruction.MODR_M12 != 2)   //第四个字节sib没有displacement
					{
						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;

					}
					else
					{
						                                                                                      //第四个字节sib有displacement
						if (Instruction.MODR_M12 == 0 && Instruction.SIB_Base == 5)//modrm和sib都有displacement
						{
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
							Instruction.Displacement.Disp32 += *(DWORD *)P_index;
							P_index+=4;
						}
						else if (Instruction.MODR_M12 == 1 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp32 = *P_index;
							P_index++;
							Instruction.Displacement.Disp32 += *(DWORD *)P_index;
							P_index+=4;
						}
						else if (Instruction.MODR_M12 = 2 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
							Instruction.Displacement.Disp32 += *(DWORD *)P_index;
							P_index+=4;
						}

						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;
					}

				}
			
	case _imme8://第二个字节opcode具有imme8属性
		Instruction.Opecode = *P_index;
		P_index++;
		Instruction.Immediate.Imme8 = *P_index;
		P_index++;
		Instruction.InstructionLength = P_index - ArrayofData;
		return TRUE;
	case _imme16://第二个字节opcode具有imme16属性
		Instruction.Opecode = *P_index;
		P_index++;
		if (Instruction.Prefix == 0x66)//具有0x66前缀
		{
			Instruction.Immediate.Imme32 = *(DWORD *)P_index;
			P_index += 4;
			Instruction.InstructionLength = P_index - ArrayofData;
			return TRUE;
		}
		else 
		{
			Instruction.Immediate.Imme16 = *(WORD *)P_index;
			P_index += 2;
			Instruction.InstructionLength = P_index - ArrayofData;
			return TRUE;
		}
	case _imme32://第二个字节opcode具有imme32属性
		Instruction.Opecode = *P_index;
		P_index++;
		if (Instruction.Prefix == 0x66)//具有0x66前缀
		{
			Instruction.Immediate.Imme16 = *(WORD *)P_index;
			P_index += 2;
			Instruction.InstructionLength = P_index - ArrayofData;
			return TRUE;
		}
		else
		{
			Instruction.Immediate.Imme32 = *(DWORD *)P_index;
			P_index += 4;
			Instruction.InstructionLength = P_index - ArrayofData;
			return TRUE;
		}
	case _onebyte://第二个字节opcode具有onebyte属性
		Instruction.Opecode = *P_index;
		P_index++;
		Instruction.InstructionLength = P_index - ArrayofData;
		return TRUE;
	case _double_opcode://第二个字节opcode具有double-opcode属性
		P_index++;    //第一个字节必为0x0f，不存储直接打印
		Instruction.Double_Opcode = *P_index;
		P_index++;
		Instruction.InstructionLength = P_index - ArrayofData;
		return TRUE;
	case _modr_m |_imme8: // 第二个字节opcode具有_modr_m |_imme8属性
		Instruction.Opecode = *P_index;
		P_index++;
		Instruction.ModR_M = *P_index;
		P_index++;
		if (Instruction.Prefix == 0x67)                                 //具有0x67前缀,使用16位地址表
		{
			switch (_16Bit_ModRM[Instruction.ModR_M])
			{
			case _nomore:  //第三个字节modrm具有nomore属性 
				Instruction.Immediate.Imme8 = *P_index;
				P_index++;
				Instruction.InstructionLength = P_index - ArrayofData;         //计算指令占的字节数
				return TRUE;

			case _disp8:   //第三个字节modrm具有disp8属性
				Instruction.Displacement.Disp8 = *P_index;
				P_index++;
				Instruction.Immediate.Imme8 = *P_index;
				P_index++;
				Instruction.InstructionLength = P_index - ArrayofData;
				return TRUE;

			case _disp16: //第三个字节modrm具有disp16属性

				Instruction.Displacement.Disp16 = *(WORD *)P_index;
				P_index += 2;
				Instruction.Immediate.Imme8 = *P_index;
				P_index++;
				Instruction.InstructionLength = P_index - ArrayofData;
				return TRUE;

			}
		}
		
			switch (_32Bit_ModRM[Instruction.ModR_M])
			{
			case _nomore:  //第三个字节modrm具有nomore属性
				Instruction.Immediate.Imme8 = *P_index;
				P_index++;
				Instruction.InstructionLength = P_index - ArrayofData;         //计算指令占的字节数
				return TRUE;

			case _disp8:   //第三个字节modrm具有disp8属性
				Instruction.Displacement.Disp8 = *P_index;
				P_index++;
				Instruction.Immediate.Imme8 = *P_index;
				P_index++;
				Instruction.InstructionLength = P_index - ArrayofData;
				return TRUE;

			case _disp32: //第三个字节modrm具有disp32属性

				Instruction.Displacement.Disp32 = *(DWORD *)P_index;
				P_index += 4;
				Instruction.Immediate.Imme8 = *P_index;
				P_index++;
				Instruction.InstructionLength = P_index - ArrayofData;
				return TRUE;
			case _sib:  //第三个字节modrm具有sib属性

				Instruction.SIB = *P_index;
				P_index++;
				FillMODR_M12345(Instruction);
				if (Instruction.MODR_M12 != 0 && Instruction.MODR_M12 != 1 && Instruction.MODR_M12 != 2)   //第四个字节sib没有displacement
				{
					Instruction.Immediate.Imme8 = *P_index;
					P_index++;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;

				}
				else                                //第四个字节sib有displacement
				{
					FillSIB_Base(Instruction);     //填充SIB_Base
					if (Instruction.MODR_M12 == 0 && Instruction.SIB_Base == 5)
					{
						Instruction.Displacement.Disp32 = *(DWORD *)P_index;
						P_index += 4;
					}
					else if (Instruction.MODR_M12 == 1 && Instruction.SIB_Base == 5)
					{
						Instruction.Displacement.Disp8 = *P_index;
						P_index++;
					}
					else if (Instruction.MODR_M12 = 2 && Instruction.SIB_Base == 5)
					{
						Instruction.Displacement.Disp32 = *(DWORD *)P_index;
						P_index += 4;
					}
					Instruction.Immediate.Imme8 = *P_index;
					P_index++;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;
				}
			case _sib | _disp8:    //第三个字节modrm具有sib|disp8属性 
				Instruction.SIB = *P_index;
				P_index++;
				FillMODR_M12345(Instruction);
				if (Instruction.MODR_M12 != 0 && Instruction.MODR_M12 != 1 && Instruction.MODR_M12 != 2)   //第四个字节sib没有displacement
				{
					Instruction.Immediate.Imme8 = *P_index;
					P_index++;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;

				}
				else
				{
					FillSIB_Base(Instruction);
					//第四个字节sib有displacement
					if (Instruction.MODR_M12 == 0 && Instruction.SIB_Base == 5)//modrm和sib都有displacement
					{
						Instruction.Displacement.Disp32 = *(DWORD *)P_index;
						P_index += 4;
						Instruction.Displacement.Disp32 += *P_index;
						P_index++;
					}
					else if (Instruction.MODR_M12 == 1 && Instruction.SIB_Base == 5)
					{
						Instruction.Displacement.Disp16 = *P_index;
						P_index++;
						Instruction.Displacement.Disp16 += *P_index;
						P_index++;
					}
					else if (Instruction.MODR_M12 = 2 && Instruction.SIB_Base == 5)
					{
						Instruction.Displacement.Disp32 = *(DWORD *)P_index;
						P_index += 4;
						Instruction.Displacement.Disp32 += *P_index;
						P_index++;
					}
					Instruction.Immediate.Imme8 = *P_index;
					P_index++;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;
				}
			case _sib | _disp32:  //第三个字节modrm具有sib|disp32属性
				Instruction.SIB = *P_index;
				P_index++;
				FillMODR_M12345(Instruction);
				if (Instruction.MODR_M12 != 0 && Instruction.MODR_M12 != 1 && Instruction.MODR_M12 != 2)   //第四个字节sib没有displacement
				{
					Instruction.Immediate.Imme8 = *P_index;
					P_index++;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;

				}
				else
				{
					//第四个字节sib有displacement
					if (Instruction.MODR_M12 == 0 && Instruction.SIB_Base == 5)//modrm和sib都有displacement
					{
						Instruction.Displacement.Disp32 = *(DWORD *)P_index;
						P_index += 4;
						Instruction.Displacement.Disp32 += *(DWORD *)P_index;
						P_index += 4;
					}
					else if (Instruction.MODR_M12 == 1 && Instruction.SIB_Base == 5)
					{
						Instruction.Displacement.Disp32 = *P_index;
						P_index++;
						Instruction.Displacement.Disp32 += *(DWORD *)P_index;
						P_index += 4;
					}
					else if (Instruction.MODR_M12 = 2 && Instruction.SIB_Base == 5)
					{
						Instruction.Displacement.Disp32 = *(DWORD *)P_index;
						P_index += 4;
						Instruction.Displacement.Disp32 += *(DWORD *)P_index;
						P_index += 4;
					}
				
					Instruction.Immediate.Imme8 = *P_index;
					P_index++;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;
				}

			}



		

		case _modr_m | _imme32:  //第二个字节opcode具有_modr_m |_imme32属性
			Instruction.Opecode = *P_index;
			P_index++;
			Instruction.ModR_M = *P_index;
			P_index++;
			if (Instruction.Prefix == 0x67)                                 //具有0x67前缀,使用16位地址表
			{
				switch (_16Bit_ModRM[Instruction.ModR_M])
				{
				case _nomore:  //第三个字节modrm具有nomore属性 
					Instruction.Immediate.Imme32 = *(DWORD *)P_index;
					P_index+=4;
					Instruction.InstructionLength = P_index - ArrayofData;         //计算指令占的字节数
					return TRUE;

				case _disp8:   //第三个字节modrm具有disp8属性
					Instruction.Displacement.Disp8 = *P_index;
					P_index++;
					Instruction.Immediate.Imme32 = *(DWORD *)P_index;
					P_index += 4;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;

				case _disp16: //第三个字节modrm具有disp16属性

					Instruction.Displacement.Disp16 = *(WORD *)P_index;
					P_index += 2;
					Instruction.Immediate.Imme32 = *(DWORD *)P_index;
					P_index += 4;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;

				}
			}
			else if (Instruction.Prefix == 0x66)                           //具有0x66前缀
			{
				switch (_32Bit_ModRM[Instruction.ModR_M])
				{
				case _nomore:  //第三个字节modrm具有nomore属性 
					Instruction.Immediate.Imme16 = *(WORD *)P_index;        //0x66前缀使得立即数变为16位
					P_index+=2;
					Instruction.InstructionLength = P_index - ArrayofData;         //计算指令占的字节数
					return TRUE;

				case _disp8:   //第三个字节modrm具有disp8属性
					Instruction.Displacement.Disp8 = *P_index;
					P_index++;
					Instruction.Immediate.Imme16 = *(WORD *)P_index;        //0x66前缀使得立即数变为16位
					P_index += 2;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;

				case _disp32: //第三个字节modrm具有disp32属性

					Instruction.Displacement.Disp32 = *(DWORD *)P_index;
					P_index += 4;
					Instruction.Immediate.Imme16 = *(WORD *)P_index;        //0x66前缀使得立即数变为16位
					P_index += 2;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;
				case _sib:  //第三个字节modrm具有sib属性

					Instruction.SIB = *P_index;
					P_index++;
					FillMODR_M12345(Instruction);
					if (Instruction.MODR_M12 != 0 && Instruction.MODR_M12 != 1 && Instruction.MODR_M12 != 2)   //第四个字节sib没有displacement
					{
						Instruction.Immediate.Imme16 = *(WORD *)P_index;        //0x66前缀使得立即数变为16位
						P_index += 2;
						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;

					}
					else                                //第四个字节sib有displacement
					{
						FillSIB_Base(Instruction);     //填充SIB_Base
						if (Instruction.MODR_M12 == 0 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
						}
						else if (Instruction.MODR_M12 == 1 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp8 = *P_index;
							P_index++;
						}
						else if (Instruction.MODR_M12 = 2 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
						}
						Instruction.Immediate.Imme16 = *(WORD *)P_index;        //0x66前缀使得立即数变为16位
						P_index += 2;
						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;
					}
				case _sib | _disp8:    //第三个字节modrm具有sib|disp8属性 
					Instruction.SIB = *P_index;
					P_index++;
					FillMODR_M12345(Instruction);
					if (Instruction.MODR_M12 != 0 && Instruction.MODR_M12 != 1 && Instruction.MODR_M12 != 2)   //第四个字节sib没有displacement
					{
						Instruction.Immediate.Imme16 = *(WORD *)P_index;        //0x66前缀使得立即数变为16位
						P_index += 2;
						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;

					}
					else
					{
						FillSIB_Base(Instruction);
						//第四个字节sib有displacement
						if (Instruction.MODR_M12 == 0 && Instruction.SIB_Base == 5)//modrm和sib都有displacement
						{
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
							Instruction.Displacement.Disp32 += *P_index;
							P_index++;
						}
						else if (Instruction.MODR_M12 == 1 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp16 = *P_index;
							P_index++;
							Instruction.Displacement.Disp16 += *P_index;
							P_index++;
						}
						else if (Instruction.MODR_M12 = 2 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
							Instruction.Displacement.Disp32 += *P_index;
							P_index++;
						}
						Instruction.Immediate.Imme16 = *(WORD *)P_index;        //0x66前缀使得立即数变为16位
						P_index += 2;
						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;
					}
				case _sib | _disp32:  //第三个字节modrm具有sib|disp32属性
					Instruction.SIB = *P_index;
					P_index++;
					FillMODR_M12345(Instruction);
					if (Instruction.MODR_M12 != 0 && Instruction.MODR_M12 != 1 && Instruction.MODR_M12 != 2)   //第四个字节sib没有displacement
					{
						Instruction.Immediate.Imme16 = *(WORD *)P_index;        //0x66前缀使得立即数变为16位
						P_index += 2;
						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;

					}
					else
					{
						//第四个字节sib有displacement
						if (Instruction.MODR_M12 == 0 && Instruction.SIB_Base == 5)//modrm和sib都有displacement
						{
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
							Instruction.Displacement.Disp32 += *(DWORD *)P_index;
							P_index += 4;
						}
						else if (Instruction.MODR_M12 == 1 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp32 = *P_index;
							P_index++;
							Instruction.Displacement.Disp32 += *(DWORD *)P_index;
							P_index += 4;
						}
						else if (Instruction.MODR_M12 = 2 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
							Instruction.Displacement.Disp32 += *(DWORD *)P_index;
							P_index += 4;
						}
						Instruction.Immediate.Imme16 = *(WORD *)P_index;        //0x66前缀使得立即数变为16位
						P_index += 2;
						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;
					}
				}
			}
			else //没有0x66前缀
{
          switch (_32Bit_ModRM[Instruction.ModR_M])
			{
			case _nomore:  //第三个字节modrm具有nomore属性 
				Instruction.Immediate.Imme32 = *(DWORD *)P_index;      
				P_index += 4;
				Instruction.InstructionLength = P_index - ArrayofData;         //计算指令占的字节数
				return TRUE;

			case _disp8:   //第三个字节modrm具有disp8属性
				Instruction.Displacement.Disp8 = *P_index;
				P_index++;
				Instruction.Immediate.Imme32 = *(DWORD *)P_index;      
				P_index += 4;
				Instruction.InstructionLength = P_index - ArrayofData;
				return TRUE;

			case _disp32: //第三个字节modrm具有disp32属性

				Instruction.Displacement.Disp32 = *(DWORD *)P_index;
				P_index += 4;
				Instruction.Immediate.Imme32 = *(DWORD *)P_index;       
				P_index += 4;
				Instruction.InstructionLength = P_index - ArrayofData;
				return TRUE;
			case _sib:  //第三个字节modrm具有sib属性

				Instruction.SIB = *P_index;
				P_index++;
				FillMODR_M12345(Instruction);
				if (Instruction.MODR_M12 != 0 && Instruction.MODR_M12 != 1 && Instruction.MODR_M12 != 2)   //第四个字节sib没有displacement
				{
					Instruction.Immediate.Imme32 = *(DWORD *)P_index;
					P_index += 4;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;

				}
				else                                //第四个字节sib有displacement
				{
					FillSIB_Base(Instruction);     //填充SIB_Base
					if (Instruction.MODR_M12 == 0 && Instruction.SIB_Base == 5)
					{
						Instruction.Displacement.Disp32 = *(DWORD *)P_index;
						P_index += 4;
					}
					else if (Instruction.MODR_M12 == 1 && Instruction.SIB_Base == 5)
					{
						Instruction.Displacement.Disp8 = *P_index;
						P_index++;
					}
					else if (Instruction.MODR_M12 = 2 && Instruction.SIB_Base == 5)
					{
						Instruction.Displacement.Disp32 = *(DWORD *)P_index;
						P_index += 4;
					}
					Instruction.Immediate.Imme32 = *(DWORD *)P_index;
					P_index += 4;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;
				}
			case _sib | _disp8:    //第三个字节modrm具有sib|disp8属性 
				Instruction.SIB = *P_index;
				P_index++;
				FillMODR_M12345(Instruction);
				if (Instruction.MODR_M12 != 0 && Instruction.MODR_M12 != 1 && Instruction.MODR_M12 != 2)   //第四个字节sib没有displacement
				{
					Instruction.Immediate.Imme32 = *(DWORD *)P_index;
					P_index += 4;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;

				}
				else
				{
					FillSIB_Base(Instruction);
					//第四个字节sib有displacement
					if (Instruction.MODR_M12 == 0 && Instruction.SIB_Base == 5)//modrm和sib都有displacement
					{
						Instruction.Displacement.Disp32 = *(DWORD *)P_index;
						P_index += 4;
						Instruction.Displacement.Disp32 += *P_index;
						P_index++;
					}
					else if (Instruction.MODR_M12 == 1 && Instruction.SIB_Base == 5)
					{
						Instruction.Displacement.Disp16 = *P_index;
						P_index++;
						Instruction.Displacement.Disp16 += *P_index;
						P_index++;
					}
					else if (Instruction.MODR_M12 = 2 && Instruction.SIB_Base == 5)
					{
						Instruction.Displacement.Disp32 = *(DWORD *)P_index;
						P_index += 4;
						Instruction.Displacement.Disp32 += *P_index;
						P_index++;
					}
					Instruction.Immediate.Imme32 = *(DWORD *)P_index;
					P_index += 4;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;
				}
			case _sib | _disp32:  //第三个字节modrm具有sib|disp32属性
				Instruction.SIB = *P_index;
				P_index++;
				FillMODR_M12345(Instruction);
				if (Instruction.MODR_M12 != 0 && Instruction.MODR_M12 != 1 && Instruction.MODR_M12 != 2)   //第四个字节sib没有displacement
				{
					Instruction.Immediate.Imme32 = *(DWORD *)P_index;
					P_index += 4;
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;

				}
				else
				{
					//第四个字节sib有displacement
					if (Instruction.MODR_M12 == 0 && Instruction.SIB_Base == 5)//modrm和sib都有displacement
					{
						Instruction.Displacement.Disp32 = *(DWORD *)P_index;
						P_index += 4;
						Instruction.Displacement.Disp32 += *(DWORD *)P_index;
						P_index += 4;
					}
					else if (Instruction.MODR_M12 == 1 && Instruction.SIB_Base == 5)
					{
						Instruction.Displacement.Disp32 = *P_index;
						P_index++;
						Instruction.Displacement.Disp32 += *(DWORD *)P_index;
						P_index += 4;
					}
					else if (Instruction.MODR_M12 = 2 && Instruction.SIB_Base == 5)
					{
						Instruction.Displacement.Disp32 = *(DWORD *)P_index;
						P_index += 4;
						Instruction.Displacement.Disp32 += *(DWORD *)P_index;
						P_index += 4;
					}
					Instruction.Immediate.Imme32 = *(DWORD *)P_index;
					P_index += 4; 
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;
				}
			
          }
			

}
        case _group:                        //第二个字节opcode具有_group属性 
		case _modr_m | _group :             //第二个字节opcode具有modrm|_group属性
		case _modr_m | _group | _imme8:     //第二个字节opcode具有_modr_m | _group | _imme8属性 
		case _modr_m | _group | _imme32:    //第二个字节opcode具有 _modr_m | _group | _imme32属性 
			Instruction.Opecode = *P_index;
			P_index++;
			Instruction.ModR_M = *P_index;   //有group属性必有modr_m属性
			P_index++;
			FillMODR_M12345(Instruction);//用于查询group表
			if (Instruction.Prefix == 0x67)                                //具有0x67前缀,使用16位地址表
			{
				switch (_16Bit_ModRM[Instruction.ModR_M])
				{
				case _nomore:  //第三个字节modrm具有nomore属性 
					if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF6)   //需要立即数
					{
						Instruction.Immediate.Imme8 = *P_index;
						P_index++;
					}
					else if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF7)
					{
						Instruction.Immediate.Imme32 = *(DWORD *)P_index;
						P_index += 4;
					}
					if (OpCode_Table[Instruction.Opecode] & 0x00000040)//opcode表有imme8属性
					{
						if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
						{
							Instruction.Immediate.Imme8 = *P_index;
							P_index++;
						}
						else
						{
							Instruction.Immediate.Imme32 += *P_index;
							P_index++;
						}
					}
					if (OpCode_Table[Instruction.Opecode] & 0x00000100)//opcode表有imme32属性
					{
						if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
						{
							Instruction.Immediate.Imme32 = *(DWORD *)P_index;
							P_index += 4;
						}
						else
						{
							Instruction.Immediate.Imme32 += *(DWORD *)P_index;
							P_index += 4;
						}
					}

					Instruction.InstructionLength = P_index - ArrayofData;         //计算指令占的字节数
					return TRUE;

				case _disp8:   //第三个字节modrm具有disp8属性
					Instruction.Displacement.Disp8 = *P_index;
					P_index++;
					if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF6)   //需要立即数
					{
						Instruction.Immediate.Imme8 = *P_index;
						P_index++;
					}
					else if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF7)
					{
						Instruction.Immediate.Imme32 = *(DWORD *)P_index;
						P_index += 4;
					}
					if (OpCode_Table[Instruction.Opecode] & 0x00000040)//opcode表有imme8属性
					{
						if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
						{
							Instruction.Immediate.Imme8 = *P_index;
							P_index++;
						}
						else
						{
							Instruction.Immediate.Imme32 += *P_index;
							P_index++;
						}
					}
					if (OpCode_Table[Instruction.Opecode] & 0x00000100)//opcode表有imme32属性
					{
						if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
						{
							Instruction.Immediate.Imme32 = *(DWORD *)P_index;
							P_index += 4;
						}
						else
						{
							Instruction.Immediate.Imme32 += *(DWORD *)P_index;
							P_index += 4;
						}
					}

					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;

				case _disp16: //第三个字节modrm具有disp16属性

					Instruction.Displacement.Disp16 = *(WORD *)P_index;
					P_index += 2;
					if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF6)   //需要立即数
					{
						Instruction.Immediate.Imme8 = *P_index;
						P_index++;
					}
					else if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF7)
					{
						Instruction.Immediate.Imme32 = *(DWORD *)P_index;
						P_index += 4;
					}
					if (OpCode_Table[Instruction.Opecode] & 0x00000040)//opcode表有imme8属性
					{
						if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
						{
							Instruction.Immediate.Imme8 = *P_index;
							P_index++;
						}
						else
						{
							Instruction.Immediate.Imme32 += *P_index;
							P_index++;
						}
					}
					if (OpCode_Table[Instruction.Opecode] & 0x00000100)//opcode表有imme32属性
					{
						if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
						{
							Instruction.Immediate.Imme32 = *(DWORD *)P_index;
							P_index += 4;
						}
						else
						{
							Instruction.Immediate.Imme32 += *(DWORD *)P_index;
							P_index += 4;
						}
					}

					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;

				}
			}
			else  //没有0x67前缀                         
			{
				switch (_32Bit_ModRM[Instruction.ModR_M])
				{
				case _nomore:  //第三个字节modrm具有nomore属性
					if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF6)   //需要立即数
					{
						Instruction.Immediate.Imme8 = *P_index;
						P_index++;
					}
					else if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF7)
					{
						if (Instruction.Prefix == 0x66)
						{
							Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
							P_index += 2;
						}
						else
						{
							Instruction.Immediate.Imme32 = *(DWORD *)P_index;
							P_index += 4;
						}
					}
					if (OpCode_Table[Instruction.Opecode] & 0x00000040)//opcode表有imme8属性
					{
						if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
						{
							Instruction.Immediate.Imme8 = *P_index;
							P_index++;
						}
						else
						{
							Instruction.Immediate.Imme32 += *P_index;
							P_index++;
						}
					}
					if (OpCode_Table[Instruction.Opecode] & 0x00000100)//opcode表有imme32属性
					{
						if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
						{
							if (Instruction.Prefix == 0x66)
							{
								Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
								P_index += 2;
							}
							else
							{
								Instruction.Immediate.Imme32 = *(DWORD *)P_index;
								P_index += 4;
							}
						}
						else
						{ 
							if (Instruction.Prefix == 0x66)
							{
								Instruction.Immediate.Imme32 += *(WORD *)P_index;
								P_index += 2;
							}
							else
							{
								Instruction.Immediate.Imme32 += *(DWORD *)P_index;
								P_index += 4;
							}
						}
					}

					Instruction.InstructionLength = P_index - ArrayofData;         //计算指令占的字节数
					return TRUE;

				case _disp8:   //第三个字节modrm具有disp8属性
					Instruction.Displacement.Disp8 = *P_index;
					P_index++;
					if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF6)   //需要立即数
					{
						Instruction.Immediate.Imme8 = *P_index;
						P_index++;
					}
					else if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF7)
					{
						if (Instruction.Prefix == 0x66)
						{
							Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
							P_index += 2;
						}
						else
						{
							Instruction.Immediate.Imme32 = *(DWORD *)P_index;
							P_index += 4;
						}
					}
					if (OpCode_Table[Instruction.Opecode] & 0x00000040)//opcode表有imme8属性
					{
						if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
						{
							Instruction.Immediate.Imme8 = *P_index;
							P_index++;
						}
						else
						{
							Instruction.Immediate.Imme32 += *P_index;
							P_index++;
						}
					}
					if (OpCode_Table[Instruction.Opecode] & 0x00000100)//opcode表有imme32属性
					{
						if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
						{
							if (Instruction.Prefix == 0x66)
							{
								Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
								P_index += 2;
							}
							else
							{
								
								Instruction.Immediate.Imme32 = *(DWORD *)P_index;
								P_index += 4;
							}
						}
						else
						{
							if (Instruction.Prefix == 0x66)
							{
								Instruction.Immediate.Imme32 += *(WORD *)P_index;
								P_index += 2;
							}
							else
							{
								Instruction.Immediate.Imme32 += *(DWORD *)P_index;
								P_index += 4;
							}
						}
					}
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;

				case _disp32: //第三个字节modrm具有disp32属性

					Instruction.Displacement.Disp32 = *(DWORD *)P_index;
					P_index += 4;
					if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF6)   //需要立即数
					{
						Instruction.Immediate.Imme8 = *P_index;
						P_index++;
					}
					else if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF7)
					{
						if (Instruction.Prefix == 0x66)
						{
							Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
							P_index += 2;
						}
						else
						{
							Instruction.Immediate.Imme32 = *(DWORD *)P_index;
							P_index += 4;
						}
					}
					if (OpCode_Table[Instruction.Opecode] & 0x00000040)//opcode表有imme8属性
					{
						if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
						{
							Instruction.Immediate.Imme8 = *P_index;
							P_index++;
						}
						else
						{
							Instruction.Immediate.Imme32 += *P_index;
							P_index++;
						}
					}
					if (OpCode_Table[Instruction.Opecode] & 0x00000100)//opcode表有imme32属性
					{
						if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
						{
							if (Instruction.Prefix == 0x66)
							{
								Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
								P_index += 2;
							}
							else
							{

								Instruction.Immediate.Imme32 = *(DWORD *)P_index;
								P_index += 4;
							}
						}
						else
						{
							if (Instruction.Prefix == 0x66)
							{
								Instruction.Immediate.Imme32 += *(WORD *)P_index;
								P_index += 2;
							}
							else
							{
								Instruction.Immediate.Imme32 += *(DWORD *)P_index;
								P_index += 4;
							}
						}
					}
					Instruction.InstructionLength = P_index - ArrayofData;
					return TRUE;

				case _sib:  //第三个字节modrm具有sib属性

					Instruction.SIB = *P_index;
					P_index++;
					FillMODR_M12345(Instruction);
					if (Instruction.MODR_M12 != 0 && Instruction.MODR_M12 != 1 && Instruction.MODR_M12 != 2)   //第四个字节sib没有displacement
					{
						if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF6)   //需要立即数
						{
							Instruction.Immediate.Imme8 = *P_index;
							P_index++;
						}
						else if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF7)
						{
							if (Instruction.Prefix == 0x66)
							{
								Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
								P_index += 2;
							}
							else
							{
								Instruction.Immediate.Imme32 = *(DWORD *)P_index;
								P_index += 4;
							}
						}
						if (OpCode_Table[Instruction.Opecode] & 0x00000040)//opcode表有imme8属性
						{
							if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
							{
								Instruction.Immediate.Imme8 = *P_index;
								P_index++;
							}
							else
							{
								Instruction.Immediate.Imme32 += *P_index;
								P_index++;
							}
						}
						if (OpCode_Table[Instruction.Opecode] & 0x00000100)//opcode表有imme32属性
						{
							if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
							{
								if (Instruction.Prefix == 0x66)
								{
									Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
									P_index += 2;
								}
								else
								{

									Instruction.Immediate.Imme32 = *(DWORD *)P_index;
									P_index += 4;
								}
							}
							else
							{
								if (Instruction.Prefix == 0x66)
								{
									Instruction.Immediate.Imme32 += *(WORD *)P_index;
									P_index += 2;
								}
								else
								{
									Instruction.Immediate.Imme32 += *(DWORD *)P_index;
									P_index += 4;
								}
							}
						}
						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;


					}
					else                                //第四个字节sib有displacement
					{
						FillSIB_Base(Instruction);     //填充SIB_Base
						if (Instruction.MODR_M12 == 0 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
						}
						else if (Instruction.MODR_M12 == 1 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp8 = *P_index;
							P_index++;
						}
						else if (Instruction.MODR_M12 = 2 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
						}
						if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF6)   //需要立即数
						{
							Instruction.Immediate.Imme8 = *P_index;
							P_index++;
						}
						else if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF7)
						{
							if (Instruction.Prefix == 0x66)
							{
								Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
								P_index += 2;
							}
							else
							{
								Instruction.Immediate.Imme32 = *(DWORD *)P_index;
								P_index += 4;
							}
						}
						if (OpCode_Table[Instruction.Opecode] & 0x00000040)//opcode表有imme8属性
						{
							if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
							{
								Instruction.Immediate.Imme8 = *P_index;
								P_index++;
							}
							else
							{
								Instruction.Immediate.Imme32 += *P_index;
								P_index++;
							}
						}
						if (OpCode_Table[Instruction.Opecode] & 0x00000100)//opcode表有imme32属性
						{
							if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
							{
								if (Instruction.Prefix == 0x66)
								{
									Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
									P_index += 2;
								}
								else
								{

									Instruction.Immediate.Imme32 = *(DWORD *)P_index;
									P_index += 4;
								}
							}
							else
							{
								if (Instruction.Prefix == 0x66)
								{
									Instruction.Immediate.Imme32 += *(WORD *)P_index;
									P_index += 2;
								}
								else
								{
									Instruction.Immediate.Imme32 += *(DWORD *)P_index;
									P_index += 4;
								}
							}
						}
						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;
					}
				case _sib | _disp8:    //第三个字节modrm具有sib|disp8属性 
					Instruction.SIB = *P_index;
					P_index++;
					FillMODR_M12345(Instruction);
					if (Instruction.MODR_M12 != 0 && Instruction.MODR_M12 != 1 && Instruction.MODR_M12 != 2)   //第四个字节sib没有displacement
					{
						if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF6)   //需要立即数
						{
							Instruction.Immediate.Imme8 = *P_index;
							P_index++;
						}
						else if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF7)
						{
							if (Instruction.Prefix == 0x66)
							{
								Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
								P_index += 2;
							}
							else
							{
								Instruction.Immediate.Imme32 = *(DWORD *)P_index;
								P_index += 4;
							}
						}
						if (OpCode_Table[Instruction.Opecode] & 0x00000040)//opcode表有imme8属性
						{
							if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
							{
								Instruction.Immediate.Imme8 = *P_index;
								P_index++;
							}
							else
							{
								Instruction.Immediate.Imme32 += *P_index;
								P_index++;
							}
						}
						if (OpCode_Table[Instruction.Opecode] & 0x00000100)//opcode表有imme32属性
						{
							if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
							{
								if (Instruction.Prefix == 0x66)
								{
									Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
									P_index += 2;
								}
								else
								{

									Instruction.Immediate.Imme32 = *(DWORD *)P_index;
									P_index += 4;
								}
							}
							else
							{
								if (Instruction.Prefix == 0x66)
								{
									Instruction.Immediate.Imme32 += *(WORD *)P_index;
									P_index += 2;
								}
								else
								{
									Instruction.Immediate.Imme32 += *(DWORD *)P_index;
									P_index += 4;
								}
							}
						}
						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;
					}
					else
					{
						FillSIB_Base(Instruction);
						//第四个字节sib有displacement
						if (Instruction.MODR_M12 == 0 && Instruction.SIB_Base == 5)//modrm和sib都有displacement
						{
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
							Instruction.Displacement.Disp32 += *P_index;
							P_index++;
						}
						else if (Instruction.MODR_M12 == 1 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp16 = *P_index;
							P_index++;
							Instruction.Displacement.Disp16 += *P_index;
							P_index++;
						}
						else if (Instruction.MODR_M12 = 2 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
							Instruction.Displacement.Disp32 += *P_index;
							P_index++;
						}
						if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF6)   //需要立即数
						{
							Instruction.Immediate.Imme8 = *P_index;
							P_index++;
						}
						else if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF7)
						{
							if (Instruction.Prefix == 0x66)
							{
								Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
								P_index += 2;
							}
							else
							{
								Instruction.Immediate.Imme32 = *(DWORD *)P_index;
								P_index += 4;
							}
						}
						if (OpCode_Table[Instruction.Opecode] & 0x00000040)//opcode表有imme8属性
						{
							if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
							{
								Instruction.Immediate.Imme8 = *P_index;
								P_index++;
							}
							else
							{
								Instruction.Immediate.Imme32 += *P_index;
								P_index++;
							}
						}
						if (OpCode_Table[Instruction.Opecode] & 0x00000100)//opcode表有imme32属性
						{
							if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
							{
								if (Instruction.Prefix == 0x66)
								{
									Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
									P_index += 2;
								}
								else
								{

									Instruction.Immediate.Imme32 = *(DWORD *)P_index;
									P_index += 4;
								}
							}
							else
							{
								if (Instruction.Prefix == 0x66)
								{
									Instruction.Immediate.Imme32 += *(WORD *)P_index;
									P_index += 2;
								}
								else
								{
									Instruction.Immediate.Imme32 += *(DWORD *)P_index;
									P_index += 4;
								}
							}
						}
						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;
					}
				case _sib | _disp32:  //第三个字节modrm具有sib|disp32属性
					Instruction.SIB = *P_index;
					P_index++;
					FillMODR_M12345(Instruction);
					if (Instruction.MODR_M12 != 0 && Instruction.MODR_M12 != 1 && Instruction.MODR_M12 != 2)   //第四个字节sib没有displacement
					{
						if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF6)   //需要立即数
						{
							Instruction.Immediate.Imme8 = *P_index;
							P_index++;
						}
						else if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF7)
						{
							if (Instruction.Prefix == 0x66)
							{
								Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
								P_index += 2;
							}
							else
							{
								Instruction.Immediate.Imme32 = *(DWORD *)P_index;
								P_index += 4;
							}
						}
						if (OpCode_Table[Instruction.Opecode] & 0x00000040)//opcode表有imme8属性
						{
							if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
							{
								Instruction.Immediate.Imme8 = *P_index;
								P_index++;
							}
							else
							{
								Instruction.Immediate.Imme32 += *P_index;
								P_index++;
							}
						}
						if (OpCode_Table[Instruction.Opecode] & 0x00000100)//opcode表有imme32属性
						{
							if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
							{
								if (Instruction.Prefix == 0x66)
								{
									Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
									P_index += 2;
								}
								else
								{

									Instruction.Immediate.Imme32 = *(DWORD *)P_index;
									P_index += 4;
								}
							}
							else
							{
								if (Instruction.Prefix == 0x66)
								{
									Instruction.Immediate.Imme32 += *(WORD *)P_index;
									P_index += 2;
								}
								else
								{
									Instruction.Immediate.Imme32 += *(DWORD *)P_index;
									P_index += 4;
								}
							}
						}
						Instruction.Displacement.Disp32 = *(DWORD *)P_index;
						P_index += 4;
						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;

					}
					else
					{
						//第四个字节sib有displacement
						if (Instruction.MODR_M12 == 0 && Instruction.SIB_Base == 5)//modrm和sib都有displacement
						{
							if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF6)   //需要立即数
							{
								Instruction.Immediate.Imme8 = *P_index;
								P_index++;
							}
							else if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF7)
							{
								Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
								P_index += 2;
							}
							if (OpCode_Table[Instruction.Opecode] & 0x00000040)//opcode表有imme8属性
							{
								if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
								{
									Instruction.Immediate.Imme8 = *P_index;
									P_index++;
								}
								else
								{
									Instruction.Immediate.Imme32 += *P_index;
									P_index++;
								}
							}
							if (OpCode_Table[Instruction.Opecode] & 0x00000100)//opcode表有imme32属性
							{
								if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
								{
									if (Instruction.Prefix == 0x66)
									{
										Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
										P_index += 2;
									}
									else
									{
										Instruction.Immediate.Imme32 = *(DWORD *)P_index;
										P_index += 4;
									}
								}
								else
								{
									if (Instruction.Prefix == 0x66)
									{
										Instruction.Immediate.Imme32 += *(WORD *)P_index;
										P_index += 2;
									}
									else 
									{
										Instruction.Immediate.Imme32 += *(DWORD *)P_index;
										P_index += 4;


										 }
								}
							}
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
							Instruction.Displacement.Disp32 += *(DWORD *)P_index;
							P_index += 4;
						}
						else if (Instruction.MODR_M12 == 1 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp32 = *P_index;
							P_index++;
							Instruction.Displacement.Disp32 += *(DWORD *)P_index;
							P_index += 4;
						}
						else if (Instruction.MODR_M12 = 2 && Instruction.SIB_Base == 5)
						{
							Instruction.Displacement.Disp32 = *(DWORD *)P_index;
							P_index += 4;
							Instruction.Displacement.Disp32 += *(DWORD *)P_index;
							P_index += 4;
						}
						if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF6)   //需要立即数
						{
							Instruction.Immediate.Imme8 = *P_index;
							P_index++;
						}
						else if (Instruction.ModR_M345 == 0 && Instruction.Opecode == 0xF7)
						{

							if (Instruction.Prefix == 0x66)
							{
								Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
								P_index += 2;
							}
							else
							{
								Instruction.Immediate.Imme32 = *(DWORD *)P_index;
								P_index += 4;
							}
						}
						if (OpCode_Table[Instruction.Opecode] & 0x00000040)//opcode表有imme8属性
						{
							if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
							{
								Instruction.Immediate.Imme8 = *P_index;
								P_index++;
							}
							else
							{
								Instruction.Immediate.Imme32 += *P_index;
								P_index++;
							}
						}
						if (OpCode_Table[Instruction.Opecode] & 0x00000100)//opcode表有imme32属性
						{
							if (Instruction.Immediate.Imme32 == INVALID)//前面没有填充立即数
							{
								if (Instruction.Prefix == 0x66)
								{
									Instruction.Immediate.Imme16 = *(WORD *)P_index;     //0x66前缀
									P_index += 2;
								}
								else
								{
									Instruction.Immediate.Imme32 = *(DWORD *)P_index;
									P_index += 4;
								}
							}
							else
							{
								if (Instruction.Prefix == 0x66)
								{
									Instruction.Immediate.Imme32 += *(WORD *)P_index;
									P_index += 2;
								}
								else
								{

									Instruction.Immediate.Imme32 += *(DWORD *)P_index;
									P_index += 4;
								}
							}
						}
						Instruction.InstructionLength = P_index - ArrayofData;
						return TRUE;
					}

				}

			}
			
	case _imme16|_imme32:   
		Instruction.Opecode = *P_index;
		Instruction.Immediate.Imme16 = *(WORD *)P_index;
		P_index += 2;
		Instruction.Displacement.Disp32 = *(DWORD *)P_index;
		Instruction.InstructionLength = P_index - ArrayofData;
		return true;
	case _imme16|_imme8:
		Instruction.Opecode = *P_index;
		Instruction.Immediate.Imme16 = *(WORD *)P_index;
		P_index += 2;
		Instruction.Displacement.Disp8 =  *P_index;
		Instruction.InstructionLength = P_index - ArrayofData;
		return true;

    }
	
	}
}

int  main()
{

                                    //用于存储解析完成的指令的各个部分
	byte ArrayOfData[50] = { 0x46 ,0xf6, 0xE4, 0xF0, 0x44, 0x6E, 0xB4};        //存储未解析的1字节大小数组，以十六进制表示
   	FillInstruction(ArrayOfData, Instruction);
	Print_Instrution(Instruction);
	InitStringTable(Instruction);	
	system("pause");
	return 0;
}
