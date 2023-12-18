#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;

std::string stemmer(std::string word)
{
    if (word.size() > 3 && word.substr(word.size() - 3) == "ing")
    {
        word = word.substr(0, word.size() - 3);
    }
    else if (word.size() > 2 && word.substr(word.size() - 2) == "ed")
    {
        word = word.substr(0, word.size() - 2);
    }
    else if (word.size() > 2 && word.substr(word.size() - 2) == "ly")
    {
        word = word.substr(0, word.size() - 2);
    }
    else if (word.size() > 2 && word.substr(word.size() - 2) == "es")
    {
        word = word.substr(0, word.size() - 2);
    }
    else if (word.size() > 1 && word.substr(word.size() - 1) == "s")
    {
        word = word.substr(0, word.size() - 1);
    }
    return word;
}
QNA_tool::QNA_tool()
{
    // Implement your function here
    prev_para = -1;
    masterList();
}

QNA_tool::~QNA_tool()
{
    // Implement your function here
}

void QNA_tool::masterList()
{
    fstream file;
    file.open("unigram_freq.csv", ios::in);
    string line;
    bool flag = false;
    while (getline(file, line))
    {
        long long count;
        char comma;
        int idx;
        if (flag)
        {
            string word = "";
            for (int i = 0; i < line.size(); i++)
            {
                if (line[i] == ',')
                {
                    idx = i + 1;
                    comma = ',';
                    break;
                }
                else
                {
                    word = word + line[i];
                }
            }
            string tempstring = "";
            for (int i = idx; i < line.size(); i++)
            {
                tempstring = tempstring + line[i];
            }
            count = std::stoll(tempstring);
            if (comma == ',')
            {

                csv_dict.insert_with_count(word, count);
            }
        }
        else
        {
            flag = true;
        }
    }
    file.close();
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    // Implement your function here
    if (prev_para != paragraph)
    {
        paragraphs.push_back({book_code, page, paragraph});
        score.push_back(0);
        score2.push_back(0);
        prev_para = paragraph;
    }
    int i = paragraphs.size();
    i = i - 1;
    words_and_add(i, sentence);
    return;
}

void QNA_tool::words_and_add(int index, string &sentence)
{
    string separators = ". , - : ! \" \' ( ) ? — [ ] “ ” ‘ ’ ˙ ; @";
    string word = "";
    int s = sentence.size();

    vector<string> words;
    for (int i = 0; i < s; i++)
    {
        if (separators.find(sentence[i]) != string ::npos)
        {
            if (word != "")
                words.push_back(word);
            word = "";
        }

        else if ('A' <= sentence[i] && sentence[i] <= 'Z')
            word += sentence[i] - 'A' + 'a';
        else
            word += sentence[i];
    }
    if (word != "")
        words.push_back(word);
    paragraphs[index].words.insert(paragraphs[index].words.end(), words.begin(), words.end());
    corpus_dict.insert_sentence(sentence);
}

double QNA_tool::calculate_score(string &word)
{
    long long int corpus_count = corpus_dict.get_word_count(word);
    long long int csv_count = csv_dict.get_word_count(word);
    double score = ((double)(1 + corpus_count)) / (1 + csv_count);
    return score;
}

double QNA_tool::calculate_score3(string &word)
{
    long long int corpus_count = corpus_dict.get_word_count(word);
    long long int csv_count = csv_dict.get_word_count(word);
    double score = ((double) 1000000/ ((1 + corpus_count)*(1 + csv_count)));
    return score;
}

Node *QNA_tool::make_list(Heap &para_heap)
{
    Node *head = new Node();
    head->right = nullptr;
    Node *temp = head;
    while (para_heap.heap.size() != 0)
    {
        int index = para_heap.heap[0]->id;
        // cout<<para_heap.heap[0]->score<<endl;
        temp->book_code = paragraphs[index].book_code;
        temp->page = paragraphs[index].page;
        temp->paragraph = paragraphs[index].para;

        para_heap.removetop();
        Node *temp1 = new Node();
        temp->left = temp1;
        temp1->right = temp;
        temp = temp1;
    }
    if (temp->right != nullptr)
    {
        temp = temp->right;
        temp->left->right = nullptr;
        temp->left = nullptr;
    }

    return temp;
}

vector<string> QNA_tool::makewords(string &query)
{
    string separators = ". , - : ! \" \' ( ) ? — [ ] “ ” ‘ ’ ˙ ; @";
    string word = "";
    int s = query.size();

    vector<string> words;
    for (int i = 0; i < s; i++)
    {
        if (separators.find(query[i]) != string ::npos)
        {
            if (word != "")
                words.push_back(word);
            word = "";
        }

        else if ('A' <= query[i] && query[i] <= 'Z')
            word += query[i] - 'A' + 'a';
        else
            word += query[i];
    }
    if (word != "")
        words.push_back(word);
    return words;
}

vector<string> QNA_tool::makewords2(string &query)
{
    string separators = ". , - : ! \" \' ( ) ? — [ ] “ ” ‘ ’ ˙ ; @";
    string word = "";
    int s = query.size();

    vector<string> words;
    for (int i = 0; i < s; i++)
    {
        if (separators.find(query[i]) != string ::npos)
        {
            if (word != "" && word != "the" && word != "of" && word != "and" && word != "to" && word != "a" && word != "in" && word != "for" && word != "is" && word != "on" && word != "that" && word != "by" && word != "this" && word != "with" && word != "i" && word != "you" && word != "it" && word != "not" && word != "or" && word != "be" && word != "are" && word != "from" && word != "at" && word != "as" && word != "your" && word != "all" && word != "have" && word != "an" && word != "was" && word != "we" && word != "can" && word != "us" && word != "if" && word != "my" && word != "has" && word != "our" && word != "do" && word != "he" && word != "she" && word != "they" && word != "their" && word != "his" && word != "her" && word != "am" && word != "these" && word != "its" && word != "what" && word != "why" && word != "where" && word != "describe" && word != "explain" && word != "tell" && word != "how" && word != "who" && word != "do" && word != "an" && word != "which" && word != "when" && word != "whose" && word != "whom")
                words.push_back(word);
            word = "";
        }

        else if ('A' <= query[i] && query[i] <= 'Z')
            word += query[i] - 'A' + 'a';
        else
            word += query[i];
    }
    if (word != "" && word != "the" && word != "of" && word != "and" && word != "to" && word != "a" && word != "in" && word != "for" && word != "is" && word != "on" && word != "that" && word != "by" && word != "this" && word != "with" && word != "i" && word != "you" && word != "it" && word != "not" && word != "or" && word != "be" && word != "are" && word != "from" && word != "at" && word != "as" && word != "your" && word != "all" && word != "have" && word != "an" && word != "was" && word != "we" && word != "can" && word != "us" && word != "if" && word != "my" && word != "has" && word != "our" && word != "do" && word != "he" && word != "she" && word != "they" && word != "their" && word != "his" && word != "her" && word != "am" && word != "these" && word != "its" && word != "what" && word != "why" && word != "where" && word != "describe" && word != "explain" && word != "tell" && word != "how" && word != "who" && word != "do" && word != "an" && word != "which" && word != "when" && word != "whose" && word != "whom")
        words.push_back(word);
    return words;
}

Node *QNA_tool::get_top_k_para(string question, int k)
{
    // Implement your function here

    vector<string> query = makewords(question);
    fill(score.begin(), score.end(), 0);

    for (string word : query)
        set_score(word);

    Heap paraheap;

    int size = min(k, (int)paragraphs.size());
    int i = 0;
    for (i = 0; i < size; i++)
    {
        HeapNode *newNode = new HeapNode(score[i], i);
        paraheap.heap.push_back(newNode);
        ;
        paraheap.heapup(paraheap.heap.size() - 1);
    }
    for (; i < paragraphs.size(); i++)
    {
        if (score[i] > paraheap.heap[0]->score)
        {
            paraheap.heap[0]->score = score[i];
            paraheap.heap[0]->id = i;

            paraheap.heapdown(0);
        }
    }
    return make_list(paraheap);

    return nullptr;
}

Node *QNA_tool::get_top_k_para2(string question, int k)
{
    // Implement your function here

    vector<string> query = makewords2(question);
    fill(score2.begin(), score2.end(), 0);

    for (string word : query)
        set_score3(word);

    Heap paraheap;

    int size = min(k, (int)paragraphs.size());
    int i = 0;
    for (i = 0; i < size; i++)
    {
        HeapNode *newNode = new HeapNode(score2[i], i);
        paraheap.heap.push_back(newNode);
        ;
        paraheap.heapup(paraheap.heap.size() - 1);
    }
 
    for (; i < paragraphs.size(); i++)
    {
        if (score2[i] > paraheap.heap[0]->score)
        {
            paraheap.heap[0]->score = score2[i];
            paraheap.heap[0]->id = i;

            paraheap.heapdown(0);
        }
    }
    return make_list(paraheap);

    return nullptr;
}


void QNA_tool::set_score(string &query)
{
    double word_score = calculate_score(query);
    int frequency = 0;
    for (int i = 0; i < paragraphs.size(); i++)
    {
        frequency = 0;
        for (int j = 0; j < paragraphs[i].words.size(); j++)
        {
            if (paragraphs[i].words[j] == query)
                frequency++;
        }
        score[i] += frequency * word_score;
    }
}

void QNA_tool::set_score3(string &query)
{
    
    double word_score = calculate_score3(query);
    int frequency = 0;
    for (int i = 0; i < paragraphs.size(); i++)
    {
        frequency = 0;
        for (int j = 0; j < paragraphs[i].words.size(); j++)
        {
            if (paragraphs[i].words[j] == query)
                frequency++;
        }
        
        score2[i] += frequency * word_score;
        
    }
    string query2 = stemmer(query);
    if(query!=query2){
        word_score = calculate_score3(query2);
    
    for (int i = 0; i < paragraphs.size(); i++)
    {
        frequency = 0;
        for (int j = 0; j < paragraphs[i].words.size(); j++)
        {
            if (paragraphs[i].words[j] == query2)
                frequency++;
        }
        score2[i] += frequency * word_score;
        
    }
    }
    
    
}
// void QNA_tool::set_score2(string &query)
// {
//     double word_score = calculate_score(query);
    
//     double word_score2 = calculate_score(query2);
//     cout<<query<<query2<<endl;
//     int frequency = 0;
//     int frequency2 = 0;

//     for (int i = 0; i < paragraphs.size(); i++)
//     {
//         frequency = 0;
//         frequency2 = 0;  
//         for (int j = 0; j < paragraphs[i].words.size(); j++)
//         {
//             if (paragraphs[i].words[j] == query)
//             {
//                 frequency++;
//             }
//             else if (paragraphs[i].words[j] == query2)
//             {
//                 frequency2++;
//             }
//         }

//         score2[i] += frequency * word_score;
//         score2[i] += frequency2 * word_score2;
//         cout<<"BBYEE"<<endl;
//     }
// }


void QNA_tool::query(string question, string filename)
{
    // Implement your function here
    // Implement your function here
    Node *head = get_top_k_para2(question, 3);
    string qwerty = "";

    Node *temp;
    temp = head;
    while (temp != nullptr)
    {
        for (int i = 0; i < paragraphs.size(); i++)
        {
            if (temp->paragraph == paragraphs[i].para && temp->book_code == paragraphs[i].book_code && temp->page == paragraphs[i].page)
            {
                qwerty+="Book code:: ";
                qwerty+=to_string(paragraphs[i].book_code);
                qwerty+="   Page:: ";
                qwerty+=to_string(paragraphs[i].page);
                qwerty+="   Paragraph:: ";
                qwerty+=to_string(paragraphs[i].para);
                qwerty+="\n";
                for (int s = 0; s < paragraphs[i].words.size(); s++)
                {
                    qwerty += paragraphs[i].words[s];
                    qwerty += " ";
                    
                }
                qwerty+=".";
                qwerty+="\n";
                

  
            }
        }
        temp = temp->right;
    }

    qwerty += "With respect to the above given context, answer the below question in multiple paragraphs the best possible way and give the necessary references in points.";
    qwerty += question;
    cout << qwerty << endl;

    query_llm(filename, head, 3, "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", qwerty);
    // std::cout << "Q: " << question << std::endl;
    // std::cout << "A: Studying COL106 :)" << std::endl;
    return;
    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph)
{

    std::string filename = "mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open())
    {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence))
    {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ','))
        {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos)
            {
                token = token.substr(start, end - start + 1);
            }

            // Check if the element is a number or a string
            if (token[0] == '\'')
            {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            }
            else
            {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if (
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph))
        {
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node *root, int k, string API_KEY, string question)
{

    // first write the k paragraphs into different files

    Node *traverse = root;
    int num_paragraph = 0;

    while (num_paragraph < k)
    {
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();

    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}

//  #include <ctime>

//  clock_t start=clock();
//  clock_t end=clock()
//  cout<<((double) end-start)/CLOCKS_PER_SC<<" seconds\n";