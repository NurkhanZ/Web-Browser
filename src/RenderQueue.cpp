#include "RenderQueue.h"

RenderQueue& RenderQueue::getInstance(){
    static RenderQueue instance;
    return instance;
}

void RenderQueue::push(Node* node){
    renderQueue.push(node);
}

void RenderQueue::populateQueue(Node* root){
    renderQueue.push(root);
    for (Node* child : root->children) {
        populateQueue(child);
    }
}

RenderQueue::RenderQueue() : fm(FontManager::getInstance()), renderer(g_renderer){

}

void RenderQueue::render(){
    while(!renderQueue.empty()){
        Node* node = renderQueue.front();
        this->renderNode(node);
        renderQueue.pop();
    }
}

void RenderQueue::renderNode(Node* node){
    ElementNode *elementNode = dynamic_cast<ElementNode*>(node);

    TextNode *textNode = dynamic_cast<TextNode*>(node);
    if(elementNode){
        // TODO
    }
    else if(textNode){
        // debug message

        TTF_Font* font = fm.getFont("Montserrat", 18);
        if(!font){
            std::cerr << "Font not loaded" << std::endl;
            return ;
        }

        y += TTF_FontHeight(font) + 5;

        SDL_Surface* textSurface = TTF_RenderText_Blended(font, textNode->textContent.c_str(), text_BLACK);

        if(textSurface){
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_renderer, textSurface);
            if(textTexture){
                SDL_Rect textRect = {100, 100 + y, textSurface->w, textSurface->h};
                SDL_RenderCopy(g_renderer, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }
    }
}