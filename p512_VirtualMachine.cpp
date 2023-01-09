#include <iostream>
#include <Windows.h>
#define REGTYPE_32B 0
#define REGTYPE_16B 1
#define REGTYPE_8 2
//#define REGTYPE_IP 3
//#define REGTYPE_IPPart 4
//#define REGTYPE_Segment 5

#define MODRMB unsigned char
#define Reg_V unsigned char
#define OPCODE unsigned char
#define POPCODE OPCODE*
#define OSIZE unsigned char
#define REG_IP int
#define REG_IPPart short*
#define REG_General int
#define REG_GeneralPart short*
#define REG_GeneralPartPart unsigned char*
#define REG_Segment short
#define EFLAG bool

namespace VM {

    #pragma region REGS
    REG_General EAX = 0;
    REG_General EBX = 0;
    REG_General ECX = 0;
    REG_General EDX = 0;
    REG_GeneralPart AX = (REG_GeneralPart)&EAX;
    REG_GeneralPart BX = (REG_GeneralPart)&EBX;
    REG_GeneralPart CX = (REG_GeneralPart)&ECX;
    REG_GeneralPart DX = (REG_GeneralPart)&EDX;
    REG_GeneralPartPart AH = (REG_GeneralPartPart)&EAX + 2;
    REG_GeneralPartPart AL = (REG_GeneralPartPart)&EAX + 3;
    REG_GeneralPartPart BH = (REG_GeneralPartPart)&EBX + 2;
    REG_GeneralPartPart BL = (REG_GeneralPartPart)&EBX + 3;
    REG_GeneralPartPart CH = (REG_GeneralPartPart)&ECX + 2;
    REG_GeneralPartPart CL = (REG_GeneralPartPart)&ECX + 3;
    REG_GeneralPartPart DH = (REG_GeneralPartPart)&EDX + 2;
    REG_GeneralPartPart DL = (REG_GeneralPartPart)&EDX + 3;
    REG_IP ESI = 0;
    REG_IP EDI = 0;
    REG_IP EBP = 0;
    REG_IP ESP = 0;
    REG_IPPart SI = (REG_IPPart)&ESI;
    REG_IPPart DI = (REG_IPPart)&EDI;
    REG_IPPart BI = (REG_IPPart)&EBI;
    REG_IPPart SP = (REG_IPPart)&ESP;

    REG_Segment CS = 0;
    REG_Segment DS = 0;
    REG_Segment ES = 0;
    REG_Segment FS = 0;
    REG_Segment GS = 0;
    REG_Segment SS = 0;
    #pragma endregion

    #pragma region REGS_Value
    Reg_V vEAX = 0x0;
    Reg_V vECX = 0x1;
    Reg_V vEDX = 0x2;
    Reg_V vEBX = 0x3;
    Reg_V vESP = 0x4;
    Reg_V vEBP = 0x5;
    Reg_V vESI = 0x6;
    Reg_V vEDI = 0x7;
    #pragma endregion

    #pragma region EFLAGS
    EFLAG CF = 0;
    EFLAG PF = 0;
    EFLAG AF = 0;
    EFLAG ZF = 0;
    EFLAG SF = 0;
    EFLAG TF = 0;
    EFLAG IF = 0;
    EFLAG DF = 0;
    EFLAG OF = 0;
    EFLAG IOPL = 0;
    EFLAG NT = 0;
    EFLAG RF = 0;
    EFLAG VM = 0;
    EFLAG AC = 0;
    EFLAG VIF = 0;
    EFLAG VIP = 0;
    EFLAG ID = 0;
    #pragma endregion


    #pragma region modRM_Modes
    MODRMB MODRM_MOD_MOV = 0xC0;
    MODRMB MODRM_MOD_MOV_RegIntoReg_RegReg = 0xC0;
    MODRMB MODRM_MOD_MOV_RegIntoReg_PtrReg = 0x0;
    MODRMB MODRM_RM_MOV_RegIntoReg_reg1 = 0x7;
    MODRMB MODRM_RM_MOV_RegIntoReg_reg2 = 0x38;
    #pragma endregion

    #pragma region PREFIXES
    OPCODE PREFIX_MOV_RegIntoReg_RegGeneralPart = 0x66;
    #pragma endregion

    #pragma region OPCODES
    OPCODE OPCODE_MOV_RegIntoReg = 0x89;
    OSIZE OPCODE_MOV_RegIntoReg_Size = 0xFF;
    #pragma endregion

    REG_IP EIP = 0;

    void* GetPtrToRegister(Reg_V reg, char type) {
        if (type == REGTYPE_32B) {
            if (reg == vEAX) return &EAX;
            if (reg == vEBX) return &EBX;
            if (reg == vECX) return &ECX;
            if (reg == vEDX) return &EDX;
            if (reg == vESP) return &ESP;
            if (reg == vEBP) return &EBP;
            if (reg == vESI) return &ESI;
            if (reg == vEDI) return &EDI;
        }
        else if (type == REGTYPE_16B) {
            if (reg == vEAX) return &AX;
            if (reg == vEBX) return &BX;
            if (reg == vECX) return &CX;
            if (reg == vEDX) return &DX;
            if (reg == vESP) return &SP;
            if (reg == vESI) return &SI;
            if (reg == vEDI) return &DI;
            if (reg == vEBP) return &BI;
        }
        
    }

    void VM_ExecInstruction(POPCODE instruction) {
        #pragma region MOV
        if ((instruction[0] & OPCODE_MOV_RegIntoReg_Size) == OPCODE_MOV_RegIntoReg) {
            int* value1 = 0;
            int* value2 = 0;
            MODRMB modRMByte = instruction[1];
            MODRMB modByte = (modRMByte & MODRM_MOD_MOV);
            if (modByte == MODRM_MOD_MOV_RegIntoReg_RegReg) { // without []
                value1 = (int*)GetPtrToRegister(modRMByte & MODRM_RM_MOV_RegIntoReg_reg1, REGTYPE_32B);
                value2 = (int*)GetPtrToRegister((modRMByte & MODRM_RM_MOV_RegIntoReg_reg2) >> 3, REGTYPE_32B);
            }
            else if (modByte == MODRM_MOD_MOV_RegIntoReg_PtrReg) {
                value1 = (int*)(((int*)GetPtrToRegister(modRMByte & MODRM_RM_MOV_RegIntoReg_reg1, REGTYPE_32B))[0]);
                value2 = (int*)GetPtrToRegister((modRMByte & MODRM_RM_MOV_RegIntoReg_reg2) >> 3, REGTYPE_32B);
            }
            *value1 = *value2;
        }
        #pragma endregion
    }
}

int main()
{
    
}
