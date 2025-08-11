#include <HTMLDocument.h>
#include <stack>

HTMLDocument::HTMLDocument(std::string html){
    root = nullptr;
    this->html = html;
    tokens = tokenize();
    root = parse();
}

HTMLDocument::~HTMLDocument(){
    deleteTree(root);
}

ElementNode* HTMLDocument::getRoot(){
    return root;
}

void HTMLDocument::deleteTree(Node* node){
    if(node){
        for(Node* child : node->children){
            deleteTree(child);
        }
        delete node;
    }
}

std::vector<Token> HTMLDocument::tokenize(){
    std::vector<Token> tokens;
    std::string tokenValue;
    bool inTag = false;

    for (size_t i = 0; i < html.size(); i++){
        char c = html[i];

        if(c == '<'){
            //start of a tag
            if(!tokenValue.empty()){
                tokens.push_back({TokenType::TEXT, tokenValue});
                tokenValue.clear();
            }
            inTag = true;
            tokenValue += c;
        }
        else if(c == '>'){
            // end of a tag
            if(tokenValue[1] == '/'){
                //close tag
                tokens.push_back({TokenType::TAG_CLOSE, tokenValue});
            }
            else if(tokenValue[tokenValue.size() - 2] == '/'){
                tokens.push_back({TokenType::TAG_SELF_CLOSE, tokenValue});
            }
            else if(tokenValue[1] == '!'){
                tokenValue.clear();
            } 
            else{
                tokens.push_back({TokenType::TAG_OPEN, tokenValue});
            }
            tokenValue.clear();
            inTag = false;
        }
        else{
            tokenValue += c;
            if(!inTag && i == html.size() - 1){
                tokens.push_back({TokenType::TEXT, tokenValue});
            }
        }
    }
    return tokens;
}

ElementNode* HTMLDocument::parse(){
    std::stack<Node*> stack;

    for(const Token& token : tokens){
        switch(token.type){
            case TokenType::TAG_OPEN:
                {
                    ElementNode* node = new ElementNode();
                    node->elementType = getElementType(token.value);
                    node->birhtOrder = stack.empty() ? 0 : stack.top()->children.size();
                    if(!stack.empty()){
                        stack.top()->children.push_back(node);
                    }
                    else{
                        root = node;
                    }
                    stack.push(node);
                }
                break;
            case TokenType::TAG_CLOSE:
            {
                if(!stack.empty()){
                    stack.pop();
                }
            }
            break;

            case TokenType::TAG_SELF_CLOSE:
            {
                ElementNode* node = new ElementNode();
                node->elementType = getElementType(token.value);
                node->attributes = parseAttributes(token.value);
                node->birhtOrder = stack.empty() ? 0 : stack.top()->children.size();
                if(!stack.empty()){
                    stack.top()->children.push_back(node);
                }
                else{
                    root = node;
                    }
                
            }
        }
    }
}