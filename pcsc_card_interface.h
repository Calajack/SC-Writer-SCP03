#ifndef PCSC_CARD_INTERFACE_H
#define PCSC_CARD_INTERFACE_H

#include <windows.h>
#include <winscard.h>
#include <vector>
#include <string>

class PCScCardInterface {
private:
    SCARDCONTEXT hContext;
    SCARDHANDLE hCard;
    DWORD dwActiveProtocol;
    bool cardConnected;
    std::string currentReaderName;
    LONG lastError;

    // Helper functions
    std::wstring StringToWString(const std::string& str);
    std::string WStringToString(const std::wstring& wstr);
    std::string formatError(LONG error) const;

public:
    PCScCardInterface();
    ~PCScCardInterface();

    // Existing methods
    bool initialize();
    std::vector<std::string> listReaders();
    bool connectToCard(const std::string& readerName = "");
    void disconnect();
    std::vector<unsigned char> sendAPDU(const std::vector<unsigned char>& apdu);
    bool isCardPresent();
    std::vector<unsigned char> getATR();
    const std::string& getReaderName() const;
    std::string getLastError() const;
    bool selectISD();

    // Enhanced disconnect methods
    void disconnectWithReset();
    void disconnectWithUnpower();
    void disconnectAndReleaseContext();
    bool forceReconnect(const std::string& readerName = "");

    // Expose card handles for wrapper
    SCARDCONTEXT getContext() const { return hContext; }
    SCARDHANDLE getCardHandle() const { return hCard; }

    // Extended APDU support (for future use)
    std::vector<unsigned char> sendExtendedAPDU(const std::vector<unsigned char>& apdu);

    // Store INIT UPDATE response (for future use)
    void storeInitUpdateResponse(const std::vector<unsigned char>& response);
    const std::vector<unsigned char>& getStoredInitUpdateResponse() const;

private:
    std::vector<unsigned char> storedInitUpdateResponse;
};

#endif // PCSC_CARD_INTERFACE_H
