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
#include <vector>
#include <sstream>
#include <bits/stdc++.h>

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
    return s << "\n-------------------------------------"
             << "\nIP           | " << info.IP
             << "\nPORT         | " << info.PORT
             << "\nSERVICENAME  | " << info.SERVICENAME
             << "\nSERVICETYPE  | " << info.SERVICETYPE
             << "\nACCESS_TOKEN | " << info.ACCESS_TOKEN
             << "\n-------------------------------------\n";
}

class FileClientSocket
{

    int PORT, REGPORT;
    int MySocket, MyRegSocket;
    int Address_length, regAddress_length;
    int ByteRecvAtOnce = 1024 * 50;

    struct sockaddr_in Address;
    struct sockaddr_in regAddress;

    FILE *file;

    Utils Util;

public:
    vector<ServerInfo> ListOfServer;
    FileClientSocket()
    {
        CreateClientSocket();
        CreateRegServerSocket();
        PORT = 8000;
        REGPORT = 9999;
        regAddress.sin_family = AF_INET;
        regAddress.sin_addr.s_addr = INADDR_ANY;
        regAddress.sin_port = htons(REGPORT);
        regAddress_length = sizeof(regAddress);
    }

    void CreateRegServerSocket()
    {
        // This Function is used for creating socket
        MyRegSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (MyRegSocket == 0)
        {
            cout << "||Server Side Error|| : Registration Socket Creation Failed! :(";
            exit(EXIT_FAILURE);
        }
        cout << "||Server Log|| : Socket Created Successfully.  :)" << endl;
    }

    void RegisterServer()
    {
        // This ClientThreadRuntion is used for sending the message/command to server
        cin.clear();
        string msg = "127.0.0.1/" + Util.IntToStr(PORT) + "/DISCOVERY/CLIENT/CLIENT/ABC123";
        cout << "msg.c_str()" << msg.c_str() << endl;
        cout << "sizeof(msg)" << sizeof(msg) << endl;
        int response = sendto(MyRegSocket, msg.c_str(), strlen(msg.c_str()), MSG_CONFIRM, (const struct sockaddr *)&regAddress, regAddress_length);
        cout << "||Client Log|| : Response - " << response << endl;
        unsigned int len;
        char MessageBuf[ByteRecvAtOnce];
        memset(MessageBuf, '\0', sizeof(MessageBuf));
        cout << "Waiting 4 msg..." << endl;
        int data = recvfrom(MyRegSocket, &MessageBuf, sizeof(MessageBuf), MSG_WAITALL, (struct sockaddr *)&regAddress, &len);
        cout << "\n||Client Log|| : DATA Received - " << data << " len: " << len << " " << MessageBuf << endl;

        vector<string> ServerList;
        vector<string> ServerDetail;
        ServerList = Util.StringTokenizerByChar(MessageBuf, '#');
        for (auto server : ServerList)
        {
            if (server != "")
            {
                ServerDetail = Util.StringTokenizerByChar(server, '/');
                ServerInfo SI(ServerDetail[0], Util.StrToInt(ServerDetail[1]), ServerDetail[2], ServerDetail[3], ServerDetail[4]);
                ListOfServer.push_back(SI);
                cout << SI;
            }
        }
    }

    vector<string> StringTokenizerByChar(string InFileLs, char Sep)
    {
        stringstream InFileLsStream(InFileLs);
        string File;
        vector<string> v;
        while (!InFileLsStream.eof())
        {
            getline(InFileLsStream, File, Sep);
            v.push_back(File);
        }
        return v;
    }

    void CreateClientSocket()
    {
        MySocket = socket(AF_INET, SOCK_STREAM, 0);
        if (MySocket < 0)
        {
            cout << "||Client Side Error|| : Socket Creation Failed, Exiting Program Now..." << endl;
            exit(EXIT_FAILURE);
        }
        cout << "||Client Log|| : Client Socket Created Successfully." << endl;
    }

    int CreateConnection(int PortNo)
    {
        // if encounter error of transport endpoint is already connected please uncomment line below
        // CreateClientSocket();
        MySocket = socket(AF_INET, SOCK_STREAM, 0);
        if (MySocket < 0)
        {
            cout << "||Client Side Error|| : Socket Creation Failed, Exiting Program Now..." << endl;
            exit(EXIT_FAILURE);
        }
        cout << "||Client Log|| : Client Socket Created Successfully." << endl;
        Address.sin_family = AF_INET;
        Address.sin_port = htons(PortNo);
        Address_length = sizeof(Address);
        if (inet_pton(AF_INET, "127.0.0.1", &Address.sin_addr) <= 0)
        {
            cout << "||Client Side Error|| : Connection Address Invalid :(" << endl;
        }
        if (connect(MySocket, (struct sockaddr *)&Address, sizeof(Address)) < 0)
        {
            cout << "||Client Side Error|| : connection attempt failed, Coudn't able to connect to server with port : " << PortNo << endl;
            return 0;
            // exit(EXIT_FAILURE);
        }
        cout << "||Client Log|| : Connection Successfull. Connected to server with port : " << PortNo << endl;
        return 1;
    }

    string WaitForMessage()
    {
        char MessageBuf[ByteRecvAtOnce];
        memset(MessageBuf, '\0', sizeof(MessageBuf));
        int datarev = recv(MySocket, &MessageBuf, sizeof(MessageBuf), 0);
        cout << "||Resv LOG|| : DATA Received - " << datarev << endl;
        return MessageBuf;
    }

    void SendMessage(string msg)
    {
        int response = send(MySocket, msg.c_str(), sizeof(msg), 0);
        cout << "||Send LOG|| : Response - " << response << endl;
    }

    int StrToInt(string n)
    {
        stringstream ss;
        ss << n;
        int i;
        ss >> i;
        return i;
    }

    string IntToStr(int n)
    {
        stringstream ss;
        ss << n;
        string s;
        ss >> s;
        return s;
    }

    void ReceiveSelectedFile(string FileName)
    {
        string ClientPath = ".//Storage//Client//";
        ClientPath.append(FileName);
        file = fopen(ClientPath.c_str(), "wb");
        if (file)
        {
            cout << "||Client Log|| : File Creted." << endl;
        }
        else
        {
            cout << "||Client Side Error|| : File creation failed, Exiting Program Now..." << endl;
            exit(EXIT_FAILURE);
        }

        int dataRecvd = 0, c = 0, flags = 0;
        long long size, received = 0;
        long long data = recv(MySocket, &size, sizeof(long long), 0);
        long long sizeleft = size;
        int temp = ByteRecvAtOnce;
        if (ByteRecvAtOnce > size)
        {
            temp = size;
        }
        char buffer[temp];
        cout << "||Client Log|| : Total Size Of File to be Receive " << size << endl;
        while (sizeleft > 0)
        {
            memset(buffer, '\0', sizeof(buffer));
            dataRecvd = recv(MySocket, &buffer, sizeof(buffer), flags);
            if (dataRecvd == -1)
            {
                cout << "Errorno : " << errno << endl;
            }
            received += dataRecvd;
            sizeleft -= dataRecvd;
            cout << "||Client Log|| : Number of Bytes Receive  : " << dataRecvd << " | Total Data Sent " << received << endl;
            c++;
            fwrite(buffer, 1, sizeof(buffer), file);
        }
        cout << endl;
        cout << "||Client Log|| : Received " << received << " Bytes of Data" << endl;
        cout << "||Client Log|| : Data Not Received " << max(0LL, size - received) << " Bytes" << endl;
        cin.clear();
        fclose(file);
        if (max(0LL, size - received) != 0LL)
            cout << "||Client Log|| : Error Occured In Transmition, File Might be Currupted." << endl;
        else
            cout << "||Client Log|| : File Saved Successfully." << endl;
    }
};

int main()
{
    FileClientSocket C;
    Utils Util;
    C.RegisterServer();
    //

    vector<string> v;
    for (auto server : C.ListOfServer)
    {
        int connRes = C.CreateConnection(server.PORT);
        if (connRes)
        {
            cout << "------------------------------------------------------" << endl;
            cout << "Do You Want to test Client Token validation : Enter[yes=1/ no=0]" << endl;
            int CT;
            cin >> CT;
            if (CT)
            {
                cout << "Will sent This Value as wrong Token : WrongValueToken1234" << endl;
                C.SendMessage("WrongValueToken1234");
            }
            else
            {
                C.SendMessage(server.ACCESS_TOKEN);
            }

            cout << "------------------------------------------------------" << endl;

            string Verify = C.WaitForMessage();
            bool flag = false;
            if (Util.StrToInt(Verify) == 200)
            {
                flag = true;
            }
            else
            {
                cout << "------------------------------------------------------" << endl;
                cout << "Invalid Client" << endl;
                cout << "------------------------------------------------------" << endl;
            }
            if (flag)
            {
                C.SendMessage("GETFL");
                cout << "----";
                string rev = C.WaitForMessage();
                cout << rev << endl;
                v = C.StringTokenizerByChar(rev, '/');
                cout << "------------------------------------------------------" << endl;
                int j = 0;
                cout << "index "
                     << "name" << endl;
                for (auto a : v)
                {
                    cout << j << "   " << a << endl;
                    j++;
                }
                cout << "------------------------------------------------------" << endl;
                cout << "USE Command GET inorder to get that file" << endl;
            }
            for (; flag;)
            {
                string val;
                cout << "------------------------------------------------------" << endl;
                cout << "Commands : " << endl
                     << " GETFL -  To Get a File List from Server" << endl
                     << " GET   -  To Get a File" << endl
                     << " BYE   -  To TERMINATE Program on Client Side" << endl
                     << endl;
                cout << "Enter Command : " << endl;

                cin >> val;
                // cin.clear();
                // getline(cin, val);

                if (val == "GETFL")
                {
                    C.SendMessage("GETFL");
                    string rev = C.WaitForMessage();
                    // cout<<rev<<endl;
                    v = C.StringTokenizerByChar(rev, '/');
                    cout << "------------------------------------------------------" << endl;
                    int j = 0;
                    cout << "index "
                         << "name" << endl;
                    for (auto a : v)
                    {
                        cout << j << "   " << a << endl;
                        j++;
                    }
                    cout << "------------------------------------------------------" << endl;
                    cout << "USE Command GET inorder to get that file" << endl;
                }
                else if (val == "GET")
                {
                    string str = "";
                    C.SendMessage("GET");
                    cout << "PLEASE ENTER INDEX OF FILE : ";
                    cin.clear();
                    string in;
                    cin >> in;
                    cout << in << endl;
                    C.SendMessage(in);
                    string rev = C.WaitForMessage();
                    if (C.StrToInt(rev) == 200)
                    {
                        cout << "------------------------------------------------------" << endl;
                        cout << "Receiving file now!" << endl;
                        C.ReceiveSelectedFile(v.at(C.StrToInt(in)));
                    }
                    else if (C.StrToInt(rev) == 100)
                    {
                        cout << "------------------------------------------------------" << endl;
                        cout << "Please Enter Correct Index, FILE NOT AVAILABEL!" << endl;
                    }
                    else
                    {
                        cout << "------------------------------------------------------" << endl;
                        cout << "Unidentified Response! | TRY AGAIN." << endl;
                    }
                }
                else if (val == "BYE")
                {
                    C.SendMessage("BYE");
                    break;
                }
                else
                {
                    cout << "------------------------------------------------------" << endl;
                    cout << "NO SUCH COMMAND, " << val << endl;
                }
            }
        }
    }
    return 0;
}