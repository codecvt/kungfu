//
// Created by PolarAir on 2019-04-18.
//

#ifndef KUNGFU_PORTFOLIO_MANAGER_HPP
#define KUNGFU_PORTFOLIO_MANAGER_HPP

#include "../include/portfolio_manager.h"
#include "../include/account_manager.h"
#include "portfolio_storage.h"
#include "util/include/business_helper.h"
#include "serialize.h"
#include <spdlog/spdlog.h>
#include <map>
#include <boost/core/ignore_unused.hpp>

namespace kungfu
{
    class PortfolioManager::impl final : public IPnLDataHandler
    {
    public:
        friend class PortfolioStorage;
        explicit impl(const char* db);
        virtual ~impl();

        Position get_long_pos(const char* account_id, const char* instrument_id, const char* exchange_id) const;
        Position get_short_pos(const char* account_id, const char* instrument_id, const char* exchange_id) const;
        double get_last_price(const char* instrument_id, const char* exchange_id) const;
        std::vector<Instrument> get_all_pos_instruments(const char* account_id) const;

        SubPortfolioInfo get_sub_portfolio(const char* account_id) const;
        PortfolioInfo get_portfolio() const;

        const AccountManagerPtr get_account(const char* account_id) const;

        // IPnLDataHandler
        void on_quote(const Quote* quote) override;
        void on_order(const Order* order) override;
        void on_trade(const Trade* trade) override;
        void on_positions(const std::vector<Position>& positions) override;
        void on_position_details(const std::vector<Position>& details) override;
        void on_account(const AccountInfo& account) override;
        void insert_order(const OrderInput* input) override;
        bool freeze_algo_order(uint64_t algo_id, const AssetsFrozen& frozen) override;
        void release_algo_order(uint64_t algo_id) override;
        void switch_day(const std::string& trading_day) override;
        int64_t get_last_update() const override;
        std::string get_current_trading_day() const override;
        void set_current_trading_day(const std::string& trading_day) override;
        void register_pos_callback(PositionCallback cb) override;
        void register_acc_callback(AccountCallback cb) override;
        void register_pnl_callback(PnLCallback cb) override;
        void set_initial_equity(double equity) override;
        void set_static_equity(double equity) override;
        // IPnLDataHandler

    private:
        bool recalc_pnl();
        void callback() const;
        void on_acc_callback(const AccountInfo& acc) const;
        void on_pos_callback(const Position& pos) const;

    private:
        std::string                                     db_;
        int64_t                                         last_update_;
        std::string                                     trading_day_;
        std::map<std::string, AccountManagerPtr>        accounts_;
        PortfolioInfo                                   pnl_;
        std::vector<PnLCallback>                        cbs_;
        std::vector<AccountCallback>                    acc_cbs_;
        std::vector<PositionCallback>                   pos_cbs_;
    };
}

#endif //KUNGFU_PORTFOLIO_MANAGER_HPP