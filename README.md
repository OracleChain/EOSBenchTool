# About


**EOSBenchTool** is a PC cross-platform, EOS pressure testing tool, which is developed by [OracleChain.io](https://oraclechain.io).

------------------------------

# Menu
* [Overview](#1)
* [Environment](#2)
* [Functions](#3)
* [About OracleChain](#4)
* [Liecnse](#4)

------------------------------

<h2 id="1">Overview</h2>


The EOSBenchTool is a EOS pressure testing tool. 

EOSBenchTool prepares a batch of transactions to save the client's packaging, signature time, then use `push_transactions` to sends transactions to EOS node, using limited clients to maximize the pressure test on the server!


------------------------------
<h2 id="2">ENVIRONMENT</h2>

**ENVIRONMENT：**

&emsp;MACOS、WINDOWS、UBUNTU

&emsp;1. install [QT](https://www.qt.io/download) >= 5.8

&emsp;2. download our git repository.
>`git clone --recurse-submodules https://github.com/OracleChain/EOSBenchTool.git`

&emsp;3. open Qt Creator and choose "Open Project", select the "src/EOSBenchTool.pro" and open

&emsp;4. select Build from QT menu and Run it.


> For ubuntu, if build with errors like `connot find -lGL`, this means the openGL development library is missing.
Just using this command to install:
`sudo apt-get install build-essential libgl1-mesa-dev`

**DOWNLOAD & TRY**

|Version|MD5|
|------|---|
|[WINDOWS_v2.0(compliant with dawn-v4.0.0)](https://github.com/OracleChain/EOSBenchTool/releases/download/dawn-4.0/EOSBenchTool_Windows_Release_Dawn4.zip)|a4d46c5902d10991e241732bf3b20f28|
|[UBUNTU_v2.0(compliant with dawn-v4.0.0)](https://github.com/OracleChain/EOSBenchTool/releases/download/dawn-4.0/EOSBenchTool_Ubuntu_Release_Dawn4.zip)|f4704058a726422bb53cbb0207c7b268|
|[WINDOWS_v1.0(compliant with DAWN-2018-04-27-ALPHA)](https://github.com/OracleChain/EOSBenchTool/releases/download/1.0/EOSBenchTool_Windows_Release.zip)|eec548fc2d760e65bed9c0c508dd6e92|
|[UBUNTU_v1.0(compliant with DAWN-2018-04-27-ALPHA)](https://github.com/OracleChain/EOSBenchTool/releases/download/1.0/EOSBenchTool_Ubuntu_Release.zip)|b9eb6dc0c8e99eb6b9d3f8e7cf605111|

>For Ubuntu: Tested on Ubuntu 16.04 x64, you should `chmod +x start.sh`, then use `./start.sh` .

**DEPENDENCYS:**

> our ECDSA is based on micro-ecc

> https://github.com/kmackay/micro-ecc

> We build a Publicly Verifiable Secret Sharing on secp256k1 which is published by Schoenmakers on crypto99 conference.

> https://github.com/songgeng87/PubliclyVerifiableSecretSharing


**Any questions pls join our official Telegram Group below**

> 中文群：https://t.me/OracleChainChatCN

> ENGLISH GROUP：https://t.me/OracleChainChat


<h2 id="3">Functions</h2>

### Prepation
>First set contract, create token, issue token, and then use this tool to testing nodeos' performance.<br>
You can refer to [Tutorial eosio token Contract](https://github.com/EOSIO/eos/wiki/Tutorial-eosio-token-Contract) or using following command:<br>
`cleos -u http://127.0.0.1:8888/  set contract eosio.token ./eosio.token -p eosio.token`<br>
`cleos -u http://127.0.0.1:8888/   push action eosio.token create '{"issuer": "eosio", "maximum_supply": "100000000.0000 EOS", "can_freeze": 0, "can_recall": 0, "can_whitelist": 0}' -p eosio.token`<br>
`cleos -u http://127.0.0.1:8888/    push action eosio.token issue '[ "eosio", "100000000.0000 EOS", "m" ]' -p eosio`

### Settings
![](https://github.com/OracleChain/EOSBenchTool/blob/master/screenshots/setting.PNG)
* Host address <br>
 EOS node ip
* Port <br>
 EOS node port
* Thread number <br>
 Recommended to be your computer's CPU number.
* Transaction Pool Size, Total tokens <br>
 "Transaction Pool Size" * 0.0001(each transaction send 0.0001 token)="Total tokens"
* Transaction batch size <br>
 Transactions number in one `push_transactions`.

### Test
![](https://github.com/OracleChain/EOSBenchTool/blob/master/screenshots/testing.png)
>First switch `Settings` page to complete settings.
>Second switch to `Benchmark Testing` page, click `Prepare` button and wait for preparation processing complete, then click `Start` button to send prepared transactions, wait until the testing result shows.

> Max tps<br>
  The max tps(transactions per second) during testing duration.

>ATTENTION<br>
 You should always reopen EOSBenchTool to restart a new testing.

------------------------------
<h2 id="4">About OracleChain</h2>


As the world’s first application built on an EOS ecosphere, OracleChain needs to meet the demands of the Oracle (oracle machine) ecosystem by efficiently linking blockchain technology services with various real-life scenarios, thereby delving into this immense tens of billions of dollars valuation market.


As a decentralized Oracle technology platform based on the EOS platform, the autonomous Proof-of-Reputation & Deposit mechanism is adopted and used as a fundamental service for other blockchain applications.In addition to Oracle services that provide real-world data to the blockchain, Oracle services that provide cross-chain data are also offered. Given that OracleChain can accomplish the functions of several prediction market applications, such as Augur and Gnosis, OracleChain can also support smart contract businesses that require high-frequency access to outside data in certain scenarios, such as Robo-Advisor.


OracleChain will nurture and serve those blockchain applications that change the real world. Our mission is to “Link Data, Link World,” with the aim of becoming the infrastructure linking the real world with the blockchain world.


By achieving intra-chain and extra-chain data connectivity, we aspire to create a service provisioning platform that can most efficiently gain access to extra-chain data in the future blockchain world.

<h2 id="5">LICENSE</h2>

**License**

Released under GNU/LGPL Version 3
