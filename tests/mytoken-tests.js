const assert = require('assert');


const TOKEN_WASM_PATH = './compiled/mytoken.wasm';

// Switch between base eosio::token and eosio::BasicToken (transledger)
const TOKEN_ABI_PATH = './contracts/mytoken/mytoken-eosio.token.abi';
// const TOKEN_ABI_PATH = './contracts/mytoken/mytoken-transledger.abi';


describe('Inherit from EOSIO Token', function (eoslime) {

    // Increase mocha(testing framework) time, otherwise tests fails
    this.timeout(15000);

    let tokenContract;
    let tokensIssuer;
    let tokensHolder;

    const TOTAL_SUPPLY = '1000000000.0000 SYS';
    const HOLDER_SUPPLY = '100.0000 SYS';

    before(async () => {
        let accounts = await eoslime.Account.createRandoms(2);
        tokensIssuer = accounts[0];
        tokensHolder = accounts[1];
    });

    beforeEach(async () => {
        /*
            `deploy` creates for you a new account behind the scene
            on which the contract code is deployed

            You can access the contract account as -> tokenContract.executor
        */
        tokenContract = await eoslime.Contract.deploy(TOKEN_WASM_PATH, TOKEN_ABI_PATH, { inline: true });
    });

    it('Should create a new token', async () => {
        await tokenContract.create(tokensIssuer.name, TOTAL_SUPPLY);

        /*
            You have access to the EOS(eosjs) instance:
                eoslime.Provider.eos
        */
        let tokenInitialization = await tokenContract.provider.eos.getCurrencyStats(tokenContract.name, 'SYS');

        assert.equal(tokenInitialization.SYS.max_supply, TOTAL_SUPPLY, 'Incorrect tokens supply');
        assert.equal(tokenInitialization.SYS.issuer, tokensIssuer.name, 'Incorrect tokens issuer');
    });

    it('Should issue tokens to issuer', async () => {
        await tokenContract.create(tokensIssuer.name, TOTAL_SUPPLY);
        await tokenContract.issue(tokensIssuer.name, HOLDER_SUPPLY, 'memo', { from: tokensIssuer });

        let holderBalance = await tokensIssuer.getBalance('SYS', tokenContract.name);
        assert.equal(holderBalance[0], HOLDER_SUPPLY, 'Incorrect holder balance');
    });

    it('Should issue tokens to another party', async () => {
        await tokenContract.create(tokensIssuer.name, TOTAL_SUPPLY);
        await tokenContract.issue(tokensHolder.name, HOLDER_SUPPLY, 'memo', { from: tokensIssuer });

        let holderBalance = await tokensHolder.getBalance('SYS', tokenContract.name);
        assert.equal(holderBalance[0], HOLDER_SUPPLY, 'Incorrect holder balance');
    });

    it('Should throw if tokens quantity is negative', async () => {
        await tokenContract.create(tokensIssuer.name, TOTAL_SUPPLY);
        const INVALID_ISSUING_AMOUNT = '-100.0000 SYS';

        /*
            eoslime provides you testing utils (Available only if testing with `eoslime test`)
        */
        await eoslime.tests.expectAssert(
            tokenContract.issue(tokensHolder.name, INVALID_ISSUING_AMOUNT, 'memo', { from: tokensIssuer })
        );

        let holderBalance = await tokensHolder.getBalance('SYS', tokenContract.name);
        assert.equal(holderBalance.length, 0, 'Incorrect holder balance');
    });
});