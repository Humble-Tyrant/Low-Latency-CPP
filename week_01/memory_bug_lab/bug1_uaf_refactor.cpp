#include<iostream>
#include<cstdint>
#include<memory>

struct MarketUpdate{
    uint64_t timestamp;
    double price;
    uint32_t quantity;
};

class MarketUpdateHandle {
    private:
    MarketUpdate* ptr_;
    public:
    //Constructor
    explicit MarketUpdateHandle(double price,uint32_t quantity):
    ptr_(new MarketUpdate{1000234,price,quantity}){}

    //Destructor
    ~MarketUpdateHandle(){
        delete ptr_;
    }

    //Operator -> overload

    const MarketUpdate* operator->() const {
        return ptr_;
    }

    //Deleting Copy Constructor and Copy Assignment Operator

    MarketUpdateHandle(const MarketUpdateHandle& other) = delete;
    MarketUpdateHandle& operator=(const MarketUpdateHandle& other) = delete;

    //Defining Move constructor and Move assignment operator

    MarketUpdateHandle(MarketUpdateHandle&& other) noexcept {
        ptr_=other.ptr_;
        other.ptr_=nullptr;
    }

    MarketUpdateHandle& operator=(MarketUpdateHandle&& other) noexcept {
        if(this != &other){
            delete ptr_;
            ptr_=other.ptr_;
            other.ptr_=nullptr;
        }
        
        return *this;
    }

};

int main(){
     MarketUpdateHandle handler{100.5,100};
     MarketUpdateHandle handler2=std::move(handler);

    std::cout<<"Price: "<<handler2->price<<" Quantity: "<<handler2->quantity<<"\n";
    return 0;
}

