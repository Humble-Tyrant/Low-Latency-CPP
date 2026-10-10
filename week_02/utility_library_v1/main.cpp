#include<iostream>
#include<memory>
#include<stdexcept>
#include "fixed_size_buffer.hpp"

int main(){

    // ==================== TEST 1: Push 10, 20, 30 into FixedBuffer<int, 3> ====================
    std::cout<<"\n\nTest 1 : Push 10, 20, 30 into FixedBuffer<int, 3>\n\n";

    FixedBuffer<int,3> buffer;
    std::cout<<"capacity (expected 3) : "<<buffer.capacity()<<"\n";
    std::cout<<"empty before push (expected 1) : "<<buffer.empty()<<"\n";
    std::cout<<"full before push (expected 0) : "<<buffer.full()<<"\n";
    std::cout<<"size before push (expected 0) : "<<buffer.size()<<"\n";

    buffer.push(10);
    buffer.push(20);
    buffer.push(30);

    std::cout<<"size (expected 3) : "<<buffer.size()<<"\n";
    std::cout<<"empty (expected 0) : "<<buffer.empty()<<"\n";
    std::cout<<"full (expected 1) : "<<buffer.full()<<"\n";
    std::cout<<"front() (expected 10) : "<<buffer.front()<<"\n";
    std::cout<<"back() (expected 30) : "<<buffer.back()<<"\n";
    std::cout<<"buffer[0] (expected 10) : "<<buffer[0]<<"\n";
    std::cout<<"buffer[1] (expected 20) : "<<buffer[1]<<"\n";
    std::cout<<"buffer[2] (expected 30) : "<<buffer[2]<<"\n";


    // ==================== TEST 2: Modify back() from 30 to 99 in-place ====================
    std::cout<<"\n\nTest 2 : Modify back() from 30 to 99 in-place\n\n";

    std::cout<<"back() before (expected 30) : "<<buffer.back()<<"\n";
    buffer.back()=99;
    std::cout<<"buffer[2] after (expected 99) : "<<buffer[2]<<"\n";
    std::cout<<"back() after (expected 99) : "<<buffer.back()<<"\n";
    std::cout<<"at(2) after (expected 99) : "<<buffer.at(2)<<"\n";
    std::cout<<"size unchanged (expected 3) : "<<buffer.size()<<"\n";
    std::cout<<"front() untouched (expected 10) : "<<buffer.front()<<"\n";

    // front() is mutable in place too
    buffer.front()=11;
    std::cout<<"buffer[0] after front()=11 (expected 11) : "<<buffer[0]<<"\n";
    buffer.front()=10;   // restore
    // operator[] is mutable too
    buffer[1]=21;
    std::cout<<"buffer[1] after buffer[1]=21 (expected 21) : "<<buffer[1]<<"\n";
    buffer[1]=20;        // restore


    // ==================== TEST 3: Push a 4th element -> overflow_error ====================
    std::cout<<"\n\nTest 3 : Push a 4th element, verify std::overflow_error is caught\n\n";

    try{
        buffer.push(40);   // rvalue push overload
        std::cout<<"ERROR : no exception thrown (rvalue push)\n";
    }
    catch(const std::overflow_error& e){
        std::cout<<"Caught overflow_error (expected) : "<<e.what()<<"\n";
    }

    int lvalue=50;
    try{
        buffer.push(lvalue);   // lvalue push overload
        std::cout<<"ERROR : no exception thrown (lvalue push)\n";
    }
    catch(const std::overflow_error& e){
        std::cout<<"Caught overflow_error on lvalue push (expected) : "<<e.what()<<"\n";
    }

    // the failed pushes must not have changed anything
    std::cout<<"size after failed pushes (expected 3) : "<<buffer.size()<<"\n";
    std::cout<<"back() after failed pushes (expected 99) : "<<buffer.back()<<"\n";
    std::cout<<"front() after failed pushes (expected 10) : "<<buffer.front()<<"\n";


    // ==================== TEST 4: Pop all, front() -> underflow_error ====================
    std::cout<<"\n\nTest 4 : Pop all elements, call front(), verify std::underflow_error is caught\n\n";

    buffer.pop();
    std::cout<<"size after 1 pop (expected 2) : "<<buffer.size()<<"\n";
    std::cout<<"back() after 1 pop (expected 20) : "<<buffer.back()<<"\n";
    buffer.pop();
    std::cout<<"size after 2 pops (expected 1) : "<<buffer.size()<<"\n";
    std::cout<<"back() after 2 pops (expected 10) : "<<buffer.back()<<"\n";
    buffer.pop();
    std::cout<<"size after 3 pops (expected 0) : "<<buffer.size()<<"\n";
    std::cout<<"empty (expected 1) : "<<buffer.empty()<<"\n";
    std::cout<<"full (expected 0) : "<<buffer.full()<<"\n";

    try{
        std::cout<<buffer.front()<<"\n";
        std::cout<<"ERROR : no exception thrown (front)\n";
    }
    catch(const std::underflow_error& e){
        std::cout<<"Caught underflow_error on front() (expected) : "<<e.what()<<"\n";
    }
    try{
        std::cout<<buffer.back()<<"\n";
        std::cout<<"ERROR : no exception thrown (back)\n";
    }
    catch(const std::underflow_error& e){
        std::cout<<"Caught underflow_error on back() (expected) : "<<e.what()<<"\n";
    }
    try{
        buffer.pop();
        std::cout<<"ERROR : no exception thrown (pop)\n";
    }
    catch(const std::underflow_error& e){
        std::cout<<"Caught underflow_error on pop() (expected) : "<<e.what()<<"\n";
    }

    // const overloads on an empty buffer
    const FixedBuffer<int,3>& const_empty=buffer;
    try{
        std::cout<<const_empty.front()<<"\n";
        std::cout<<"ERROR : no exception thrown (const front)\n";
    }
    catch(const std::underflow_error& e){
        std::cout<<"Caught underflow_error on const front() (expected) : "<<e.what()<<"\n";
    }
    try{
        std::cout<<const_empty.back()<<"\n";
        std::cout<<"ERROR : no exception thrown (const back)\n";
    }
    catch(const std::underflow_error& e){
        std::cout<<"Caught underflow_error on const back() (expected) : "<<e.what()<<"\n";
    }


    // ==================== TEST 5: FixedBuffer<unique_ptr<int>, 2> ====================
    std::cout<<"\n\nTest 5 : FixedBuffer<std::unique_ptr<int>, 2>, push two with std::move, verify ownership\n\n";

    FixedBuffer<std::unique_ptr<int>,2> ubuf;

    std::unique_ptr<int> p1=std::make_unique<int>(1);
    std::unique_ptr<int> p2=std::make_unique<int>(2);
    int* raw1=p1.get();
    int* raw2=p2.get();

    ubuf.push(std::move(p1));
    ubuf.push(std::move(p2));

    std::cout<<"p1 is null after move (expected 1) : "<<(p1==nullptr)<<"\n";
    std::cout<<"p2 is null after move (expected 1) : "<<(p2==nullptr)<<"\n";
    std::cout<<"ubuf size (expected 2) : "<<ubuf.size()<<"\n";
    std::cout<<"ubuf full (expected 1) : "<<ubuf.full()<<"\n";
    std::cout<<"*ubuf.front() (expected 1) : "<<*ubuf.front()<<"\n";
    std::cout<<"*ubuf.back() (expected 2) : "<<*ubuf.back()<<"\n";
    std::cout<<"*ubuf[0] (expected 1) : "<<*ubuf[0]<<"\n";
    std::cout<<"*ubuf[1] (expected 2) : "<<*ubuf[1]<<"\n";
    // same heap object, no copy was made
    std::cout<<"ubuf[0] holds the same address p1 had (expected 1) : "<<(ubuf[0].get()==raw1)<<"\n";
    std::cout<<"ubuf[1] holds the same address p2 had (expected 1) : "<<(ubuf[1].get()==raw2)<<"\n";

    // push into a full buffer: must throw and must NOT consume the pointer
    std::unique_ptr<int> p3=std::make_unique<int>(3);
    try{
        ubuf.push(std::move(p3));
        std::cout<<"ERROR : no exception thrown\n";
    }
    catch(const std::overflow_error& e){
        std::cout<<"Caught overflow_error (expected) : "<<e.what()<<"\n";
    }
    std::cout<<"p3 still owns its int after failed push (expected 1) : "<<(p3!=nullptr)<<"\n";
    std::cout<<"*p3 (expected 3) : "<<*p3<<"\n";

    // move ownership back OUT of the buffer
    std::unique_ptr<int> out=std::move(ubuf.front());
    std::cout<<"*out (expected 1) : "<<*out<<"\n";
    std::cout<<"ubuf.front() is null after moving out (expected 1) : "<<(ubuf.front()==nullptr)<<"\n";
    std::cout<<"ubuf size still counts the slot (expected 2) : "<<ubuf.size()<<"\n";

    // modify through the pointer stored in the buffer
    *ubuf.back()=22;
    std::cout<<"*ubuf[1] after *ubuf.back()=22 (expected 22) : "<<*ubuf[1]<<"\n";

    ubuf.pop();
    std::cout<<"ubuf size after pop (expected 1) : "<<ubuf.size()<<"\n";
    ubuf.pop();
    std::cout<<"ubuf empty after 2nd pop (expected 1) : "<<ubuf.empty()<<"\n";
    try{
        std::cout<<ubuf.front().get()<<"\n";
    }
    catch(const std::underflow_error& e){
        std::cout<<"Caught underflow_error (expected) : "<<e.what()<<"\n";
    }

    // does pop() release the unique_ptr stored in the slot?
    FixedBuffer<std::unique_ptr<int>,2> ubuf2;
    ubuf2.push(std::make_unique<int>(5));
    ubuf2.pop();
    std::cout<<"size after pop (expected 0) : "<<ubuf2.size()<<"\n";
    std::cout<<"popped slot [0] still holds the int, pop() did not release it (prints 1 with your current pop) : "<<(ubuf2[0]!=nullptr)<<"\n";
    ubuf2.push(std::make_unique<int>(6));   // overwrites the stale slot
    std::cout<<"*ubuf2.front() after refill (expected 6) : "<<*ubuf2.front()<<"\n";


    // ==================== TEST 6: lvalue (copy) push ====================
    std::cout<<"\n\nTest 6 : lvalue push copies, original stays untouched\n\n";

    FixedBuffer<int,3> cbuf;
    int v=7;
    cbuf.push(v);
    cbuf.back()=70;
    std::cout<<"v after modifying buffer copy (expected 7) : "<<v<<"\n";
    std::cout<<"cbuf.back() (expected 70) : "<<cbuf.back()<<"\n";


    // ==================== TEST 7: at() bounds checking ====================
    std::cout<<"\n\nTest 7 : at() bounds checking (checks against size, not capacity)\n\n";

    FixedBuffer<int,3> abuf;
    abuf.push(1);
    abuf.push(2);
    std::cout<<"at(0) (expected 1) : "<<abuf.at(0)<<"\n";
    std::cout<<"at(1) (expected 2) : "<<abuf.at(1)<<"\n";
    try{
        std::cout<<abuf.at(2)<<"\n";   // idx 2 < Capacity but >= size
        std::cout<<"ERROR : no exception thrown (at(2))\n";
    }
    catch(const std::out_of_range& e){
        std::cout<<"Caught out_of_range on at(2) (expected, size is 2) : "<<e.what()<<"\n";
    }
    try{
        std::cout<<abuf.at(100)<<"\n";
        std::cout<<"ERROR : no exception thrown (at(100))\n";
    }
    catch(const std::out_of_range& e){
        std::cout<<"Caught out_of_range on at(100) (expected) : "<<e.what()<<"\n";
    }
    abuf.at(0)=100;
    std::cout<<"abuf[0] after at(0)=100 (expected 100) : "<<abuf[0]<<"\n";

    const FixedBuffer<int,3>& const_abuf=abuf;
    std::cout<<"const at(1) (expected 2) : "<<const_abuf.at(1)<<"\n";
    std::cout<<"const [0] (expected 100) : "<<const_abuf[0]<<"\n";
    std::cout<<"const front() (expected 100) : "<<const_abuf.front()<<"\n";
    std::cout<<"const back() (expected 2) : "<<const_abuf.back()<<"\n";
    try{
        std::cout<<const_abuf.at(2)<<"\n";
        std::cout<<"ERROR : no exception thrown (const at(2))\n";
    }
    catch(const std::out_of_range& e){
        std::cout<<"Caught out_of_range on const at(2) (expected) : "<<e.what()<<"\n";
    }


    // ==================== TEST 8: clear() and reuse ====================
    std::cout<<"\n\nTest 8 : clear() and reuse\n\n";

    FixedBuffer<int,3> rbuf;
    rbuf.push(1);
    rbuf.push(2);
    rbuf.push(3);
    std::cout<<"full before clear (expected 1) : "<<rbuf.full()<<"\n";
    rbuf.clear();
    std::cout<<"size after clear (expected 0) : "<<rbuf.size()<<"\n";
    std::cout<<"empty after clear (expected 1) : "<<rbuf.empty()<<"\n";
    try{
        std::cout<<rbuf.front()<<"\n";
    }
    catch(const std::underflow_error& e){
        std::cout<<"Caught underflow_error after clear (expected) : "<<e.what()<<"\n";
    }
    rbuf.push(8);
    rbuf.push(9);
    std::cout<<"size after refill (expected 2) : "<<rbuf.size()<<"\n";
    std::cout<<"front() after refill (expected 8) : "<<rbuf.front()<<"\n";
    std::cout<<"back() after refill (expected 9) : "<<rbuf.back()<<"\n";
    rbuf.push(10);
    std::cout<<"full after refill to capacity (expected 1) : "<<rbuf.full()<<"\n";
    try{
        rbuf.push(11);
    }
    catch(const std::overflow_error& e){
        std::cout<<"Caught overflow_error after refill (expected) : "<<e.what()<<"\n";
    }


    // ==================== TEST 9: push/pop cycles at the boundaries ====================
    std::cout<<"\n\nTest 9 : fill / drain cycles\n\n";

    FixedBuffer<int,3> cyc;
    int cycle_errors=0;
    for(int round=0;round<1000;round++){
        for(int k=0;k<3;k++){
            cyc.push(round*10+k);
        }
        if(!cyc.full()){
            cycle_errors++;
        }
        for(int k=2;k>=0;k--){
            if(cyc.back()!=round*10+k){
                cycle_errors++;
            }
            cyc.pop();
        }
        if(!cyc.empty()){
            cycle_errors++;
        }
    }
    std::cout<<"1000 fill/drain cycles, errors (expected 0) : "<<cycle_errors<<"\n";

    // pop then push overwrites the right slot
    FixedBuffer<int,3> ow;
    ow.push(1);
    ow.push(2);
    ow.pop();
    ow.push(3);
    std::cout<<"size (expected 2) : "<<ow.size()<<"\n";
    std::cout<<"ow[0] (expected 1) : "<<ow[0]<<"\n";
    std::cout<<"ow[1] (expected 3) : "<<ow[1]<<"\n";
    std::cout<<"back() (expected 3) : "<<ow.back()<<"\n";

    // capacity 1 edge case
    FixedBuffer<int,1> one;
    one.push(5);
    std::cout<<"cap 1 full (expected 1) : "<<one.full()<<"\n";
    std::cout<<"cap 1 front()==back() (expected 1) : "<<(one.front()==one.back())<<"\n";
    try{
        one.push(6);
    }
    catch(const std::overflow_error& e){
        std::cout<<"Caught overflow_error cap 1 (expected) : "<<e.what()<<"\n";
    }
    one.pop();
    one.push(7);
    std::cout<<"cap 1 front() after pop/push (expected 7) : "<<one.front()<<"\n";

    return 0;
}