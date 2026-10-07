#pragma once

struct Node
{
    int data;
    Node *next;
};

using NodeLink = Node *&;
using ConstNode = const Node *;