
#include "MarketMakerBot.h"

#include <thread>
#include <functional>


MarketMakerBot::MarketMakerBot(): m_balanceETH(10), m_balanceUSD(2000), m_keepRunning(true) {
    m_simulator = DvfSimulator::Create();
}


// can take any custom third party client trading algo
void MarketMakerBot::run(std::function<std::vector<OrderSuggestion>(double, double)> tradingAlgo)
{
    m_clientTradingAlgo = tradingAlgo;
    m_balanceDisplayer = std::thread (&MarketMakerBot::displayPortfolio, this);
    
    startTrading();
}

MarketMakerBot::~MarketMakerBot()
{
    m_keepRunning = false;
    if (m_balanceDisplayer.joinable())
        m_balanceDisplayer.join();
    delete m_simulator;
}



// cancel orders that may damage your portfolio 
void MarketMakerBot::cancelOutlierOrders(const double bestBid, const double bestAsk)
{

    auto avgBid = m_placedBidOrders.lower_bound (OrderSuggestion(bestBid, 1));
    for (auto it = avgBid; (it != m_placedBidOrders.end() && it->price > (bestBid - bestBid * sweetSpotForOrderCancel)); ++it ) {
        std::cout << RED << "[DEBUG] cancelling bid: " << it->price << NC << std::endl ;
        m_simulator->CancelOrder(it->orderId);  
    }
    m_placedBidOrders.erase(avgBid, m_placedBidOrders.end()); 

    auto avgAsk = m_placedAskOrders.lower_bound (OrderSuggestion(bestAsk, -1));
    for (auto it = avgAsk; (it != m_placedAskOrders.end() && it->price < (bestAsk + bestAsk * sweetSpotForOrderCancel)); ++it ) {
        std::cout << RED << "[DEBUG] cancelling ask: " << it->price << NC << std::endl ;
        m_simulator->CancelOrder(it->orderId);  
    }
    m_placedAskOrders.erase(avgAsk, m_placedAskOrders.end()); 

}

/* checks for any existing order gets filled (clause 5). 
            If yes, remove from placedOrders, and update PnL   ****************/
void MarketMakerBot::updatePnL(const double bestBid, const double bestAsk)
{
    std::cout << RED << "[DEBUG] received BestBid " << bestBid << " BestAsk: " << bestAsk << NC << std::endl ;

    /*  when you execute bid, you want to spend $ to buy ETH
        when you execute ask, you want to sell ETH and earn $   */

    // Any bid orders that are above the best bid are filled 
    auto aboveBestBid = m_placedBidOrders.upper_bound (OrderSuggestion(bestBid, 1));
    for (auto it = m_placedBidOrders.begin(); it != aboveBestBid; ++it )
    {
        m_balanceUSD.store(m_balanceUSD - it->price * it->amount);
        m_balanceETH.store(m_balanceETH + it->amount);
    }
    m_placedBidOrders.erase(m_placedBidOrders.begin(), aboveBestBid);
    // std::erase_if(m_placedBidOrders, [bestBid](auto x) { return x.price > bestBid; });

    // Any sell orders that are below the best ask are filled 
    auto belowBestAsk = m_placedAskOrders.lower_bound (OrderSuggestion(bestAsk, 1));
    for (auto it = belowBestAsk; it != m_placedAskOrders.end(); ++it ){
        m_balanceUSD.store(m_balanceUSD - it->price * it->amount);
        m_balanceETH.store(m_balanceETH + it->amount);
    }
    m_placedAskOrders.erase(m_placedAskOrders.begin(), belowBestAsk);
    // std::erase_if(m_placedAskOrders, [bestAsk](auto x) { return x.price < bestAsk; });
}

bool MarketMakerBot::creditCheckFailed (const bool isBid, const double usdPrice) const
{
    const bool isAsk = !isBid;

    if (isBid && m_balanceUSD < usdPrice * sweetSpotForCreditCheck) 
        return true;
    else if (isAsk && m_balanceETH < sweetSpotForCreditCheck) 
        return true;

    return false;
}

// void displayPortfolio(const MarketMakerBot * mmb) const
void MarketMakerBot::displayPortfolio() const
{
    using namespace std::chrono_literals;
    std::string msg;
    msg.reserve(200);
    while(this->m_keepRunning) {
        msg.clear();
        msg.append(GRN).append("\n____________PORTFOLIO SNAPSHOT____________\n ETH: ") 
            .append(std::to_string(this->m_balanceETH)).append(" \t USD: ")
            .append(std::to_string(this->m_balanceUSD))
            .append("\n------------------------------------------\n").append(NC);
        std::cout << msg;
        std::this_thread::sleep_for(PortfolioDisplayThreshold * 1000ms);
    }
}


void MarketMakerBot::startTrading() 
{
    std::map<double, double, std::greater<double>> bids;
    std::map<double, double> asks;
    
    // keep trading until broke :) 
    while (m_balanceETH > 0.0 || m_balanceUSD > 0.0 )
    {
        auto freshOrderBook = m_simulator->GetOrderBook();
        bids.clear(); asks.clear();

        for(auto& level :freshOrderBook)
            level.second > 0.0 
                ? bids[level.first]=level.second
                : asks[level.first]=level.second;
        
        updatePnL(bids.begin()->first, asks.begin()->first);

        // remove potential PortfolioDamagers
        cancelOutlierOrders(bids.begin()->first, asks.begin()->first);
        // cancelOutlierOrders((++(++(++bids.begin())))->first, (++(++(++asks.begin())))->first);


        // now make new suggestions 
        auto newSuggestions = this->m_clientTradingAlgo(bids.begin()->first, asks.begin()->first);
        
        for (auto& suggestion : newSuggestions)
        {
            const bool isBid = (suggestion.amount > 0.0);
            if (creditCheckFailed(isBid, suggestion.price)) 
                continue;   // bad credit. don't execute suggestion

            auto placedOrder = m_simulator->PlaceOrder(suggestion.price, suggestion.amount);
            if (placedOrder.has_value())
            { // order succesfully placed
                suggestion.orderId = placedOrder.value();
                isBid   ? m_placedBidOrders.insert(suggestion)
                        : m_placedAskOrders.insert(suggestion);
            }
        }
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(RefreshThreshold * 1000ms);
    }
}

