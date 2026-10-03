#include<iostream>
#include<cstdint>

struct Order {
    uint64_t order_id;
    double price;
    uint32_t quantity;
};

void process_order(double price, uint32_t quantity){
    Order* o1=new Order{1000234,price,quantity};

    if(price>1000.0 ){
        std::cout<<"Order rejected: price too high!\\n";
        return;
    }

    std::cout<<"Order processed at price: "<<o1->price<<"\n"; 
    delete o1;
}

int main() {
    process_order(1050.0, 50);  
    process_order(500.0, 50);   
    return 0;
}