// Do NOT add any other includes
#include "search.h"
vector <int> bad_table_vector(string &pattern);
vector<int> boyer_moore(string &sentence, string &pattern);

SearchEngine::SearchEngine(){
    // Implement your function here  
}

SearchEngine::~SearchEngine(){
    // Implement your function here  
    long long s=text.size();
    for(long long i=0; i<s; i++){
        delete text[i];
        text[i]=nullptr;
    }
    text.clear();
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here
    for(int i=0; i<sentence.size(); i++){
        if('A'<= sentence[i] && sentence[i]<='Z') sentence[i]=sentence[i]-'A'+'a';
    }
    Textnode* newNode= new Textnode(book_code, page, paragraph, sentence_no, sentence);
    text.push_back(newNode);
    return;
}

Node* SearchEngine::search(string pattern, int& n_matches){
    // Implement your function here
    for(int i=0; i<pattern.size(); i++){
        if('A'<= pattern[i] && pattern[i]<='Z') pattern[i]=pattern[i]-'A'+'a';
    }
    Node *res=new Node(); 
    Node* temp=new Node();
    res->right=temp;
    temp->left=res;
    for(long long i=0;i<text.size();i++){
        vector<int> v=boyer_moore(text[i]->sentence,pattern);
        if(v.size()==0) continue;
        for(int j=0;j<v.size();j++)
        {
            n_matches++;
            temp->book_code=text[i]->book_code;
            temp->page=text[i]->page;
            temp->paragraph=text[i]->paragraph;
            temp->sentence_no=text[i]->sentence_no;
            temp->offset=v[j];
            temp->right=new Node();
            temp->right->left=temp;
            temp=temp->right;
        }

    }
    if(temp->left==res&&temp->paragraph==-1)
    return nullptr;
    temp=temp->left;
    delete temp->right;
    temp->right=nullptr;
    Node* ans=res->right;
    if(ans!=nullptr) ans->left=nullptr;
    return ans;
}

vector <int> bad_table_vector(string &pattern){
        int n=pattern.size();
        vector<int> bad_table(256, -1);
        for(int i=0; i<n; i++){
            bad_table[pattern[i]]=i;
        }
        return bad_table;
    }
    vector<int> boyer_moore(string &sentence, string &pattern){
        vector<int> offsets;
        int pattern_length=pattern.length();
        int sentence_length=sentence.length();
        if(pattern_length>sentence_length)
        return offsets;
        vector<int> bct=bad_table_vector(pattern);
        int index=0;
        while(index<=sentence_length-pattern_length){
            int i=pattern_length-1;
            while(i>=0&&pattern[i]==sentence[index+i]){
                i=i-1;
            }
            if(i<0){
                offsets.push_back(index);
                if(index+pattern_length<sentence_length){
                    int bct_value;
                    bct_value=bct[sentence[index+pattern_length]];
                    index+=pattern_length-bct_value;
                }
                else{
                    index+=1;
                }
            } 
            else{
                int bct_value;
                bct_value=bct[sentence[index+i]];
                index+=max(1, i-bct_value);
            }
        }
        return offsets;
    }

    // int main(){
    //     SearchEngine s;
    //     s.insert_sentence(0,0,0,0,"h         ehehehe");
    //     int count=0;
    //     Node* obj=s.search("he", count);
    //     cout<<count;

    // }