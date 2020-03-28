# eosio-token-inherit

**This project is just for demostration of a contract inheritance problem in EOSIO - please do not use!** 

## What we found

As C++ is the object oriented programming (OOP) language of excellence, and EOSIO a C++ smart-contract platform, we think that it should support the basis principles of OOP, meaning: encapsulation, abstraction, **inheritance** and polymorphism.

Inheritance is a very important principle of OOP, as it enables the developer to evolve code based on other parts already exixting, without having to copy and paste code around.

EOSIO.CDT tools actually support inheritance, but lack support of it in the contract interface layer, meaning that it is impossible to inherit a contract from another, as it is possible in other platforms like Ethereum.

We find this a very important lacune in the EOSIO platform that we want to clarify with this project, showcasing how an inherited contract could look like and what are the problems and limitations of EOSIO platform in this regard.

## How to reproduce the problems

In this example we have a `mytoken` contract that can inherit from `eosio.token` or Transledger's `basiccontract`. This `mytoken` contract only overrides the issue method, allowing the issuer to issue tokens to another thirdparty, instead of only for himself, as per the standard implementations.

To implement this we could only change the `private` part of the base contracts to `protected` to make those parts of the base contract accessible to derived classes.

This seems as a really basic thing in C++ OOP, but it just doesn't work in EOSIO platform.

##### Problems found:

1. The ABI generator doesn't generate the inherited actions or tables.
1. To make it generate them, we can use [[eosio::contract("mycontract")]] (notice the contract name) on the base contract, and then the ABI is generated correctly, but the overriden method will not work, being available only the base method.
1. If we remove the above change and add some bypass methods on the derived contract, then the ABI is falty but it compiles ok and generates the correct WASM interface methods, but still only the base method is working, not the derived one.

The included project showcases all these problems and how to reproduce them, please check the file tree below and follow the comments in the several files...

## Conclusions

We kindly urge the EOSIO community to check this and to put some effort in solving this issue, as we find this very important for the EOSIO developer community to be able to compete with Ethereum, as they can rely on code audits of base contracts and only audit the derived modifications, which is very significant in terms of security efforts for projects. In EOSIO we have a better architecture all around, but this is very limiting for the projects to evolve in a safe and cost effective way.

Please prioritize this as a very important issue to be resolved! Reported in EOSIO.CDT issue [EOSIO Contract Inheritance not working #816](https://github.com/EOSIO/eosio.cdt/issues/816)

## File tree

- src/ - source directory that contains `config.hpp` main project code file

- contracts/ - test contract directory

- tests/ - tests specs

- scripts/ - helper scripts for running nodeos locally

## Install EOSLime
```bash
$ npm install -g eoslime
```

## Compile the example contract
```bash
$ eoslime compile
```

## Run a local EOSIO node
```bash
$ ./scripts/nodeos.sh
```
**NOTE**: Please customize the script to your local development needs. This might be made
easier in the future with configuration and a better script...

## Run tests
```bash
$ eoslime test
```
