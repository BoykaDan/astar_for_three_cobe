//
//  main.cpp
//  AI_lab1_IDA*H2
//
//  Created by Danlang on 16/5/2.
//  Copyright © 2016年 Danlang. All rights reserved.
//



#include <cstdio>
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

enum Action {
    U,D,L,R,F,B
};

int iabs(int x)
{
    return x > 0 ? x : -x;
}


class Node
{
public:
    Node(int s[3][3][3], enum Action a = U, int g = 0, int h = 0, Node *p = NULL)
    : action(a), gx(g), hx(h), parent(p)
    {
        for (int i = 0; i < 3; i ++)
            for (int j = 0; j < 3; j ++)
                for(int k = 0; k < 3; k ++)
                    status[i][j][k] = s[i][j][k];
        
    }
    Node(const Node& n)
    : action(n.action), gx(n.gx), hx(n.hx), parent(n.parent)
    {
        for (int i = 0; i < 3; i ++)
            for (int j = 0; j < 3; j ++)
                for(int k = 0; k < 3; k ++)
                    status[i][j][k] = n.status[i][j][k];
    }
    
    void show()
    {
        for (int i = 0; i < 3; i ++)
            for (int j = 0; j < 3; j ++)
            {
                for(int k = 0; k < 3; k ++)
                    cout << status[i][j][k] << " ";
                cout << endl;
            }
        
    }
    Node *parent;
    enum Action action;
    int gx;
    int hx;	// h(x) + g(x)
    int status[3][3][3];
};

int goalStatus[3][3][3];
double duration;


int h2(Node* currentStatus)
{
    int result = 0;
    for (int i = 0; i < 3; i ++)
    {
        for (int j = 0; j < 3; j ++)
            for (int k = 0;k < 3;k ++)
            {
                if (currentStatus -> status[i][j][k] == goalStatus[i][j][k])
                    continue;
                int l,m, n;
                int flag = false;
                for (l = 0; l < 3; l++)
                {
                    for (m = 0; m < 3; m ++)
                    {
                        for (n = 0; n < 3; n ++)
                        {
                            if (currentStatus -> status[i][j][k] == goalStatus[l][m][n])
                            {
                                flag = true;
                                break;
                            }
                        }
                        if (flag)
                            break;
                    }
                    if (flag)
                        break;
                }
                
                result += (iabs(i - l) + iabs(j - m) + iabs(k-n));
            }
    }
    return result;
}

int minFx(vector<Node *>ls)
{
    if (ls.empty())
        return -1;
    int minNum = 0;
    int minValue = ls[0] -> hx + ls[0] -> gx;
    for (int i = 1; i != ls.size(); i ++) {
        int tmp = ls[i] -> hx + ls[i] -> gx;
        if (tmp < minValue) {
            minValue = tmp;
            minNum = i;
        }
    }
    return minNum;
}

bool testGoal(Node *s)
{
    return s -> hx == 0;
}

vector<Node *> nextStatus(Node *node)
{
    vector<Node *> result;
    int i, j, k;
    int flag = false;
    for (i = 0; i < 3; i ++)
    {	// Find 0
        for (j = 0; j < 3; j ++)
        {
            for(k = 0; k < 3; k ++)
                if (node -> status[i][j][k] == 0)
                {
                    flag = true;
                    break;
                }
            if (flag) break;
        }
        if (flag) break;
    }
    //	cout << "i = " << i << ", j = " << j << endl;
    // Up direction
    if ((j - 1 >= 0) && !(i == 1 && j==2 && (k==0 || k==2))) {
        Node *upNode = new Node(*node);
        upNode -> status[i][j][k] = upNode -> status[i][j - 1][k];
        upNode -> status[i][j - 1][k] = 0;
        upNode -> action = U;
        upNode -> hx = h2(upNode);
        upNode -> gx ++;
        upNode -> parent = node;
        result.push_back(upNode);
    }
    // Down direction
    if ((j + 1 <= 2) && !(i == 1 && j==0 && (k==0 || k==2))) {
        Node *downNode = new Node(*node);
        downNode -> status[i][j][k] = downNode -> status[i][j+1][k];
        downNode -> status[i][j+1][k] = 0;
        downNode -> action = D;
        downNode -> hx = h2(downNode);
        downNode -> gx ++;
        downNode -> parent = node;
        result.push_back(downNode);
    }
    // Left direction
    if (k - 1 >= 0 && !(i==1 && j==1 && k==1)) {
        Node *leftNode = new Node(*node);
        leftNode -> status[i][j][k] = leftNode -> status [i][j][k - 1];
        leftNode -> status[i][j][k - 1] = 0;
        leftNode -> action = L;
        leftNode -> hx = h2(leftNode);
        leftNode -> gx ++;
        leftNode -> parent = node;
        result.push_back(leftNode);
    }
    // Right direction
    if (k + 1 <= 2 && !(i==1 && j==1 && k==1)) {
        Node *rightNode = new Node(*node);
        rightNode -> status[i][j][k] = rightNode -> status[i][j][k + 1];
        rightNode -> status[i][j][k + 1] = 0;
        rightNode -> action = R;
        rightNode -> hx = h2(rightNode);
        rightNode -> gx ++;
        rightNode -> parent = node;
        result.push_back(rightNode);
    }
    // back direction
    if (i - 1 >= 0 && !(i==2 && j==1 && (k==0||k==2))) {
        Node *backNode = new Node(*node);
        backNode -> status[i][j][k] = backNode -> status[i - 1][j][k];
        backNode -> status[i - 1][j][k] = 0;
        backNode -> action = B;
        backNode -> hx = h2(backNode);
        backNode -> gx ++;
        backNode -> parent = node;
        result.push_back(backNode);
    }
    // forward direction
    if (i + 1 <= 2 && !(i==0 && j==1 && (k==0||k==2))) {
        Node *forwardNode = new Node(*node);
        forwardNode -> status[i][j][k] = forwardNode -> status[i + 1][j][k];
        forwardNode -> status[i + 1][j][k] = 0;
        forwardNode -> action = F;
        forwardNode -> hx = h2(forwardNode);
        forwardNode -> gx ++;
        forwardNode -> parent = node;
        result.push_back(forwardNode);
    }
    
    return result;
}

void showResult(Node *status)
{
    char arr[100];
    int i;
    for (i = 0; status -> parent != NULL;  i ++) {
        switch (status -> action) {
            case U:
                arr[i] = 'U';
                break;
            case D:
                arr[i] = 'D';
                break;
            case L:
                arr[i] = 'L';
                break;
            case R:
                arr[i] = 'R';
                break;
            case F:
                arr[i] = 'F';
                break;
            case B:
                arr[i] = 'B';
                break;
        }
        status = status -> parent;
    }
    i --;
    fstream out("/Users/danlang/Desktop/2016/AI/AI_lab1_IDA*H2/output20.txt");
    out << duration << endl;
    for (; i >= 0; i --) {
        out << arr[i];
    }
    out << endl;
    out.close();
    
}

const int LIMIT = 1000;
bool IDAStar(Node *root)
{
    clock_t start, finish;
    start = clock();
    vector<Node *>ls;
    ls.push_back(root);
    Node *selectedStatus;
    
    int d_limit = LIMIT;
    int next_d_limit = LIMIT;
    while (!ls.empty()) {
        int minNum = minFx(ls);
        selectedStatus = ls[minNum];
        ls.erase(ls.begin() + minNum);
        int ds = selectedStatus -> hx + selectedStatus -> gx;
        if (ds > d_limit) {
            next_d_limit = min(next_d_limit, ds);
        }
        else {
            if (testGoal(selectedStatus))
                break;
            vector<Node *>nStatus = nextStatus(selectedStatus);
            for (int i = 0; i != nStatus.size(); i ++)
                ls.push_back(nStatus[i]);
        }
        d_limit = next_d_limit;
    }
    if (selectedStatus -> hx != 0)
        return false;
    
    finish = clock();
    duration = (double) (finish-start)*1000/CLOCKS_PER_SEC;
    //  selectedStatus -> show();
    showResult(selectedStatus);
    return true;
}


int main()
{
    int initStatus[3][3][3];
    
    fstream in("/Users/danlang/Desktop/2016/AI/AI_lab1_IDA*H2/source.txt");
    fstream goal("/Users/danlang/Desktop/2016/AI/AI_lab1_IDA*H2/target.txt");
    
    for (int i = 0; i < 3; i ++)
        for (int j=0;j<3;j++) 
            in >> initStatus[i][j][0] >> initStatus[i][j][1] >> initStatus[i][j][2];
    
    for (int i = 0; i < 3; i ++)
        for (int j=0;j<3;j++) 
            goal >> goalStatus[i][j][0] >> goalStatus[i][j][1] >> goalStatus[i][j][2];
    
    in.close();
    goal.close();	
    
    
    Node *root = new Node(initStatus);
    root -> hx = h2(root);
    root -> gx = 0;
    root -> parent = NULL;
    
    if (!IDAStar(root))
        cout << "No result" << endl;
    return 0;
    
}
