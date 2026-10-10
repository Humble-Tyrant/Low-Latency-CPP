#include<iostream>
#include<memory>
#include "stack.hpp"
#include "queue.hpp"

int main(){

    // ==================== TEST 1: Stack move constructor ====================
    std::cout<<"\n\nTest 1 : Stack move constructor\n\n";

    Stack<int> a;
    a.push(1);
    a.push(2);
    a.push(3);

    Stack<int> b(std::move(a));

    std::cout<<"b size (expected 3) : "<<b.size()<<"\n";
    std::cout<<"b top() (expected 3) : "<<b.top()<<"\n";
    b.pop();
    std::cout<<"b top() (expected 2) : "<<b.top()<<"\n";
    b.pop();
    std::cout<<"b top() (expected 1) : "<<b.top()<<"\n";
    b.pop();
    std::cout<<"b empty (expected 1) : "<<b.empty()<<"\n";

    std::cout<<"a empty after move (expected 1) : "<<a.empty()<<"\n";
    std::cout<<"a size after move (expected 0) : "<<a.size()<<"\n";
    try{
        std::cout<<a.top()<<"\n";
    }
    catch(const std::out_of_range& e){
        std::cout<<"Caught exception (expected) : "<<e.what()<<"\n";
    }
    try{
        a.pop();
    }
    catch(const std::out_of_range& e){
        std::cout<<"Caught exception (expected) : "<<e.what()<<"\n";
    }

    // moved-from stack must still be usable
    a.push(99);
    std::cout<<"a reused, top() (expected 99) : "<<a.top()<<"\n";
    std::cout<<"a reused, size (expected 1) : "<<a.size()<<"\n";


    // ==================== TEST 2: Stack move assignment ====================
    std::cout<<"\n\nTest 2 : Stack move assignment\n\n";

    Stack<int> c;
    c.push(1);
    c.push(2);
    c.push(3);

    Stack<int> d;
    d.push(100);   // old contents of d must be deleted by operator=
    d.push(200);

    d = std::move(c);

    std::cout<<"d size (expected 3) : "<<d.size()<<"\n";
    std::cout<<"d top() (expected 3) : "<<d.top()<<"\n";
    d.pop();
    std::cout<<"d top() (expected 2) : "<<d.top()<<"\n";
    d.pop();
    std::cout<<"d top() (expected 1) : "<<d.top()<<"\n";
    d.pop();
    std::cout<<"d empty (expected 1) : "<<d.empty()<<"\n";

    std::cout<<"c empty after move (expected 1) : "<<c.empty()<<"\n";
    std::cout<<"c size after move (expected 0) : "<<c.size()<<"\n";
    c.push(7);
    std::cout<<"c reused, top() (expected 7) : "<<c.top()<<"\n";

    // move assign into an empty stack
    Stack<int> e;
    Stack<int> f;
    f.push(5);
    e = std::move(f);
    std::cout<<"e top() (expected 5) : "<<e.top()<<"\n";
    std::cout<<"f empty (expected 1) : "<<f.empty()<<"\n";

    // move assign FROM an empty stack
    Stack<int> g;
    Stack<int> h;
    h.push(1);
    h = std::move(g);
    std::cout<<"h empty after assigning from empty (expected 1) : "<<h.empty()<<"\n";
    std::cout<<"h size (expected 0) : "<<h.size()<<"\n";

    // self move assignment (alias avoids compiler self-move warning)
    Stack<int> i;
    i.push(1);
    i.push(2);
    Stack<int>& i_alias = i;
    i = std::move(i_alias);
    std::cout<<"i size after self move (expected 2) : "<<i.size()<<"\n";
    std::cout<<"i top() after self move (expected 2) : "<<i.top()<<"\n";

    // move only elements through a container move
    Stack<std::unique_ptr<int>> up1;
    up1.push(std::make_unique<int>(1));
    up1.push(std::make_unique<int>(2));
    Stack<std::unique_ptr<int>> up2(std::move(up1));
    std::cout<<"up1 empty (expected 1) : "<<up1.empty()<<"\n";
    std::cout<<"up2 top() (expected 2) : "<<*up2.top()<<"\n";
    up2.pop();
    std::cout<<"up2 top() (expected 1) : "<<*up2.top()<<"\n";


    // ==================== TEST 3: Queue move constructor ====================
    std::cout<<"\n\nTest 3 : Queue move constructor\n\n";

    Queue<int> qa;
    qa.push(1);
    qa.push(2);
    qa.push(3);

    Queue<int> qb(std::move(qa));

    std::cout<<"qb size (expected 3) : "<<qb.size()<<"\n";
    std::cout<<"qb front() (expected 1) : "<<qb.front()<<"\n";
    qb.pop();
    std::cout<<"qb front() (expected 2) : "<<qb.front()<<"\n";
    qb.pop();
    std::cout<<"qb front() (expected 3) : "<<qb.front()<<"\n";
    qb.pop();
    std::cout<<"qb empty (expected 1) : "<<qb.empty()<<"\n";

    std::cout<<"qa empty after move (expected 1) : "<<qa.empty()<<"\n";
    std::cout<<"qa size after move (expected 0) : "<<qa.size()<<"\n";
    try{
        std::cout<<qa.front()<<"\n";
    }
    catch(const std::out_of_range& ex){{
        std::cout<<"Caught exception (expected) : "<<ex.what()<<"\n";
    }

    // moved-from queue must still be usable
    qa.push(10);
    qa.push(20);
    std::cout<<"qa reused, front() (expected 10) : "<<qa.front()<<"\n";
    qa.pop();
    std::cout<<"qa reused, front() (expected 20) : "<<qa.front()<<"\n";

    // moved-to queue must be appendable (tail_ was stolen correctly)
    Queue<int> qc;
    qc.push(1);
    qc.push(2);
    Queue<int> qd(std::move(qc));
    qd.push(3);
    std::cout<<"qd size after push (expected 3) : "<<qd.size()<<"\n";
    qd.pop();
    qd.pop();
    std::cout<<"qd front() (expected 3) : "<<qd.front()<<"\n";


    // ==================== TEST 4: Queue move assignment ====================
    std::cout<<"\n\nTest 4 : Queue move assignment\n\n";

    Queue<int> qe;
    qe.push(1);
    qe.push(2);
    qe.push(3);

    Queue<int> qf;
    qf.push(100);
    qf.push(200);

    qf = std::move(qe);

    std::cout<<"qf size (expected 3) : "<<qf.size()<<"\n";
    std::cout<<"qf front() (expected 1) : "<<qf.front()<<"\n";
    qf.push(4);   // tail_ must point at the last stolen node
    std::cout<<"qf size after push (expected 4) : "<<qf.size()<<"\n";
    qf.pop();
    qf.pop();
    qf.pop();
    std::cout<<"qf front() (expected 4) : "<<qf.front()<<"\n";

    std::cout<<"qe empty after move (expected 1) : "<<qe.empty()<<"\n";
    qe.push(9);
    std::cout<<"qe reused, front() (expected 9) : "<<qe.front()<<"\n";

    Queue<int>& qf_alias = qf;
    qf = std::move(qf_alias);
    std::cout<<"qf size after self move (expected 1) : "<<qf.size()<<"\n";
    std::cout<<"qf front() after self move (expected 4) : "<<qf.front()<<"\n";


    // ==================== TEST 5: 10,000 pushes ====================
    std::cout<<"\n\nTest 5 : 10,000 pushes\n\n";

    Stack<int> big_s;
    for(int k=0;k<10000;k++){
        big_s.push(k);
    }
    std::cout<<"big_s size (expected 10000) : "<<big_s.size()<<"\n";
    std::cout<<"big_s top() (expected 9999) : "<<big_s.top()<<"\n";
    int stack_wrong_order=0;
    for(int k=9999;k>=0;k--){
        if(big_s.top()!=k){
            stack_wrong_order++;
        }
        big_s.pop();
    }
    std::cout<<"stack pops out of order (expected 0) : "<<stack_wrong_order<<"\n";
    std::cout<<"big_s empty (expected 1) : "<<big_s.empty()<<"\n";

    Queue<int> big_q;
    for(int k=0;k<10000;k++){
        big_q.push(k);
    }
    std::cout<<"big_q size (expected 10000) : "<<big_q.size()<<"\n";
    std::cout<<"big_q front() (expected 0) : "<<big_q.front()<<"\n";
    int queue_wrong_order=0;
    for(int k=0;k<10000;k++){
        if(big_q.front()!=k){
            queue_wrong_order++;
        }
        big_q.pop();
    }
    std::cout<<"queue pops out of order (expected 0) : "<<queue_wrong_order<<"\n";
    std::cout<<"big_q empty (expected 1) : "<<big_q.empty()<<"\n";


    // ==================== TEST 6: Queue tail_ reset on empty ====================
    std::cout<<"\n\nTest 6 : Queue tail_ reset when popped to exactly 0\n\n";

    Queue<int> t;
    t.push(1);
    t.pop();   // exactly 0 elements now
    std::cout<<"t empty (expected 1) : "<<t.empty()<<"\n";
    std::cout<<"t size (expected 0) : "<<t.size()<<"\n";

    t.push(2);   // must not write through a stale tail_
    t.push(3);
    t.push(4);
    std::cout<<"t size (expected 3) : "<<t.size()<<"\n";
    std::cout<<"t front() (expected 2) : "<<t.front()<<"\n";
    t.pop();
    std::cout<<"t front() (expected 3) : "<<t.front()<<"\n";
    t.pop();
    std::cout<<"t front() (expected 4) : "<<t.front()<<"\n";
    t.pop();
    std::cout<<"t empty (expected 1) : "<<t.empty()<<"\n";

    // drain to 0 and refill, many times
    Queue<int> cyc;
    int cycle_errors=0;
    for(int round=0;round<1000;round++){
        for(int k=0;k<3;k++){
            cyc.push(round*10+k);
        }
        for(int k=0;k<3;k++){
            if(cyc.front()!=round*10+k){
                cycle_errors++;
            }
            cyc.pop();
        }
        if(!cyc.empty()){
            cycle_errors++;
        }
    }
    std::cout<<"1000 drain/refill cycles, errors (expected 0) : "<<cycle_errors<<"\n";

    // push/pop at the 1 element boundary
    Queue<int> edge;
    edge.push(1);
    edge.push(2);
    edge.pop();      // 1 left : head_ == tail_
    edge.push(3);    // append after the single remaining node
    std::cout<<"edge size (expected 2) : "<<edge.size()<<"\n";
    std::cout<<"edge front() (expected 2) : "<<edge.front()<<"\n";
    edge.pop();
    std::cout<<"edge front() (expected 3) : "<<edge.front()<<"\n";
    edge.pop();      // 0 again
    edge.push(4);
    std::cout<<"edge front() after hitting 0 again (expected 4) : "<<edge.front()<<"\n";
    std::cout<<"edge size (expected 1) : "<<edge.size()<<"\n";

    // same with move only elements (rvalue push after reset)
    Queue<std::unique_ptr<int>> uq;
    uq.push(std::make_unique<int>(1));
    uq.pop();
    uq.push(std::make_unique<int>(2));
    uq.push(std::make_unique<int>(3));
    std::cout<<"uq front() (expected 2) : "<<*uq.front()<<"\n";
    uq.pop();
    std::cout<<"uq front() (expected 3) : "<<*uq.front()<<"\n";

    // same with lvalue push after reset
    Queue<int> lq;
    int x=5;
    int y=6;
    lq.push(x);
    lq.pop();
    lq.push(y);
    lq.push(x);
    std::cout<<"lq front() (expected 6) : "<<lq.front()<<"\n";
    lq.pop();
    std::cout<<"lq front() (expected 5) : "<<lq.front()<<"\n";

    return 0;
}
}