#pragma once
#include "node.hpp"
#include<cstddef>
#include<utility>
#include <stdexcept>

template <typename T>

class Queue{
    private:
    Node<T>* head_{nullptr};
    Node<T>* tail_{nullptr};
    std::size_t size_{0};

    public:
    Queue() = default;
    ~Queue(){
        while(head_!=nullptr){
            Node<T>* temp=head_;
            head_=head_->next_;
            delete temp;
        }
    }

    std::size_t size() const{
        return size_;
    }

    bool empty() const{
        return head_==nullptr;
    }

    //Copying lvalue
    void push(const T& value){
       Node<T>* temp=new Node<T>{value,nullptr};
       if(empty()){
        head_=temp;
        tail_=temp;
       }
       else{
        tail_->next_=temp;
        tail_=tail_->next_;
       }
        ++size_;
    }

    //Moving rvalue
    void push(T&& value){
        Node<T>* temp=new Node<T>{std::move(value),nullptr};
        if(empty()){
        head_=temp;
        tail_=temp;
       }
       else{
        tail_->next_=temp;
        tail_=tail_->next_;
       }
        ++size_;
    }

    //Applying Rule of 5
    Queue(const Queue& other)=delete;
    Queue& operator=(const Queue& other)=delete;

    Queue(Queue&& other) noexcept
    :size_(other.size_),head_(other.head_),tail_(other.tail_)
    {
        other.head_=nullptr;
        other.tail_=nullptr;
        other.size_=0;
    }

    Queue& operator=(Queue&& other) noexcept{
        if(this!=&other){
            while(head_!=nullptr){
                Node<T>* temp=head_;
                head_=head_->next_;
                delete temp;
            }
            head_=other.head_;
            tail_=other.tail_;
            size_=other.size_;
            other.head_=nullptr;
            other.tail_=nullptr;
            other.size_=0;

        }


        return *this;
    }


    //const front()
    const T& front() const{
        if(empty()){
            throw std::out_of_range("Queue is empty");
        }
        return head_->value;
    }

    //Modifiable top()front()
    T& front(){
        if(empty()){
            throw std::out_of_range("Queue is empty");
        }
        return head_->value;
    }

    //Pop function
    void pop(){
        if(empty()){
            throw std::out_of_range("Queue is empty");
        }
        Node<T>* temp=head_;
        head_=head_->next_;
        delete temp;
        --size_;
        if (size_ == 0){
        tail_ = nullptr;
        }
    }
};