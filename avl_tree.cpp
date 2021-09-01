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

public:
    AVL_Tree()
    {
        head = new AVL_Node(INT_MAX);
    }

    //~avl_tree();

    void AVL_Insertion(int k)
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

    void printTree()
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
};

int main()
{
    AVL_Tree tree;
    tree.AVL_Insertion(21);
    tree.printTree();
    tree.AVL_Insertion(9);
    tree.printTree();
    tree.AVL_Insertion(5);
    tree.printTree();
    tree.AVL_Insertion(14);
    tree.printTree();
    tree.AVL_Insertion(28);
    tree.printTree();
    tree.AVL_Insertion(18);
    tree.printTree();
    tree.AVL_Insertion(3);
    tree.printTree();
    tree.AVL_Insertion(26);
    tree.printTree();
    tree.AVL_Insertion(30);
    tree.printTree();
    tree.AVL_Insertion(15);
    tree.printTree();
    tree.AVL_Insertion(16);
    tree.printTree();
}