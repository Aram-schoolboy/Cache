#include "AsmExecutor.h"


void AsmExecutor::Execute(int c_max) {
    int c = 0;
    while (PC / 4 < instructions.size() && c < c_max) {
        ExecuteInstruction();
        ++c;
    }
}

float AsmExecutor::PrintPercentLRU() const {
    float a = cache.GetHitsCount() * 100;
    float b = cache.GetRequestsCount();
    return a / b;
}

float AsmExecutor::PrintPercentPLRU() const {
    float a = cache_p.GetHitsCount() * 100;
    float b = cache_p.GetRequestsCount();
    return a / b;
}

std::vector<std::string> AsmExecutor::ParseInstruction(const std::string& instruction) {
    std::vector<std::string> instruction_elements;

    std::string word;
    for (char i : instruction) {
        if (i == '#') { break; }
        if ((i == ' ' || i == ',' || i == '\t') && (!word.empty())) {
            if (word == "fp") { word = "s0"; }
            instruction_elements.push_back(ToLower(word));
            word = "";
        } else {
            if (i != ' ' && i != ',' && i != '\t')
                word += i;
        }
    }
    if (!word.empty()) { instruction_elements.push_back(ToLower(word)); }

    return instruction_elements;
}

void AsmExecutor::ExecuteInstruction() {
    int old_PC = PC;
    std::vector<std::string> instruction_elements = ParseInstruction(instructions[PC / 4]);
    std::string opcode = instruction_elements[0];

    static const std::set<std::string> reg_reg_reg = {
            "add", "sub", "xor", "or", "and",
            "sll", "srl", "sra", "slt", "sltu",
            "mul", "mulh", "mulsu", "mulu", "div",
            "divu", "rem", "remu",
    };

    static const std::set<std::string> reg_reg_int = {
            "addi", "xori", "ori", "andi", "slli",
            "srli", "srai", "slti", "sltiu", "beq",
            "bne", "blt", "bge", "bltu", "bgeu",
            "jalr",
    };

    static const std::set<std::string> reg_int_reg {
            "lb", "lh", "lw", "lbu", "lhu",
            "sb", "sh", "sw",
    };

    static const std::set<std::string> reg_int {
            "jal", "lui", "auipc",
    };

    if (reg_reg_reg.contains(opcode)) {
        InstructionRRR instruction(instruction_elements[0],
                                   instruction_elements[1],
                                   instruction_elements[2],
                                   instruction_elements[3]);
        if (opcode == "add") {
            Add(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "sub") {
            Sub(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "xor") {
            Xor(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "or") {
            Or(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "and") {
            And(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "sll") {
            Sll(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "srl") {
            Srl(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "sra") {
            Sra(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "slt") {
            Slt(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "sltu") {
            Sltu(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "mul") {
            Mul(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "mulh") {
            Mulh(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "mulsu") {
            Mulsu(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "mulu") {
            Mulu(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "div") {
            Div(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "divu") {
            Divu(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "rem") {
            Rem(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "remu") {
            Remu(instruction.reg1, instruction.reg2, instruction.reg3);
        }

    } else if (reg_reg_int.contains(opcode)) {
        InstructionRRI instruction(instruction_elements[0],
                                   instruction_elements[1],
                                   instruction_elements[2],
                                   IntegersParsing::GetNumberFromString(instruction_elements[3]));

        if (opcode == "addi") {
            Addi(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "xori") {
            Xori(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "ori") {
            Ori(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "andi") {
            Andi(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "slli") {
            Slli(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "srli") {
            Srli(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "srai") {
            Srai(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "slti") {
            Slti(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "sltiu") {
            Sltiu(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "beq") {
            Beq(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "bne") {
            Bne(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "blt") {
            Blt(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "bge") {
            Bge(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "bltu") {
            Bltu(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "bgeu") {
            Bgeu(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "jalr") {
            Jalr(instruction.reg1, instruction.reg2, instruction.reg3);
        }

    } else if (reg_int_reg.contains(opcode)) {
        InstructionRIR instruction(instruction_elements[0],
                                   instruction_elements[1],
                                   IntegersParsing::GetNumberFromString(instruction_elements[2]),
                                   instruction_elements[3]);
        if (opcode == "lb") {
            Lb(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "lh") {
            Lh(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "lw") {
            Lw(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "lbu") {
            Lbu(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "lhu") {
            Lhu(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "sb") {
            Sb(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "sh") {
            Sh(instruction.reg1, instruction.reg2, instruction.reg3);
        } else if (opcode == "sw") {
            Sw(instruction.reg1, instruction.reg2, instruction.reg3);
        }

    } else {
        InstructionRI instruction(instruction_elements[0],
                                  instruction_elements[1],
                                  IntegersParsing::GetNumberFromString(instruction_elements[2]));
        if (opcode == "jal") {
            Jal(instruction.reg1, instruction.reg2);
        } else if (opcode == "lui") {
            Lui(instruction.reg1, instruction.reg2);
        } else if (opcode == "auipc") {
            Auipc(instruction.reg1, instruction.reg2);
        }
    }
    if (old_PC == PC) { PC += 4; }
}

void AsmExecutor::InitializeRegisters() {
    std::map<std::string, int> temp = {
            std::make_pair("zero", 0),
            std::make_pair("ra", 0),
            std::make_pair("sp", 0),
            std::make_pair("gp", 0),
            std::make_pair("tp", 0),
            std::make_pair("t0", 0),
            std::make_pair("t1", 0),
            std::make_pair("t2", 0),
            std::make_pair("t3", 0),
            std::make_pair("t4", 0),
            std::make_pair("t5", 0),
            std::make_pair("t6", 0),
            std::make_pair("s0", 0),
            std::make_pair("s1", 0),
            std::make_pair("s2", 0),
            std::make_pair("s3", 0),
            std::make_pair("s4", 0),
            std::make_pair("s5", 0),
            std::make_pair("s6", 0),
            std::make_pair("s7", 0),
            std::make_pair("s8", 0),
            std::make_pair("s9", 0),
            std::make_pair("s10", 0),
            std::make_pair("s11", 0),
            std::make_pair("a0", 0),
            std::make_pair("a1", 0),
            std::make_pair("a2", 0),
            std::make_pair("a3", 0),
            std::make_pair("a4", 0),
            std::make_pair("a5", 0),
            std::make_pair("a6", 0),
            std::make_pair("a7", 0)
    };
    registers = temp;
}


void AsmExecutor::Add(const std::string& rd, const std::string& r1, const std::string& r2) {
    registers[rd] = registers[r1] + registers[r2];
}

void AsmExecutor::Sub(const std::string& rd, const std::string& r1, const std::string& r2) {
    registers[rd] = registers[r1] - registers[r2];
}

void AsmExecutor::Xor(const std::string& rd, const std::string& r1, const std::string& r2) {
    registers[rd] = registers[r1] ^ registers[r2];
}

void AsmExecutor::Or(const std::string& rd, const std::string& r1, const std::string& r2) {
    registers[rd] = registers[r1] | registers[r2];
}

void AsmExecutor::And(const std::string& rd, const std::string& r1, const std::string& r2) {
    registers[rd] = registers[r1] & registers[r2];
}

void AsmExecutor::Sll(const std::string& rd, const std::string& r1, const std::string& r2) {
    registers[rd] = registers[r1] << registers[r2];
}

void AsmExecutor::Srl(const std::string& rd, const std::string& r1, const std::string& r2) {
    registers[rd] = registers[r1] >> registers[r2];
}

void AsmExecutor::Sra(const std::string& rd, const std::string& r1, const std::string& r2) {
    Srl(rd, r1, r2);
}

void AsmExecutor::Slt(const std::string& rd, const std::string& r1, const std::string& r2) {
    registers[rd] = registers[r1] < registers[r2] ? 1 : 0;
}

void AsmExecutor::Sltu(const std::string& rd, const std::string& r1, const std::string& r2) {
    registers[rd] = std::abs(registers[r1]) < std::abs(registers[r2]) ? 1 : 0;
}

void AsmExecutor::Addi(const std::string& rd, const std::string& r, int x) {
    registers[rd] = registers[r] + x;
}

void AsmExecutor::Xori(const std::string& rd, const std::string& r, int x) {
    registers[rd] = registers[r] ^ x;
}

void AsmExecutor::Ori(const std::string& rd, const std::string& r, int x) {
    registers[rd] = registers[r] | x;
}

void AsmExecutor::Andi(const std::string& rd, const std::string& r, int x) {
    registers[rd] = registers[r] & x;
}

void AsmExecutor::Slli(const std::string& rd, const std::string& r, int x) {
    registers[rd] = registers[r] << (x & 0x1f);
}

void AsmExecutor::Srli(const std::string& rd, const std::string& r, int x) {
    registers[rd] = registers[r] >> (x & 0x1f);
}

void AsmExecutor::Srai(const std::string& rd, const std::string& r, int x) {
    Srli(rd, r, x);
}

void AsmExecutor::Slti(const std::string& rd, const std::string& r, int x) {
    registers[rd] = registers[r] < x ? 1 : 0;
}

void AsmExecutor::Sltiu(const std::string& rd, const std::string& r, int x) {
    registers[rd] = std::abs(registers[r]) < std::abs(x) ? 1 : 0;
}

void AsmExecutor::Lb(const std::string& rd, int x, const std::string& r) {
    registers[rd] = cache.LoadByte(x + registers[r]);
    cache_p.LoadByte(x + registers[r]);
    // rd <- M[r + x][0:7]
}

void AsmExecutor::Lh(const std::string& rd, int x, const std::string& r) {
    registers[rd] = cache.LoadHalf(x + registers[r]);
    cache_p.LoadHalf(x + registers[r]);
    // rd <- M[r + x][0:15]
}

void AsmExecutor::Lw(const std::string& rd, int x, const std::string& r) {
    registers[rd] = cache.LoadWord(x + registers[r]);
    cache_p.LoadWord(x + registers[r]);
    // rd <- M[r + x][0:31]
}

void AsmExecutor::Lbu(const std::string& rd, int x, const std::string& r) {
    registers[rd] = cache.LoadByte(x + registers[r]);
    cache_p.LoadByte(x + registers[r]);
    // rd <- M[r + x][0:7]
}

void AsmExecutor::Lhu(const std::string& rd, int x, const std::string& r) {
    registers[rd] = cache.LoadHalf(x + registers[r]);
    cache_p.LoadHalf(x + registers[r]);
    // rd <- M[r + x][0:15]
}

void AsmExecutor::Lui(const std::string& rd, int x) {
    registers[rd] = (x << 12);
    // rd <- x << 12
}

void AsmExecutor::Sb(const std::string& rd, int x, const std::string& r) {
    uint8_t value = registers[rd];
    cache.StoreByte(x + registers[r], value);
    cache_p.StoreByte(x + registers[r], value);
    // M[r + x] <- rd   [0:7]
}

void AsmExecutor::Sh(const std::string& rd, int x, const std::string& r) {
    uint16_t value = registers[rd];
    cache.StoreHalf(x + registers[r], value);
    cache_p.StoreHalf(x + registers[r], value);
    // M[r + x] <- rd   [0:15]
}

void AsmExecutor::Sw(const std::string& rd, int x, const std::string& r) {
    uint32_t value = registers[rd];
    cache.StoreWord(x + registers[r], value);
    cache_p.StoreWord(x + registers[r], value);
    // M[r + x] <- rd   [0:31]
}

void AsmExecutor::Beq(const std::string& r1, const std::string& r2, int x) {
    if (registers[r1] == registers[r2]) {
        PC += x;
    }
}

void AsmExecutor::Bne(const std::string& r1, const std::string& r2, int x) {
    if (registers[r1] != registers[r2]) {
        PC += x;
    }
}

void AsmExecutor::Blt(const std::string& r1, const std::string& r2, int x) {
    if (registers[r1] < registers[r2]) {
        PC += x;
    }
}

void AsmExecutor::Bge(const std::string& r1, const std::string& r2, int x) {
    if (registers[r1] >= registers[r2]) {
        PC += x;
    }
}

void AsmExecutor::Bltu(const std::string& r1, const std::string& r2, int x) {
    if (std::abs(registers[r1]) < std::abs(registers[r2])) {
        PC += x;
    }
}

void AsmExecutor::Bgeu(const std::string& r1, const std::string& r2, int x) {
    if (std::abs(registers[r1]) >= std::abs(registers[r2])) {
        PC += x;
    }
}

void AsmExecutor::Jal(const std::string& rd, int x) {
    registers[rd] = PC + 4;
    PC += x;
}

void AsmExecutor::Jalr(const std::string& rd, const std::string& r, int x) {
    registers[r] = PC + 4;
    PC += registers[rd] + x;
}

void AsmExecutor::Auipc(const std::string& rd, int x) {
    registers[rd] = PC + (x << 12);
}

void AsmExecutor::Mul(const std::string& rd, const std::string& r1, const std::string& r2) {
    registers[rd] = (static_cast<long long>(registers[r1]) * registers[r2]) & 0xffffffff;
}

void AsmExecutor::Mulh(const std::string& rd, const std::string& r1, const std::string& r2) {
    registers[rd] = (static_cast<long long>(registers[r1]) * registers[r2]) & 0xffffffff00000000;
}

void AsmExecutor::Mulsu(const std::string& rd, const std::string& r1, const std::string& r2) {
    registers[rd] = (static_cast<long long>(registers[r1]) * registers[r2]) & 0xffffffff00000000;
}

void AsmExecutor::Mulu(const std::string& rd, const std::string& r1, const std::string& r2) {
    registers[rd] = (std::abs(static_cast<long long>(registers[r1])) * std::abs(registers[r2])) & 0xffffffff00000000;
}

void AsmExecutor::Div(const std::string& rd, const std::string& r1, const std::string& r2) {
    registers[rd] = registers[r1] / registers[r2];
}

void AsmExecutor::Divu(const std::string& rd, const std::string& r1, const std::string& r2) {
    registers[rd] = std::abs(registers[r1] / registers[r2]);
}

void AsmExecutor::Rem(const std::string& rd, const std::string& r1, const std::string& r2) {
    registers[rd] = registers[r1] % registers[r2];
}

void AsmExecutor::Remu(const std::string& rd, const std::string& r1, const std::string& r2) {
    registers[rd] = std::abs(registers[r1] % registers[r2]);
}

