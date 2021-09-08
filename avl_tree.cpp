#include <iostream>
#include <limits.h>
#include <queue>
#include <string>
#include <sstream>
#include <bits/stdc++.h>
#include <fstream>
#include <cmath>

using namespace std;

class AVL_Node
{
private:
    int key;
    int bf; // height of left subtree - height of right subtree
    AVL_Node *LChild, *RChild;

public:
    AVL_Node(int data)
    {
        key = data;
        LChild = nullptr;
        RChild = nullptr;
        bf = 0;
    }
    friend class AVL_Tree;
};

class AVL_Tree
{
private:
    AVL_Node *head; // head points to dummy node
    void AVL_RR_Rotation(AVL_Node *, AVL_Node *, int);
    void AVL_LL_Rotation(AVL_Node *, AVL_Node *, int);
    void AVL_LR_Rotation(AVL_Node *, AVL_Node *, int, string);
    void AVL_RL_Rotation(AVL_Node *, AVL_Node *, int, string);

public:
    AVL_Tree()
    {
        head = new AVL_Node(INT_MAX);
    }
    void AVL_Insert(int);
    void AVL_Delete(int);
    bool AVL_Search(int);
    void AVL_Print(const char *);
    ~AVL_Tree()
    {
        delete head;
    }
};

void AVL_Tree::AVL_Insert(int k)
{
    AVL_Node *newNode = new AVL_Node(k);
    AVL_Node *root = head->RChild;

    // first insertion
    if (root == nullptr)
    {
        head->RChild = newNode;
        return;
    }

    // rebalancingPoint points to node where rebalancing may be necessary
    // parent points to parent of rebalancingPoint
    // cursor is used for traversal
    // cursorNext point to the child of cursor
    AVL_Node *parent = head, *rebalancingPoint = root;
    AVL_Node *cursor = root, *cursorNext;

    while (true)
    {
        if (k < cursor->key) // if k is smaller than cursor->key
        {
            cursorNext = cursor->LChild;
            if (cursorNext == nullptr) // if cursor reached the leaf node
            {
                cursorNext = newNode;
                cursor->LChild = cursorNext; // make newNode as left child of cursor
                break;
            }
            else if (cursorNext->bf != 0) // if balance factor of cursorNext is non-zero
            {
                parent = cursor;
                rebalancingPoint = cursorNext;
            }
            cursor = cursorNext;
        }
        else if (k > cursor->key) // if k is greater than cursor->key
        {
            cursorNext = cursor->RChild;
            if (cursorNext == nullptr) // if cursor reached the leaf node
            {
                cursorNext = newNode;
                cursor->RChild = cursorNext; // make newNode as right child of cursor
                break;
            }
            else if (cursorNext->bf != 0) // if balance factor of cursorNext is non-zero
            {
                parent = cursor;
                rebalancingPoint = cursorNext;
            }
            cursor = cursorNext;
        }
        else                    // if k already exists in the tree
            throw newNode->key; // throw exception
    }

    // a denotes in which side of rebalancing point the insertion took place
    int a = k < rebalancingPoint->key ? 1 : -1;
    cursor = (a == 1) ? rebalancingPoint->LChild : rebalancingPoint->RChild;
    /* temp points to the insertion side child of rebalancing point
        i.e., if insertion happened on left side then temp point to left child
        else right child */
    AVL_Node *temp = cursor;

    // updating the balance factor of nodes until cursor != cursorNext
    while (cursor != cursorNext)
    {
        if (k < cursor->key)
        {
            cursor->bf = 1;
            cursor = cursor->LChild;
        }
        else if (k > cursor->key)
        {
            cursor->bf = -1;
            cursor = cursor->RChild;
        }
    }

    // if balance factor of rebalancing point is 0
    if (rebalancingPoint->bf == 0)
    {
        rebalancingPoint->bf = a; //set the new balance factor in it
        return;
    }
    /* if balance factor of rebalancing point is opposite to the new
     balance factor i.e., tree is more balanced now */
    else if (rebalancingPoint->bf == -1 * a)
    {
        rebalancingPoint->bf = 0;
        return;
    }
    else // rebalancingPoint->bf == a
    {
        /* insertion is done on the same side of temp as that of rebalancing point
           so, single rotation is required */
        if (temp->bf == a)
        {
            cursor = temp;
            /* insertion is done in the left side of rebalancing point
               and left side of temp */
            if (a == 1)
                AVL_LL_Rotation(rebalancingPoint, temp, a);
            /* insertion is done in the right side of rebalancing point
               and right side of temp */
            else if (a == -1)
                AVL_RR_Rotation(rebalancingPoint, temp, a);
        }
        /* insertion is done on the opposite side of temp as that of rebalancing point
           double rotation  */
        else if (temp->bf == -1 * a)
        {
            /* insertion is done in the left side of rebalancing point
               and right side of temp */
            if (a == 1)
            {
                cursor = temp->RChild;
                AVL_LR_Rotation(rebalancingPoint, temp, a, "insertion");
            }
            /* insertion is done in the right side of rebalancing point
               nd left side of temp */
            else if (a == -1)
            {
                cursor = temp->LChild;
                AVL_RL_Rotation(rebalancingPoint, temp, a, "insertion");
            }
        }
    }

    if (rebalancingPoint == parent->RChild)
        parent->RChild = cursor;
    else
        parent->LChild = cursor;
}

void AVL_Tree::AVL_RL_Rotation(AVL_Node *rebalancingPoint, AVL_Node *temp, int a, string operation)
{
    // first rotaion
    AVL_Node *cursor = temp->LChild;
    temp->LChild = cursor->RChild;
    cursor->RChild = temp;

    // second rotation
    rebalancingPoint->RChild = cursor->LChild;
    cursor->LChild = rebalancingPoint;

    // update balance factor
    if (operation.compare("insertion") == 0)
    {
        rebalancingPoint->bf = cursor->bf == 0 ? 0 : (cursor->bf == 1 ? 0 : 1);
        temp->bf = cursor->bf == 0 ? 0 : (cursor->bf == 1 ? -1 : 0);
    }
    else if (operation.compare("deletion") == 0)
    {
        rebalancingPoint->bf = cursor->bf == 0 ? 0 : (cursor->bf == 1 ? 0 : 1);
        temp->bf = cursor->bf == 0 ? 0 : (cursor->bf == 1 ? -1 : 0);
    }
    cursor->bf = 0;
}

void AVL_Tree::AVL_LR_Rotation(AVL_Node *rebalancingPoint, AVL_Node *temp, int a, string operation)
{
    // first rotation
    AVL_Node *cursor = temp->RChild;
    temp->RChild = cursor->LChild;
    cursor->LChild = temp;

    // second rotation
    rebalancingPoint->LChild = cursor->RChild;
    cursor->RChild = rebalancingPoint;

    // update balance factor
    if (operation.compare("insertion") == 0)
    {
        rebalancingPoint->bf = (cursor->bf == 0) ? 0 : (cursor->bf == 1 ? -1 : 0);
        temp->bf = (cursor->bf == 0) ? 0 : (cursor->bf == 1 ? 0 : 1);
    }
    else if (operation.compare("deletion") == 0)
    {
        rebalancingPoint->bf = cursor->bf == 0 ? 0 : (cursor->bf == 1 ? -1 : 0);
        temp->bf = cursor->bf == 0 ? 0 : (cursor->bf == 1 ? 0 : 1);
    }
    cursor->bf = 0;
}

void AVL_Tree::AVL_RR_Rotation(AVL_Node *rebalancingPoint, AVL_Node *temp, int a)
{
    // rotation
    AVL_Node *cursor = temp;
    rebalancingPoint->RChild = temp->LChild;
    temp->LChild = rebalancingPoint;

    // update balance factor
    if (temp->bf == 0)
    {
        temp->bf = a;
    }
    else
    {
        rebalancingPoint->bf = 0;
        temp->bf = 0;
    }
}

void AVL_Tree::AVL_LL_Rotation(AVL_Node *rebalancingPoint, AVL_Node *temp, int a)
{
    // rotation
    AVL_Node *cursor = temp;
    rebalancingPoint->LChild = temp->RChild;
    temp->RChild = rebalancingPoint;

    // update balance factor
    if (temp->bf == 0)
    {
        temp->bf = a;
    }
    else
    {
        rebalancingPoint->bf = 0;
        temp->bf = 0;
    }
}

void AVL_Tree::AVL_Delete(int k)
{
    stack<AVL_Node *> stack; // store the nodes which comes in the path upto the deleted node
    stack.push(head);
    AVL_Node *root = head->RChild; // root is the root of the tree which is right child of head node
    AVL_Node *curr = root;         // curr used for path traversal
    AVL_Node *prev = head;         //prev points to prev of curr

    // finding the node with key = k
    while (curr != nullptr && curr->key != k)
    {
        prev = curr;             // updating prev
        stack.push(prev);        // pushing curr into stack
        if (curr->key < k)       // if curr->key is smaller than k
            curr = curr->RChild; // update curr to right child of curr
        else if (curr->key > k)  // if curr->key is greater than k
            curr = curr->LChild; // update curr to right child of curr
    }

    // k doesn't exists
    if (curr == nullptr)
        throw k;

    // leaf node deletion i.e., k is present in the leaf node
    if (curr->LChild == nullptr && curr->RChild == nullptr)
    {
        if (prev == head) // if deleted node is root node itself with one child
            head->RChild = curr->RChild;
        else if (curr->key < prev->key) // if node to be deleted is left child of prev
            prev->LChild = nullptr;
        else // if node to be deleted is right child of prev
            prev->RChild = nullptr;
        // free(curr);
    }
    // node with single child deletion i.e., k has one child
    else if (curr->LChild == nullptr || curr->RChild == nullptr)
    {
        AVL_Node *temp;
        // if curr has no left child
        if (curr->LChild == nullptr)
            temp = curr->RChild;
        else // otherwise
            temp = curr->LChild;

        if (prev == head) // if deleted node is root node itself with one child
            head->RChild = temp;
        else if (curr->key > prev->key) // if the node to be deleted is right child of prev
            prev->RChild = temp;
        else // if the node to be deleted is left child of prev
            prev->LChild = temp;
        // free(curr);
    }
    // node with double child deletion i.e., k has two children
    else
    {
        AVL_Node *temp;                // temp is used for traversal to find out inorder successor of curr
        AVL_Node *parOfTemp = nullptr; // parOfTemp points to parent of temp
        temp = curr->RChild;
        stack.push(curr);

        // finding inorder successor of curr
        while (temp->LChild != nullptr)
        {
            stack.push(temp);
            parOfTemp = temp;
            temp = temp->LChild;
        }

        /* if the right child of curr is itself the
            inorder successor of curr */
        if (parOfTemp == nullptr)
            curr->RChild = temp->RChild;
        else
            parOfTemp->LChild = temp->RChild;
        curr->key = temp->key; // copying the key value
        k = temp->key;
        // free(temp);
    }

    // rebalancingPoint points to node where rebalancing may be necessary
    // parent points to parent of rebalancingPoint
    // cursor is used for traversal
    // temp point to the child of rebalancingPoint
    AVL_Node *rebalancingPoint, *parent;
    AVL_Node *temp, *cursor;

    while (stack.top() != head)
    {
        rebalancingPoint = stack.top();
        int a = k < rebalancingPoint->key ? 1 : -1; // a denotes in which side of rebalancing point the deletion took place
        stack.pop();
        parent = stack.top();

        if (rebalancingPoint->bf == a) //if balance factor of rebalancingPoint is same as new balance factor
        {
            rebalancingPoint->bf = 0;
            continue;
        }
        // if balance factor of rebalancing point is 0
        else if (rebalancingPoint->bf == 0)
        {
            rebalancingPoint->bf = -1 * a;
            return;
        }
        //if (rebalancingPoint->bF == -1*a) i.e., if balance factor of rebalancingPoint is opposite of a
        else
        {
            temp = rebalancingPoint->bf == 1 ? rebalancingPoint->LChild : rebalancingPoint->RChild;
            //single rotation when temp has one child
            if (temp->bf == -1 * a)
            {
                cursor = temp;
                if (a == -1)
                    AVL_LL_Rotation(rebalancingPoint, temp, a);
                else if (a == 1)
                    AVL_RR_Rotation(rebalancingPoint, temp, a);
            }
            //single rotation when temp has two children
            else if (temp->bf == 0)
            {
                cursor = temp;
                if (a == -1)
                    AVL_LL_Rotation(rebalancingPoint, temp, a);
                else if (a == 1)
                    AVL_RR_Rotation(rebalancingPoint, temp, a);
            }
            //double rotation
            else if (temp->bf == a)
            {
                if (a == -1)
                {
                    cursor = temp->RChild;
                    AVL_LR_Rotation(rebalancingPoint, temp, a, "deletion");
                }
                else if (a == 1)
                {
                    cursor = temp->LChild;
                    AVL_RL_Rotation(rebalancingPoint, temp, a, "deletion");
                }
            }

            // No further rotations required if the balance factor has not changed for the subtree
            if (cursor->bf == 1 || cursor->bf == -1)
            {
                if (rebalancingPoint == parent->RChild)
                    parent->RChild = cursor;
                else
                    parent->LChild = cursor;
                return;
            }
        }

        if (rebalancingPoint == parent->RChild)
            parent->RChild = cursor;
        else
            parent->LChild = cursor;
    }
}

bool AVL_Tree::AVL_Search(int k)
{
    AVL_Node *temp = head->RChild; // temp points to root of the tree which is the right child of head node
    if (temp == nullptr)           // if tree is empty
        return false;

    // iterate until temp becomes null i.e., temp reaches leaf node
    while (temp != nullptr)
    {
        if (temp->key == k) // if k is found
            return true;
        else if (temp->key < k) // if k is greater than key of root
            temp = temp->RChild;
        else // if k is smaller than key of root
            temp = temp->LChild;
    }
    return false; // if k is not present in the tree
}

void AVL_Tree::AVL_Print(const char *filename)
{
    // forming initial part of "graph.gv file"
    ofstream stream;
    string str = "graph";
    str.append(".gv");
    stream.open(str);
    stream << "digraph g { \n\tnode[shape = record, height = .1];\n";
    stream << "\tnode" << INT_MAX << "[label = \"<l> | <d> Head Node | <r> \"];\n";

    AVL_Node *root = head->RChild; // curr points to left child of dummy node i.e., root of actual TBST
    if (root == nullptr)           // when tree is empty
        cout << "Tree is empty" << endl;
    else
    {
        string nodeString = "";    // to store the details of every node
        string pointerString = ""; // to store the pointers details
        ostringstream oss;
        queue<AVL_Node *> q; // queue to perform level order traversal
        q.push(root);

        while (!q.empty())
        {
            AVL_Node *node = q.front();
            // storing node details
            oss << "\tnode" << node->key << "[label = \"<l> | <d> " << node->key << "," << node->bf << " | <r>\"];\n";
            nodeString.append(oss.str());
            oss.str("");
            oss.clear();

            // Enqueue left child
            if (node->LChild != nullptr)
            {
                q.push(node->LChild);
                // storing node details
                oss << "\t\"node" << node->key << "\":l -> \"node" << node->LChild->key << "\":d;\n";
                pointerString.append(oss.str());
                oss.str("");
                oss.clear();
            }

            // Enqueue right child
            if (node->RChild != nullptr)
            {
                q.push(node->RChild);
                // storing node details
                oss << "\t\"node" << node->key << "\":r -> \"node" << node->RChild->key << "\":d;\n";
                pointerString.append(oss.str());
                oss.str("");
                oss.clear();
            }
            q.pop();
        }
        stream << nodeString;
        stream << "\t\"node" << head->key << "\":r -> \"node" << head->RChild->key << "\":d;\n";
        stream << pointerString;
    }
    stream << "}";
    stream.close();

    // generate tree image
    string cmd = "dot.exe -Tpng graph.gv -o " + string(filename) + ".png";
    system((const char *)cmd.c_str());
}

int main()
{
    AVL_Tree tree;
    string filename;
    bool iteration = true;
    while (iteration)
    {
        int choice = 0;
        cout << "\n---------------------------------------";
        cout << "\n\tAVL Tree Implementation";
        cout << "\n---------------------------------------";
        cout << "\nWhat do you want to perform?";
        cout << "\n1. Insertion";
        cout << "\n2. Deletion";
        cout << "\n3. Search";
        cout << "\n4. Print tree";
        cout << "\n5. Quit";
        cout << "\nEnter your choice: ";
        cin >> choice;
        int userData;

        switch (choice)
        {
        // Insertion operation
        case 1:
            try
            {
                cout << "\nEnter the number: ";
                cin >> userData;
                if (floor(userData) == userData) // checking whether the input data is a number or not
                {
                    tree.AVL_Insert(userData);
                    cout << "\nInsertion done successfully.";
                }
                else
                    cout << "\n Enter a valid integer!!";
            }
            catch (int x)
            {
                cout << x << " is already present in the tree.";
            }
            break;

        // Deletion operation
        case 2:
            try
            {
                cout << "\nEnter the number: ";
                cin >> userData;
                if (floor(userData) == userData) // checking whether the input data is a number or not
                {
                    tree.AVL_Delete(userData);
                    cout << "\nDeletion done successfully";
                }
                else
                    cout << "\n Enter a valid integer!!";
            }
            catch (int x)
            {
                cout << x << " is not present in the tree.";
            }
            break;
        // Search operation
        case 3:
            cout << "\nEnter the number: ";
            cin >> userData;
            if (floor(userData) == userData) // checking whether the input data is a number or not
            {
                try
                {
                    if (tree.AVL_Search(userData))
                        cout << "\nElement " << userData << " is present in the tree ";
                    else
                        throw userData;
                }
                catch (int x)
                {
                    cout << "\nElement " << userData << " is not present in the tree ";
                }
            }
            else
                cout << "\n Enter a valid integer!!";
            break;
        // Print the tree
        case 4:
            cout << "Enter a file name: ";
            cin >> filename;
            tree.AVL_Print(filename.c_str());
            cout << filename << ".png has been generated!\n";
            break;
        // Quit
        case 5:
            iteration = false;
            break;
        default:
            cout << "\nInvalid Choice";
        }
    }
    return 0;
}