#pragma once

class IMessage {
public:
    IMessage() {}
    virtual ~IMessage() {}
    virtual std::string getKeyPressed() { return std::string(""); }
    virtual glm::ivec2 getMousePosition() {return glm::ivec2(0.0); }
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

void sendEvents();