## "# slucore"  

Silubium公链第二代SLU，于新加坡时间2018-07-28 17:33:01启动产生创世块。
SLU是一条集多种开放功能于一体的公链，可以实现基于UTXO安全模型的智能合约，并创新了POS共识协议新机制SILKWORM，发明了数据上链协议SILKSCREEN。主要特点有：
1. 比特币网络是目前最大的一个区块链技术的生态系统，以太坊第一次将智能合约的概念从理论变成实际，拓展了区块链技术的边界。SLU从设计之初就充分考虑了兼容性，在SLU上拥有比特币所有特性，基于EVM的智能合约可以直接在SLU上部署运行。
2. 与比特币和以太坊采用POW机制挖矿不一样，SLU提出了SILKWORM共识机制。SILKWORM在POS基础上进行了创新：
（1）以用户持有的UTXO进行挖矿，UTXO不论大小都有机会获得挖矿资格，这种方式大大减小了POW机制下的能源浪费；
（2）区块打包成功后，矿工会获得区块内所有交易的手续费，另外获得1SLU区块奖励，手续费和区块奖励会分5次发放，第1次及时发放，后4次在后续101-104个区块发放，这种方式提高了潜在攻击者的成本，降低了安全隐患；
（3）UTXO在获得区块打包资格时，SLU会判断持有期是否超过30天，若超过按0.58%月利息发放，但最多不超过360天，这种方式鼓励SLU持有者在线并获得奖励，在线节点越多SLU越稳定、安全。
3. 区块链除了具备价值传递的功能外，其去中心化、不可篡改的等特性更能解决在实体经济具体场景一些痛点，SLU的区块大小增至8M，数据直接上链1024B，智能合约承载最大量32M，并发明了数据上链协议SILKSCREEN：
（1）上链数据参数通过智能合约设定，可以设置是否加密、是否授权第三方查看；
（2）约定了数据封装格式；
（3）设计了配套的数据上链和查询接口。
4. SLU设计了低交易成本机制，SLU直接转帐按转帐金额比例计算0.00005%-0.00001%，交易量越大费率越低，并限定了最小值为0.0001SLU，最大值为0.5SLU。智能合约调用免费，发送操作需消耗少量GAS，最低GAS价格为0.00000010SLU。

### 官网地址：http://www.silubium.org 

### 区块浏览器地址：https://silkchain.silubium.org 

### 钱包下载地址：http://update.silubium.org 
