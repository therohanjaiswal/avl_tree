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

class AVL_Tree
{
private:
    AVL_Node *head;

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
    stack<AVL_Node *> stack;
    stack.push(head);
    try
    {
        AVL_Node *root = head->RChild;
        AVL_Node *curr = root, *par = head;
        while (curr != nullptr && curr->key != k)
        {
            par = curr;
            stack.push(par);
            if (curr->key < k)
                curr = curr->RChild;
            else if (curr->key > k)
                curr = curr->LChild;
        }

        // k doesn't exists
        if (curr == nullptr)
            throw k;

        // leaf node deletion
        if (curr->LChild == nullptr && curr->RChild == nullptr)
        {
            if (par == head)
                head->RChild = curr->RChild;
            else if (curr->key < par->key)
                par->LChild = nullptr;
            else
                par->RChild = nullptr;
            // free(curr);
        }
        // node with single child deletion
        else if (curr->LChild == nullptr || curr->RChild == nullptr)
        {
            AVL_Node *temp;
            if (curr->LChild == nullptr)
                temp = curr->RChild;
            else
                temp = curr->LChild;

            // if deleted node is root node itself with one child
            if (par == head)
                head->RChild = temp;
            else if (curr->key > par->key)
                par->RChild = temp;
            else
                par->LChild = temp;
            // free(curr);
        }
        // node with double child deletion
        else
        {
            AVL_Node *prev = nullptr, *temp;
            temp = curr->RChild;
            stack.push(curr);
            // inorder successor
            while (temp->LChild != nullptr)
            {
                stack.push(temp);
                prev = temp;
                temp = temp->LChild;
            }

            if (prev != nullptr)
                prev->LChild = temp->RChild;
            else
                curr->RChild = temp->RChild;
            curr->key = temp->key;
            k = temp->key;
            // free(temp);
        }
    }
    catch (int x)
    {
        cout << x << " is not present in the tree.";
        return;
    }

    AVL_Node *s, *t, *r, *p;
    while (stack.top() != head)
    {
        s = stack.top();
        int a = k < s->key ? 1 : -1;
        stack.pop();
        t = stack.top();

        if (s->bf == a)
        {
            s->bf = 0;
            continue;
        }
        else if (s->bf == 0)
        {
            s->bf = -1 * a;
            return;
        }
        else
        {
            r = s->bf == 1 ? s->LChild : s->RChild;
            //single rotation
            if (r->bf == -1 * a)
            {
                if (a == -1)
                {
                    p = r;
                    s->LChild = r->RChild;
                    r->RChild = s;
                    s->bf = 0;
                    r->bf = 0;
                }
                else if (a == 1)
                {
                    p = r;
                    s->RChild = r->LChild;
                    r->LChild = s;
                    s->bf = 0;
                    r->bf = 0;
                }
            }
            //single rotation
            else if (r->bf == 0)
            {
                if (a == -1)
                {
                    p = r;
                    s->LChild = r->RChild;
                    r->RChild = s;
                    r->bf = a;
                }
                else if (a == 1)
                {
                    p = r;
                    s->RChild = r->LChild;
                    r->LChild = s;
                    r->bf = a;
                }
            }
            //double rotation
            else if (r->bf == a)
            {
                if (a == -1)
                {
                    p = r->RChild;
                    r->RChild = p->LChild;
                    p->LChild = r;
                    s->LChild = p->RChild;
                    p->RChild = s;
                    s->bf = p->bf == 0 ? 0 : (p->bf == 1 ? -1 : 0);
                    r->bf = p->bf == 0 ? 0 : (p->bf == 1 ? 0 : 1);
                    p->bf = 0;
                }
                else if (a == 1)
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

    string cmd = "dot.exe -Tpng graph.gv -o " + string(filename) + ".png";
    system((const char *)cmd.c_str());
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
    tree.AVL_Insert(35);
    //tree.AVL_Print();
    // tree.AVL_Insert(11);
    //tree.AVL_Print();
    tree.AVL_Insert(17);
    //tree.AVL_Print();
    // tree.AVL_Insert(35);
    // tree.AVL_Print();
    // tree.AVL_Delete(15);
    // tree.AVL_Print();
    // tree.AVL_Delete(30);
    // tree.AVL_Print();
    // tree.AVL_Delete(15);
    // tree.AVL_Print();
    // tree.AVL_Delete(20);
    // tree.AVL_Print();
    // tree.AVL_Delete(30);
    // tree.AVL_Delete(20);
    // tree.AVL_Print();
    // tree.AVL_Delete(40);
    // tree.AVL_Print();
    // tree.AVL_Delete(20);
    // tree.AVL_Print();
    // tree.AVL_Delete(28);
    // tree.AVL_Print();
    // tree.AVL_Delete(5);
    // tree.AVL_Print();
    // cout << tree.AVL_Search(100) << endl;
    string filename;
    cout << "Enter file name: ";
    cin >> filename;
    tree.AVL_Print(filename.c_str());
}