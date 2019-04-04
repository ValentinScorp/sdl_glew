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