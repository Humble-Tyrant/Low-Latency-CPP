#pragma once
#include<utility>

template <typename T>

struct Node{
    T value;
    Node* next_{nullptr};

    //An lvalue constructor that copies T, and an rvalue constructor that moves T
    //explicit constructors to prevent accidental implicit conversion
    explicit Node(const T& val,Node* next=nullptr): value(val),next_(next){}

    explicit Node(T&& val,Node* next=nullptr):value(std::move(val)),next_(next){}
};
