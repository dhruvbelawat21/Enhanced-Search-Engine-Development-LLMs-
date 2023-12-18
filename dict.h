// // Do NOT add any other includes
// #include <string>
// #include <vector>
// #include <iostream>
// #include <fstream>
// using namespace std;

// // recursive delete

// class TrieNode
// {
// public:
//     long long int count;
//     bool endofword;
//     vector<TrieNode *> alphabets;

//     TrieNode()
//     {
//         count = 0;
//         endofword = false;
//         for (int i = 0; i < 256; i++)
//         {
//             alphabets.push_back(nullptr);
//         }
//     }

//     ~TrieNode()
//     {
//         for (int i = 0; i < 256; i++)
//         {
//             del(alphabets[i]);
//             alphabets[i] = nullptr;
//         }
//         count = 0;
//         endofword = false;
//         alphabets.clear();
//     }
//     void del(TrieNode *root)
//     {
//         if (root == nullptr)
//             return;
//         for (int i = 0; i < 256; i++)
//         {
//             del(root->alphabets[i]);
//             root->alphabets[i] = nullptr;
//         }
//         delete root;
//     }
// };

// class Dict
// {
// public:
//     // You can add attributes/helper functions here
//     TrieNode *root;
//     void insert_word(string &word);
//     void recursive_print(TrieNode *node, string currword, string &filename);

//     void insert_with_count(string &word, long long new_count);
//     /* Please do not touch the attributes and
//     functions within the guard lines placed below  */
//     /* ------------------------------------------- */
//     Dict();

//     ~Dict();

//     void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

//     long long int get_word_count(string word);

//     void dump_dictionary(string filename);

//     /* -----------------------------------------*/
// };
// Do NOT add any other includes
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

// recursive delete

class TrieNode
{
public:
    long long int count;
    bool endofword;
    vector<TrieNode *> alphabets;

    TrieNode()
    {
        count = 0;
        endofword = false;
        for (int i = 0; i < 256; i++)
        {
            alphabets.push_back(nullptr);
        }
    }

    ~TrieNode()
    {
        for (int i = 0; i < 256; i++)
        {
            del(alphabets[i]);
            alphabets[i] = nullptr;
        }
        count = 0;
        endofword = false;
        alphabets.clear();
    }
    void del(TrieNode *root)
    {
        if (root == nullptr)
            return;
        for (int i = 0; i < 256; i++)
        {
            del(root->alphabets[i]);
            root->alphabets[i] = nullptr;
        }
        delete root;
    }
};

class Dict
{
public:
    // You can add attributes/helper functions here
    TrieNode *root;
    void insert_word(string &word);
    void recursive_print(TrieNode *node, string currword, string &filename);

    void insert_with_count(string &word, long long new_count);
    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(string sentence);

    long long int get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
};