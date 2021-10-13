# Deversifi C++ Coding challenge

This repo contains the single header starting point for the C++ coding challenge.

Rather than request participants to spend time setting up a project from scratch complete with libraries for performing REST requests and serialising JSON, the DvfSimulator class provides a simplified trading API with a radically simplified approximation of how an exchange might operate.

```
#include "DvfSimulator.h"

int main()
{
    auto* sim = DvfSimulator::Create();
    
    auto ob = sim->GetOrderBook();
    
    // ... decide price and amount
    sim->PlaceOrder(price, amount);
    sim->CancelOrder(oid);

    return true;
}
```


# After completion of challenge:
# How to Run:
$ ./runBotSimulator.sh
# Sample output snippet 
```
____________PORTFOLIO SNAPSHOT____________
 ETH: 10.000000          USD: 2000.000000
------------------------------------------
Placed order id 1629467539 @ 208.95 : 1 BID
Placed order id 1629467540 @ 206.96 : 1 BID
Placed order id 1629467541 @ 204.97 : 1 BID
Placed order id 1629467542 @ 202.98 : 1 BID
Placed order id 1629467543 @ 200.99 : 1 BID
Placed order id 1629467544 @ 202.35 : -1 ASK
Placed order id 1629467545 @ 204.48 : -1 ASK
Placed order id 1629467546 @ 206.61 : -1 ASK
Placed order id 1629467547 @ 208.74 : -1 ASK
Placed order id 1629467548 @ 210.87 : -1 ASK
BB 202 BA 212
Filled ASK order id 1629467544 @ 202.35 : -1
Filled ASK order id 1629467545 @ 204.48 : -1
Filled ASK order id 1629467546 @ 206.61 : -1
Filled ASK order id 1629467547 @ 208.74 : -1
Filled ASK order id 1629467548 @ 210.87 : -1
[DEBUG] received BestBid 200 BestAsk: 214
Placed order id 1629467549 @ 210 : 1 BID
Placed order id 1629467550 @ 208 : 1 BID
Placed order id 1629467551 @ 206 : 1 BID
Placed order id 1629467552 @ 204 : 1 BID
Placed order id 1629467553 @ 202 : 1 BID
Placed order id 1629467554 @ 203.3 : -1 ASK
Placed order id 1629467555 @ 205.44 : -1 ASK
Placed order id 1629467556 @ 207.58 : -1 ASK
Placed order id 1629467557 @ 209.72 : -1 ASK
Placed order id 1629467558 @ 211.86 : -1 ASK

____________PORTFOLIO SNAPSHOT____________
 ETH: 15.000000          USD: 975.150000
------------------------------------------
BB 212 BA 222
Filled ASK order id 1629467554 @ 203.3 : -1
Filled ASK order id 1629467555 @ 205.44 : -1
Filled ASK order id 1629467556 @ 207.58 : -1
Filled ASK order id 1629467557 @ 209.72 : -1
Filled ASK order id 1629467558 @ 211.86 : -1
[DEBUG] received BestBid 209 BestAsk: 223
```
