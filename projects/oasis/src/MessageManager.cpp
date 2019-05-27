#include "Precompiled.h"

void SMessageManager::distributeMessages() {
    for (auto m: messages) {
        if (!m->destroyed)
            invokeMessage(m);
    }
    messages.clear();
}

void SMessageManager::invokeMessage(IMessage *message) {
    for (size_t i = 0; i < recipients.size(); i++) {
        if (recipients[i]) {
            if (!message->destroyed)
                recipients[i]->onMessage(message);
        }
    }
    delete message;
}

IMessageRecipient::IMessageRecipient() {
    SMessageManager::getInstance().registerRecipient(this);
}
IMessageRecipient::~IMessageRecipient() {
    SMessageManager::getInstance().unregisterRecipient(this);
}

void sendEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                SMessageManager::getInstance().invokeMessage(new SystemMessage(true));
                break;
            case SDL_MOUSEMOTION:
                SMessageManager::getInstance().invokeMessage(new MouseMessage("motion", event.motion.x, event.motion.y));
                break;
            case SDL_MOUSEWHEEL:
                SMessageManager::getInstance().invokeMessage(new MouseMessage("wheel", event.wheel.x, event.wheel.y));
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    SMessageManager::getInstance().invokeMessage(new MouseMessage("left_mouse_button_pressed", event.button.x, event.button.y));
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    SMessageManager::getInstance().invokeMessage(new MouseMessage("right_mouse_button_pressed", event.button.x, event.button.y));
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    SMessageManager::getInstance().invokeMessage(new MouseMessage("left_mouse_button_released", event.button.x, event.button.y));
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    SMessageManager::getInstance().invokeMessage(new MouseMessage("right_mouse_button_released", event.button.x, event.button.y));
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_q) {
                    if (event.key.keysym.mod & KMOD_SHIFT) {
                    }
                }
                if (event.key.keysym.sym == SDLK_e) {
                    SMessageManager::getInstance().invokeMessage(new KeyboardMessage("e"));
                }
                if (event.key.keysym.sym == SDLK_a) {
                    SMessageManager::getInstance().invokeMessage(new KeyboardMessage("a"));
                }
                if (event.key.keysym.sym == SDLK_s) {
                    SMessageManager::getInstance().invokeMessage(new KeyboardMessage("s"));
                }
                if (event.key.keysym.sym == SDLK_w) {
                    SMessageManager::getInstance().invokeMessage(new KeyboardMessage("w"));
                }
                if (event.key.keysym.sym == SDLK_LEFT) {
                    SMessageManager::getInstance().invokeMessage(new KeyboardMessage("arrow_left"));
                }
                if (event.key.keysym.sym == SDLK_RIGHT) {
                    SMessageManager::getInstance().invokeMessage(new KeyboardMessage("arrow_right"));
                }
                if (event.key.keysym.sym == SDLK_UP) {
                    SMessageManager::getInstance().invokeMessage(new KeyboardMessage("arrow_up"));
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    SMessageManager::getInstance().invokeMessage(new KeyboardMessage("arrow_down"));
                }
                break;
            default:
                break;
        }
    }
}