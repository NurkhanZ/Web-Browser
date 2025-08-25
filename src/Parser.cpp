#include "Parser.hpp"

Parser::Parser(string address){
    this->file.open(address);
    this->root = new Root();
    this->current_tag = root;

    #ifdef DEBUG    // debug private methods here
    string temp = parseSize("30px");
    cout << temp << endl;
    #endif    
}

BaseTag* Parser::parse(){
    nextChar(); 
    while(this->file.is_open() && this->file.eof()){
        if(current_char == '<'){
            newMarkup();
            nextChar();
        }
        else if(current_char == '\n'){
            nextChar();
        }
        else{
            newText();
        }
    }

    return this->root;
}

//reads next character
char Parser::nextChar(){
    if(!file.eof()){
        this->file.get(this->current_char);
        return this->current_char;
    }
    #ifdef DEBUG
        cout << current_tag->type << " - End of file!" << endl;
    #endif
    return '\0';
}

void Parser::newMarkup(){
    bool closing_tag = false;
    bool close = false;
    string type;

    // closing tag check
    if(nextChar() == '/'){
        closing_tag = true;
        nextChar();
    }

    // comment tag check
    else if(current_char == '!'){
        string comment;
        while(comment != ">"){
            if(comment.size() == 1){
                comment.erase(0, 1);
                comment += nextChar();
            }
            else comment += nextChar();
        }
    }

    for(; current_char != ' ' && current_char != '>'; nextChar()){
        type += current_char;
    }

    // get text inside <>
    type = toLower(type);
    if(type.back() == '/'){
        close = true;
        type = type.substr(0, type.size() - 1);
    }
    
    // title tag check
    if(type == "title"){
        string combination, value;
        while(current_char != '>'){
            nextChar();
        }
        while(toLower(combination) != "</style>"){
            if(combination.size() == 7){
                combination.erase(0, 1);
                combination += nextChar();
            }
            else combination += nextChar();
            value += current_char;
        }
        value = value.substr(0, value.size() - 7);
        this->addCSS(cleanWhitespace(value));
        while(current_char != '>'){
            nextChar();
        }
        return ;
    }

    // close tag
    if(closing_tag){
        for(; current_char != '>'; nextChar()){
            #ifdef DEBUG
                cout << ". ";
            #endif
        }
        closeTag(type);
        return;
    }

    // create tag
    if(!createTag(type)){
        current_tag = current_tag->addChild(new GenericTag(type));
    }

    // attributes
    while(current_char = ' '){
        string attribute_key;
        string attribute_value;

        // type of attribute
        for(nextChar(); current_char != '=' && current_char != ' ' && current_char != '>'; nextChar()){
            attribute_key += current_char;
        }
        if(current_char != '=') break;

        nextChar();

        // value of attribute
        if(current_char != '\"'){
            for(nextChar(); current_char != '\"'; nextChar()){
                attribute_value += current_char;
            }
            nextChar();
        }
        
        else if(current_char != '\''){
            for(nextChar(); current_char != '\''; nextChar()){
                attribute_value += current_char;
            }
            nextChar();
        }

        else{
            for(; current_char != ' ' && current_char != '>'; nextChar()){
                attribute_value += current_char;
            }
        }
        this->setAttribute(toLower(attribute_key), attribute_value);
    }

    //set style
    setStyle(getCSS(type));
    setStyle(getCSSByClass(current_tag->getAttribute(CLASS)));
    setStyle(getCSSById(current_tag->getAttribute(ID)));
    setStyle(current_tag->getAttribute(STYLE));

    // standalone check
    if(current_tag->standalone){
        closeTag(type);
    }
    else if(close){
        closeTag(type);
    }
}   

void Parser::newText(){
    string text;
    bool in_whitespace = false;
    for(; current_char != '<' && !this-file.eof(); nextChar()){
        if(current_char == ' ' || current_char == '\n'){
            if(!in_whitespace){
                text += ' ';
                in_whitespace = true;
            }
        }
        else{
            text += current_char;
            in_whitespace = false;
        }
    }
    BaseTag* raw_text = new Text;
    raw_text->setAttribute(TEXT_CONTENT, text);
    this->current_tag->addChild(raw_text);
}

void Parser::closeTag(string type){
    // find ancestor
    BaseTag* closing_tag = this->current_tag;
    while(!closing_tag){
        closing_tag = closing_tag->getParent();
        if(closing_tag == nullptr){
            return ;
        }
    }

    // copy lineage
    BaseTag* current_destination = closing_tag->getParent();
    if(closing_tag != this->current_tag){
        BaseTag* current_source = *(closing_tag->getLastChild() - 1);
        while(current_destination != this->current_tag){
            current_destination = current_destination->addChild(current_source->copy());
            current_source = *(current_source->getLastChild() - 1);
        }
    }
    this->current_tag = current_destination;
}

bool Parser::createTag(string type){
    // include here all html tags
    if(type == "br") current_tag = current_tag->addChild(new BreakTag);
    // else if(type == "b") current_tag = current_tag->addChild(new BoldTag);
    // else if(type == "i") current_tag = current_tag->addChild(new ItalicTag);
    // else if(type == "body") current_tag = current_tag->addChild(new BodyTag);
    // else if(type == "p") current_tag = current_tag->addChild(new ParagraphTag);
    // else if(type == "h1") current_tag = current_tag->addChild(new H1Tag);
    // else if(type == "h2") current_tag = current_tag->addChild(new H2Tag);
    // else if(type == "h3") current_tag = current_tag->addChild(new H3Tag);
    // else if(type == "h4") current_tag = current_tag->addChild(new H4Tag);
    // else if(type == "h5") current_tag = current_tag->addChild(new H5Tag);
    // else if(type == "h6") current_tag = current_tag->addChild(new H6Tag);
    else return false;
    return true;
}

string Parser::getCSS(string type){
    try{
        return this->css.at(type);
    }
    catch(...){
        return "";
    }
}   

string Parser::getCSSByClass(string _class){
    _class.insert(0, 1, '.');
    try{
        return this->css.at(_class);
    }
    catch(...){
        return "";
    }
}

string Parser::getCSSById(string id){
    id.insert(0, 1, '#');
    try{
        return this->css.at(id);
    }
    catch(...){
        return "";
    }
}

void Parser::setAttribute(string key, string value){
    if(key == "id") current_tag->setAttribute(ID, value);
    else if(key == "class") current_tag->setAttribute(CLASS, value);
    else if(key == "style") current_tag->setAttribute(STYLE, cleanWhitespace(value));
    else if(key == "width") current_tag->setAttribute(WIDTH, parseSize(toLower(value)));
    else if(key == "height") current_tag->setAttribute(HEIGHT, parseSize(toLower(value)));
    else if(key == "color") current_tag->setAttribute(COLOR, parseColor(value));
    else if(key == "background-color") current_tag->setAttribute(BACKGROUNDCOLOR, parseColor(value));
    else if(key == "font-size") current_tag->setAttribute(FONTSIZE, value);
    else if(key == "padding"){
        current_tag->setAttribute(PADDING_TOP, value);
        current_tag->setAttribute(PADDING_LEFT, value);
        current_tag->setAttribute(PADDING_BOTTOM, value);
        current_tag->setAttribute(PADDING_RIGHT, value);
    }
}
void Parser::setStyle(string raw_style){
    while(raw_style != ""){
        int top = raw_style.find(':');
        string key = toLower(raw_style.substr(0, top-1));
        top = raw_style.find(';');
        string value;
        if(top == -1){
            value = raw_style;
            raw_style = "";
        }
        else{
            value = toLower(raw_style.substr(0, top-1));
        }
        
        // apply style
        setAttribute(key, value);
    }
}

void Parser::addCSS(string raw_css){
    while(!raw_css.empty()){
        int top = raw_css.find('{');
        string key = toLower(raw_css.substr(0, top-1));
        top = raw_css.find('}');
        string value;
        if(top == -1){
            value = raw_css;
            raw_css = "";
        }
        else{
            value = toLower(raw_css.substr(0, top-1));
        }
        if(value.size() != 0 && value.at(value.size()- 1) != ';'){
            value.append(1, ';');
        }
        if(this->getCSS(key) == ""){
            this->css.at(key) = value;
        }
        else{
            this->css.at(key) += value;
        }
    }
}

string Parser::parseColor(string value){
    ColorType colorType;
    switch (colorType) {
        case WHITE:
            return "ffffff";
        case BLACK:
            return "000000";
        case RED:
            return "ff0000";
        case GREEN:
            return "00ff00";
        case BLUE:
            return "0000ff";
        case YELLOW:
            return "ffff00";
        default:
            return "000000";
    }
    return nullptr;
}

// isn't used
string Parser::parseSize(string value){
    if(value != ""){
        if(value.substr(value.size() - 2, 2) == "px"){
            return value.substr(0, value.size() - 2);
        }
        else if(value.at(value.size() - 1) == '%'){
            return value.substr(0, value.size() - 1).insert(0, 1, '%');
        }
    }
    return nullptr;
}
string Parser::toLower(string str){
    string lower_str;
    for(string::iterator it = str.begin(); it != str.end(); it++){
        if('A' <= (*it) && (*it) >= 'Z'){
            lower_str += (*it + 32);
        }
        else{
            lower_str += *it;
        }
    }
    return lower_str;
}
string Parser::cleanWhitespace(string raw){
    while(true){
        if(raw.find(' ') != -1){
            raw.erase(raw.find(' '), 1);
        }
        else if(raw.find('\n') != -1){
            raw.erase(raw.find('\n'), 1);
        }
        else return raw;
    }
    return nullptr;
}
