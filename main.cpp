/*  Dictionary with meanings and examples
 *
 * Using trie and linked List data structures
 *
 * by Muhammad Usman Tahir and Farhan Majeed
 *                  BSCS 9A
 *
*/



#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

//Making node class for examples
class Node {
public:
    string example;
    Node* pNext;
};

//Examples will be saved in a singly linked list
//Singly Licked List Class
class LinkedList {
public:

    Node* pStart;
    Node* pLast;

    LinkedList() {
        pStart = NULL;
    }

    //Check if list is empty or not
    bool isEmpty() {
        if (pStart == NULL)
            return true;
        else
            return false;
    }

    //inserts value to end of the list.
    void insertNode(string example) {
        Node* pNewnode = new Node();
        pNewnode->example = example;
        if (!isEmpty()) {
            pLast->pNext = pNewnode;
            pLast = pNewnode;
        }
        else {
            pStart = pNewnode;
            pLast = pNewnode;
        }
    }

    //prints the list
    void printExamples() {
        if (!isEmpty()) {
            Node* pTemp = pStart; //initializing temp pointer as start
            cout << "Examples: " << endl;
            int count = 1;
            while (pTemp != NULL) { //loop continues until last
                cout << count++ <<") " <<  pTemp->example << endl; //prints value of 'data'
                pTemp = pTemp->pNext; //promotes temp node to the next node
            }
            cout << endl;
        }
        else {
            cout << "There are no examples\n";
        }
    }

    //deletes the entire list
    void destroyList() {
        while (pStart != NULL) {
            Node* temp = pStart;
            pStart = pStart->pNext;
            delete temp;
        }
        pLast = NULL;
    }
};


//Word object that hold the word, its meaning and the linked list of examples.
class Word {
public:
    string theWord;
    LinkedList* pExamples;
    string meaning;

    //constructor
    Word(string theWord, string meaning, LinkedList* pExamples) {
        this->theWord = theWord;
        this->pExamples = pExamples;
        this->meaning = meaning;
    }

    //prints all relevant information of the word
    void printWord()
    {
        cout << "Word: " << theWord << endl;
        cout << "Meaning: " << meaning << endl;
        pExamples->printExamples();
        cout << endl;
    }

};


class TrieNode {
public:
    Word* word;  //Holds pointer to the word node that stores all info of the entered word.
    TrieNode* pDescendants[28];  //28 possible links for each TrieNode. This includes 26 alphabets, ' ' and '-'

    int let(char c) {
        if (c == ' ')
            return 0;
        if (c == '-')
            return 1;
        return c - 'a' + 2;
    }

    char firstChar(string key) { //returns the first character of the string.
        return key[0];
    }

    string rest(string key) {           // returns rest of the string after first character
        return key.substr(1);
    }

    TrieNode* descendant(string key) //
    {
        return pDescendants[let(firstChar(key))]; //returns the node which is needed.
    }

    void makeDescendant(string key)           // makes a new TrieNode at a link
    {
        pDescendants[let(firstChar(key))] = new TrieNode();
    }

    void deleteDescendant(string key)
    {
        delete pDescendants[let(firstChar(key))];
        pDescendants[let(firstChar(key))] = nullptr;
    }

    bool anyDescendants() //checks if there is a node.
    {
        for (int i = 0; i < 28; i++)
        {
            if (pDescendants[i] != nullptr)
                return true;
        }

        return false;
    }

    Word* find(string key)
    {
        if (key.length() == 0)  //base case. It is zero if length of key is zero.
            return word;
        else
        {
            if (descendant(key) == nullptr) //if there is no node it doesnt exist
                return nullptr;
            else return descendant(key)->find(rest(key)); //calls the next alphabet (node) to search.
        }
    }

    void insert(Word* word, string toHere)  //Inserts a word
    {
        if (toHere.length() == 0)  //base case. length is zero. insert word at this node.
            this->word = word;
        else
        {
            if (descendant(toHere) == nullptr)  //if there is no descendant then make node.
                makeDescendant(toHere);
            descendant(toHere)->insert(word, rest(toHere));
        }
    }

    void deleteWord(string key)
    {
        if (key.length() == 0)
        {
            word->pExamples->destroyList();
            delete word;
            word = nullptr;
        }

        else
        {
            descendant(key)->deleteWord(rest(key));
            if (!descendant(key)->anyDescendants() && descendant(key)->word == nullptr)  //check if descendant doesn't have any grand descendands and nor does it have a word node.
                deleteDescendant(key);
        }
    }

    void printAllNodes()
    {
        if (word != nullptr)
            word->printWord();

        for (int i = 0; i < 28; i++)
            if (pDescendants[i] != nullptr)
                pDescendants[i]->printAllNodes();
    }

    void prefixMatch(string prefix)
    {
        if (prefix.length() == 0)  //base case
            printAllNodes();

        else if (descendant(prefix) != nullptr)  //descendant is not null.
        {
            descendant(prefix)->prefixMatch(rest(prefix));
        }
    }
};

class Trie {
public:
    TrieNode* root;
    Trie()
    {
        root = new TrieNode();
    }

    Word* find(string key)
    {
        return root->find(key);
    }

    void printWord(string key)
    {
        Word* pTemp = find(key);

        if (pTemp == nullptr)
            cout << "Word not found. " << endl;
        else
            pTemp->printWord();
    }

    void insert(Word* pWord)
    {
        if (root->find(pWord->theWord) == nullptr)
        {
            root->insert(pWord, pWord->theWord);
        }
    }

    bool deleteWord(string key)
    {
        if (root->find(key) == nullptr)
            return false;

        else {
            root->deleteWord(key);
            cout << key << " has been deleted from the dictionary." << endl;
            return true;
        }
    }

    void prefixMatch(string key)
    {
        root->prefixMatch(key);
    }

    void printAll(){
        root->printAllNodes();
    }
    void InsertIntoDictionary() {

        LinkedList *pExamples = new LinkedList();
        string word, meaning;

        do {
            cout << "Enter the word you want to register:" << endl;
            cin.ignore();
            getline(cin, word);
            if (word == "")
                cout << "Please enter a word\n";
        }while (word == "");


        do{
        cout << "Enter its meaning:" << endl;
        getline(cin, meaning);
            if (meaning == "")
                cout << "Please enter a meaning\n";
        }while (meaning == "");

        int a = 0, b = 0;
        string ex;

        do{

            cout << "Enter example number " << a + 1 << endl;
            getline(cin, ex);
            pExamples->insertNode(ex);

            cout << "Press 2 if you want to make another example. Press 4 if you do not want to make another example.";
            cin >> b;
            if (b == 4) {
                a = 4;
            }
            cin.ignore();
            a++;
        } while (a <= 3);
        Word *wordNode = new Word(word, meaning, pExamples);
        insert(wordNode);
    }

};


void parser(Trie* dictionary) {
    fstream myFileStream("C:\\Users\\Computer\\CLionProjects\\FinalProject\\proj2.csv");

    if (!myFileStream.is_open()) {
        cout << "File failed to open." << endl;
        return;
    }

    string line, word, meaning, temp, example[9];

    while (getline(myFileStream, line)) {
        stringstream ss(line);


        getline(ss, word, ',');
        //cout << word << endl;



        getline(ss, meaning, '%');
        if (meaning[0] == '"')
            meaning = meaning.substr(1);
       // cout << meaning << endl;

        getline(ss, temp, ','); //ignoring comma

        LinkedList* examples = new LinkedList();

        for (int i = 0; i < 10; i++) {

            getline(ss, example[i], '%');
            if (example[i][0] == '\"')
                example[i] = example[i].substr(1);

            getline(ss, temp, ','); //ignoring comma

            if (example[i] == "") {
                break;
            }
            examples->insertNode(example[i]);
           // cout << i + 1 << ") " << example[i] << endl;
        }
        //cout<< "out of examples Loop" << endl;
        Word* wordNode = new Word(word, meaning, examples);
        //cout << "word made" << endl;
        //wordNode->printWord();
        dictionary->insert(wordNode);
        //cout << "" << endl;
    }
    cout << "Parsed\n\n";
}


int main() {
    Trie* dictionary = new Trie();
    parser(dictionary);

    int j = 0;
    while(j !=-1) {
    cout << "Enter 1 to search a word" << endl;
    cout << "Enter 2 to search prefixes" << endl;
    cout << "Enter 3 to insert a new word" << endl;
    cout << "Enter 4 to print the whole dictionary." << endl;
    cout << "Enter 5 to delete a word." << endl;

        int i = 0;
        cin >> i;

        string word, prefix, meaning;

        switch (i) {
            case 1 :
                cout << "Enter a word: " << endl;
                cin.ignore();
                getline(cin, word);
                dictionary->printWord(word);
                break;
            case 2 :
                cout << "Enter a prefix you want check: " << endl;
                cin.ignore();
                getline(cin, prefix);
                dictionary->prefixMatch(prefix);
                break;

            case 3 :
                dictionary->InsertIntoDictionary();
                break;

            case 4 :
                dictionary->printAll();
                break;

            case 5 :
                cout << "Enter a word to delete: " << endl;
                cin.ignore();
                getline(cin, word);
                dictionary->deleteWord(word);
                break;

            default :
                cout << "Invalid input" << endl;
        }
        cout<< "Press -1 to exit. Press any other number to continue.\n";
        cin >> j;
    }

    return 0;
}


