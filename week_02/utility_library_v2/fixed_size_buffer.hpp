#pragma once
#include<array>
#include<cstddef>
#include<stdexcept>
#include<utility>

template <typename T, std::size_t Capacity>
class FixedBuffer{
    private:
    std::array<T,Capacity>  data{};
    std::size_t size_{0};
    public:

    constexpr std::size_t capacity() const noexcept{
        return Capacity;
    }
    std::size_t size() const noexcept{
        return size_;
    }
    bool empty() const noexcept{
        return size_==0;
    }
    bool full() const noexcept{
        return size_==Capacity;
    }

    //MOVING RValue
    void push(T&& elem){
        if(full()){
            throw std::overflow_error("Buffer is Full");
        }
        data[size_]=std::move(elem);
        ++size_;

    }

    //Copying LValue 
    void push(const T& elem){
        if(full()){
            throw std::overflow_error("Buffer is Full");
        }
        data[size_]=elem;
        ++size_;
    }

    void pop(){
        if(empty()){
            throw std::underflow_error("Buffer is Empty");
        }
        --size_;
    }

    void clear(){
        size_=0;
    }

    //const front & back
    const T& front() const{
        if(empty()){
            throw std::underflow_error("Buffer is Empty");
        }
        return data[0];
    }
    const T& back() const{
        if(empty()){
            throw std::underflow_error("Buffer is Empty");
        }
        return data[size_-1];
    }
    //mutable front & back
    T& front(){
        if(empty()){
            throw std::underflow_error("Buffer is Empty");
        }
        return data[0];
    }
    T& back(){
        if(empty()){
            throw std::underflow_error("Buffer is Empty");
        }
        return data[size_-1];
    }

    //Subscript operator
    T& operator[](std::size_t idx){
        return data[idx];
    }
    const T& operator[](std::size_t idx) const{
        return data[idx];
    }

    T& at(std::size_t idx){
        if(idx>=size_){
            throw std::out_of_range("Out of range!");
        }
        return data[idx];
    }
    const T& at(std::size_t idx) const{
        if(idx>=size_){
            throw std::out_of_range("Out of range!");
        }
        return data[idx];
    }
    void push_back(const T& value) {
        push(value);
    }
    void push_back(T&& value){
        push(std::move(value));
    }
    void pop_back() {
        pop();
    }

    
};
