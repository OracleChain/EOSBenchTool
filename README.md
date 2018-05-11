# 关于/About

**EOSBenchTool**是一个跨平台PC端EOS节点压力测试工具，由[OracleChain团队](https://oraclechain.io)研发。

**EOSBenchTool** is a cross-platform EOS PC benchmark testing tool, which is developed by [OracleChain.io](https://oraclechain.io).

------------------------------

# 目录/Menu
* [简介/Overview](#1)
* [开发和使用环境/Environment](#2)
* [功能介绍/Functions](#3)
* [有关欧链/About OracleChain](#4)
* [版权/Liecnse](#4)

------------------------------

<h2 id="1">简介/Overview</h2>

EOSBenchTool是一个建立在EOSIO生态上的节点压力测试工具。它提供了基础的账户创建，转账等功能，用于测试节点的TPS(每秒处理交易)。测试人员可以使用EOSBenchTool连接EOSIO网络，对EOS的节点进行压力测试。

The EOSBenchTool program is a benchmark testing tool build on EOSIO ecology. It provides create account, transfer functions, and etc, to test the TPS(transactions per second) of EOS node. Testers can use this tool connect the EOSIO testnet, and test the EOS node's performance.


------------------------------
<h2 id="2">开发和使用环境/ENVIRONMENT</h2>

**开发运行环境/ENVIRONMENT：**

> MACOS、WINDOWS、UBUNTU

> [QT](https://www.qt.io/download) >= 5.8(使用QT导入项目，编译即可/just need to import and build the project with [QT](https://www.qt.io/download))

**依赖库/DEPENDENCYS:**

> 1.椭圆曲线算法实现来自micro-ecc

> our ECDSA is based on micro-ecc

> https://github.com/kmackay/micro-ecc

> 2.我们在椭圆曲线secp256k1上实现了Schoenmakers在crypto99上提出的Publicly Verifiable Secret Sharing

> We build a Publicly Verifiable Secret Sharing on secp256k1 which is published by Schoenmakers on crypto99 conference.

> https://github.com/songgeng87/PubliclyVerifiableSecretSharing


**相关问题反馈，请加欧链官方Telegram群组/Any questions pls join our official Telegram Group below**

> 中文群：https://t.me/OracleChainChatCN

> ENGLISH GROUP：https://t.me/OracleChainChat


<h2 id="3">功能介绍/Functions</h2>

### 设置/Settings
![](https://github.com/OracleChain/EOSBenchTool/blob/master/screenshots/setting.PNG)

### 测试/Test
![](https://github.com/OracleChain/EOSBenchTool/blob/master/screenshots/testing.png)

------------------------------
<h2 id="4">有关欧链/About OracleChain</h2>

OracleChain（欧链）作为全球第一个直面区块链生态Oracle（预言机）需求的基础应用，将区块链技术服务和现实生活中的多种需求场景直接高效对接，深耕这个百亿美金估值的巨大市场。

As the world’s first application built on an EOS ecosphere, OracleChain needs to meet the demands of the Oracle (oracle machine) ecosystem by efficiently linking blockchain technology services with various real-life scenarios, thereby delving into this immense tens of billions of dollars valuation market.

OracleChain是一个多区块链的去中心化Oracle技术平台，采用自主的PoRD机制，将现实世界数据引入区块链，并将此作为基础设施为其他区块链应用提供服务。
OracleChain将在区块链内提供现实世界数据的Oracle服务，同时还可以提供跨链数据的Oracle服务。基于OracleChain除了能实现Augur、Gnosis等预测市场（Prediction Market）应用的功能之外，还能支撑对链外数据有更高频率访问需求的智能合约业务，比如智能投顾等场景。

As a decentralized Oracle technology platform based on the EOS platform, the autonomous Proof-of-Reputation & Deposit mechanism is adopted and used as a fundamental service for other blockchain applications.In addition to Oracle services that provide real-world data to the blockchain, Oracle services that provide cross-chain data are also offered. Given that OracleChain can accomplish the functions of several prediction market applications, such as Augur and Gnosis, OracleChain can also support smart contract businesses that require high-frequency access to outside data in certain scenarios, such as Robo-Advisor.

OracleChain将改变当前区块链应用的开发模式，建立全新的生态圈，服务于真正能改变现实世界的区块链应用。

OracleChain will nurture and serve those blockchain applications that change the real world. Our mission is to “Link Data, Link World,” with the aim of becoming the infrastructure linking the real world with the blockchain world.

OracleChain的使命是“让世界与区块链互联”，立志成为链接现实世界与区块链世界的基础设施，通过把外部数据引入区块链来实现链内链外的数据互通，OracleChian将是未来区块链世界中最高效的获取链外数据的服务提供平台。

By achieving intra-chain and extra-chain data connectivity, we aspire to create a service provisioning platform that can most efficiently gain access to extra-chain data in the future blockchain world.

<h2 id="5">版权/LICENSE</h2>

**License**

Released under GNU/LGPL Version 3
