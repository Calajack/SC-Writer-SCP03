#pragma once
#include <string>
#include <vector>
#include "crypto_engine_scp03.h"
#include "pcsc_card_interface.h"
#include "dat_file_parser.cpp"

class SCWriterSCP03 {
private:
    SmartCardCryptoSCP03 crypto;
    PCScCardInterface cardInterface;
    bool scp03Active;
    bool dryRunMode;
    int dryRunStep;
    std::vector<unsigned char> simulatedATR;

    bool establishSCP03(uint8_t securityLevel = 0x03);
    bool installDATFile(const std::string& filename);
    bool executePersonalizationScript(const std::string& scriptFile);
    bool verifyCompleteInstallation();
    

    bool sendExtendedAPDU(const std::vector<unsigned char>& apdu,
        std::vector<unsigned char>& response);
    std::vector<std::vector<unsigned char>> splitIntoChunks(
        const std::vector<unsigned char>& data, size_t chunkSize);

    std::vector<unsigned char> hexStringToBytes(const std::string& hex);
    std::string bytesToHexString(const std::vector<unsigned char>& bytes);
    std::vector<unsigned char> safeSendAPDU(const std::vector<unsigned char>& apdu);
    bool checkResponse(const std::vector<unsigned char>& response,
        const std::string& operation);
    bool isDestructiveCommand(const std::vector<unsigned char>& apdu);

    bool establishSCP03(uint8_t securityLevel = 0x03, bool skipISDSelection = false);

    bool isCardAlreadyInitialized();
    void testGemaltoDiversification(const unsigned char* cplcData);
    bool testWithDiversification();
    void logCardState(const std::string& operation);

    bool prePersonalizeForSCP03();
    bool setMasterKeys(const std::vector<unsigned char>& enc,
        const std::vector<unsigned char>& mac,
        const std::vector<unsigned char>& dek);

public:
    SCWriterSCP03();
    ~SCWriterSCP03();

    void setDryRunMode(bool enabled);
    bool isDryRunMode() const { return dryRunMode; }

    bool writeCard(const std::string& datFile);
    bool verifyCard();

    bool probeCard();
    bool testSCP03Handshake();
    bool testExtendedAPDU();
    bool testCompleteWorkflow();
    bool getCardInformation();
    void testEMVDiversification(const unsigned char* serial, size_t serialLen);
};

