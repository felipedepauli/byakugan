#ifndef SIGHTGUARD_HPP
#define SIGHTGUARD_HPP

#include "Eys.hpp"
#include <memory>

enum class SightGuardResource {
    EYS
};

class SightGuard {
private:
    static SightGuard* instance;
    std::shared_ptr<Eys> eyes_;

    // Construtor privado
    SightGuard();

public:
    // Evitar cópia e atribuição
    SightGuard(const SightGuard&) = delete;
    SightGuard& operator=(const SightGuard&) = delete;

    // Método para obter a instância do Singleton
    static SightGuard* getInstance();

    // Método para obter o recurso desejado
    std::shared_ptr<Eys> getResource(SightGuardResource resource);
};

#endif // SIGHTGUARD_HPP
