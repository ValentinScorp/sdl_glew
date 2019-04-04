#pragma once

//#include "Precompiled.h"

class IMessage {
public:
    IMessage() {}
    virtual ~IMessage() {}
    virtual std::string getKeyPressed() {}
    virtual glm::ivec2 getMousePosition() {}
};

class IMessageRecipient {
public:
    IMessageRecipient();
    virtual ~IMessageRecipient();
    
    virtual void onMessage(IMessage *message) = 0;
};

class SMessageManager {
private:
    SMessageManager() {}
    SMessageManager(SMessageManager const&) = delete;
    SMessageManager& operator=(SMessageManager const&) = delete;
public:
    static SMessageManager& getInstance() {
        static SMessageManager instance;
        return instance;
    }
    
    void registerRecipient(IMessageRecipient *recipient) {
        recipients.push_back(recipient);
    }
    void unregisterRecipient(IMessageRecipient *recipient) {
        for (auto& r: recipients) {
        if (r == recipient) {
                r = 0;
            }
        }
    }
    
    void registerMessage(IMessage *m) {
        messages.push_back(m);
    }
    
    void distributeMessages();
    void invokeMessage(IMessage *message);

private:
    std::vector<IMessage*> messages;
    std::vector<IMessageRecipient*> recipients;
};

