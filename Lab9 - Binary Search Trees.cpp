// Author: Aayush Keshari
// M Number: 15039880
// Email: kesharah@mail.uc.edu
// --------------------------------------------------------------------------------------------------------------
// Please ensure that any text files you want to scan for are present in the same directory. I have included two 
// sample test files named MobyDick.txt and PeterPan.txt in the same directory.
// --------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

template <typename DataType>
class WordFrequencyTree {
private:
    struct TreeNode {
        DataType data;
        int height;
        TreeNode* left;
        TreeNode* right;
        int count;

        TreeNode(const DataType& value) : data(value), height(1), left(nullptr), right(nullptr), count(1) {}
    };

    TreeNode* root;

    int NodeHeight(TreeNode* node) {
        return (node != nullptr) ? node->height : 0;
    }

    int BalanceFactor(TreeNode* node) {
        return NodeHeight(node->left) - NodeHeight(node->right);
    }

    void UpdateHeight(TreeNode* node) {
        int leftHeight = NodeHeight(node->left);
        int rightHeight = NodeHeight(node->right);
        node->height = 1 + std::max(leftHeight, rightHeight);
    }

    TreeNode* RotateRight(TreeNode* y) {
        TreeNode* x = y->left;
        TreeNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        UpdateHeight(y);
        UpdateHeight(x);

        return x;
    }

    TreeNode* RotateLeft(TreeNode* x) {
        TreeNode* y = x->right;
        TreeNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        UpdateHeight(x);
        UpdateHeight(y);

        return y;
    }

    TreeNode* Insert(TreeNode* node, const DataType& value) {
        if (node == nullptr) {
            return new TreeNode(value);
        }

        if (value < node->data) {
            node->left = Insert(node->left, value);
        }
        else if (value > node->data) {
            node->right = Insert(node->right, value);
        }
        else {
            node->count++;
            return node;
        }

        UpdateHeight(node);
        int balance = BalanceFactor(node);

        if (balance > 1) {
            if (value < node->left->data) {
                return RotateRight(node);
            }
            else {
                node->left = RotateLeft(node->left);
                return RotateRight(node);
            }
        }
        if (balance < -1) {
            if (value > node->right->data) {
                return RotateLeft(node);
            }
            else {
                node->right = RotateRight(node->right);
                return RotateLeft(node);
            }
        }

        return node;
    }

    TreeNode* Find(TreeNode* node, const DataType& value) {
        if (node == nullptr) {
            return nullptr;
        }

        if (value < node->data) {
            return Find(node->left, value);
        }
        else if (value > node->data) {
            return Find(node->right, value);
        }
        else {
            return node;
        }
    }

    TreeNode* GetMinNode(TreeNode* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    TreeNode* Remove(TreeNode* node, const DataType& value) {
        if (node == nullptr) {
            return nullptr;
        }

        if (value < node->data) {
            node->left = Remove(node->left, value);
        }
        else if (value > node->data) {
            node->right = Remove(node->right, value);
        }
        else {
            if (node->count > 1) {
                node->count--;
            }
            else {
                if (node->left == nullptr || node->right == nullptr) {
                    TreeNode* temp = (node->left != nullptr) ? node->left : node->right;
                    if (temp == nullptr) {
                        temp = node;
                        node = nullptr;
                    }
                    else {
                        *node = *temp;
                    }
                    delete temp;
                }
                else {
                    TreeNode* temp = GetMinNode(node->right);
                    node->data = temp->data;
                    node->count = temp->count;
                    node->right = Remove(node->right, temp->data);
                }
            }
        }

        if (node == nullptr) {
            return node;
        }

        UpdateHeight(node);
        int balance = BalanceFactor(node);

        if (balance > 1) {
            if (BalanceFactor(node->left) >= 0) {
                return RotateRight(node);
            }
            else {
                node->left = RotateLeft(node->left);
                return RotateRight(node);
            }
        }
        if (balance < -1) {
            if (BalanceFactor(node->right) <= 0) {
                return RotateLeft(node);
            }
            else {
                node->right = RotateRight(node->right);
                return RotateLeft(node);
            }
        }

        return node;
    }

    void GetAllAscending(TreeNode* node, std::vector<DataType>& result) {
        if (node != nullptr) {
            GetAllAscending(node->left, result);
            for (int i = 0; i < node->count; i++) {
                result.push_back(node->data);
            }
            GetAllAscending(node->right, result);
        }
    }

    void GetAllDescending(TreeNode* node, std::vector<DataType>& result) {
        if (node != nullptr) {
            GetAllDescending(node->right, result);
            for (int i = 0; i < node->count; i++) {
                result.push_back(node->data);
            }
            GetAllDescending(node->left, result);
        }
    }

    void EmptyTree(TreeNode* node) {
        if (node != nullptr) {
            EmptyTree(node->left);
            EmptyTree(node->right);
            delete node;
        }
        root = nullptr;
    }

public:
    WordFrequencyTree() : root(nullptr) {}

    ~WordFrequencyTree() {
        EmptyTree(root);
    }

    void Insert(const DataType& value) {
        root = Insert(root, value);
    }

    TreeNode* Find(const DataType& value) {
        return Find(root, value);
    }

    int Size() {
        return (root != nullptr) ? root->count : 0;
    }

    std::vector<DataType> GetAllAscending() {
        std::vector<DataType> result;
        GetAllAscending(root, result);
        return result;
    }

    std::vector<DataType> GetAllDescending() {
        std::vector<DataType> result;
        GetAllDescending(root, result);
        return result;
    }

    void EmptyTree() {
        EmptyTree(root);
    }

    TreeNode* Remove(const DataType& value) {
        return Remove(root, value);
    }
};

class WordFrequencyData {
public:
    WordFrequencyData() : word(""), count(0) {}
    WordFrequencyData(const std::string& w, int c) : word(w), count(c) {}

    bool operator<(const WordFrequencyData& other) const {
        return word < other.word;
    }
    bool operator>(const WordFrequencyData& other) const {
        return word > other.word;
    }
    bool operator==(const WordFrequencyData& other) const {
        return word == other.word;
    }

    void IncrementCount() { count++; }

    void SetWord(const std::string& w) {
        word = w;
    }
    void SetCount(int c) {
        count = c;
    }
    std::string GetWord() {
        return word;
    }
    int GetCount() {
        return count;
    }

private:
    std::string word;
    int count;
};

int main() {
    WordFrequencyTree<WordFrequencyData> wordFrequencyTree;

    std::string filename;
    std::cout << "Enter the name of the text file: ";
    std::cin >> filename;

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'" << std::endl;
        return 1;
    }

    std::string word;
    while (file >> word) {
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);

        auto wordNode = wordFrequencyTree.Find(WordFrequencyData(word, 0));
        if (wordNode == nullptr) {
            wordFrequencyTree.Insert(WordFrequencyData(word, 1));
        }
        else {
            wordNode->data.IncrementCount();
        }
    }

    file.close();

    while (true) {
        std::cout << "Options:" << std::endl;
        std::cout << "Press 1 to search for a word" << std::endl;
        std::cout << "Press 2 to list words in ascending order of frequency" << std::endl;
        std::cout << "Press 3 to list words in descending order of frequency" << std::endl;
        std::cout << "Press 4 to quit" << std::endl;
        std::cout << "Please enter your choice: ";

        int choice;
        // Loop to handle invalid input
        while (!(std::cin >> choice) || choice < 1 || choice > 4) {
            std::cin.clear(); // clears the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discards invalid input
            std::cout << "Invalid choice. Please enter a valid option (1-4): ";
        }

        if (choice == 1) {
            std::cout << "Enter the word to search: ";
            std::string searchWord;
            std::cin >> searchWord;
            std::transform(searchWord.begin(), searchWord.end(), searchWord.begin(), ::tolower);

            auto wordNode = wordFrequencyTree.Find(WordFrequencyData(searchWord, 0));
            if (wordNode != nullptr) {
                std::cout << "Frequency of '" << searchWord << "': " << wordNode->data.GetCount() << std::endl;
            }
            else {
                std::cout << "'" << searchWord << "' not found in the text." << std::endl;
            }
        }
        else if (choice == 2) {
            std::vector<WordFrequencyData> ascendingWords = wordFrequencyTree.GetAllAscending();
            for (auto& wordData : ascendingWords) {
                std::cout << wordData.GetWord() << ": " << wordData.GetCount() << std::endl;
            }
        }
        else if (choice == 3) {
            std::vector<WordFrequencyData> descendingWords = wordFrequencyTree.GetAllDescending();
            for (auto& wordData : descendingWords) {
                std::cout << wordData.GetWord() << ": " << wordData.GetCount() << std::endl;
            }
        }
        else if (choice == 4) {
            break;
        }
    }

    return 0;
}
