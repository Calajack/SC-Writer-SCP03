#pragma once
#include <windows.h>
#include <openssl/evp.h>
#include <openssl/cmac.h>
#include <vector>
#include <cstdint>

#pragma comment(lib, "bcrypt.lib")

class SmartCardCryptoSCP03 {
private:
    struct SCP03State {
        unsigned char senc[16];
        unsigned char smac[16];
        unsigned char srmac[16];
        unsigned char dek[16];
        unsigned char macChaining[16];
        unsigned char encCounter[16];
        unsigned char hostChallenge[8];
        unsigned char cardChallenge[8];
        unsigned char sequenceCounter[3];
        unsigned char cardCryptogram[8];
        unsigned char initUpdateResponse[32];
        bool channelActive;
        uint8_t securityLevel;
        uint32_t commandCounter;
    } state;

    bool performAES_ECB_Encrypt(const unsigned char* key, const unsigned char* input,
        unsigned char* output, size_t dataLen);
    bool performAES_CBC_Encrypt(const unsigned char* key, const unsigned char* iv,
        const unsigned char* input, unsigned char* output,
        size_t dataLen);
    bool performAES_CMAC(const unsigned char* key, const unsigned char* data,
        size_t dataLen, unsigned char* mac);

    bool deriveSessionKey_SCP03(const unsigned char* masterKey, uint8_t keyType,
        const unsigned char* context, size_t contextLen,
        unsigned char* derivedKey);

    bool calculateSCP03Cryptogram(const unsigned char* key,
        const unsigned char* hostChallenge,
        const unsigned char* cardChallenge,
        unsigned char* cryptogram);

    bool calculateCommandMAC(const std::vector<unsigned char>& apdu,
        unsigned char* mac);
    bool calculateResponseMAC(const std::vector<unsigned char>& response,
        unsigned char* mac);

    bool encryptCommandData(const unsigned char* plainData, size_t dataLen,
        unsigned char* encryptedData);
    bool decryptResponseData(const unsigned char* encryptedData, size_t dataLen,
        unsigned char* plainData);

    void addISO9797_M2Padding(std::vector<unsigned char>& data);
    void removeISO9797_M2Padding(std::vector<unsigned char>& data);

public:
    SmartCardCryptoSCP03();
    ~SmartCardCryptoSCP03();

    void setMasterKeys(const unsigned char* enc, const unsigned char* mac,
        const unsigned char* dek);
    void setHostChallenge(const unsigned char* challenge, size_t length);

    int initializeSecureChannel(const std::vector<unsigned char>& initUpdateResponse);
    std::vector<unsigned char> createExternalAuthenticateCommand(uint8_t securityLevel = 0x03);

    std::vector<unsigned char> wrapCommandAPDU(const std::vector<unsigned char>& plainAPDU);
    std::vector<unsigned char> unwrapResponseAPDU(const std::vector<unsigned char>& wrappedResponse);

    bool isSecureChannelActive() const;
    uint8_t getSecurityLevel() const;
    uint32_t getCommandCounter() const;

    void printSessionKeys() const;
    void printMacChainingValue() const;
    void resetChannel();
};
