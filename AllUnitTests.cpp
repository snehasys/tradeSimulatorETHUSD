
#include "MarketMakerBot.h"
#include "ClientTradingAlgo.h"
#include <cassert>  // should have ideally used BOOST_TEST or GoogleUnitTest


/* INSTRUCTION 

1. How to compile and run the program
   $  g++ MarketMakerBot.cpp  main.cpp --std=c++2a -lpthread  && ./a.out

2. How to compile and run Unittests
   $  g++ MarketMakerBot.cpp  AllUnitTests.cpp --std=c++2a -lpthread  -o unittest && ./unittest 

*/


class TestMarketMakerBot : public MarketMakerBot 
{
private:
   void fakePlacedBids(double i){
      m_placedBidOrders.clear();
      m_placedBidOrders.insert(OrderSuggestion(i++,1));
      m_placedBidOrders.insert(OrderSuggestion(i++,1));
      m_placedBidOrders.insert(OrderSuggestion(i++,1));
      m_placedBidOrders.insert(OrderSuggestion(i++,1));
      m_placedBidOrders.insert(OrderSuggestion(i++,1));
   }
   void fakePlacedAsks(double i){
      m_placedAskOrders.clear();
      m_placedAskOrders.insert(OrderSuggestion(i++,1));
      m_placedAskOrders.insert(OrderSuggestion(i++,1));
      m_placedAskOrders.insert(OrderSuggestion(i++,1));
      m_placedAskOrders.insert(OrderSuggestion(i++,1));
      m_placedAskOrders.insert(OrderSuggestion(i++,1));
   }

public:
   void testCreditCheckFailed()
   {
      m_balanceUSD.store(double(200.0));
      // check for bids
      assert(creditCheckFailed(1,199) == false );  // with USD = 200
      assert(creditCheckFailed(1,500.0) == true ); // with USD = 200
      m_balanceUSD.store(double(0.0));
      assert(creditCheckFailed(1,1) == true); 

      m_balanceETH.store(double(10.0));
      // check for asks
      assert(creditCheckFailed(0,1) == false); // with ETH = 10
      m_balanceETH.store(double(0.0));
      assert(creditCheckFailed(0,1) == true); 
   }

   void testUpdatePnL()
   {
      {
         m_balanceUSD = 100.0;
         m_balanceETH = 10.0;
         fakePlacedBids(5);  // i.e. bids 5 6 7 8 9
         fakePlacedAsks(15); // i.e. asks 15 16 17 18 19
         updatePnL(7,18); // best bid 7, best ask 18.. Spread is huge in this example.. :) 
         assert (m_placedBidOrders.size() == 3); // i.e. remaining 5 6 7 (above the best bid are already filled)
         assert (m_placedAskOrders.size() == 2); // i.e. remaining 18 19 (below the best ask are already filled)
         assert (m_balanceUSD == 46.0);
         assert (m_balanceETH == 14.0);
      }
      {
         m_balanceUSD = 200.0;
         m_balanceETH = 10.0;
         fakePlacedBids(25);  
         fakePlacedAsks(26); 
         updatePnL(37,30); 
         assert (m_placedBidOrders.size() == 5); 
         assert (m_placedAskOrders.size() == 1); 
         assert (m_balanceUSD == 170.0);
         assert (m_balanceETH == 11.0);
      }
   }

   ~TestMarketMakerBot() = default;
};

int main()
{
   std::cerr << RED;
   TestMarketMakerBot testbot;
   testbot.testCreditCheckFailed();
   testbot.testUpdatePnL();

   std::cout << GRN << "All Unit Tests Passed!" << NC << std::endl;

   return 0;
}


