#include "include/AudioBook.hpp"


int main(int argc, char **argv){
    std::string title = "学习";
    std::string author = "me";
    std::string narrow = "66666";
    AudioBook auditBook(title, author, narrow);
    auditBook.print();

    return 0;
}