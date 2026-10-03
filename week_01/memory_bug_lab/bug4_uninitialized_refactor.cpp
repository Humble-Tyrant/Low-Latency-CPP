#include<iostream>
#include<cstdint>

struct Orders{
    uint64_t order_id{};
    double price{};
    uint32_t quantity{};
    char side{}; //B for buy & S for sell
};

void send_order(const Orders& order){
    if(order.side=='B') std::cout<< "Submitting BUY order "<<order.order_id<<" at "<<order.price<<"\n";
    else if(order.side=='S') std::cout<<"Submitting SELL order "<<order.order_id<<" at "<<order.price<<"\n";
    else std::cout<<"Rejected! Unknown order side\n";
}

int main(){
    Orders order{};
    order.order_id=1000234;
    order.price=101.0;
    order.quantity=100;

    
    send_order(order);

}