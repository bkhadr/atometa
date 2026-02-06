#include "Atometa/Chemistry/Atom.h"

namespace Atometa {
    Atom::Atom(AtomType type, const glm::vec3& pos)
        : Type(type), Position(pos), Velocity(0.0f), Force(0.0f) {
        
        switch (type) {
            case AtomType::Hydrogen:
                Mass = 1.008f; Radius = 0.25f; Symbol = "H"; break;
            case AtomType::Carbon:
                Mass = 12.011f; Radius = 0.70f; Symbol = "C"; break;
            case AtomType::Nitrogen:
                Mass = 14.007f; Radius = 0.65f; Symbol = "N"; break;
            case AtomType::Oxygen:
                Mass = 15.999f; Radius = 0.60f; Symbol = "O"; break;
        }
    }

    void Atom::Update(float dt) {
        glm::vec3 accel = Force / Mass;
        Velocity += accel * dt;
        Position += Velocity * dt;
        Force = glm::vec3(0.0f);
    }

    void Atom::ApplyForce(const glm::vec3& force) {
        Force += force;
    }
}