#pragma once

class IMessage {
public:
    IMessage() {}
    virtual ~IMessage() {}
    
    virtual std::string getKeyPressed() { return std::string(""); }
    virtual glm::fvec2 getMousePosition() {return glm::fvec2(0.0); }
    virtual glm::fvec2 getWheelDirection() { return glm::fvec2(0.0); }
    virtual std::string getMessage() { return std::string(""); }
    virtual glm::fvec3 getPosition() { return glm::fvec3(0.0); }
    virtual std::string getParentId(){ return std::string(""); }
    virtual std::string getSenderId(){ return std::string(""); }
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
        std::cout << "registering recip" << std::endl;
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
    glm::fvec2 getWheelDirection() {
        return glm::fvec2(pos.first, pos.second);
    }
private:
    std::string buttonPressed;
    std::pair<Sint32, Sint32> pos = {0, 0};
};

class GuiButtonMessage : public IMessage {
public:
    GuiButtonMessage() {}
    GuiButtonMessage(std::string m) : message(m) {}
    ~GuiButtonMessage() {}
    
    std::string getMessage() {
        return message;
    }
private:
    std::string message;
};

class UnitMessage : public IMessage {
public:
    UnitMessage() {}
    UnitMessage(std::string m, glm::fvec3 pos) : message(m), position(pos) {}
    ~UnitMessage() {}
    
    std::string getMessage() { return message; }
    glm::fvec3 getPosition() { return position; }
private:
    std::string message;
    glm::fvec3 position = {0.0f, 0.0f, 0.0f };
};

class GuiMessage : public IMessage {
public:
    GuiMessage() {}
    GuiMessage(std::string m, std::string s_id, std::string p_id) 
        : message(m), senderId(s_id), parentId(p_id) {}
    ~GuiMessage() {}
    
    std::string getMessage() { return message; }
    std::string getSenderId(){ return senderId; }
    std::string getParentId(){ return parentId; }
    
private:
    std::string message;
    std::string senderId;
    std::string parentId;
};

void sendEvents();