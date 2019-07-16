#ifndef SILUBIUMDGP_H
#define SILUBIUMDGP_H

#include <silubium/silubiumstate.h>
#include <primitives/block.h>
#include <validation.h>
#include <utilstrencodings.h>

static const dev::Address GasScheduleDGP = dev::Address("0000000000000000000000000000000000000080");
static const dev::Address BlockSizeDGP = dev::Address("0000000000000000000000000000000000000081");
static const dev::Address GasPriceDGP = dev::Address("0000000000000000000000000000000000000082");
static const dev::Address DGPCONTRACT4 = dev::Address("0000000000000000000000000000000000000083");
static const dev::Address BlockGasLimitDGP = dev::Address("0000000000000000000000000000000000000084");
//static const dev::Address AddressListDGP = dev::Address("0000000000000000000000000000000000000085");//dev::Address("b81f9da23f834a49e1a60fb1913339e4146c5ab8");
static const dev::Address AddressMineMain=dev::Address("3615db4ff9925bd8105a47dc490bf000ef958a7f");//dev::Address("5a9435bcb013ec4a3e8850a94560b1c21ee676c1");//dev::Address("9e3589f1083175bc6e146b07be40981cd625445d");
static const dev::Address AddressMineTest=dev::Address("d8c2882b8512d5db371f4cdcca74e181d020811e");//2019.4.8

static const uint32_t MIN_BLOCK_SIZE_DGP = 8000000;//500000;
static const uint32_t MAX_BLOCK_SIZE_DGP = 32000000;
static const uint32_t DEFAULT_BLOCK_SIZE_DGP = 8000000;//2000000;

static const uint64_t MIN_MIN_GAS_PRICE_DGP = 1;
static const uint64_t MAX_MIN_GAS_PRICE_DGP = 10000;
static const uint64_t DEFAULT_MIN_GAS_PRICE_DGP = 10;//40;

static const uint64_t MIN_BLOCK_GAS_LIMIT_DGP = 1000000;
static const uint64_t MAX_BLOCK_GAS_LIMIT_DGP = 1000000000;
static const uint64_t DEFAULT_BLOCK_GAS_LIMIT_DGP = 40000000;

class SilubiumDGP {
    
public:

    SilubiumDGP(SilubiumState* _state, bool _dgpevm = true) : dgpevm(_dgpevm), state(_state) { initDataEIP158(); }

    dev::eth::EVMSchedule getGasSchedule(unsigned int blockHeight);

    uint32_t getBlockSize(unsigned int blockHeight);

    uint64_t getMinGasPrice(unsigned int blockHeight);

    uint64_t getBlockGasLimit(unsigned int blockHeight);

    bool isHaveContract(unsigned int blockHeight);
    std::string getMainNodeAddress(unsigned int blockHeight);
    uint32_t getMineTime(unsigned int blockHeight);
    uint32_t getMineTimeMax(unsigned int blockHeight);
private:

    dev::Address getParamAddress(unsigned int blockHeight);

    bool initStorages(const dev::Address& addr, unsigned int blockHeight, std::vector<unsigned char> data = std::vector<unsigned char>());

    void initStorageDGP(const dev::Address& addr);

    void initStorageTemplate(const dev::Address& addr);

    void initDataTemplate(const dev::Address& addr, std::vector<unsigned char>& data);

    void initDataEIP158();

    bool checkLimitSchedule(const std::vector<uint32_t>& defaultData, const std::vector<uint32_t>& checkData);

    void createParamsInstance();

    dev::Address getAddressForBlock(unsigned int blockHeight);

    uint64_t getUint64FromDGP(unsigned int blockHeight, const dev::Address& contract, std::vector<unsigned char> data);

    void parseStorageScheduleContract(std::vector<uint32_t>& uint32Values);
    
    void parseDataScheduleContract(std::vector<uint32_t>& uint32Values);

    void parseStorageOneUint64(uint64_t& blockSize);

    void parseDataOneUint64(uint64_t& value);

    dev::eth::EVMSchedule createEVMSchedule();

    void clear();    



    bool dgpevm;

    const SilubiumState* state;

    dev::Address templateContract;

    std::map<dev::h256, std::pair<dev::u256, dev::u256>> storageDGP;

    std::map<dev::h256, std::pair<dev::u256, dev::u256>> storageTemplate;

    std::vector<unsigned char> dataTemplate;

    std::vector<std::pair<unsigned int, dev::Address>> paramsInstance;

    std::vector<uint32_t> dataEIP158Schedule;

    uint32_t silkworm_blockSize=DEFAULT_BLOCK_SIZE_DGP;



    uint64_t silkworm_minGasPrice=DEFAULT_MIN_GAS_PRICE_DGP;



    uint32_t silkworm_minheight=10050;
    dev::Address silkworm_paramcontractaddr=dev::Address("0000000000000000000000000000000000000000");
    std::string silkworm_mainnodeaddress="0000000000000000000000000000000000000000";
    uint32_t silkworm_minetime=2;
    uint32_t silkworm_minetimemax=600;

    bool bFindAddress=false;
    std::string getAddress(std::string strFunction);
    uint32_t getHeight(std::string strFunction);
};
#endif
