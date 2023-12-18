// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include "Node.h"
using namespace std;

class Textnode{
public:
    int book_code;
    int page;
    int paragraph;
    int sentence_no;
    string sentence;

    Textnode(int b_code, int pg, int para, int s_no, string s){
        book_code=b_code;
        page=pg;
        paragraph=para;
        sentence_no=s_no;
        sentence=s;
    }
    ~Textnode(){
        book_code=0;
        page=0;
        paragraph=0;
        sentence_no=0;
        sentence.clear();
    }
};

class SearchEngine {
private:
    // You can add attributes/helper functions here
    vector <Textnode*> text;

public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    SearchEngine();

    ~SearchEngine();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    Node* search(string pattern, int& n_matches);

    /* -----------------------------------------*/
};