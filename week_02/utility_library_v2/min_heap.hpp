#pragma once
#include<vector>
#include<cstddef>
#include<stdexcept>
#include<utility>

template<typename T,typename Container=std::vector<T>>

class MinHeap{
    private:
    Container data_;
    static constexpr std::size_t parent_index(std::size_t i){
        return (i-1)/2;
    }
    static constexpr std::size_t left_child_index(std::size_t i){
        return 2*i +1;
    }
    static constexpr std::size_t right_child_index(std::size_t i){
        return 2*i +2;
    }
    void shift_up(std::size_t idx){
        while(idx>0){
            std::size_t parent_idx=parent_index(idx);
            if(data_[idx]<data_[parent_idx]){
                std::swap(data_[idx],data_[parent_idx]);
                idx=parent_idx;
            }
            else{
                break;
            }
        }
    }
    void shift_down(std::size_t idx,std::size_t size){
        while(idx<size){
            std::size_t smallest=idx;
            std::size_t left_cidx=left_child_index(idx);
            if(left_cidx<size && data_[smallest]>data_[left_cidx]){
                smallest=left_cidx;
            }
            std::size_t right_cidx=right_child_index(idx);
            if(right_cidx<size && data_[smallest]>data_[right_cidx]){
                smallest=right_cidx;
            }
            if(smallest==idx){
                break;
            }
            else{
                std::swap(data_[idx],data_[smallest]);
                idx=smallest;
            }
        }
    }

    public:
    MinHeap()=default;


    bool empty() const noexcept{
        return data_.empty();
    }
    std::size_t size() const noexcept{
        return data_.size();
    }

    const T& top() const{
        if(empty()){
            throw std::underflow_error("MinHeap is Empty.");
        }
        return data_[0];
    }
    void push(const T& value){
        data_.push_back(value);
        std::size_t idx=data_.size()-1;
        shift_up(idx);
        
    }
    void push(T&& value){
        data_.push_back(std::move(value));
        std::size_t idx=data_.size()-1;
        shift_up(idx);
    }

    void pop(){
        if(empty()){
            throw std::underflow_error("MinHeap is Empty.");
        }
        std::size_t size=data_.size();
        std::size_t last_idx=size-1;
        std::swap(data_[0],data_[last_idx]);
        data_.pop_back();
        --size;
        shift_down(0,size);
        
    }


 

};