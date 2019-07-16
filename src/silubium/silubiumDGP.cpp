#include <silubium/silubiumDGP.h>
#include <iostream>
#include <exception>
using namespace std;

void SilubiumDGP::initDataEIP158(){
    std::vector<uint32_t> tempData = {dev::eth::EIP158Schedule.tierStepGas[0], dev::eth::EIP158Schedule.tierStepGas[1], dev::eth::EIP158Schedule.tierStepGas[2],
                                      dev::eth::EIP158Schedule.tierStepGas[3], dev::eth::EIP158Schedule.tierStepGas[4], dev::eth::EIP158Schedule.tierStepGas[5],
                                      dev::eth::EIP158Schedule.tierStepGas[6], dev::eth::EIP158Schedule.tierStepGas[7], dev::eth::EIP158Schedule.expGas,
                                      dev::eth::EIP158Schedule.expByteGas, dev::eth::EIP158Schedule.sha3Gas, dev::eth::EIP158Schedule.sha3WordGas,
                                      dev::eth::EIP158Schedule.sloadGas, dev::eth::EIP158Schedule.sstoreSetGas, dev::eth::EIP158Schedule.sstoreResetGas,
                                      dev::eth::EIP158Schedule.sstoreRefundGas, dev::eth::EIP158Schedule.jumpdestGas, dev::eth::EIP158Schedule.logGas,
                                      dev::eth::EIP158Schedule.logDataGas, dev::eth::EIP158Schedule.logTopicGas, dev::eth::EIP158Schedule.createGas,
                                      dev::eth::EIP158Schedule.callGas, dev::eth::EIP158Schedule.callStipend, dev::eth::EIP158Schedule.callValueTransferGas,
                                      dev::eth::EIP158Schedule.callNewAccountGas, dev::eth::EIP158Schedule.suicideRefundGas, dev::eth::EIP158Schedule.memoryGas,
                                      dev::eth::EIP158Schedule.quadCoeffDiv, dev::eth::EIP158Schedule.createDataGas, dev::eth::EIP158Schedule.txGas,
                                      dev::eth::EIP158Schedule.txCreateGas, dev::eth::EIP158Schedule.txDataZeroGas, dev::eth::EIP158Schedule.txDataNonZeroGas,
                                      dev::eth::EIP158Schedule.copyGas, dev::eth::EIP158Schedule.extcodesizeGas, dev::eth::EIP158Schedule.extcodecopyGas,
                                      dev::eth::EIP158Schedule.balanceGas, dev::eth::EIP158Schedule.suicideGas, dev::eth::EIP158Schedule.maxCodeSize};
    dataEIP158Schedule = tempData;
}

bool SilubiumDGP::checkLimitSchedule(const std::vector<uint32_t>& defaultData, const std::vector<uint32_t>& checkData){
    if(defaultData.size() == 39 && checkData.size() == 39){
        for(size_t i = 0; i < defaultData.size(); i++){
            uint32_t max = defaultData[i] * 1000 > 0 ? defaultData[i] * 1000 : 1 * 1000;
            uint32_t min = defaultData[i] / 100 > 0 ? defaultData[i] / 100 : 1;
            if(checkData[i] > max || checkData[i] < min){
                return false;
            }
        }
        return true;
    }
    return false;
}

dev::eth::EVMSchedule SilubiumDGP::getGasSchedule(unsigned int blockHeight){
    clear();
    dev::eth::EVMSchedule schedule = dev::eth::EIP158Schedule;
    if(initStorages(GasScheduleDGP, blockHeight, ParseHex("26fadbe2"))){
        schedule = createEVMSchedule();
    }
    return schedule;
}

uint64_t SilubiumDGP::getUint64FromDGP(unsigned int blockHeight, const dev::Address& contract, std::vector<unsigned char> data){
    uint64_t value = 0;
    if(initStorages(contract, blockHeight, data)){
        if(!dgpevm){
            parseStorageOneUint64(value);
        } else {
            parseDataOneUint64(value);
        }
    }
    return value;
}

uint32_t SilubiumDGP::getBlockSize(unsigned int blockHeight){
    clear();
    uint32_t result = DEFAULT_BLOCK_SIZE_DGP;
    uint32_t blockSize = getUint64FromDGP(blockHeight, BlockSizeDGP, ParseHex("92ac3c62"));
    if(blockSize <= MAX_BLOCK_SIZE_DGP && blockSize >= MIN_BLOCK_SIZE_DGP){
        result = blockSize;
    }
    return result;
}

uint64_t SilubiumDGP::getMinGasPrice(unsigned int blockHeight){
    clear();
    uint64_t result = DEFAULT_MIN_GAS_PRICE_DGP;
    uint64_t minGasPrice = getUint64FromDGP(blockHeight, GasPriceDGP, ParseHex("3fb58819"));
    if(minGasPrice <= MAX_MIN_GAS_PRICE_DGP && minGasPrice >= MIN_MIN_GAS_PRICE_DGP){
        result = minGasPrice;
    }
    return result;
}

uint64_t SilubiumDGP::getBlockGasLimit(unsigned int blockHeight){
    clear();
    uint64_t result = DEFAULT_BLOCK_GAS_LIMIT_DGP;
    uint64_t blockGasLimit = getUint64FromDGP(blockHeight, BlockGasLimitDGP, ParseHex("2cc8377d"));
    if(blockGasLimit <= MAX_BLOCK_GAS_LIMIT_DGP && blockGasLimit >= MIN_BLOCK_GAS_LIMIT_DGP){
        result = blockGasLimit;
    }
    return result;
}

bool SilubiumDGP::initStorages(const dev::Address& addr, unsigned int blockHeight, std::vector<unsigned char> data){
    initStorageDGP(addr);
    createParamsInstance();
    dev::Address address = getAddressForBlock(blockHeight);
    if(address != dev::Address()){
        if(!dgpevm){
            initStorageTemplate(address);
        } else {
            initDataTemplate(address, data);
        }
        return true;
    }
    return false;
}

void SilubiumDGP::initStorageDGP(const dev::Address& addr){
    storageDGP = state->storage(addr);
}

void SilubiumDGP::initStorageTemplate(const dev::Address& addr){
    storageTemplate = state->storage(addr);
}

void SilubiumDGP::initDataTemplate(const dev::Address& addr, std::vector<unsigned char>& data){
    dataTemplate = CallContract(addr, data)[0].execRes.output;
}

void SilubiumDGP::createParamsInstance(){
    dev::h256 paramsInstanceHash = sha3(dev::h256("0000000000000000000000000000000000000000000000000000000000000000"));
    if(storageDGP.count(paramsInstanceHash)){
        dev::u256 paramsInstanceSize = storageDGP.find(paramsInstanceHash)->second.second;
        for(size_t i = 0; i < size_t(paramsInstanceSize); i++){
            std::pair<unsigned int, dev::Address> params;
            params.first = dev::toUint64(storageDGP.find(sha3(paramsInstanceHash))->second.second);
            ++paramsInstanceHash;
            params.second = dev::right160(dev::h256(storageDGP.find(sha3(paramsInstanceHash))->second.second));
            ++paramsInstanceHash;
            paramsInstance.push_back(params);
        }
    }
}

dev::Address SilubiumDGP::getAddressForBlock(unsigned int blockHeight){
    for(auto i = paramsInstance.rbegin(); i != paramsInstance.rend(); i++){
        if(i->first <= blockHeight)
            return i->second;
    }
    return dev::Address();
}

static inline bool sortPairs(const std::pair<dev::u256, dev::u256>& a, const std::pair<dev::u256, dev::u256>& b){
    return a.first < b.first;
}

void SilubiumDGP::parseStorageScheduleContract(std::vector<uint32_t>& uint32Values){
    std::vector<std::pair<dev::u256, dev::u256>> data;
    for(size_t i = 0; i < 5; i++){
        dev::h256 gasScheduleHash = sha3(dev::h256(dev::u256(i)));
        if(storageTemplate.count(gasScheduleHash)){
            dev::u256 key = storageTemplate.find(gasScheduleHash)->second.first;
            dev::u256 value = storageTemplate.find(gasScheduleHash)->second.second;
            data.push_back(std::make_pair(key, value));
        }
    }

    std::sort(data.begin(), data.end(), sortPairs);

    for(std::pair<dev::u256, dev::u256> d : data){
        dev::u256 value = d.second;
        for(size_t i = 0; i < 4; i++){
            uint64_t uint64Value = dev::toUint64(value);
            value = value >> 64;

            uint32Values.push_back(uint32_t(uint64Value));
            uint64Value = uint64Value >> 32;
            uint32Values.push_back(uint32_t(uint64Value));
        }
    }
}

void SilubiumDGP::parseDataScheduleContract(std::vector<uint32_t>& uint32Values){
    size_t size = dataTemplate.size() / 32;
    for(size_t i = 0; i < size; i++){
        std::vector<unsigned char> value = std::vector<unsigned char>(dataTemplate.begin() + (i * 32), dataTemplate.begin() + ((i+1) * 32));
        dev::h256 valueTemp(value);
        uint32Values.push_back(dev::toUint64(dev::u256(valueTemp)));
    }
}

void SilubiumDGP::parseStorageOneUint64(uint64_t& value){
    dev::h256 blockSizeHash = sha3(dev::h256(dev::u256(0)));
    if(storageTemplate.count(blockSizeHash)){
        value = dev::toUint64(storageTemplate.find(blockSizeHash)->second.second);
    }
}

void SilubiumDGP::parseDataOneUint64(uint64_t& value){
    if(dataTemplate.size() == 32){
        value = dev::toUint64(dev::u256(dev::h256(dataTemplate)));
    }
}

dev::eth::EVMSchedule SilubiumDGP::createEVMSchedule(){
    dev::eth::EVMSchedule schedule = dev::eth::EIP158Schedule;
    std::vector<uint32_t> uint32Values;

    if(!dgpevm){
        parseStorageScheduleContract(uint32Values);
    } else {
        parseDataScheduleContract(uint32Values);
    }

    if(!checkLimitSchedule(dataEIP158Schedule, uint32Values))
        return schedule;

    if(uint32Values.size() >= 39){
        schedule.tierStepGas = {{uint32Values[0], uint32Values[1], uint32Values[2], uint32Values[3],
                                uint32Values[4], uint32Values[5], uint32Values[6], uint32Values[7]}};
        schedule.expGas = uint32Values[8];
        schedule.expByteGas = uint32Values[9];
        schedule.sha3Gas = uint32Values[10];
        schedule.sha3WordGas = uint32Values[11];
        schedule.sloadGas = uint32Values[12];
        schedule.sstoreSetGas = uint32Values[13];
        schedule.sstoreResetGas = uint32Values[14];
        schedule.sstoreRefundGas = uint32Values[15];
        schedule.jumpdestGas = uint32Values[16];
        schedule.logGas = uint32Values[17];
        schedule.logDataGas = uint32Values[18];
        schedule.logTopicGas = uint32Values[19];
        schedule.createGas = uint32Values[20];
        schedule.callGas = uint32Values[21];
        schedule.callStipend = uint32Values[22];
        schedule.callValueTransferGas = uint32Values[23];
        schedule.callNewAccountGas = uint32Values[24];
        schedule.suicideRefundGas = uint32Values[25];
        schedule.memoryGas = uint32Values[26];
        schedule.quadCoeffDiv = uint32Values[27];
        schedule.createDataGas = uint32Values[28];
        schedule.txGas = uint32Values[29];
        schedule.txCreateGas = uint32Values[30];
        schedule.txDataZeroGas = uint32Values[31];
        schedule.txDataNonZeroGas = uint32Values[32];
        schedule.copyGas = uint32Values[33];
        schedule.extcodesizeGas = uint32Values[34];
        schedule.extcodecopyGas = uint32Values[35];
        schedule.balanceGas = uint32Values[36];
        schedule.suicideGas = uint32Values[37];
        schedule.maxCodeSize = uint32Values[38];
    }
    return schedule;
}

void SilubiumDGP::clear(){
    templateContract = dev::Address();
    storageDGP.clear();
    storageTemplate.clear();
    paramsInstance.clear();
}

std::string UcToHex(unsigned char uc)//Added by Wujiang.
{
    std::string strRes;
    strRes+=uc/16>9?(uc/16-10+'a'):(uc/16+'0');
    strRes+=uc%16>9?(uc%16-10+'a'):(uc%16+'0');
    return strRes;
}

bool SilubiumDGP::isHaveContract(unsigned int blockHeight)//Added by Wujiang.
{
    if(blockHeight<10050)// || blockHeight%10!=0)
        return false;
    else
        return true;
}

std::string SilubiumDGP::getAddress(std::string strFunction)//std::vector<unsigned char> data)
{
    std::string result="0000000000000000000000000000000000000000";
    try{
        std::vector<unsigned char> datatemp;
        if(!gArgs.IsArgSet("-testnet"))
            datatemp = CallContract(AddressMineMain, ParseHex(strFunction))[0].execRes.output;
        else
            datatemp = CallContract(AddressMineTest, ParseHex(strFunction))[0].execRes.output;
        std::string strtemp="";
        if(datatemp.size()==32)
        {
            for(int pos=12;pos<32;pos++)
            {
                strtemp+=UcToHex(datatemp[pos]);
            }
        }
        result=strtemp;
    }catch(exception& e)
    {
        cout <<"getAddressForMine "<< e.what() << endl;
        LogPrint(BCLog::SILKWORM,"getAddress:%s\n",e.what());
    }
    return result;
}

uint32_t SilubiumDGP::getHeight(std::string strFunction)
{
    uint32_t nResult=0;
    try{
        std::vector<unsigned char> datatemp;
        if(!gArgs.IsArgSet("-testnet"))
            datatemp = CallContract(AddressMineMain, ParseHex(strFunction))[0].execRes.output;
        else
            datatemp = CallContract(AddressMineTest, ParseHex(strFunction))[0].execRes.output;
        nResult=dev::toUint64(dev::u256(dev::h256(datatemp)));
    }catch(exception& e)
    {
        cout <<"getAddressForMine "<< e.what() << endl;
        LogPrint(BCLog::SILKWORM,"getHeight:%s\n",e.what());
    }
    return nResult;
}

dev::Address SilubiumDGP::getParamAddress(unsigned int blockHeight)//Added by Wujiang.
{
    if(isHaveContract(blockHeight)  && !bFindAddress)
    {
        if(silkworm_paramcontractaddr.hex().compare("0000000000000000000000000000000000000000")!=0)// && blockHeight%100!=0)
            return silkworm_paramcontractaddr;
        try{
            LogPrint(BCLog::SILKWORM,"getParamAddress() call CallContract\n");
            std::string strParamAddress1,strParamAddress2;
            uint32_t nParamHeight1,nParamHeight2;
            strParamAddress1=getAddress("e57d6f00");
            strParamAddress2=getAddress("41c3277f");
            nParamHeight1=getHeight("a3ddbf91");
            nParamHeight2=getHeight("c463addf");
            LogPrint(BCLog::SILKWORM,"getParamAddress() call CallContract {%s:%d},{%s:%d}\n",strParamAddress1.c_str(),nParamHeight1,strParamAddress2.c_str(),nParamHeight2);
            if(nParamHeight1==0)
            {
                silkworm_paramcontractaddr=dev::Address("0000000000000000000000000000000000000000");
                bFindAddress=false;
            }
            else if(nParamHeight1>=nParamHeight2)
            {
                if(blockHeight>=nParamHeight1)
                {
                    silkworm_paramcontractaddr=dev::Address(strParamAddress1);
                    bFindAddress=true;
                }
                else
                {
                    silkworm_paramcontractaddr=dev::Address("0000000000000000000000000000000000000000");
                    bFindAddress=false;
                }
            }
            else
            {
                if(nParamHeight1>blockHeight)
                {
                    silkworm_paramcontractaddr=dev::Address("0000000000000000000000000000000000000000");
                    bFindAddress=false;
                }
                else if(blockHeight>=nParamHeight1 && blockHeight<nParamHeight2)
                {
                    silkworm_paramcontractaddr=dev::Address(strParamAddress1);
                    bFindAddress=true;
                }
                else
                {
                    silkworm_paramcontractaddr=dev::Address(strParamAddress2);
                    bFindAddress=true;
                }
            }
            LogPrint(BCLog::SILKWORM,"getParamAddress() call CallContract 2\n");
            //            bFindAddress=true;
        }
        catch(exception& e)
        {
            cout <<"getParamAddress "<< e.what() << endl;
            LogPrint(BCLog::SILKWORM,"getParamAddress:%s\n",e.what());
        }
    }


    return silkworm_paramcontractaddr;
}

uint32_t SilubiumDGP::getMineTime(unsigned int blockHeight)//Added by Wujiang.
{
    if(isHaveContract(blockHeight))
    {
        try{
            LogPrint(BCLog::SILKWORM,"getMineTime() call CallContract\n");
            if(bFindAddress)
            {
                std::vector<unsigned char> datatemp= CallContract(silkworm_paramcontractaddr, ParseHex("18c7e4d7"))[0].execRes.output;
                silkworm_minetime= dev::toUint64(dev::u256(dev::h256(datatemp)));
            }
            else
            {
                std::vector<unsigned char> datatemp= CallContract(getParamAddress(blockHeight), ParseHex("18c7e4d7"))[0].execRes.output;
                silkworm_minetime= dev::toUint64(dev::u256(dev::h256(datatemp)));
            }


        }
        catch(exception& e)
        {
            cout <<"getMineTime " <<e.what() << endl;
            LogPrint(BCLog::SILKWORM,"getMineTime:%s\n",e.what());
        }
    }
    return silkworm_minetime;
}

uint32_t SilubiumDGP::getMineTimeMax(unsigned int blockHeight)//Added by Wujiang.
{
    if(isHaveContract(blockHeight))
    {
        try{
            LogPrint(BCLog::SILKWORM,"getMineTimeMax() call CallContract\n");
            if(bFindAddress)
            {
                std::vector<unsigned char> datatemp= CallContract(silkworm_paramcontractaddr, ParseHex("d942fe09"))[0].execRes.output;
                silkworm_minetimemax=dev::toUint64(dev::u256(dev::h256(datatemp)));//;16000;//
            }else
            {
                std::vector<unsigned char> datatemp= CallContract(getParamAddress(blockHeight), ParseHex("d942fe09"))[0].execRes.output;
                silkworm_minetimemax=dev::toUint64(dev::u256(dev::h256(datatemp)));//;16000;//
            }


        }
        catch(exception& e)
        {
            cout <<"getMineTimeMax "<< e.what() << endl;
            LogPrint(BCLog::SILKWORM,"getMineTimeMax:%s\n",e.what());
        }
    }
    return silkworm_minetimemax;
}

std::string SilubiumDGP::getMainNodeAddress(unsigned int blockHeight)//Added by Wujiang.
{

    if(isHaveContract(blockHeight))
    {
        std::string mainnode;
        if(silkworm_mainnodeaddress.compare("0000000000000000000000000000000000000000")!=0)// && blockHeight%100!=0)
            return silkworm_mainnodeaddress;
        try{
            LogPrint(BCLog::SILKWORM,"getMainNodeAddress() call CallContract\n");
            if(bFindAddress)
            {
                std::vector<unsigned char> datatemp= CallContract(silkworm_paramcontractaddr, ParseHex("eaa55c91"))[0].execRes.output;
                if(datatemp.size()==32)
                {
                    for(int pos=12;pos<32;pos++)
                    {
                        mainnode+=UcToHex(datatemp[pos]);
                    }
                    silkworm_mainnodeaddress=mainnode;
                }
                LogPrint(BCLog::SILKWORM,"%d getMainNodeAddress() call CallContract From %s\n",blockHeight,silkworm_mainnodeaddress.c_str());
            }
            else
            {
                std::vector<unsigned char> datatemp= CallContract(getParamAddress(blockHeight), ParseHex("eaa55c91"))[0].execRes.output;
                if(datatemp.size()==32)
                {
                    for(int pos=12;pos<32;pos++)
                    {
                        mainnode+=UcToHex(datatemp[pos]);
                    }
                    silkworm_mainnodeaddress=mainnode;
                }
                LogPrint(BCLog::SILKWORM,"%d getMainNodeAddress() call CallContract getParamAddres %s\n",blockHeight,silkworm_mainnodeaddress.c_str());
            }

        }
        catch(exception& e)
        {
            cout << "getMainNodeAddress "<< e.what() << endl;
            LogPrint(BCLog::SILKWORM,"getMainNodeAddress:%s\n",e.what());
        }
    }

    return silkworm_mainnodeaddress;
}
