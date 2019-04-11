#pragma once

class IMessage {
public:
    IMessage() {}
    virtual ~IMessage() {}
    virtual std::string getKeyPressed() { return std::string(""); }
    virtual glm::fvec2 getMousePosition() {return glm::fvec2(0.0); }
    virtual bool isQuit() { return false; }
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

class SystemMessage : public IMessage {
public:
    SystemMessage() {}
    SystemMessage(bool q = false) : quit(q) {}
    ~SystemMessage() {}
    bool isQuit() {
        return quit;
    }
    
private:
    bool quit = false;
};

class KeyboardMessage : public IMessage {
public:
    KeyboardMessage() {}
    KeyboardMessage(std::string bp) : buttonPressed(bp) {}
    ~KeyboardMessage() {}
    
    std::string getKeyPressed() {
        return buttonPressed;
    }
private:
    std::string buttonPressed;
};

class MouseMessage : public IMessage {
public:
    MouseMessage() {}
    MouseMessage(std::string bp, Sint32 x, Sint32 y) : buttonPressed(bp), pos(x, y) {}
    ~MouseMessage() {}
    
    std::string getKeyPressed() {
        return buttonPressed;
    }
    glm::fvec2 getMousePosition() {
        return glm::fvec2(pos.first, pos.second);
    }
private:
    std::string buttonPressed;
    std::pair<Sint32, Sint32> pos = {0, 0};
};

void sendEvents();