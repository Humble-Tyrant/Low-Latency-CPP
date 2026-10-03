#include<iostream>
#include<cstdint>

struct MarketUpdate {
    uint64_t timestamp;
    double price;
    uint32_t quantity;
};

MarketUpdate* create_update(double price, uint32_t quantity){
    MarketUpdate* update=new MarketUpdate{100234,price,quantity};
    delete update;
    return update;
} 

int main(){
    MarketUpdate* data = create_update(100.5,100);
    std::cout<<"Price: "<<data->price<<" Quantity: "<<data->quantity<<"\n";
    return 0;
}