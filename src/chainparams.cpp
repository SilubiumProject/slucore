// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>
#include <consensus/consensus.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>

#include <chainparamsseeds.h>

///////////////////////////////////////////// // silubium
#include <libdevcore/SHA3.h>
#include <libdevcore/RLP.h>
#include "arith_uint256.h"
/////////////////////////////////////////////

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 00 << 488804799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    genesis.hashStateRoot = uint256(h256Touint(dev::h256("e965ffd002cd6ad0e2dc402b8044de833e06b23127ea8c3d80aec91410771495"))); // silubium
    genesis.hashUTXORoot = uint256(h256Touint(dev::sha3(dev::rlp("")))); // silubium
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "The silubium second edition(SLU) software released. The price is $0.09055";
    const CScript genesisOutputScript = CScript() << ParseHex("040d61d8653448c98731ee5fffd303c15e71ec2057b77f11ab3601979728cdaff2d68afbba14e4fa0bc44f2072b0b23ef63717f8cdfbe58dcd33f32b6afe98741a") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 985500; // silubium halving every 4 years
        consensus.BIP16Exception = uint256S("0x000075aef83cf2853580f8ae8ce6f8c3096cfa21d98334d6e3f95e5582ed986c");
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x000075aef83cf2853580f8ae8ce6f8c3096cfa21d98334d6e3f95e5582ed986c");
        consensus.BIP65Height = 0; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.BIP66Height = 0; // 00000000000000000379eaa19dce8c9b722d46ae6a57c2f1a988119488b50931
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 16 * 60; // 16 minutes
        consensus.nPowTargetSpacing = 2 * 64;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = true;
        consensus.fPoSNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0");//uint256S("0x000000000000000000000000000000000000000000000036a70ae675b98824c5"); // silubium

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x739f1472b1725adcd493fc5ccefa5fec81066f4dcbf18c9e7ac4f217ef98def5"); // 363604

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xce;//0xf1;
        pchMessageStart[1] = 0xf2;//0xcf;
        pchMessageStart[2] = 0xa7;//0xa6;
        pchMessageStart[3] = 0xd4;//0xd3;
        nDefaultPort = 5906;//3888;
        nPruneAfterHeight = 100000;

        //genesis = CreateGenesisBlock(1504695029,8026361, 0x1f00ffff, 1, 50 * COIN);
        genesis = CreateGenesisBlock(1532770381,35869 , 0x1f00ffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
	assert(consensus.hashGenesisBlock == uint256S("0x00001d10d5f89e792a149a66d48a89037f8c915c29f2cfd4f251cd2bab216b3b"));
        assert(genesis.hashMerkleRoot == uint256S("0xb105667bffbd7e59f2f0c49e4386438caef9376b9292e119b216b04346064898"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        //vSeeds.emplace_back("silubium3.dynu.net"); // Silubium mainnet
        //vSeeds.emplace_back("silubium5.dynu.net"); // Silubium mainnet
        //vSeeds.emplace_back("silubium6.dynu.net"); // Silubium mainnet
        //vSeeds.emplace_back("silubium7.dynu.net"); // Silubium mainnet
        vSeeds.emplace_back("slunode1.silubium.org");
        vSeeds.emplace_back("slunode2.silubium.org");
        vSeeds.emplace_back("slunode3.silubium.org");
        vSeeds.emplace_back("slunode4.silubium.org");
        vSeeds.emplace_back("slunode5.silubium.org");
        vSeeds.emplace_back("slunode6.silubium.org");
        vSeeds.emplace_back("slunode7.silubium.org");
        vSeeds.emplace_back("slunode8.silubium.org");
        vSeeds.emplace_back("slunode9.silubium.org");
        vSeeds.emplace_back("slunode10.silubium.org");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,68);//o:115  U:68 Q:58;
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,130);//std::vector<unsigned char>(1,50);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "qc";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
        	{0,uint256S("00001d10d5f89e792a149a66d48a89037f8c915c29f2cfd4f251cd2bab216b3b")},
        	{15000,uint256S("b85f95a372c8b03beea7d481c550a8a7efd5022df39ec265a7aa79ffbe83dc22")},
            {194830,uint256S("2877df13618eda388ea4c477394927b31e995a066b95e4ef6688c148a4bd824a")},
                //{ 0, uint256S("000075aef83cf2853580f8ae8ce6f8c3096cfa21d98334d6e3f95e5582ed986c")},
                //{ 5000, uint256S("00006a5338e5647872bd91de1d291365e941e14dff1939b5f16d1804d1ce61cd")}, //last PoW block
                //{ 45000, uint256S("060c6af680f6975184c7a17059f2ff4970544fcfd4104e73744fe7ab7be14cfc")},
                //{ 90000, uint256S("66fcf426b0aa6f2c9e3330cb2775e9e13c4a2b8ceedb50f8931ae0e12078ad50")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block a1bab8db27f26952ce94fff6563931943554e36fc3a23f99cc8513270d685b2c (height 92662)
            0,//0,//1517516912, // * UNIX timestamp of last known number of transactions
            0,//904678, // * total number of transactions between genesis and that timestamp
                            //   (the tx=... number in the SetBestChain debug.log lines)
            0//0.125 // * estimated number of transactions per second after that timestamp
        };

        /* disable fallback fee on mainnet */
        m_fallback_fee_enabled = false;

        consensus.nLastPOWBlock = 10000;
        consensus.nMPoSRewardRecipients = 5;//10;
        consensus.nFirstMPoSBlock = consensus.nLastPOWBlock + 
                                    consensus.nMPoSRewardRecipients + 
                                    COINBASE_MATURITY;

        consensus.nFixUTXOCacheHFHeight=100000;
        consensus.nEnableHeaderSignatureHeight = 399100;
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 985500; // silubium halving every 4 years
        consensus.BIP16Exception = uint256S("0x0000e803ee215c0684ca0d2f9220594d3f828617972aad66feb2ba51f5e14222");
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x0000e803ee215c0684ca0d2f9220594d3f828617972aad66feb2ba51f5e14222");
        consensus.BIP65Height = 0; // 00000000007f6655f22f98e72ed80d8b06dc761d5da09df0fa1dc4be4f861eb6
        consensus.BIP66Height = 0; // 000000002104c8c45e99a8853285a3b592602a3ccde2b832481da85e9e4ba182
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 16 * 60; // 16 minutes
        consensus.nPowTargetSpacing = 2 * 64;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = true;
        consensus.fPoSNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0");//uint256S("0x00000000000000000000000000000000000000000000000d6e528bcf5a3a556f"); // silubium

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x91dc4e3901b6f00a14db9d9bd43a1e8700510c24b692294fdf7fc0b53af76899"); // 320000

        pchMessageStart[0] = 0xad;//0x0d;
        pchMessageStart[1] = 0xf2;//0x22;
        pchMessageStart[2] = 0xa7;//0x15;
        pchMessageStart[3] = 0xd4;//0x06;
        nDefaultPort = 15906;//13888;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1532770711, 7523, 0x1f00ffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00007277f7e3dd31107c9033a619b8d5885ff5da8561979754cdf1cf74f87ee5"));
        assert(genesis.hashMerkleRoot == uint256S("0xb105667bffbd7e59f2f0c49e4386438caef9376b9292e119b216b04346064898"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        //        vSeeds.emplace_back("silubium4.dynu.net"); // Silubium testnet
        vSeeds.emplace_back("slunodetest.silubium.org");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,63);//std::vector<unsigned char>(1,120);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,125);//std::vector<unsigned char>(1,110);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tq";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = {
            {
        	{0,uint256S("00007277f7e3dd31107c9033a619b8d5885ff5da8561979754cdf1cf74f87ee5")},
            //{14000,uint256S("b8c71ac74402b782e10c6ae09a376358b0085a8509ab06cf72a5e65a57d18abb")},
                //{0, uint256S("0000e803ee215c0684ca0d2f9220594d3f828617972aad66feb2ba51f5e14222")},
                //{5000, uint256S("000000302bc22f2f65995506e757fff5c824545db5413e871d57d27a0997e8a0")}, //last PoW block
                //{77000, uint256S("f41e2e8d09bca38827c23cad46ed6d434902da08415d2314d0c8ce285b1970cb")},
            }
        };

        chainTxData = ChainTxData{
                // Data as of block 493cccf2ba87ffdabd7afc0f3242c1357fdebdc0b8c7e7adc3c6dc2b1c8ca797 (height 79167)
                0,//1517517152,
                0,//162802,
                0//0.0132
    };

        /* enable fallback fee on testnet */
        m_fallback_fee_enabled = true;

        consensus.nLastPOWBlock = 10000;//5000;
        consensus.nMPoSRewardRecipients = 5;
        consensus.nFirstMPoSBlock = consensus.nLastPOWBlock + 
                                    consensus.nMPoSRewardRecipients + 
                                    COINBASE_MATURITY;

        consensus.nFixUTXOCacheHFHeight=84500;
        consensus.nEnableHeaderSignatureHeight = 391993;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Exception = uint256S("0x665ed5b402ac0b44efc37d8926332994363e8a7278b7ee9a58fb972efadae943");
        consensus.BIP34Height = 0; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests) // activate for silubium
        consensus.BIP34Hash = uint256S("0x665ed5b402ac0b44efc37d8926332994363e8a7278b7ee9a58fb972efadae943");
        consensus.BIP65Height = 0; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 0; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 16 * 60; // 16 minutes
        consensus.nPowTargetSpacing = 2 * 64;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.fPoSNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0xdd;
        pchMessageStart[2] = 0xc6;
        pchMessageStart[3] = 0xe1;
        nDefaultPort = 23888;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1532770711, 7523,0x1f00ffff, 1, 50 * COIN);//CreateGenesisBlock(1504695029, 17,0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00007277f7e3dd31107c9033a619b8d5885ff5da8561979754cdf1cf74f87ee5"));
        assert(genesis.hashMerkleRoot == uint256S("0xb105667bffbd7e59f2f0c49e4386438caef9376b9292e119b216b04346064898"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {
            {
                {0, uint256S("00007277f7e3dd31107c9033a619b8d5885ff5da8561979754cdf1cf74f87ee5")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };
        consensus.nLastPOWBlock = 0x7fffffff;
        consensus.nMPoSRewardRecipients = 10;
        consensus.nFirstMPoSBlock = 10000;//5000;

        consensus.nFixUTXOCacheHFHeight=0;
        consensus.nEnableHeaderSignatureHeight = 0;

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,120);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,110);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "qcrt";

        /* enable fallback fee on regtest */
        m_fallback_fee_enabled = true;
    }
};

/**
 * Regression network parameters overwrites for unit testing
 */
class CUnitTestParams : public CRegTestParams
{
public:
    CUnitTestParams()
    {
        // Activate the the BIPs for regtest as in Bitcoin
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)

        // SILUBIUM have 500 blocks of maturity, increased values for regtest in unit tests in order to correspond with it
        consensus.nSubsidyHalvingInterval = 750;
        consensus.nRuleChangeActivationThreshold = 558; // 75% for testchains
        consensus.nMinerConfirmationWindow = 744; // Faster than normal for regtest (744 instead of 2016)
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    else if (chain == CBaseChainParams::UNITTEST)
        return std::unique_ptr<CChainParams>(new CUnitTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
