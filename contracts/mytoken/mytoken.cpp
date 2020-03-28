#include "mytoken.hpp"

// NOTE: this is only needed because of eoslime not allowing to have multiple cpp files with contracts
// and linking them together to the same contract. It just thinks it's separate contracts.
// Using CMake this is not needed as we can specify what files to link for the final contract.
//
// Switch between base eosio::token and eosio::BasicToken (transledger)
#include "../../thirdparty/eosio.token/src/eosio.token.cpp"
// #include "../../thirdparty/transledger/src/basiccontract.cpp"


// overrides base token action
void mytoken::issue(eosio::name to, eosio::asset quantity, std::string memo)
{
    auto sym = quantity.symbol;
    stats statstable( get_self(), sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    eosio::check( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
    const auto& st = *existing;

    // perform base token issue
    BaseToken::issue(st.issuer, quantity, memo);

    if( to != st.issuer ) {
        eosio::action( eosio::permission_level{ get_self(), "active"_n },
            get_self(),
            "transfer"_n,
            std::make_tuple()
        ).send();
    }
}
