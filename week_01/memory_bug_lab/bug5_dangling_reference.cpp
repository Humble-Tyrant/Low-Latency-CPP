#include<iostream>
#include<cstdint>

struct RiskLimits {
    double price {};
    uint32_t quantity {};
};

const RiskLimits& get_default_limits(){
    RiskLimits limits{1000.10,100};
    return limits;
}

int main(){
    const RiskLimits& limits=get_default_limits();

    std::cout<<"Max order quantity: "<<limits.quantity<<" Max order price: "<<limits.price<<"\n";
    return 0;
}