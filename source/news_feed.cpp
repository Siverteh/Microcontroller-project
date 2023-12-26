#include "libs/tinyxml2.h"
#include <cstdio>
#include <string>
#include <cstdlib>
#include "include/declarations.h"
#include "include/network_requests.h"
#include "source/lcd_scrolling.cpp"

void parseXml(char *xmlSource)
{

    char *xmlSourceStart = strchr(xmlSource, '<');

    if (xmlSourceStart == nullptr) {
        printf("Could not find XML data!\n");
        return;
    }
    // Ensures final character is '>'
    int xmlSourceSize = strlen(xmlSource);
    while (xmlSourceSize > 0)
    {
        if (xmlSource[xmlSourceSize] == '>')
            break;
        else
            xmlSource[xmlSourceSize] = '\0';

        xmlSourceSize -= 1;
    }
    if ((xmlSourceSize == 0) || (xmlSource + xmlSourceSize < xmlSourceStart))
    {
        printf("No ending \">\" found, invalid XML\n");
        return;
    }
    
    // Pointer to the first "<"
    static const char *xml = strchr(xmlSource, '<');

    tinyxml2::XMLDocument doc;
    doc.Parse(xml);

    // Finds root element and then the  pointer to  the channel element in the XML data
    tinyxml2::XMLElement* rootElement = doc.RootElement();
    tinyxml2::XMLElement* channelElement = rootElement->FirstChildElement("channel");

    // Finds pointer to first article's headline
    tinyxml2::XMLElement* itemElement = channelElement->FirstChildElement("item");
    tinyxml2::XMLElement* titleElement = itemElement->FirstChildElement("title");
    
    // Finds pointer to second article's headline
    tinyxml2::XMLElement* itemElement2 = channelElement->FirstChildElement("item")->NextSiblingElement();
    tinyxml2::XMLElement* titleElement2 = itemElement2->FirstChildElement("title");

    // Finds pointer to third article's headline
    tinyxml2::XMLElement* itemElement3 = channelElement->FirstChildElement("item")->NextSiblingElement()->NextSiblingElement();
    tinyxml2::XMLElement* titleElement3 = itemElement3->FirstChildElement("title");

    // Stores the headlines
    const char* tmpFirstArticle = titleElement->GetText();
    const char* tmpSecondArticle = titleElement2->GetText();
    const char* tmpThirdArticle = titleElement3->GetText();

    // Update the global variables
    firstArticle = tmpFirstArticle;
    secondArticle = tmpSecondArticle;
    thirdArticle = tmpThirdArticle;

    // Info print to console
    printf("------------------------- XML PARSED HEADLINES HERE -------------------------\n");
    printf("1. %s\n2. %s\n3. %s\n", firstArticle.c_str(), secondArticle.c_str(), thirdArticle.c_str());
    printf("-----------------------------------------------------------------------------\n");
}

char* getNews()
{
    const char *host = "feeds.bbci.co.uk";
    // We allow 20kB to allow the whole response. This is because the parser wants the whole response
    // and some days there are a lot of news.
    static constexpr size_t HTTP_RESPONSE_BUF_SIZE = 20480;
    static char xmlSource[HTTP_RESPONSE_BUF_SIZE] = { 0 };
    nsapi_size_or_error_t result;

    const char request[] = "GET /news/world/rss.xml HTTP/1.1\r\n"
                                "Host: feeds.bbci.co.uk\r\n"
                                "Connection: close\r\n"
                                "\r\n";

    printf("Getting the recent news...\n");
    while (true) 
    {
        socket.open(network);

        result = network->gethostbyname(host, &socket_address);

        if (result != 0) {
            printf("Failed to get IP address of host: %s %d\n", host, result);
            socket.close();
            continue;
        }

        printf("IP address of server %s is %s\n", host, socket_address.get_ip_address());

        socket_address.set_port(80);

        result = socket.connect(socket_address);

        if (result != 0) {
            printf("Failed to connect to server at %s: %d\n", host, result);
            socket.close();
            continue;
        }

        printf("Sucessfully connected to server %s\n", host);

        result = sendRequest(socket, request);

        if (result != 0) {
            printf("Failed to send request: %d\n", result);
            socket.close();
            continue;
        }

        result = readResponse(socket, xmlSource, HTTP_RESPONSE_BUF_SIZE);

        if (result < 0) {
            printf("Failed to read response: %d\n", result);
            socket.close();
            continue;
        }

        xmlSource[result] = '\0';

        socket.close();

        /*  UNCOMMENT IF YOU WANT TO SEE THE WHOLE RESPONSE IN THE CONSOLE
        // Info print to console
        printf("-------------------------- THE HTTP RESPONSE IS -----------------------------\n");
        printf("%s\n", xmlSource);
        printf("-----------------------------------------------------------------------------\n");
        */

        return xmlSource;
    }
}

void fetchNewsFeed() {
    while (true) {
        while (true) {
            socketMutex.lock();
            parseXml(getNews());
            socketMutex.unlock();

            // Check if articles were propperly fetched. If not, try again
            if (!firstArticle.empty() && !secondArticle.empty() && !thirdArticle.empty())
                break;
        }

        newsFethced = true;
        eventFlags.set(NEWS_FLAG);

        ThisThread::sleep_for(30min);
    }
}

void displayNewsFeed() {
    while (true) {
        string articles[] = {firstArticle, secondArticle, thirdArticle};
        lcdScrolling(articles, 3, true, NEWS_SCROLLING_FLAG, newsFethced);
    }
}