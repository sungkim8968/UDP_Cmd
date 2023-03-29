#include <iostream>
#include <thread>
#include <cstring>
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define PORT 30100

using namespace std;

void receiveCommand(int sockfd, struct sockaddr_in clientaddr) {
    char buf[BUF_SIZE];
    int addrlen = sizeof(clientaddr);
    while (true) {
        int nbytes = recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr*)&clientaddr, (socklen_t*)&addrlen);
        if (nbytes < 0) {
            cerr << "recvfrom error" << endl;
            break;
        }
        buf[nbytes] = '\0';
        cout << "Received command from " << inet_ntoa(clientaddr.sin_addr) << ":" << ntohs(clientaddr.sin_port) << ": " << buf << endl;
    }
}

void sendInfo(int sockfd, struct sockaddr_in clientaddr) {
    char buf[BUF_SIZE];
    while (true) {
        cout << "Enter message to send: ";
        cin.getline(buf, BUF_SIZE);
        int nbytes = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
        if (nbytes < 0) {
            cerr << "sendto error" << endl;
            break;
        }
        cout << "Sent " << nbytes << " bytes to " << inet_ntoa(clientaddr.sin_addr) << ":" << ntohs(clientaddr.sin_port) << endl;
    }
}

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        cerr << "socket error" << endl;
        return 1;
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        cerr << "bind error" << endl;
        return 1;
    }

    struct sockaddr_in clientaddr;
    memset(&clientaddr, 0, sizeof(clientaddr));
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    clientaddr.sin_port = htons(0);

    if (connect(sockfd, (struct sockaddr*)&clientaddr, sizeof(clientaddr)) < 0) {
        cerr << "connect error" << endl;
        return 1;
    }

    thread t1(receiveCommand, sockfd, clientaddr);
    thread t2(sendInfo, sockfd, clientaddr);

    t1.join();
    t2.join();

    close(sockfd);

    return 0;
}
