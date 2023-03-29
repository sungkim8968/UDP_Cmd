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

void receiveInfo(int sockfd, struct sockaddr_in servaddr) {
    char buf[BUF_SIZE];
    int addrlen = sizeof(servaddr);
    while (true) {
        int nbytes = recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr*)&servaddr, (socklen_t*)&addrlen);
        if (nbytes < 0) {
            cerr << "recvfrom error" << endl;
            break;
        }
        buf[nbytes] = '\0';
        cout << "Received " << nbytes << " bytes from " << inet_ntoa(servaddr.sin_addr) << ":" << ntohs(servaddr.sin_port) << ": " << buf << endl;
    }
}

void sendCommand(int sockfd, struct sockaddr_in servaddr) {
    char buf[BUF_SIZE];
    while (true) {
        cout << "Enter command to send: ";
        cin.getline(buf, BUF_SIZE);
        int nbytes = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
        if (nbytes < 0) {
            cerr << "sendto error" << endl;
            break;
        }
        cout << "Sent " << nbytes << " bytes to " << inet_ntoa(servaddr.sin_addr) << ":" << ntohs(servaddr.sin_port) << endl;
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
    servaddr.sin_port = htons(0);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        cerr << "bind error" << endl;
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    thread t1(receiveInfo, sockfd, servaddr);
    thread t2(sendCommand, sockfd, servaddr);

    t1.join();
    t2.join();

    close(sockfd);

    return 0;
}
