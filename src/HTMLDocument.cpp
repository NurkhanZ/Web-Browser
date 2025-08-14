#include <HTMLDocument.h>
#include <stack>
#include <unordered_map>

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
                break;

            case TokenType::TEXT:
                {
                    if(!stack.empty()){
                        TextNode* textNode = new TextNode();
                        textNode->textContent = token.value;
                        stack.top()->children.push_back(textNode);
                    }
                }
                break;
        }
    }
    return root;
}

std::pair<std::string, std::vector<std::string>> HTMLDocument::parseAttributes(std::string tag){
    std::pair<std::string, std::vector<std::string>> attributes;
    size_t space = tag.find(' ');
    if(space != std::string::npos){
        attributes.first = tag.substr(0, space);
        tag = tag.substr(space + 1);
    }
    else{
        attributes.first = tag;
        return attributes;
    }

    size_t start = 0;
    size_t end = 0;
    while(end != std::string::npos){
        end =  tag.find(' ', start);
        if(end != std::string::npos){
            attributes.second.push_back(tag.substr(start, end - start));
            start = end + 1;
        }
        else{
            attributes.second.push_back(tag.substr(start));
        }
    }
    return attributes;
}

ElementType HTMLDocument::getElementType(std::string tag){
    static const std::unordered_map<std::string, ElementType> elementTypes = {
        {
            {"a", ElementType::A},
            {"abbr", ElementType::ABBR},
            {"address", ElementType::ADDRESS},
            {"area", ElementType::AREA},
            {"article", ElementType::ARTICLE},
            {"aside", ElementType::ASIDE},
            {"audio", ElementType::AUDIO},
            {"b", ElementType::B},
            {"base", ElementType::BASE},
            {"bdi", ElementType::BDI},
            {"bdo", ElementType::BDO},
            {"blockquote", ElementType::BLOCKQUOTE},
            {"body", ElementType::BODY},
            {"br", ElementType::BR},
            {"button", ElementType::BUTTON},
            {"canvas", ElementType::CANVAS},
            {"caption", ElementType::CAPTION},
            {"cite", ElementType::CITE},
            {"code", ElementType::CODE},
            {"col", ElementType::COL},
            {"colgroup", ElementType::COLGROUP},
            {"data", ElementType::DATA},
            {"datalist", ElementType::DATALIST},
            {"dd", ElementType::DD},
            {"del", ElementType::DEL},
            {"details", ElementType::DETAILS},
            {"dfn", ElementType::DFN},
            {"dialog", ElementType::DIALOG},
            {"div", ElementType::DIV},
            {"dl", ElementType::DL},
            {"dt", ElementType::DT},
            {"em", ElementType::EM},
            {"embed", ElementType::EMBED},
            {"fieldset", ElementType::FIELDSET},
            {"figcaption", ElementType::FIGCAPTION},
            {"figure", ElementType::FIGURE},
            {"footer", ElementType::FOOTER},
            {"form", ElementType::FORM},
            {"h1", ElementType::H1},
            {"h2", ElementType::H2},
            {"h3", ElementType::H3},
            {"h4", ElementType::H4},
            {"h5", ElementType::H5},
            {"h6", ElementType::H6},
            {"head", ElementType::HEAD},
            {"header", ElementType::HEADER},
            {"hr", ElementType::HR},
            {"html", ElementType::HTML},
            {"i", ElementType::I},
            {"iframe", ElementType::IFRAME},
            {"img", ElementType::IMG},
            {"input", ElementType::INPUT},
            {"ins", ElementType::INS},
            {"kbd", ElementType::KBD},
            {"label", ElementType::LABEL},
            {"legend", ElementType::LEGEND},
            {"li", ElementType::LI},
            {"link", ElementType::LINK},
            {"main", ElementType::MAIN},
            {"map", ElementType::MAP},
            {"mark", ElementType::MARK},
            {"meta", ElementType::META},
            {"meter", ElementType::METER},
            {"nav", ElementType::NAV},
            {"noscript", ElementType::NOSCRIPT},
            {"object", ElementType::OBJECT},
            {"ol", ElementType::OL},
            {"optgroup", ElementType::OPTGROUP},
            {"option", ElementType::OPTION},
            {"output", ElementType::OUTPUT},
            {"p", ElementType::P},
            {"picture", ElementType::PICTURE},
            {"pre", ElementType::PRE},
            {"progress", ElementType::PROGRESS},
            {"q", ElementType::Q},
            {"rp", ElementType::RP},
            {"rt", ElementType::RT},
            {"ruby", ElementType::RUBY},
            {"s", ElementType::S},
            {"samp", ElementType::SAMP},
            {"script", ElementType::SCRIPT},
            {"section", ElementType::SECTION},
            {"select", ElementType::SELECT},
            {"small", ElementType::SMALL},
            {"source", ElementType::SOURCE},
            {"span", ElementType::SPAN},
            {"strong", ElementType::STRONG},
            {"style", ElementType::STYLE},
            {"sub", ElementType::SUB},
            {"summary", ElementType::SUMMARY},
            {"sup", ElementType::SUP},
            {"table", ElementType::TABLE},
            {"tbody", ElementType::TBODY},
            {"td", ElementType::TD},
            {"template", ElementType::TEMPLATE},
            {"textarea", ElementType::TEXTAREA},
            {"tfoot", ElementType::TFOOT},
            {"th", ElementType::TH},
            {"thead", ElementType::THEAD},
            {"time", ElementType::TIME},
            {"title", ElementType::TITLE},
            {"tr", ElementType::TR},
            {"track", ElementType::TRACK},
            {"u", ElementType::U},
            {"ul", ElementType::UL},
            {"var", ElementType::VAR},
            {"video", ElementType::VIDEO},
            {"wbr", ElementType::WBR}
        }
    };

    tag = tag.substr(1, tag.size() - 2);
    size_t space = tag.find(' ');
    if(space != std::string::npos){
        tag = tag.substr(0, space);
    }

    auto it = elementTypes.find(tag);
    if(it != elementTypes.end()){
        return it->second;
    }

    #ifdef DEBUG
        std::cerr << "Unknown element type: " << tag << std::endl;
    #endif

    return ElementType::UNKNOWN;
}