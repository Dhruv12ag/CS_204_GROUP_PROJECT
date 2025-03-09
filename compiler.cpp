#include<bits./stdc++.h>
using namespace std;
vector<int> registers(32, 0);
struct datafile
{
    string name;
    string type;
    vector <long long> value;
    string value1;
};
struct seg
{
    string name;
    string position;
};

string lstrip(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\v\f\r");
    return (start == string::npos) ? "" : s.substr(start);
}
string rstrip(const string& s) {
    size_t end = s.find_last_not_of(" \t\n\v\f\r");
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}

string strip(const string& s) {
    return rstrip(lstrip(s));
}


vector<string> splitstr(const string& input) {
    vector<string> tokens;
    string currentToken;

    for (size_t i = 0; i < input.size(); ++i) {
        char ch = input[i];
        bool isDelimiter = (ch == ' ' || ch == ',' || ch == '\t');

        if (isDelimiter) {
            if (!currentToken.empty()) {
                tokens.emplace_back(currentToken);
                currentToken.clear();
            }
        } else {
            currentToken.push_back(ch);
        }
    }

    if (!currentToken.empty()) {
        tokens.emplace_back(currentToken);
    }

    return tokens;
}
string dec_to_bin(int num, int bits) {
    bitset<32> binary(num); 
    return binary.to_string().substr(32 - bits); 
}

int string_to_int(const string& s) {
    int result = 0;
    int multiplier = 1;
    size_t idx = 0;

    
    if (s.length() > 1 && s[0] == '0' && s[1] == 'x') {
        return stoi(s, nullptr, 16);
    }
    if (!s.empty() && s[0] == '-') {
        multiplier = -1;
        idx = 1; // Start parsing from index 1
    }

    for (; idx < s.length(); ++idx) {
        char digit = s[idx];
        result = (result * 10) + (digit - '0');
    }

    return result * multiplier;
}

long long int bin_to_dec(bitset<32> bin) {
    long long int dec = 0;
    for (int i = 0; i < 32; i++) {
        dec += bin[i] * pow(2, i);
    }
    return dec;
}

string dec_to_hex_1(long long int dec) {
    
    if (dec < 0) {
        dec = 0xFFFFFFFF + dec + 1;
    }

    string hexResult;

    
    do {
        int remainder = dec % 16;
        char hexDigit = (remainder < 10) ? ('0' + remainder) : ('A' + remainder - 10);
        hexResult += hexDigit;
        dec /= 16;
    } while (dec != 0);
    hexResult += "x0";

    
    reverse(hexResult.begin(), hexResult.end());

    return hexResult;
}

string dec_to_hex(long long int dec) {
    if (dec < 0) {
        dec = 0xFFFFFFFF + dec + 1;
    }

    string hexResult;

    
    do {
        int remainder = dec % 16;
        char hexDigit = (remainder < 10) ? ('0' + remainder) : ('A' + remainder - 10);
        hexResult += hexDigit;
        dec /= 16;
    } while (dec != 0);

    while (hexResult.length() < 8) {
        hexResult += '0';
    }

    hexResult += "x0";
    reverse(hexResult.begin(), hexResult.end());

    return hexResult;
}
std::map<string,vector<std::string>> encodes_map = {
    {"add", {"0110011", "000", "0000000", "R"}},   // add
    {"and", {"0110011", "111", "0000000", "R"}},   // and
    {"or",  {"0110011", "110", "0000000", "R"}},   // or
    {"sll", {"0110011", "001", "0000000", "R"}},   // sll
    {"slt", {"0110011", "010", "0000000", "R"}},   // slt
    {"sra", {"0110011", "101", "0100000", "R"}},   // sra
    {"srl", {"0110011", "101", "0000000", "R"}},   // srl
    {"sub", {"0110011", "000", "0100000", "R"}},   // sub
    {"xor", {"0110011", "100", "0000000", "R"}},   // xor
    {"mul", {"0110011", "000", "0000001", "R"}},   // mul
    {"div", {"0110011", "100", "0000001", "R"}},   // div
    {"rem", {"0110011", "110", "0000001", "R"}},   // rem
    {"addi", {"0010011", "000", "", "I"}},        // addi   
    {"andi", {"0010011", "111", "", "I"}},        // andi
    {"ori",  {"0010011", "110", "", "I"}},        // ori
    {"lb",   {"0000011", "000", "", "L"}},        // lb
    {"ld",   {"0000011", "011", "", "L"}},        // ld
    {"lh",   {"0000011", "001", "", "L"}},        // lh
    {"lw",   {"0000011", "010", "", "L"}},        // lw
    {"jalr", {"1100111", "000", "", "I"}},       // jalr
    {"sb",   {"0100011", "000", "", "S"}},       // sb
    {"sw",   {"0100011", "010", "", "S"}},       // sw
    {"sd",   {"0100011", "011", "", "S"}},       // sd
    {"sh",   {"0100011", "001", "", "S"}},       // sh
    {"beq",  {"1100011", "000", "", "B"}},      // beq
    {"bne",  {"1100011", "001", "", "B"}},      // bne
    {"bge",  {"1100011", "101", "", "B"}},      // bge
    {"blt",  {"1100011", "100", "", "B"}},      // blt
    {"auipc", {"0010111", "000", "", "U"}},     // auipc
    {"lui",   {"0110111", "000", "", "U"}},     // lui
    {"jal",   {"1101111", "000", "", "J"}}     // jal
};

int reg(const string& register_name) {
    
    if (register_name == "sp") return 2;
    if (register_name == "s0" || register_name == "fp") return 8;
    if (register_name == "tp") return 4;

    char prefix = register_name[0];
    
    switch (prefix) {
        case 'x':
            return string_to_int(register_name.substr(1));
        
        case 'z':
            return 0;
        
        case 'r':
            return 1;
        
        case 'g':
            return 3;
        
        case 's':
            return 16 + string_to_int(register_name.substr(1));
        
        case 't': {
            int reg_num = string_to_int(register_name.substr(1));
            if (reg_num < 3)
                return reg_num + 5;
            else
                return reg_num + 25;
        }
        
        default:
            return 10 + string_to_int(register_name.substr(1));
    }
}

string r_encode(vector<string> tokens) {
    string instruction = tokens[0];
    int rd_idx = reg(tokens[1]);
    int rs1_idx = reg(tokens[2]);
    int rs2_idx = reg(tokens[3]);

    if (instruction == "add")
        registers[rd_idx] = registers[rs1_idx] + registers[rs2_idx];
    else if (instruction == "and")
        registers[rd_idx] = registers[rs1_idx] & registers[rs2_idx];
    else if (instruction == "or")
        registers[rd_idx] = registers[rs1_idx] | registers[rs2_idx];
    else if (instruction == "sll")
        registers[rd_idx] = registers[rs1_idx] << registers[rs2_idx];
    else if (instruction == "slt")
        registers[rd_idx] = (registers[rs1_idx] < registers[rs2_idx]) ? 1 : 0;
    else if (instruction == "sra")
        registers[rd_idx] = registers[rs1_idx] >> registers[rs2_idx];
    else if (instruction == "srl")
        registers[rd_idx] = (unsigned int)(registers[rs1_idx]) >> registers[rs2_idx];
    else if (instruction == "sub")
        registers[rd_idx] = registers[rs1_idx] - registers[rs2_idx];
    else if (instruction == "xor")
        registers[rd_idx] = registers[rs1_idx] ^ registers[rs2_idx];
    else if (instruction == "mul")
        registers[rd_idx] = registers[rs1_idx] * registers[rs2_idx];
    else if (instruction == "div")
        registers[rd_idx] = registers[rs1_idx] / registers[rs2_idx];
    else if (instruction == "rem")
        registers[rd_idx] = registers[rs1_idx] % registers[rs2_idx];

    bitset<32> opcode(encodes_map[instruction][0]);
    bitset<32> rd(rd_idx << 7);
    bitset<32> funct3(encodes_map[instruction][1]);
    funct3 <<= 12;
    bitset<32> rs1(rs1_idx << 15);
    bitset<32> rs2(rs2_idx << 20);
    bitset<32> funct7(encodes_map[instruction][2]);
    funct7 <<= 25;

    bitset<32> machine_code = opcode | rd | funct3 | rs1 | rs2 | funct7;

    string opcode_str   = encodes_map[instruction][0]; // opcode
    string funct3_str   = encodes_map[instruction][1]; // funct3
    string funct7_str   = encodes_map[instruction][2]; // funct7

    string rd_bin   = dec_to_bin(rd_idx, 5); // Convert rd index to 5-bit binary
    string rs1_bin  = dec_to_bin(rs1_idx, 5); // Convert rs1 index to 5-bit binary
    string rs2_bin  = dec_to_bin(rs2_idx, 5); // Convert rs2 index to 5-bit binary

    string assembly_instr = instruction + " " + tokens[1] + "," + tokens[2] + "," + tokens[3];

    stringstream final_output;
    
    final_output << dec_to_hex(bin_to_dec(machine_code)) << " , "
                 << assembly_instr << " # "
                 << opcode_str << "-" 
                 << funct3_str << "-"
                 << funct7_str << "-"
                 << rd_bin << "-"
                 << rs1_bin << "-"
                 << rs2_bin 
                 << "-NULL";
    return final_output.str();
}

string i_encode(vector<string> tokens) {
    string instruction = tokens[0];
    int rd_idx = reg(tokens[1]);
    int rs1_idx;
    int immediate;

    // For load instructions and jalr, immediate is in format imm(rs1)
    if (instruction == "lb" || instruction == "ld" || instruction == "lh" || instruction == "lw" || instruction == "jalr") {
        int bracket_pos = tokens[2].find('(');
        string imm_str = tokens[2].substr(0, bracket_pos);
        immediate = string_to_int(imm_str);
        string rs1_str = tokens[2].substr(bracket_pos + 1, tokens[2].size() - bracket_pos - 2);
        rs1_idx = reg(rs1_str);
    } else {
        // For arithmetic/logical immediate instructions
        rs1_idx = reg(tokens[2]);
        immediate = string_to_int(tokens[3]);
    }

    // Validate immediate range (-2048 to 2047)
    if (immediate < -2048 || immediate > 2047) {
        throw invalid_argument("Immediate value out of range for I-type instruction (-2048 to 2047)");
    }

    // Perform sign extension for immediate (12-bit signed)
    bitset<12> imm_bits(immediate & 0xFFF); // Mask to 12 bits
    int sign_extended_imm = imm_bits.to_ulong();
    if (imm_bits.test(11)) { // If MSB is set, extend sign
        sign_extended_imm |= 0xFFFFF000;
    }

    // Simulate operation based on instruction type
    if (instruction == "addi") {
        registers[rd_idx] = registers[rs1_idx] + immediate;
    } else if (instruction == "andi") {
        registers[rd_idx] = registers[rs1_idx] & immediate;
    } else if (instruction == "ori") {
        registers[rd_idx] = registers[rs1_idx] | immediate;
    } else if (instruction == "lb" || instruction == "ld" || instruction == "lh" || instruction == "lw") {
        // Load instructions: Assuming memory[] is defined elsewhere
        // registers[rd_idx] = memory[registers[rs1_idx] + immediate];
        // For now, we leave it as a placeholder
    
    }

    // Construct machine code fields
    bitset<32> opcode(encodes_map[instruction][0]);
    bitset<32> rd(rd_idx << 7);
    bitset<32> funct3(encodes_map[instruction][1]);
    funct3 <<= 12;
    bitset<32> rs1(rs1_idx << 15);
    bitset<32> imm(sign_extended_imm << 20);

    bitset<32> machine_code = opcode | rd | funct3 | rs1 | imm;

    // Prepare additional details for output
    string opcode_str   = encodes_map[instruction][0]; 
    string funct3_str   = encodes_map[instruction][1]; 

    // Convert register indices and immediate to binary strings
    string rd_bin       = dec_to_bin(rd_idx, 5); 
    string rs1_bin      = dec_to_bin(rs1_idx, 5); 
    string immediate_bin= dec_to_bin(immediate & 0xFFF, 12);

    // Assembly instruction string formatting
    string assembly_instr;
    
    if (instruction == "lb" || instruction == "ld" || instruction == "lh" || instruction == "lw" || instruction == "jalr") {
        assembly_instr = instruction + " " + tokens[1] + "," + to_string(immediate) + "(" + tokens[2].substr(tokens[2].find('(')+1, tokens[2].find(')')-tokens[2].find('(')-1) + ")";
    } else {
        assembly_instr = instruction + " " + tokens[1] + "," + tokens[2] + "," + tokens[3];
    }

    // Constructing the final formatted output string
    stringstream final_output;
    
    final_output << dec_to_hex(bin_to_dec(machine_code)) << " , "
                 << assembly_instr << " # "
                 << opcode_str << "-" 
                 << funct3_str << "-"
                 << "NULL-"
                 << rd_bin << "-"
                 << rs1_bin << "-"
                 << immediate_bin;

    return final_output.str();
}


string s_encode(vector<string> tokens) {
    string instruction = tokens[0];
    int rs2_idx = reg(tokens[1]); // Extract rs2 index (source register)

    // Parse immediate and rs1 from tokens[2] (format: imm(rs1))
    int bracket_pos = tokens[2].find('(');
    string imm_str = tokens[2].substr(0, bracket_pos);
    int immediate = string_to_int(imm_str);
    string rs1_str = tokens[2].substr(bracket_pos + 1, tokens[2].size() - bracket_pos - 2);
    int rs1_idx = reg(rs1_str);

    // Validate immediate range (-2048 to 2047)
    if (immediate < -2048 || immediate > 2047) {
        throw invalid_argument("Immediate value out of range for S-type instruction (-2048 to 2047)");
    }

    // Perform sign extension for immediate (12-bit signed)
    bitset<12> imm_bits(immediate & 0xFFF); // Mask to 12 bits
    int sign_extended_imm = imm_bits.to_ulong();
    if (imm_bits.test(11)) { // If MSB is set, extend sign
        sign_extended_imm |= 0xFFFFF000;
    }

    // Construct machine code fields
    bitset<32> opcode(encodes_map[instruction][0]);
    bitset<32> rs2(rs2_idx << 20);
    bitset<32> funct3(encodes_map[instruction][1]);
    funct3 <<= 12;
    bitset<32> rs1(rs1_idx << 15);

    // Split immediate into two parts as per S-type instruction format
    bitset<32> imm_4_0 = (sign_extended_imm & 0x1F) << 7;      // Lower 5 bits (4:0)
    bitset<32> imm_11_5 = (sign_extended_imm & 0xFE0) << 20;   // Upper bits (11:5)

    // Combine all fields to form machine code
    bitset<32> machine_code = opcode | rs2 | funct3 | rs1 | imm_4_0 | imm_11_5;

    // Prepare additional details for output
    string opcode_str   = encodes_map[instruction][0]; // opcode
    string funct3_str   = encodes_map[instruction][1]; // funct3

    // Convert register indices to binary strings
    string rs1_bin  = dec_to_bin(rs1_idx, 5); // Convert rs1 index to binary
    string rs2_bin  = dec_to_bin(rs2_idx, 5); // Convert rs2 index to binary
    string immediate_bin = dec_to_bin(immediate & 0xFFF, 12); // Convert immediate to binary

    // Assembly instruction string
    string assembly_instr = instruction + " " + tokens[1] + "," + to_string(immediate) + "(" + rs1_str + ")";

    // Constructing the final formatted output string
    stringstream final_output;

    final_output << dec_to_hex(bin_to_dec(machine_code)) << " , "
                 << assembly_instr << " # "
                 << opcode_str << "-" 
                 << funct3_str << "-"
                 << "NULL-NULL-"
                 << rs1_bin << "-"
                 << rs2_bin 
                 << "-"
                 << immediate_bin;

    return final_output.str();
}


string lw_encode(vector<string> tokens, int pc) {
    string instruction = tokens[0];
    int rd_idx = reg(tokens[1]);
    int bracket_pos = tokens[2].find('(');
    string imm_str = tokens[2].substr(0, bracket_pos);
    int immediate = string_to_int(imm_str);
    string rs1_str = tokens[2].substr(bracket_pos + 1, tokens[2].size() - bracket_pos - 2);
    int rs1_idx = reg(rs1_str);
    bitset<32> opcode(encodes_map[instruction][0]);
    bitset<32> rd(rd_idx << 7);
    bitset<32> funct3(encodes_map[instruction][1]);
    funct3 <<= 12;
    bitset<32> rs1(rs1_idx << 15);
    bitset<32> imm(immediate << 20);

    bitset<32> machine_code = opcode | rd | funct3 | rs1 | imm;
    string opcode_str   = encodes_map[instruction][0]; // opcode
    string funct3_str   = encodes_map[instruction][1]; // funct3

    string rd_bin   = dec_to_bin(rd_idx, 5); // Convert rd index to 5-bit binary
    string rs1_bin  = dec_to_bin(rs1_idx, 5); // Convert rs1 index to 5-bit binary
    string assembly_instr = instruction + " " + tokens[1] + "," + to_string(immediate) + "(" + rs1_str + ")";

    stringstream final_output;

    final_output << dec_to_hex(bin_to_dec(machine_code)) << " , "
                 << assembly_instr << " # "
                 << opcode_str << "-" 
                 << funct3_str << "-"
                 << "NULL-"
                 << rd_bin << "-"
                 << rs1_bin
                 << "-NULL-"
                 << immediate;

    return final_output.str();
}

map<string, int> labels;
int pc1 = 0x0;
void processLabels(const string& line) {
    if (line.find(':') != string::npos  and line.find('.') == string::npos){
        string label = strip(line.substr(0, line.find(':')));
        labels[label] = pc1;
        if (splitstr(line).size() > 2){
            pc1 += 4 ;
        }
    }
    else if (line.find('.') == string::npos){
        pc1 += 4;
    }
}
void loadLabels(const string& filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        else
        {
            processLabels(line);
        }
    }
    file.close();
}
string sb_encode(vector<string> tokens, int pc) {
    string instruction = tokens[0];
    int rs1_idx = reg(tokens[1]); // Extract rs1 index (source register)
    int rs2_idx = reg(tokens[2]); // Extract rs2 index (source register)
    int imm = labels[tokens[3]] - pc; // Calculate immediate value relative to PC

    // Validate immediate range (-4096 to 4095 for SB-type instructions)
    if (imm < -4096 || imm > 4095) {
        throw invalid_argument("Immediate value out of range for SB-type instruction (-4096 to 4095)");
    }

    // Perform sign extension for immediate (13-bit signed)
    bitset<13> imm_bits(imm & 0x1FFF); // Mask to 13 bits
    int sign_extended_imm = imm_bits.to_ulong();
    if (imm_bits.test(12)) { // If MSB is set, extend sign
        sign_extended_imm |= 0xFFFFE000;
    }

    // Construct machine code fields
    bitset<32> opcode(encodes_map[instruction][0]);
    bitset<32> rs1(rs1_idx << 15);
    bitset<32> rs2(rs2_idx << 20);
    bitset<32> funct3(encodes_map[instruction][1]);
    funct3 <<= 12;

    // Split immediate into parts as per SB-type instruction format
    bitset<32> imm_11 = (sign_extended_imm & 0x400) >> 10;   // Extract bit 11
    bitset<32> imm_12 = (sign_extended_imm & 0x800) >> 11;   // Extract bit 12
    bitset<32> imm_10to5 = (sign_extended_imm & 0x7E0) >> 5; // Extract bits [10:5]
    bitset<32> imm_4to1 = (sign_extended_imm & 0x1E) >> 1;   // Extract bits [4:1]

    // Combine immediate parts into final format
    bitset<32> imm_final = (imm_12 << 31) | (imm_11 << 7) | (imm_10to5 << 25) | (imm_4to1 << 8);

    // Combine all fields to form machine code
    bitset<32> machine_code = opcode | rs1 | rs2 | funct3 | imm_final;

    // Prepare additional details for output
    string opcode_str   = encodes_map[instruction][0]; // opcode
    string funct3_str   = encodes_map[instruction][1]; // funct3

    // Convert register indices to binary strings
    string rs1_bin      = dec_to_bin(rs1_idx, 5); // Convert rs1 index to binary
    string rs2_bin      = dec_to_bin(rs2_idx, 5); // Convert rs2 index to binary
    string immediate_bin= dec_to_bin(imm & 0xFFF, 13); // Convert immediate to binary

    // Assembly instruction string
    string assembly_instr = instruction + " " + tokens[1] + "," + tokens[2] + "," + tokens[3];

    // Constructing the final formatted output string
    stringstream final_output;

    final_output << dec_to_hex(bin_to_dec(machine_code)) << " , "
                 << assembly_instr << " # "
                 << opcode_str << "-" 
                 << funct3_str << "-"
                 << "NULL-NULL-"
                 << rs1_bin << "-"
                 << rs2_bin 
                 << "-"
                 << immediate_bin;

    return final_output.str();
}


string u_encode(vector<string> tokens) {
    string instruction = tokens[0];
    int rd_idx = reg(tokens[1]);
    int immediate = string_to_int(tokens[2]);
    bitset<32> opcode(encodes_map[instruction][0]);
    bitset<32> rd(rd_idx << 7);
    bitset<32> imm(immediate << 12);
    bitset<32> machine_code = opcode | rd | imm;
    string opcode_str   = encodes_map[instruction][0]; // opcode

    string rd_bin   = dec_to_bin(rd_idx, 5);
    string assembly_instr = instruction + " " + tokens[1] + "," + tokens[2];
    stringstream final_output;

    final_output << dec_to_hex(bin_to_dec(machine_code)) << " , "
                 << assembly_instr << " # "
                 << opcode_str << "-"
                 << "NULL-NULL-"
                 << rd_bin << "-" 
                 << "-NULL-NULL-"
                 << immediate;

    return final_output.str();
}
string uj_encode(vector<string> tokens, int pc) {
    // Extract instruction components
    string instruction = tokens[0];
    int rd_idx = reg(tokens[1]);
    int immediate = labels[tokens[2]] - pc; // Calculate immediate value relative to PC
    bitset<32> opcode(encodes_map[instruction][0]);
    bitset<32> rd(rd_idx << 7);

    // Split immediate into parts as per UJ-type instruction format
    bitset<32> bit_20((immediate >> 1) & 0x80000);   // Extract bit 20
    bitset<32> bit_10to1((immediate << 8) & 0x7FE00); // Extract bits [10:1]
    bitset<32> bit_11((immediate >> 2) & 0x100);      // Extract bit 11
    bitset<32> bit_19to12((immediate >> 12) & 0xFF);  // Extract bits [19:12]
    bitset<32> imm_final = (bit_20 | bit_19to12 | bit_11 | bit_10to1);
    bitset<32> machine_code = opcode | rd | (imm_final << 12);
    string opcode_str   = encodes_map[instruction][0]; // opcode

    string rd_bin   = dec_to_bin(rd_idx, 5);
    string assembly_instr = instruction + " " + tokens[1] + "," + tokens[2];
    stringstream final_output;

    final_output << dec_to_hex(bin_to_dec(machine_code)) << " , "
                 << assembly_instr << " # "
                 << opcode_str 
                 << "-NULL-NULL-"
                 << rd_bin << "-"
                 << "-NULL-NULL-"
                 << immediate;

    return final_output.str();
}

string inc_hex(string address, int bytes) {
    unsigned long long int_value;
    stringstream ss;
    ss << hex << address;
    ss >> int_value;
    int_value += bytes;
    stringstream result;
    result << hex << int_value;
    return result.str();
}

typedef pair<string,string> MemoryPair;
MemoryPair datamemory; // pair of data memory with its size in bytes
vector<MemoryPair> datamemoryList; // vector of data memory pairs with their addresses and values
vector<seg> datalabels; // vector of data labels with its position in memory
const int START =0;

void data_read() {
    ifstream file("assemblycode.asm");
    string line;
    vector<datafile> stored;
    int start = 0;

    if (file.is_open()) {
        while (getline(file, line)) {
            vector<string> tokens = splitstr(line);

            if (line.empty()) 
            {
                continue;
            }
            if (start > 1) 
            {
                continue;
            }
            else if (start == 1) 
            {
                if (tokens[0] == ".text:" || tokens[0] == ".text") 
                { // Data part ends
                    start = 2;
                }
                else 
                {
                    datafile temp;
                    int flag = 0;
                    int index = -1;

                    for (int i = 0; i < tokens.size(); i++) 
                    {
                        if (tokens[i].find(":") != string::npos && tokens[i+1].find(".") != string::npos)
                        {
                            flag = 1;
                            index = i;
                            break;
                        }
                    }

                    if (flag == 1) 
                    {
                        string nameT = tokens[index].substr(0, tokens[index].size() - 1);
                        string typeT = tokens[index + 1].substr(1);
                        temp.name = nameT;
                        temp.type = typeT;
                        if (typeT == "asciiz") 
                        {
                            for (int i = index + 2; i < tokens.size(); i++) 
                            {
                                string a = tokens[i];
                                for (int j = 1; j < a.size()-1; j++) 
                                {
                                    temp.value1.push_back(a[j]);
                                }
                                // temp.value1.push_back(0);
                            }
                        } 
                        else 
                        {
                            for (int i = index + 2; i < tokens.size(); i++) 
                            {
                                int a = string_to_int(tokens[i]);
                
                                temp.value.push_back(a);
                            }
                        }
                    } 
                    // else 
                    // {
                    //     temp.name = tokens[0];
                    //     temp.type = tokens[1].substr(1);
                        
                    //     for (int i = 2; i < tokens.size(); i++) 
                    //     {
                    //         int a = string_to_int(tokens[i]);
                    //         temp.value.push_back(a);
                    //     }
                    // }
                    stored.push_back(temp);
                }
            } 
            else if (start == 0) 
            {
                if (tokens[0] == ".data:" || tokens[0] == ".data") 
                { // Data part starts
                    start = 1;
                }
            }
        }
    }

    file.close();

    int pos = 0;
    string address = "0x10000000";
    int update = string_to_int(address);

    for (int i = 0; i < stored.size(); i++) {

        seg temp;
        
        if (stored[i].type == "byte") 
        {
            temp.name = stored[i].name;
            temp.position = address;
            for (int j = 0; j < stored[i].value.size(); j++) 
            {
                MemoryPair data;
                data.first = address;
                int val = stored[i].value[j];
                string hex = dec_to_hex_1(val);
                data.second = hex;
                datamemoryList.push_back(data);
                pos++;
                update += 1;
                address = dec_to_hex(update);
            }
        } 
        else if (stored[i].type == "word") 
        {
            temp.name = stored[i].name;
            temp.position = address;
            for (int j = 0; j < stored[i].value.size(); j++) 
            {
                MemoryPair data;
                data.first = address;
                int val = stored[i].value[j];
                string hex = dec_to_hex_1(val);

                data.second = hex;
                datamemoryList.push_back(data);
                pos += 4;
                update += 4;
                address = dec_to_hex(update);
            }
        } 
        else if (stored[i].type == "half") 
        {
            temp.name = stored[i].name;
            temp.position = address;
            for (int j = 0; j < stored[i].value.size(); j++) 
            {
                MemoryPair data;
                data.first = address;
                int val = stored[i].value[j];
                string hex = dec_to_hex_1(val);
                data.second = hex;
                datamemoryList.push_back(data);
                pos += 2;
                update += 2;
                address = dec_to_hex(update);
                
            }
        }
        else if (stored[i].type == "double")
        {
            temp.name = stored[i].name;
            temp.position = address;
            for (int j = 0; j < stored[i].value.size(); j++) 
            {
                MemoryPair data;
                data.first = address;
                int val = stored[i].value[j];
                string hex = dec_to_hex_1(val);
                data.second = hex;
                datamemoryList.push_back(data);
                pos += 8;
                update += 8;
                address = dec_to_hex(update);
            }
        }
        else if (stored[i].type == "asciiz")
        {
            temp.name = stored[i].name;
            temp.position = address;
            for (int j = 0; j < stored[i].value1.size(); j++) 
            {
                MemoryPair data;
                data.first = address;
                data.second.push_back(stored[i].value1[j]);
                datamemoryList.push_back(data);
                pos++;
                update += 1;
                address = dec_to_hex(update);
            }
        
        }
        datalabels.push_back(temp);
    }
    cout << endl;
     

    ofstream mc("machinecode.mc", ios::app);
    if (mc.is_open())
    {
        for (const auto &memory : datamemoryList) 
        {  
        mc << memory.first << " " ;
            for (auto value : memory.second)  
            {
                mc << value ;
            }
        mc << endl;
        }
    }
}
map<int, string> instructions;

int main() {
    // Load labels and initialize file streams
    ifstream file("assemblycode.asm");
    int pc = 0x0;
    loadLabels("assemblycode.asm");
    ofstream mc("machinecode.mc");
    string line;

    // Process each line of the assembly code
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        // Handle labels and strip them from the line if present
        if (line.find('.') == string::npos) {
            if (line.find(':') != string::npos) {
                if (splitstr(line).size() > 2) {
                    line = strip(line.substr(line.find(':') + 1));
                } else {
                    continue;
                }
            }

            // Tokenize the instruction line
            vector<string> tokens = splitstr(line);
            string instruction = tokens[0];
            char format = encodes_map[instruction][3][0];

            // Switch based on instruction format and call appropriate encoding function
            switch (format) {
                case 'R':
                    mc << dec_to_hex_1(pc) << " " << r_encode(tokens) << endl;
                    instructions[pc] = line;
                    break;
                case 'I':
                    mc << dec_to_hex_1(pc) << " " << i_encode(tokens) << endl;
                    instructions[pc] = line;
                    break;
                case 'S':
                    mc << dec_to_hex_1(pc) << " " << s_encode(tokens) << endl;
                    instructions[pc] = line;
                    break;
                case 'B':
                    mc << dec_to_hex_1(pc) << " " << sb_encode(tokens, pc) << endl;
                    instructions[pc] = line;
                    break;
                case 'U':
                    mc << dec_to_hex_1(pc) << " " << u_encode(tokens) << endl;
                    instructions[pc] = line;
                    break;
                case 'J':
                    mc << dec_to_hex_1(pc) << " " << uj_encode(tokens, pc) << endl;
                    instructions[pc] = line;
                    break;
                case 'L':
                    mc << dec_to_hex_1(pc) << " " << lw_encode(tokens, pc) << endl;
                    instructions[pc] = line;
                    break;
            }

            // Increment program counter for next instruction
            pc += 4;
        }
    }

    // Write a separator to the output file
    mc << "--------------------------------------------------------" << endl;

    // Read data section (if applicable)
    data_read();

    // Close the machine code output file
    mc.close();

    // Print all instructions with their program counter values
    for (auto it : instructions) {
        cout << it.first << " " << it.second << endl;
    }

    return 0;
}


