#include<iostream>
#include<cstdint>

struct MarketDepth {
    double bid_prices[5];
    double ask_prices[5];
};

void update_bids(MarketDepth* depth,const double* new_bids,size_t count){
    for(size_t i {} ; i <= count ; ++i){
        depth->bid_prices[i]=new_bids[i];
    }

}

int main(){
    MarketDepth depth{};
    double bids[5]={100.1,100.2,100.3,100.4,100.5};

    update_bids(&depth,bids,5);
    std::cout<<"Top ask price: "<< depth.ask_prices[0]<<"\n";
    return 0;
}