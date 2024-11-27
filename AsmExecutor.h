#pragma once
#include "Cash.h"
#include "Parser.h"
#include <vector>
#include <string>
#include <map>
#include <cstdint>

class AsmExecutor {
public:
    AsmExecutor(std::vector<std::string> instructions): PC(0), instructions(instructions),
                                                        cache(memory), cache_p(memory) {
        InitializeRegisters();
    }

    void Execute(int c_max = 1000000000);

    float PrintPercentLRU() const;

    float PrintPercentPLRU() const;

private:
    Memory memory;
    CacheLRU cache;
    CachePLRU cache_p;
    std::vector<std::string> instructions;
    std::map<std::string, int> registers;
    int PC;

    struct InstructionRRR {
        std::string opcode;
        std::string reg1;
        std::string reg2;
        std::string reg3;
    };

    struct InstructionRRI {
        std::string opcode;
        std::string reg1;
        std::string reg2;
        int reg3;
    };

    struct InstructionRIR {
        std::string opcode;
        std::string reg1;
        int reg2;
        std::string reg3;
    };

    struct InstructionRI {
        std::string opcode;
        std::string reg1;
        int reg2;
    };

    std::vector<std::string> ParseInstruction(const std::string& instruction);

    void ExecuteInstruction();

    void Add(const std::string& rd, const std::string& r1, const std::string& r2);

    void Sub(const std::string& rd, const std::string& r1, const std::string& r2);

    void Xor(const std::string& rd, const std::string& r1, const std::string& r2);

    void Or(const std::string& rd, const std::string& r1, const std::string& r2);

    void And(const std::string& rd, const std::string& r1, const std::string& r2);

    void Sll(const std::string& rd, const std::string& r1, const std::string& r2);

    void Srl(const std::string& rd, const std::string& r1, const std::string& r2);

    void Sra(const std::string& rd, const std::string& r1, const std::string& r2);

    void Slt(const std::string& rd, const std::string& r1, const std::string& r2);

    void Sltu(const std::string& rd, const std::string& r1, const std::string& r2);

    void Addi(const std::string& rd, const std::string& r, int x);

    void Xori(const std::string& rd, const std::string& r, int x);

    void Ori(const std::string& rd, const std::string& r, int x);

    void Andi(const std::string& rd, const std::string& r, int x);

    void Slli(const std::string& rd, const std::string& r, int x);

    void Srli(const std::string& rd, const std::string& r, int x);

    void Srai(const std::string& rd, const std::string& r, int x);

    void Slti(const std::string& rd, const std::string& r, int x);

    void Sltiu(const std::string& rd, const std::string& r, int x);

    void Lb(const std::string& rd, int x, const std::string& r);

    void Lh(const std::string& rd, int x, const std::string& r);

    void Lw(const std::string& rd, int x, const std::string& r);

    void Lbu(const std::string& rd, int x, const std::string& r);

    void Lhu(const std::string& rd, int x, const std::string& r);

    void Lui(const std::string& rd, int x);

    void Sb(const std::string& rd, int x, const std::string& r);

    void Sh(const std::string& rd, int x, const std::string& r);

    void Sw(const std::string& rd, int x, const std::string& r);

    void Beq(const std::string& r1, const std::string& r2, int x);

    void Bne(const std::string& r1, const std::string& r2, int x);

    void Blt(const std::string& r1, const std::string& r2, int x);

    void Bge(const std::string& r1, const std::string& r2, int x);

    void Bltu(const std::string& r1, const std::string& r2, int x);

    void Bgeu(const std::string& r1, const std::string& r2, int x);

    void Jal(const std::string& rd, int x);

    void Jalr(const std::string& rd, const std::string& r, int x);

    void Auipc(const std::string& rd, int x);

    void Mul(const std::string& rd, const std::string& r1, const std::string& r2);

    void Mulh(const std::string& rd, const std::string& r1, const std::string& r2);

    void Mulsu(const std::string& rd, const std::string& r1, const std::string& r2);

    void Mulu(const std::string& rd, const std::string& r1, const std::string& r2);

    void Div(const std::string& rd, const std::string& r1, const std::string& r2);

    void Divu(const std::string& rd, const std::string& r1, const std::string& r2);

    void Rem(const std::string& rd, const std::string& r1, const std::string& r2);

    void Remu(const std::string& rd, const std::string& r1, const std::string& r2);

    void InitializeRegisters();
};