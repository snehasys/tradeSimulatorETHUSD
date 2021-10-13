
#include <thread>
#include "MarketMakerBot.h"
#include "ClientTradingAlgo.h"


/* 
$ g++ --version
  g++ (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0 */


/* INSTRUCTION 

1. How to compile and run the program
   $  g++ MarketMakerBot.cpp  main.cpp --std=c++2a -lpthread   -o botSimulator && ./botSimulator 

2. How to compile and run Unittests
   $  g++ MarketMakerBot.cpp  AllUnitTests.cpp --std=c++2a -lpthread  -o unittest && ./unittest 
*/

int main()
{
    MarketMakerBot bot;
    bot.run(tradingAlgorithm);
    return 0;
}


