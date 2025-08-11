#pragma once
#include <string>
#include <vector>
#include "HTMLElementTypes.h"
#include "globals.h"

struct Node{
    virtual ~Node(){}
    std::vector<Node*> children;
    uint32_t birhtOrder;
};

struct DocumentTypeNode : Node{
    std::string name;
    std::string publicId;
    std::string systemId;
};

struct ElementNode : Node{
    ElementType elementType;
    std::pair<std::string, std::vector<std::string>> attributes;
};

struct TextNode : Node{
    std::string textContent;
};

struct ProcessingInstructionNode : Node{
    std::string target;
    std::string data;
};

enum TokenType {
    TAG_OPEN,
    TAG_CLOSE,
    TAG_SELF_CLOSE,
    TEXT
};

struct Token {
    TokenType type;
    std::string value;
};

class HTMLDocument{
    public:
        ElementNode* root;
        std::string html;
        std::vector<Token> tokens;

        HTMLDocument(std::string html);
        ~HTMLDocument();
        ElementNode* getRoot();
    
    private:
        std::vector<Token> tokenize(); 
        ElementNode* parse();
        std::pair<std::string, std::vector<std::string>> parseAttributes(std::string tag);
        ElementType getElementType(std::string tag);
        void deleteTree(Node* node);
};