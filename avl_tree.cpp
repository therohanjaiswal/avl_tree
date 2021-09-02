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
    int bf;
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

int result = 0;
class AVL_Tree
{
private:
    AVL_Node *head;
    AVL_Node *parentOfLeftMostNode(AVL_Node *);

public:
    AVL_Tree()
    {
        head = new AVL_Node(INT_MAX);
    }
    void AVL_Insert(int);
    void AVL_Delete(int);
    bool AVL_Search(int);
    void AVL_Print();
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

    AVL_Node *t = head, *s = root, *p = root, *q, *r;

    while (true)
    {
        try
        {
            if (k < p->key)
            {
                q = p->LChild;
                if (q == nullptr)
                {
                    q = newNode;
                    p->LChild = q;
                    break;
                }
                else if (q->bf != 0)
                {
                    t = p;
                    s = q;
                }
                p = q;
            }
            else if (k > p->key)
            {
                q = p->RChild;
                if (q == nullptr)
                {
                    q = newNode;
                    p->RChild = q;
                    break;
                }
                else if (q->bf != 0)
                {
                    t = p;
                    s = q;
                }
                p = q;
            }
            else
            {
                throw newNode->key;
            }
        }
        catch (int x)
        {
            cout << x << " is already present";
            return;
        }
    }

    int a = k < s->key ? 1 : -1; // a denotes in which side of s the insertion took place
    p = (a == 1) ? s->LChild : s->RChild;
    r = p;
    while (p != q)
    {
        if (k < p->key)
        {
            p->bf = 1;
            p = p->LChild;
        }
        else if (k > p->key)
        {
            p->bf = -1;
            p = p->RChild;
        }
    }

    if (s->bf == 0)
    {
        s->bf = a;
        return;
    }
    else if (s->bf == -1 * a)
    {
        s->bf = 0;
        return;
    }
    else
    {
        // single rotation
        if (r->bf == a)
        {
            if (a == 1)
            {
                p = r;
                s->LChild = r->RChild;
                r->RChild = s;
                s->bf = 0;
                r->bf = 0;
            }
            else if (a == -1)
            {
                p = r;
                s->RChild = r->LChild;
                r->LChild = s;
                s->bf = 0;
                r->bf = 0;
            }
        }

        //double rotation
        else if (r->bf == -1 * a)
        {
            if (a == 1)
            {
                p = r->RChild;
                r->RChild = p->LChild;
                p->LChild = r;
                s->LChild = p->RChild;
                p->RChild = s;
                s->bf = (p->bf == 0) ? 0 : (p->bf == 1 ? -1 : 0);
                r->bf = (p->bf == 0) ? 0 : (p->bf == 1 ? 0 : 1);
                p->bf = 0;
            }
            else if (a == -1)
            {
                p = r->LChild;
                r->LChild = p->RChild;
                p->RChild = r;
                s->RChild = p->LChild;
                p->LChild = s;
                s->bf = p->bf == 0 ? 0 : (p->bf == 1 ? 0 : 1);
                r->bf = p->bf == 0 ? 0 : (p->bf == 1 ? -1 : 0);
                p->bf = 0;
            }
        }
    }

    if (s == t->RChild)
        t->RChild = p;
    else
        t->LChild = p;
}

void AVL_Tree::AVL_Delete(int k)
{
    try
    {
        if (AVL_Search(k))
        {
            AVL_Node *root = head->RChild;
            AVL_Node *temp = root, *par = head;
            while (temp->key != k)
            {
                par = temp;
                if (temp->key < k)
                    temp = temp->RChild;
                else if (temp->key > k)
                    temp = temp->LChild;
            }

            // leaf node deletion
            if (temp->LChild == nullptr && temp->RChild == nullptr)
            {
                if (temp->key < par->key)
                    par->LChild = nullptr;
                else
                    par->RChild = nullptr;
            }

            // if temp has single right child
            if (temp->LChild == nullptr && temp->RChild != nullptr)
            {
                temp->key = temp->RChild->key;
                // free right child node later
                temp->RChild = nullptr;
            }

            // if temp has single left child
            if (temp->LChild != nullptr && temp->RChild == nullptr)
            {
                temp->key = temp->LChild->key;
                // free left child node later
                temp->LChild = nullptr;
            }

            if (temp->LChild != nullptr && temp->RChild != nullptr)
            {
                AVL_Node *leftMostPar = parentOfLeftMostNode(temp->RChild);
                if (leftMostPar == nullptr)
                {
                    temp->key = temp->RChild->key;
                    temp->RChild = nullptr;
                }
                else
                {
                    AVL_Node *leftMost = leftMostPar->LChild;
                    // if lefmost node has no child
                    if (leftMost->RChild == nullptr)
                    {
                        temp->key = leftMost->key;
                        leftMostPar->LChild = nullptr;
                    }
                    else
                    { // if leftmost node has right child
                        temp->key = leftMost->key;
                        leftMost->key = leftMost->RChild->key;
                        // free right child later
                    }
                }
            }
        }
        else
        {
            throw k;
        }
    }
    catch (int x)
    {
        cout << x << " is not presen the tree.";
    }
}

AVL_Node *AVL_Tree::parentOfLeftMostNode(AVL_Node *curr)
{
    if (curr->LChild == nullptr)
    {
        return nullptr;
    }
    while (curr->LChild->LChild != nullptr)
        curr = curr->LChild;
    return curr;
}

bool AVL_Tree::AVL_Search(int k)
{
    AVL_Node *root = head->RChild;
    if (root == nullptr)
        return false;

    while (root != nullptr)
    {
        if (root->key == k)
            return true;
        else if (root->key < k)
            root = root->RChild;
        else
            root = root->LChild;
    }
    return false;
}

void AVL_Tree::AVL_Print()
{
    // forming initial part of "graph.gv file"
    ofstream stream;
    string str = "graph";
    str.append(to_string(result++));
    str.append(".gv");
    stream.open(str);
    stream << "digraph g { \n\tnode[shape = record, height = .1];\n";
    stream << "\tnode" << INT_MAX << "[label = \"<l> | <d> Head Node | <r> \"];\n";

    AVL_Node *root = head->RChild; // curr points to left child of dummy node i.e., root of actual TBST
    // when tree is empty
    if (root == nullptr)
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
        // stream << "\t\"node" << root->key << "\":l -> \"node" << root->leftChild->data << "\":d;\n";
        stream << "\t\"node" << head->key << "\":r -> \"node" << head->RChild->key << "\":d;\n";
        stream << pointerString;
    }

    // writing nodes details and pointer details on graph.gv
    stream << "}";
    stream.close();
}

int main()
{
    AVL_Tree tree;
    tree.AVL_Insert(20);
    //tree.AVL_Print();
    tree.AVL_Insert(10);
    //tree.AVL_Print();
    tree.AVL_Insert(30);
    //tree.AVL_Print();
    tree.AVL_Insert(3);
    //tree.AVL_Print();
    tree.AVL_Insert(15);
    //tree.AVL_Print();
    tree.AVL_Insert(25);
    //tree.AVL_Print();
    tree.AVL_Insert(40);
    //tree.AVL_Print();
    tree.AVL_Insert(11);
    //tree.AVL_Print();
    tree.AVL_Insert(17);
    //tree.AVL_Print();
    tree.AVL_Insert(35);
    //tree.AVL_Print();
    tree.AVL_Print();
    tree.AVL_Delete(3);
    tree.AVL_Print();
    tree.AVL_Delete(40);
    tree.AVL_Print();
    tree.AVL_Delete(20);
    tree.AVL_Print();
    // tree.AVL_Delete(28);
    // tree.AVL_Print();
    // tree.AVL_Delete(5);
    // tree.AVL_Print();
    // cout << tree.AVL_Search(100) << endl;
}