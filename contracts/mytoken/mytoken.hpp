#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>

// Switch between base eosio::token and eosio::BasicToken (transledger)
#include "../../thirdparty/eosio.token/include/eosio.token/eosio.token.hpp"
// #include "../../thirdparty/transledger/include/transledger/basiccontract.hpp"

#include <string>


// Switch between base eosio::token and eosio::BasicToken (transledger)
typedef typename eosio::token BaseToken;
// typedef typename eosio::BasicToken BaseToken;


CONTRACT mytoken : public BaseToken {

public:
   mytoken(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds) : BaseToken(receiver, code, ds) {}

   // overrides base token action
   ACTION issue(eosio::name to, eosio::asset quantity, std::string memo);

   // NOTE: Without these all tests fail, as if these methods were not present in the WASM interface.
   // But with these, the ABI is generated with all methods still lacking the tables, but the test of the overriden
   // method fails, as if only the base method was included, not the derived one.
   //
   // base token functions for ABI generator to be aware of
   ACTION create(eosio::name issuer, eosio::asset maximum_supply) { BaseToken::create(issuer, maximum_supply); }
   ACTION retire(eosio::asset quantity, std::string memo) { BaseToken::retire(quantity, memo); }
   ACTION open(eosio::name owner, const eosio::symbol &symbol, eosio::name ram_payer) { BaseToken::open(owner, symbol, ram_payer); }
   ACTION close(eosio::name owner, const eosio::symbol &symbol) { BaseToken::close(owner, symbol); }
   // Only for Transledger
   // ACTION transferfrom(eosio::name from, eosio::name to, eosio::name spender, eosio::asset quantity, std::string memo) { BaseToken::transferfrom(from, to, spender, quantity, memo); }
   // ACTION approve(eosio::name owner, eosio::name spender, eosio::asset quantity) { BaseToken::approve(owner, spender, quantity); }

};

EOSIO_DISPATCH( mytoken, (open)(create)(issue)(retire)(close) )
