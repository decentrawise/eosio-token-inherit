/**
 *  @file
 *  @copyright (c) 2019 Copyright Transledger inc. All rights reserved
**/

#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

#include <string>

namespace eosiosystem
{
class system_contract;
}

namespace eosio
{
using std::string;

class[[eosio::contract]] BasicToken : public contract
{
public:
    using contract::contract;

    /**
    * Create action.
    *
    * @details Allows `issuer` account to create a token in supply of `maximum_supply`.
    * @param issuer - the account that creates the token,
    * @param maximum_supply - the maximum supply set for the token created.
    *
    * @pre Token symbol has to be valid,
    * @pre Token symbol must not be already created,
    * @pre maximum_supply has to be smaller than the maximum supply allowed by the system: 1^62 - 1.
    * @pre Maximum supply must be positive;
    *
    * If validation is successful a new entry in statstable for token symbol scope gets created.
    */
    [[eosio::action]] void create(name issuer,
                                  asset maximum_supply);

    /**
    * Issue action.
    *
    * @details This action issues to `to` account a `quantity` of tokens.
    *
    * @param to - the account to issue tokens to, it must be the same as the issuer,
    * @param quntity - the amount of tokens to be issued,
    * @memo - the memo string that accompanies the token issue transaction.
    */
    [[eosio::action]] void issue(name to, asset quantity, string memo);

    /**
    * Retire action.
    *
    * @details The opposite for create action, if all validations succeed,
    * it debits the statstable.supply amount.
    *
    * @param quantity - the quantity of tokens to retire,
    * @param memo - the memo string to accompany the transaction.
    */
    [[eosio::action]] void retire(asset quantity, string memo);

    /**
    * Transfer action.
    *
    * @details Allows `from` account to transfer to `to` account the `quantity` tokens.
    * One account is debited and the other is credited with quantity tokens.
    *
    * @param from - the account to transfer from,
    * @param to - the account to be transferred to,
    * @param quantity - the quantity of tokens to be transferred,
    * @param memo - the memo string to accompany the transaction.
    */
    [[eosio::action]] void transfer(name from,
                                    name to,
                                    asset quantity,
                                    string memo);


    [[eosio::action]] void transferfrom(name from,
                                        name to,
                                        name spender,
                                        asset quantity,
                                        string memo);

    [[eosio::action]] void approve(name owner,
                                   name spender,
                                   asset quantity);

    /**
    * Open action.
    *
    * @details Allows `ram_payer` to create an account `owner` with zero balance for
    * token `symbol` at the expense of `ram_payer`.
    *
    * @param owner - the account to be created,
    * @param symbol - the token to be payed with by `ram_payer`,
    * @param ram_payer - the account that supports the cost of this action.
    *
    * More information can be read [here](https://github.com/EOSIO/eosio.contracts/issues/62)
    * and [here](https://github.com/EOSIO/eosio.contracts/issues/61).
    */
    [[eosio::action]] void open(name owner, const symbol &symbol, name ram_payer);

    /**
    * Close action.
    *
    * @details This action is the opposite for open, it closes the account `owner`
    * for token `symbol`.
    *
    * @param owner - the owner account to execute the close action for,
    * @param symbol - the symbol of the token to execute the close action for.
    *
    * @pre The pair of owner plus symbol has to exist otherwise no action is executed,
    * @pre If the pair of owner plus symbol exists, the balance has to be zero.
    */
    [[eosio::action]] void close(name owner, const symbol &symbol);

    /**
    * Get supply method.
    *
    * @details Gets the supply for token `sym_code`, created by `token_contract_account` account.
    *
    * @param token_contract_account - the account to get the supply for,
    * @param sym_code - the symbol to get the supply for.
    */
    static asset get_supply(name token_contract_account, symbol_code sym_code)
    {
        stats statstable(token_contract_account, sym_code.raw());
        const auto &st = statstable.get(sym_code.raw());
        return st.supply;
    }

    /**
    * Get max supply method.
    *
    * @details Gets the max supply for token `sym_code`, created by `token_contract_account` account.
    *
    * @param token_contract_account - the account to get the supply for,
    * @param sym_code - the symbol to get the supply for.
    */
    static asset get_maxsupply(name token_contract_account, symbol_code sym_code)
    {
        stats statstable(token_contract_account, sym_code.raw());
        const auto &st = statstable.get(sym_code.raw());
        return st.max_supply;
    }

    /**
    * Get balance method.
    *
    * @details Get the balance for a token `sym_code` created by `token_contract_account` account,
    * for account `owner`.
    *
    * @param token_contract_account - the token creator account,
    * @param owner - the account for which the token balance is returned,
    * @param sym_code - the token for which the balance is returned.
    */
    static asset get_balance(name token_contract_account, name owner, symbol_code sym_code)
    {
        accounts accountstable(token_contract_account, owner.value);
        const auto &ac = accountstable.get(sym_code.raw());
        return ac.balance;
    }

    using create_action = eosio::action_wrapper<"create"_n, &BasicToken::create>;
    using issue_action = eosio::action_wrapper<"issue"_n, &BasicToken::issue>;
    using retire_action = eosio::action_wrapper<"retire"_n, &BasicToken::retire>;
    using transfer_action = eosio::action_wrapper<"transfer"_n, &BasicToken::transfer>;
    using transferfrom_action = eosio::action_wrapper<"transferfrom"_n, &BasicToken::transferfrom>;
    using approve_action = eosio::action_wrapper<"approve"_n, &BasicToken::approve>;
    using open_action = eosio::action_wrapper<"open"_n, &BasicToken::open>;
    using close_action = eosio::action_wrapper<"close"_n, &BasicToken::close>;

protected:
    struct [[eosio::table]] account
    {
        asset balance;

        uint64_t primary_key() const { return balance.symbol.code().raw(); }
    };

    struct [[eosio::table]] currency_stats
    {
        asset supply;
        asset max_supply;
        name issuer;

        uint64_t primary_key() const { return supply.symbol.code().raw(); }
    };

    struct [[eosio::table]] allowed_struct
    {
        uint64_t key;
        name spender;
        asset quantity;

        uint64_t primary_key() const { return key; }
    };

    typedef eosio::multi_index<"accounts"_n, account> accounts;
    typedef eosio::multi_index<"stat"_n, currency_stats> stats;
    typedef eosio::multi_index<"allowed"_n, allowed_struct> allowed;

    void sub_balance(name owner, asset value);
    void sub_balancefrom(name owner, name spender, asset value);
    void add_balance(name owner, asset value, name ram_payer);
};

} // namespace eosio
