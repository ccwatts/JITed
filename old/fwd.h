// #pragma once
// #include <string>
// #include <memory>

// namespace minic {
// class Instruction {
// public:
//     // virtual void replace(ValuePtr v, ValuePtr with);
//     virtual std::string toString() {
//         throw std::runtime_error("error: unimplemented instruction method");
//     }
//     virtual RegisterPtr getTarget() {
//         throw std::runtime_error("error: unimplemented instruction method");
//     }
//     virtual Values getSources() {
//         throw std::runtime_error("error: unimplemented instruction method");
//     }
//     virtual Values allValues() {
//         throw std::runtime_error("error: unimplemented instruction method");
//     }
// };
// typedef std::shared_ptr<Instruction> InstructionPtr;

// class PhiInstruction : public Instruction {
// public:
//     RegisterPtr target;
//     std::vector<Block*> blocks;
//     Values values;

//     PhiInstruction(RegisterPtr target, std::vector<Block*> blocks, Values values);

//     std::string toString() override;
//     RegisterPtr getTarget() override;
//     Values getSources() override;
//     Values allValues() override;
//     bool isTrivial();
// };
// typedef std::shared_ptr<PhiInstruction> PhiInstructionPtr;
// }