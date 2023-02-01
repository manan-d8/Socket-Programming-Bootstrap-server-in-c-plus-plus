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

#define LOCALIP "127.0.0.1"
#define MYTOKEN "IMAGE1234"
#define STORAGE "SERVER"
#define SERNAME "IMAGE_SERVER"
#define SERTYPE "IMAGE"

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
class FileServerSocket
{
    // This is class we used for creating connection with server
    int PORT, REGPORT;
    int flags = 0, DataSent = 0;
    int MySocket, MyRegSocket;
    int address_length, regAddress_length;
    int ByteSentAtOnce = 1024 * 50;

    struct sockaddr_in regAddress;
    // struct sockaddr_in addr;
    struct sockaddr_in address;
    FILE *file;

    Utils Util;

public:
    FileServerSocket()
    {

        CreateServerSocket();
        CreateRegServerSocket();

        PORT = 8000;
        REGPORT = 9999;
        cout << "Please Enter PORT Number to Start Server[8888] On: ";
        cin >> PORT;

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);
        address_length = sizeof(address);

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
        // This RegisterServer Function is used for sending the message/command to server
        cin.clear();
        string msg = "127.0.0.1/" + Util.IntToStr(PORT) + "/REGISTRN/" + SERNAME + "/" + SERTYPE + "/" + MYTOKEN;
        // cout << "msg.c_str()" << msg.c_str() << endl;
        // cout << "sizeof(msg)" << sizeof(msg) << endl;
        int response = sendto(MyRegSocket, msg.c_str(), strlen(msg.c_str()), MSG_CONFIRM, (const struct sockaddr *)&regAddress, regAddress_length);
        cout << "||Server Log|| : Response - " << response << endl;

        unsigned int len;
        char MessageBuf[ByteSentAtOnce];
        memset(MessageBuf, '\0', sizeof(MessageBuf));

        // cout << "Waiting 4 msg..." << endl;
        int data = recvfrom(MyRegSocket, &MessageBuf, sizeof(MessageBuf), MSG_WAITALL, (struct sockaddr *)&regAddress, &len);
        cout << endl
             << "||Server Log|| : DATA Received - " << data << " len: " << len << " " << MessageBuf << endl;
    }

    void CreateServerSocket()
    {
        // This Function is used for creating socket
        MySocket = socket(AF_INET, SOCK_STREAM, 0);
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

    void ServerListen()
    {
        // This Function is used for listning/waiting for client to arrive
        if (listen(MySocket, 3) < 0)
        {
            cout << "||Server Side Error|| : Listen Failed!";
            exit(EXIT_FAILURE);
        }
        cout << "||Server Log|| : Listening on port " << PORT << "..." << endl;
    }

    int AcceptClientConnection()
    {
        // This Function is used for accepting a client request and assigning new socket id for new client
        int MyNewSocket = accept(MySocket, (struct sockaddr *)&address, (socklen_t *)&address_length);
        if (MyNewSocket < 0)
        {
            cout << "||Server Side Error|| : Accept Connection Issue!";
            exit(EXIT_FAILURE);
        }
        cout << "||Server Log|| : Connected to New Client Successfully." << endl;
        return MyNewSocket;
    }
};

class FileServerClientConnSocket
{
    // This is class we use to create seprate socket for each of the clients
    int PORT;
    int flags = 0, DataSent = 0;
    int MySocket;
    int address_length;
    int ByteSentAtOnce = 1024 * 50;

    struct sockaddr_in address;
    FILE *file;

public:
    string GetFilesOnServer()
    {
        DIR *ServerDir;
        string FileLs = "";
        struct dirent *obj;
        string store = STORAGE;
        string path = ".//Storage//"+store;
        ServerDir = opendir(".//Storage//Server_IMAGE"); // Server directory
        if (ServerDir)
        {
            while ((obj = readdir(ServerDir)) != NULL)
            {
                FileLs += obj->d_name;
                FileLs += "/";
            }
            closedir(ServerDir);
        }
        cout<<" files "<<FileLs;
        return FileLs;
    }

    void CreateServerSocket()
    {
        // This ClientThreadRuntion is used for creating the main socket
        MySocket = socket(AF_INET, SOCK_STREAM, 0);
        if (MySocket == 0)
        {
            cout << "||Server Side Error|| : Socket Creation Failed! :(";
            exit(EXIT_FAILURE);
        }
        cout << "||Server Log|| : Socket Created Successfully.  :)" << endl;
    }

    string WaitForMessage(int MyNewSocket)
    {
        // This ClientThreadRuntion is used for creating the connection with the server
        char MessageBuf[ByteSentAtOnce];
        memset(MessageBuf, '\0', sizeof(MessageBuf));
        int data = recv(MyNewSocket, &MessageBuf, sizeof(MessageBuf), 0);
        cout << "||Server Log|| : DATA Received - " << data << " " << MessageBuf << endl;
        return MessageBuf;
    }
    void SendMessage(int MyNewSocket, string msg)
    {
        // This ClientThreadRuntion is used for sending the message/command to server
        cin.clear();
        // cout << "msg.c_str()" << msg.c_str() << endl;
        // cout << "sizeof(msg)" << sizeof(msg) << endl;
        int response = send(MyNewSocket, msg.c_str(), strlen(msg.c_str()), 0);
        cout << "||Server Log|| : Response - " << response << endl;
    }

    void SendSelectedFile(int MyNewSocket, string FileName)
    {
        // This ClientThreadRuntion is used for sending selected file to the client
        string ServerPath = ".//Storage//Server_IMAGE//";
        ServerPath.append(FileName);
        file = fopen(ServerPath.c_str(), "rb");

        if (file == NULL)
        {
            cout << "||Server Log|| : File Error Occured! :( " << ServerPath << endl;
            exit(EXIT_FAILURE);
        }
        fseek(file, 0, SEEK_END);
        long long size = ftell(file);
        cout << "||Server Log|| : Size Of File to be Sent : " << size << endl;
        send(MyNewSocket, &size, sizeof(long long), 0);
        int temp = ByteSentAtOnce;
        if (ByteSentAtOnce > size)
        {
            temp = size;
        }
        char buffer[temp];
        rewind(file);
        int DataSent = 0, DataSentCur = 0;
        while (!feof(file))
        {
            memset(buffer, '\0', sizeof(buffer));
            fread(buffer, temp, 1, file);
            DataSentCur = send(MyNewSocket, &buffer, sizeof(buffer), flags);
            DataSent += DataSentCur;
            cout << "||Server Log|| : Number of Bytes Sent : " << DataSentCur << " | Total Data Sent " << DataSent << endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        cout << endl;
        cout << "||Server Log|| : Sent " << DataSent << " Bytes of Data" << endl;
        cout << "||Server Log|| : Data Not Sent " << max(0LL, size - DataSent) << " Bytes" << endl;
        cin.clear();
        fclose(file);
    }
};

void *ClientThreadRun(void *sockId)
{
    FileServerClientConnSocket FS;
    Utils Util;
    int MyNewSocket = *((int *)sockId);
    vector<string> v;
    string files = FS.GetFilesOnServer();
    v = Util.StringTokenizerByChar(files, '/');
    v.pop_back();
    string CliTOKEN = FS.WaitForMessage(MyNewSocket);
    cout << "||Server Log|| : Client Token : " << CliTOKEN << endl;
    if (CliTOKEN == MYTOKEN)
    {
        FS.SendMessage(MyNewSocket, "200");
    }
    else
    {
        FS.SendMessage(MyNewSocket, "100");
    }
    // this is main loop for a perticular client to interact with server
    for (;;)
    {
        string rev = FS.WaitForMessage(MyNewSocket);
        cout << "||Server Log|| : Command Received : " << rev << endl;
        if (rev == "GETFL")
        {
            cout << "||Server Log|| : File List Sent" << endl;
            FS.SendMessage(MyNewSocket, files);
        }
        else if (rev == "GET")
        {
            string index = FS.WaitForMessage(MyNewSocket);
            int ind = Util.StrToInt(index);
            if (ind < v.size())
            {
                FS.SendMessage(MyNewSocket, "200");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                FS.SendSelectedFile(MyNewSocket, v.at(ind));
            }
            else
            {
                FS.SendMessage(MyNewSocket, "100");
            }
        }
        else if (rev == "BYE")
        {
            cout << "||Server Log|| : Client Disconnected!" << endl;
            break;
        }
        else if (rev != "")
        {
            cout << "||Server Log|| : No Such Command : " << rev << endl;
            FS.SendMessage(MyNewSocket, "No Such Command! - " + rev);
        }
        else
        {
            cout << "||Server Log|| : Client Left..." << endl;
            break;
        }
    }
}

int main()
{
    FileServerSocket FS1;
    FS1.RegisterServer();
    FS1.BindServerSocket();
    FS1.ServerListen();
    // this is a main loop for diffrent clients
    for (;;)
    {
        int MyNewSocket = FS1.AcceptClientConnection();
        if (!MyNewSocket)
        {
            cout << "||Server Error Log|| : Error in new client." << endl;
        }
        pthread_t MyThread;
        int *pc = NULL;
        pc = new int;
        *pc = MyNewSocket;
        pthread_create(&MyThread, NULL, ClientThreadRun, pc);
    }
    return 0;
}