#include<iostream>
#include<cstdint>
#include<memory>

struct Order {
    uint64_t order_id;
    double price;
    uint32_t quantity;
};

void process_order(double price, uint32_t quantity){
    auto o1=std::make_unique<Order>(Order{1000234,price,quantity});

    if(price>1000.0 ){
        std::cout<<"Order rejected: price too high!\n";
        return;
    }

    std::cout<<"Order processed at price: "<<o1->price<<"\n"; 
    
}

int main() {
    process_order(1050.0, 50);  
    process_order(500.0, 50);   
    return 0;
}