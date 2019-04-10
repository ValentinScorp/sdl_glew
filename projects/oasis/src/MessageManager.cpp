#include "Precompiled.h"

void SMessageManager::distributeMessages() {
    for (auto m: messages) {
        for (auto r: recipients) {
            if (r) {
                r->onMessage(m);
            }
        }
    }
    for (auto m: messages) {
        delete m;
    }
    messages.clear();
}

void SMessageManager::invokeMessage(IMessage *message) {
    for (auto r: recipients) {
        if (r) {
            r->onMessage(message);
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
                //runMainLoop = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_q) {
                    if (event.key.keysym.mod & KMOD_SHIFT) {
                    }
                }
                if (event.key.keysym.sym == SDLK_e) {
                    SMessageManager::getInstance().invokeMessage(new KeyboardMessage("e"));
                }
                if (event.key.keysym.sym == SDLK_w) {
                }
                if (event.key.keysym.sym == SDLK_s) {
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