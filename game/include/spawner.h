#ifndef SOMTD_SPAWNER_H
#define SOMTD_SPAWNER_H

namespace SoMTD {
    template <class T>
    class Spawner {
    public:
        Spawner(T *prototype) {
            m_prototype = prototype;
        };

        ~Spawner() {
        };

        T* spawn_unit() {
            return m_prototype->clone();
        };

    private:
        T *m_prototype;
    };
}

#endif

