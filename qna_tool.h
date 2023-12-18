#pragma once
#include <iostream>
#include <fstream>
#include "Node.h"
#include "dict.h"
#include "search.h"

using namespace std;

struct HeapNode
{
    double score;
    int id;
    HeapNode(double s, int i)
    {
        score = s;
        id = i;
    }
    bool operator>(HeapNode const &oth) const
    {
        return score > oth.score;
    }
    bool operator<(HeapNode const &oth) const
    {
        return score < oth.score;
    }
    ~HeapNode()
    {
        score = 0;
        id = 0;
    }
};

class Heap
{
public:
    vector<HeapNode *> heap;
    long long parent(long long index)
    {
        if (index == 0)
            return -1;
        return (index - 1) / 2;
    }
    long long left(long long index)
    {
        return 2 * index + 1;
    }
    long long right(long long index)
    {
        return 2 * index + 2;
    }
    void heapup(long long index)
    {
        if (index == 0)
            return;
        long long par = parent(index);
        if (heap[par]->score > heap[index]->score)
        {
            swap(heap[par], heap[index]);
            heapup(par);
        }
    }
    void heapdown(long long index)
    {
        long long lt = left(index);
        if (heap.size() <= lt)
            return;

        long long minind = index;
        if (heap[lt]->score < heap[minind]->score)
            minind = lt;
        long long rt = right(index);
        if (rt < heap.size())
        {
            if (heap[rt]->score < heap[minind]->score)
                minind = rt;
        }
        // cout<<"Hi "<<minind;
        if (minind == index)
            return;
        swap(heap[index], heap[minind]);
        heapdown(minind);
    }
    void swap(HeapNode *a, HeapNode *b)
    {
        double t1 = a->score;
        int t2 = a->id;
        a->score = b->score;
        a->id = b->id;
        b->score = t1;
        b->id = t2;
    }
    void removetop()
    {
        if (heap.size() == 1)
        {
            delete heap[0];
            heap[0] = nullptr;
            heap.pop_back();
            return;
        }
        swap(heap[0], heap.back());
        delete heap[heap.size() - 1];
        heap[heap.size() - 1] = nullptr;
        heap.pop_back();
        heapdown(0);
    }
};

class Paragraph
{
public:
    int book_code;
    int page;
    int para;
    vector<string> words;

    Paragraph(int b, int pg, int p)
    {
        book_code = b;
        page = pg;
        para = p;
    }
};

class QNA_tool
{

private:
    int prev_para;

    // You are free to change the implementation of this function
    void query_llm(string filename, Node *root, int k, string API_KEY, string question);
    // filename is the python file which will call ChatGPT API
    // root is the head of the linked list of paragraphs
    // k is the number of paragraphs (or the number of nodes in linked list)
    // API_KEY is the API key for ChatGPT
    // question is the question asked by the user

    // You can add attributes/helper functions here

public:
    vector<Paragraph> paragraphs;
    Dict corpus_dict;
    Dict csv_dict;
    vector<double> score;
    vector<double> score2;
    void words_and_add(int i, string &sentence);
    double calculate_score(string &word);
    double calculate_score3(string &word);
    void set_score(string &query);
    void set_score3(string &query);
    void masterList();
    vector<string> makewords(string &query);
    vector<string> makewords2(string &query);
    Node *make_list(Heap &para_heap);
    Node *get_top_k_para2(string question, int k);
    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */

    QNA_tool();  // Constructor
    ~QNA_tool(); // Destructor

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    // This function is similar to the functions in dict and search
    // The corpus will be provided to you via this function
    // It will be called for each sentence in the corpus
    
    Node *get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    std::string get_paragraph(int book_code, int page, int paragraph);
    // Given the book_code, page number, and the paragraph number, returns the string paragraph.
    // Searches through the corpus.

    void query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
    
};