/*
Assignment 3 - CS5060: ACN
Socket Programming

Name     : Manan Bharatkumar Darji
Roll No. : CS22MTECH14004


*/
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <dirent.h>
#include <sys/types.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <bits/stdc++.h>
#include <pthread.h>

using namespace std;

class Utils
{
public:
    vector<string> StringTokenizerByChar(string InputString, char Sep)
    {
        // This ClientThreadRuntion is used for seperating the string by some char which i used to seprate the list of files
        stringstream InFileLsStream(InputString);
        string File;
        vector<string> v;
        while (!InFileLsStream.eof())
        {
            getline(InFileLsStream, File, Sep);
            v.push_back(File);
        }
        return v;
    }
    int StrToInt(string n)
    {
        // This ClientThreadRuntion is used for converting string to int
        stringstream ss;
        ss << n;
        int i;
        ss >> i;
        return i;
    }
    string IntToStr(int n)
    {
        // This ClientThreadRuntion is used for converting the int to string
        stringstream ss;
        ss << n;
        string s;
        ss >> s;
        return s;
    }
};

class ServerInfo
{
public:
    Utils Util;
    string IP;
    int PORT;
    string SERVICENAME;
    string SERVICETYPE;
    string ACCESS_TOKEN;

    ServerInfo(string ip, int port, string servicename, string servicetype, string access_token)
    {
        this->IP = ip;
        this->PORT = port;
        this->SERVICENAME = servicename;
        this->SERVICETYPE = servicetype;
        this->ACCESS_TOKEN = access_token;
    }

    string GetServerObject()
    {
        return IP + "/" + Util.IntToStr(PORT) + "/" + SERVICENAME + "/" + SERVICETYPE + "/" + ACCESS_TOKEN + "/" + IP + "#";
    }
};
std::ostream &operator<<(std::ostream &s, const ServerInfo &info)
{
    return s << endl << "-------------------------------------"
             << endl << "IP           | " << info.IP
             << endl << "PORT         | " << info.PORT
             << endl << "SERVICENAME  | " << info.SERVICENAME
             << endl << "SERVICETYPE  | " << info.SERVICETYPE
             << endl << "ACCESS_TOKEN | " << info.ACCESS_TOKEN
             << endl << "-------------------------------------";
}

class FileServerSocket
{
    // This is class we used for creating connection with server
    int PORT;
    int flags = 0, DataSent = 0;
    int MySocket;
    int address_length, ReqServerAddressLen;
    int ByteSentAtOnce = 1024 * 50;

    struct sockaddr_in address, addr;
    struct sockaddr_in ReqServerAddress;
    FILE *file;
    Utils Util;

    vector<ServerInfo> ServerRecords;

public:
    FileServerSocket()
    {
        CreateServerSocket();
        memset(&address, '\0', sizeof(address));
        memset(&ReqServerAddress, '\0', sizeof(ReqServerAddress));
        PORT = 9999;
        cout << "Please Enter PORT Number to Start Bootstrap Server [9999] On: ";
        cin >> PORT;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);
        address_length = sizeof(address);

    }

    void CreateServerSocket()
    {
        // This Function is used for creating socket
        MySocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (MySocket == 0)
        {
            cout << "||Server Side Error|| : Socket Creation Failed! :(";
            exit(EXIT_FAILURE);
        }
        cout << "||Server Log|| : Socket Created Successfully.  :)" << endl;
    }

    void BindServerSocket()
    {
        // This Function is used for bind operation
        if (bind(MySocket, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            cout << "||Server Side Error|| : Bind Failed!";
            exit(EXIT_FAILURE);
        }
        cout << "||Server Log|| : Successfully Binded." << endl;
    }

    void RecvFromClient()
    {
        // This Function is used for listning/waiting for client to arrive
        unsigned int len;
        char MessageBuf[ByteSentAtOnce];
        memset(MessageBuf, '\0', sizeof(MessageBuf));
        // memset(&addr, 0, sizeof(addr));

        len = sizeof(ReqServerAddress);
        int data = recvfrom(MySocket, &MessageBuf, sizeof(MessageBuf), MSG_WAITALL, (struct sockaddr *)&ReqServerAddress, &len);

        cout << endl << "||Server Log|| : DATA Received - " << data << "Bytes | len - " << len << endl;
        if (data > 0)
        {
            vector<string> DataRev;
            DataRev = Util.StringTokenizerByChar(MessageBuf, '/');
            if (DataRev[2] == "REGISTRN")
            {
                ServerInfo SI(DataRev[0], Util.StrToInt(DataRev[1]), DataRev[3], DataRev[4], DataRev[5]);
                cout << SI;
                ServerRecords.push_back(SI);
                string msg = "";
                msg = "200/Registaration Successfull!";
                int response = sendto(MySocket, msg.c_str(), strlen(msg.c_str()), MSG_CONFIRM, (const struct sockaddr *)&ReqServerAddress, len);
                cout << "||Server Log|| : response" << response << endl;
                if (response != -1)
                {
                    cout << "||Server Log|| : Server Registered Successfully." << endl;
                }
                else
                {
                    cout << "Errorno : " << errno << endl;
                }
            }
            else if (DataRev[2] == "DISCOVERY")
            {
                string msg = "";
                cout << endl << " Client Is HERE" << endl;
                for (auto rec : ServerRecords)
                {
                    msg += rec.GetServerObject();
                }
                int response = sendto(MySocket, msg.c_str(), strlen(msg.c_str()), MSG_CONFIRM, (const struct sockaddr *)&ReqServerAddress, len);
                cout << "||Server Log|| : response" << response << endl;
                if (response != -1)
                {
                    cout << "||Server Log|| : Data Shared With Client Successfully." << endl;
                }
                else
                {
                    cout << "Errorno : " << errno << endl;
                }
            }
            else
            {
                string msg = "100/INVALID REQUEST!";
                int response = sendto(MySocket, msg.c_str(), strlen(msg.c_str()), MSG_CONFIRM, (const struct sockaddr *)&ReqServerAddress, len);
                cout << "||Server Log|| : response : " << response << endl;
                if (response != -1)
                {
                    cout << "||Server Log|| : Data Shared With Client Successfully." << endl;
                }
                else
                {
                    cout << "Errorno : " << errno << endl;
                }
            }
        }
        else
        {
            cout << "||Server Side Error|| : Data Receive Error :(";
        }
    }
};

int main()
{
    FileServerSocket FS1;
    FS1.BindServerSocket();
    while (true)
    {
        FS1.RecvFromClient();
        cout << endl << "***********************************************\n";
    }
    return 0;
}