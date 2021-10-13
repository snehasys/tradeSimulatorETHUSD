
// Silly Naive client Trading Algo

#pragma once

#include "Common.h"
#include <vector>

//  The standalone Client algo                     bestBid       BestAsk
std::vector<OrderSuggestion> tradingAlgorithm(double bBid, double bAsk){
    // The bot should place 5x BID orders and 5x ASK orders within 5% of the best bid and best ask prices.

    std::vector<OrderSuggestion> os;

    os.emplace_back(OrderSuggestion{ (bBid * (0.95)) , 1.5 });
    os.emplace_back(OrderSuggestion{ (bBid * (0.96)) , 1.4 });
    os.emplace_back(OrderSuggestion{ (bBid * (0.97)) , 1.3 });
    os.emplace_back(OrderSuggestion{ (bBid * (0.98)) , 1.2 });
    os.emplace_back(OrderSuggestion{ (bBid * (0.99)) , 1.1 });
    
    os.emplace_back(OrderSuggestion{ (bAsk * (1.05)) , -1.1 });
    os.emplace_back(OrderSuggestion{ (bAsk * (1.04)) , -1.2 });
    os.emplace_back(OrderSuggestion{ (bAsk * (1.03)) , -1.3 });
    os.emplace_back(OrderSuggestion{ (bAsk * (1.02)) , -1.4 });
    os.emplace_back(OrderSuggestion{ (bAsk * (1.01)) , -1.5 });

    return os;
}