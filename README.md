# Assignment 3 - CS5060: ACN
## Socket Programming

```
Name     : Manan Darji
Roll No. : ************
```

**I Have Submitted Both Part A & B In One Folder But Functionality of both are implemented.**
---
* This Repo Contain 6 cpp files
  * 1 Bootstap Server File.
  * 4 Diffrent Servers Files. [as mentioned below]
  * 1 Client File.
```
1 | Bootstrap_Server.cpp  |
2 | Server_IMAGE.cpp      | 
3 | Server_TEXT.cpp       | 
4 | Server_PDF.cpp        | 
5 | Server_VIDEO.cpp      | 
6 | Client.cpp            | 
```
<br/>

 **I Have Folder Structure for Storage as follows**
---
```
Storage
    |---Client        : Will Contain Files Which Will be downloded from any server
    |---Server_IMAGE  : Contains Images For Server With Same Name [JPG / PNG]
    |---Server_TEXT   : Contains Txt Files For Server With Same Name 
    |---Server_VIDEO  : Contains Video Files For Server With Same Name [mp4]
    |---Server_PDF    : Contains  PDFs For Server With Same Name 
```


# ``How To Run Code?``


 * First we Have to compile the code
```bash
g++ -o Bootstrap Bootstrap_Server.cpp 

g++ -o Server_IMAGE Server_IMAGE.cpp -lpthread 
g++ -o Server_TEXT Server_TEXT.cpp -lpthread 
g++ -o Server_PDF Server_PDF.cpp -lpthread 
g++ -o Server_VIDEO Server_VIDEO.cpp -lpthread 

g++ -o client Client.cpp
```
* copy below for one line command for convince. [ Compile ]
```
g++ -o Server_IMAGE Server_IMAGE.cpp -lpthread ; g++ -o Server_TEXT Server_TEXT.cpp -lpthread ; g++ -o Server_PDF Server_PDF.cpp -lpthread ; g++ -o Server_VIDEO Server_VIDEO.cpp -lpthread ; g++ -o Bootstrap Bootstrap_Server.cpp ; g++ -o client Client.cpp
```


<br/>

### Run **Bootstrap** Server with
---  
```bash
./Bootstrap
```
* follow this step by step
  * Bootstrap Server will run on 127.0.0.1 by default
  * Enter Port No to host server on [ I took 9999 ]*
    * this is statically defined in each server so only use 9999.
  * it will start listing on that port No : `9999` 

<br/>

### Run Diffrent **Servers** with this commands Using this 4 Diffrent Terminals.
---
```bash
./Server_IMAGE
./Server_TEXT
./Server_PDF
./Server_VIDEO
```
* follow this step by step for each server
  * Bootstrap Server will run on 127.0.0.1 by default
  * Enter Port No to host server on 
    * You can take any port number and it will be register on Bootsrap server
  * it will register with port no and other detail on Bootstrap server
  * then will start listing on that port number given 

<br/>

### Run Client with 
##### you can run multiple client using same
---
```bash
./client
```

* follow this step by step
  * Server will run on 127.0.0.1 by default
  * first it will fetch info from the Bootstrap for available servers list
  * it will look something like this
    ```bash
    -------------------------------------
    IP           | 127.0.0.1
    PORT         | 8888
    SERVICENAME  | IMAGE_SERVER
    SERVICETYPE  | IMAGE
    ACCESS_TOKEN | IMAGE1234
    -------------------------------------

    -------------------------------------
    IP           | 127.0.0.1
    PORT         | 7777
    SERVICENAME  | VIDEO_SERVER
    SERVICETYPE  | VIDEO
    ACCESS_TOKEN | VIDEO1234
    -------------------------------------

    -------------------------------------
    IP           | 127.0.0.1
    PORT         | 6666
    SERVICENAME  | TEXT_SERVER
    SERVICETYPE  | TEXT
    ACCESS_TOKEN | TXT1234
    -------------------------------------

    -------------------------------------
    IP           | 127.0.0.1
    PORT         | 4444
    SERVICENAME  | PDF_SERVER
    SERVICETYPE  | PDF
    ACCESS_TOKEN | ABC1234
    -------------------------------------

    ```
  * **Now client will connect with each server `Sequentially` and give you options for the files which are availabel on that server.**
  * something like this:
    ```
    // For Pdf Server
    ------------------------------------------------------
    index name
    0   .
    1   ..
    2   CPP_CHEAT_SHEET.pdf
    3   Assignment3.pdf
    4   
    ------------------------------------------------------

    ```
  
  * now you can use below commands to interact with client
    ```
    ------------------------------------------------------
    Commands : 
    GETFL -  To Get a File List from Server
    GET   -  To Get a File
    BYE   -  To TERMINATE Program on Client Side

    Enter Command :
    ```
  
  * Enter `GETFL` in order to get list of files available on server 
  * you will get something like this
  * ```
    index name
    0   .
    1   ..
    2   SUN_GRASS.png
    3   CITY_OF_NIGHT.jpg
    4   KING.png
    5  
    ------------------------------------------------------
    USE Command GET inorder to get that file
    ------------------------------------------------------
    ```
  * Now Use `GET` Command to Get Requird File with appropriate Index Number
  * It will ask for Index of file
  * ```
    PLEASE ENTER INDEX OF FILE :  
    ```
  * Enter proper index number - `EX : 3 for CITY_OF_NIGHT.jpg`
  * **Please do not enter improper index number** `EX : 0,1,5 in this case`
    * this will crash server [ this is not handled due to time constraint ]
    * on Entering Index Which is not there in List it will give something like:
    ```
    ------------------------------------------------------
    Please Enter Correct Index, FILE NOT AVAILABEL!
    ------------------------------------------------------
    ```
  * it will Download that file like
  * ```
    ||Client Log|| : File Creted.
    ||Client Log|| : Total Size Of File to be Receive 3459455
    ||Client Log|| : Number of Bytes Receive  : 51200 | Total Data Sent 51200
    ||Client Log|| : Number of Bytes Receive  : 51200 | Total Data Sent 102400
    ||Client Log|| : Number of Bytes Receive  : 51200 | Total Data Sent 153600
    .
    .
    .
    ||Client Log|| : Number of Bytes Receive  : 51200 | Total Data Sent 3430400
    ||Client Log|| : Number of Bytes Receive  : 51200 | Total Data Sent 3481600

    ||Client Log|| : Received 3481600 Bytes of Data
    ||Client Log|| : Data Not Received 0 Bytes
    ||Client Log|| : File Saved Successfully.
    ```
  * You can Use `BYE` Command to Terminate the connection with current Server And Goto Next Server in a list if available.
  * To test The `INVALID CLIENT` you will get option at start to send wrong token just to demonstrate.
  ```
    ------------------------------------------------------
    Do You Want to test Client Token validation : Enter[yes=1/ no=0]
    1
    Will sent This Value as wrong Token : WrongValueToken1234
    ||Send LOG|| : Response - 32
    ------------------------------------------------------
    ||Resv LOG|| : DATA Received - 3
    ------------------------------------------------------
    Invalid Client
    ------------------------------------------------------

  ```


### PLAGIARISM STATEMENT 
I certify that this assignment/report is my own work, based on my personal study and/or research and that I have acknowledged all material and sources used in its preparation, whether they be books, articles, reports, lecture notes, and any other kind of document, electronic or personal ommunication. I also certify that this assignment/report has not reviously been submitted for assessment in any other course, except where specific permission has been granted from all course instructors involved, or at any other time in this course, and that I have not copied in part or whole or otherwise plagiarized the work of other students and/or persons. I pledge to uphold the principles of honesty and responsibility at CSE@IITH. In addition, I understand my responsibility to report honor violations by other students if I become aware of it.

### `Name of the student : Manan Darji`
### `Roll No : ************`

---

---
## PS : IF SOME ERROR OCCURED OR DATA IS CORRUPTED PLEASE DO RESTART THE SERVER, AND TRY AGAIN!  :) 
# `Thank You!`
---
