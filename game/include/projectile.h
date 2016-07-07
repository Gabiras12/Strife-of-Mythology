#ifndef SOMTD_PROJECTILE_H
#define SOMTD_PROJECTILE_H

#include "movable_unit.h"

namespace SoMTD {
    class Projectile {
    public:
        Projectile(MovableUnit *newtarget, std::pair<double, double> to, std::string texture, std::pair<double, double>origin,
                int new_frame_per_state, int new_total_states, double newdamage);
        ~Projectile();

        void draw_self(ijengine::Canvas *canvas, unsigned now, unsigned last);
        void draw_self_after(ijengine::Canvas *canvas, unsigned now, unsigned last);
        void update_self(unsigned now, unsigned last);
        Animation *animation() const;
        bool done() const;
        double damage() const;

    private:
        MovableUnit *m_target;
        std::pair<double, double> m_destination;
        std::pair<double, double> m_movement_speed;
        std::string m_texture_path;
        double m_x;
        double m_y;
        Animation *m_animation;
        bool m_done;
        double m_damage;
    };
}

#endif
