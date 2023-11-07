#include "SightGuard.hpp"

// Inicialização da instância estática
SightGuard* SightGuard::instance = nullptr;

SightGuard::SightGuard() {
    eyes_ = std::make_shared<Eys>(false);
}

SightGuard* SightGuard::getInstance() {
    if (!instance) {
        instance = new SightGuard();
    }
    return instance;
}

std::shared_ptr<Eys> SightGuard::getResource(SightGuardResource resource) {
    switch (resource) {
        case SightGuardResource::EYS:
            if (!eyes_->isOpened()) {
                eyes_->openEyes();
            }
            return eyes_;
        default:
            return nullptr;
    }
}
