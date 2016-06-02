#ifndef SOMTD_VIRTUAL_MACHINE_H
#define SOMTD_VIRTUAL_MACHINE_H

#include <vector>
#include <queue>
#include <map>

namespace SoMTD {
    template <class T>
    class VirtualMachine {
    public:
        VirtualMachine() {
            m_busy = false;
        }

        std::queue<int> instructions_queue;

        ~VirtualMachine(){ }

        void add_instruction(int instruction) {
            instructions_queue.push(instruction);
        }

        T digest_instruction() {
            printf("digesting...\n");
            m_busy = true;
            if (not instructions_queue.empty()) {
                auto idx = instructions_queue.front();
                instructions_queue.pop();
                return m_instruction_set[idx];
            }
            m_busy = false;
        }

        void register_instruction(std::pair<int, T> instr) {
            m_instruction_set.insert(instr);
        }

        bool busy() const {
            return m_busy;
        }

    private:
        bool m_busy;
        std::map<int, T> m_instruction_set;
    };
}
#endif
