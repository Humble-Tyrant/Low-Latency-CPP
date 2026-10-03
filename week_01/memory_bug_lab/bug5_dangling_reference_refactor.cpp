#include<iostream>
#include<cstdint>

struct RiskLimits {
    double price {};
    uint32_t quantity {};
};

RiskLimits get_default_limits(){
    return RiskLimits{1000.10,100};
}

int main(){
    RiskLimits limits=get_default_limits();

    std::cout<<"Max order quantity: "<<limits.quantity<<" Max order price: "<<limits.price<<"\n";
    return 0;
}