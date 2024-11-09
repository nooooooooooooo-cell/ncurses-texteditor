#include "editor.h"

int main(int argc, char* argv[]){
    try{
        texteditor start(argv[1]);
    }
    catch(...){
        std::cout << "./texteditor <file>" << std::endl;
    }
    return 0;
}
