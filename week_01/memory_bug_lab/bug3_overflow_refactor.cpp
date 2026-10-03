#include<iostream>
#include<cstdint>
#include<array>

struct MarketDepth {
    std::array <double,5> bid_prices {};
    std::array <double,5> ask_prices {};
};

void update_bids(MarketDepth* depth,const std::array<double,5>& new_bids){
    depth->bid_prices=new_bids;

}

int main(){
    MarketDepth depth{};
    std::array <double,5> new_bids{100.1,100.2,100.3,100.4,100.5};

    update_bids(&depth,new_bids);
    std::cout<<"Top ask price: "<< depth.ask_prices[0]<<"\n";
    return 0;
}