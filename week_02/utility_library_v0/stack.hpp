#pragma once
#include "node.hpp"
#include<cstddef>
#include<utility>
#include <stdexcept>

template <typename T>
class Stack{
    private:
    Node<T>* head_{nullptr};
    std::size_t size_{0};

    public:
    Stack() = default;
    ~Stack(){
        while(head_!=nullptr){
            Node<T>* temp = head_;
            head_=head_->next_;
            delete temp;
        }
    }
    

    bool empty() const{
        return head_==nullptr;
    }

    std::size_t size() const{
       return size_;
    }


    //Copying lvalue push
    void push(const T& value){
        head_=new Node<T>{value,head_};
        ++size_;
    }
    //Moving rvalues
    void push(T&& value){
        head_=new Node<T>{std::move(value),head_};
        ++size_;
    }

    //Const top()
    const T& top() const{
        if(empty()){
            throw std::out_of_range("Stack is empty");
        }
        return head_->value;
    }

    //Modifiable top()
    T& top(){
        if(empty()){
            throw std::out_of_range("Stack is empty");
        }
       return head_->value; 
    }

    //Pop
    void pop(){
        if(empty()){
            throw std::out_of_range("Stack is empty");
        }
        Node<T>* temp=head_;
        head_=head_->next_;
        delete temp;
        --size_;
    }

    //Applying Rule of Five
    Stack(const Stack& other)=delete;
    Stack& operator=(const Stack& other)=delete;

    Stack(Stack&& other) noexcept
    :head_(other.head_),size_(other.size_)
    {
        other.head_=nullptr;
        other.size_=0;
    }

    Stack& operator=(Stack&& other) noexcept{
        if(this!=&other){
            //delete existing
            while(head_!=nullptr){
                Node<T>* temp=head_;
                head_=head_->next_;
                delete temp;
            }

            //Steal other nodes
            head_=other.head_;
            size_=other.size_;

            //Leave other empty
            other.head_=nullptr;
            other.size_=0;
        }
        return *this;
    }


};