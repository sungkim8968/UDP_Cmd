#include <iostream>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

void recvThread(int sockfd, struct sockaddr_in* servaddr) {
    char buffer[1024];
    while (true) {
        int len = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
        if (len == -1) {
            cerr << "Error receiving data" << endl;
            continue;
        }
        buffer[len] = '\0';
        cout << "Received message: " << buffer << endl;
    }
}

void sendThread(int sockfd, struct sockaddr_in* servaddr) {
    string message;
    while (true) {
        cout << "Enter message to send: ";
        getline(cin, message);
        int len = sendto(sockfd, message.c_str(), message.size(), 0, (struct sockaddr*) servaddr, sizeof(*servaddr));
        if (len == -1) {
            cerr << "Error sending data" << endl;
            continue;
        }
    }
}

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        cerr << "Error creating socket" << endl;
        return 1;
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(8888);

    thread t1(recvThread, sockfd, &servaddr);
    thread t2(sendThread, sockfd, &servaddr);

    t1.join();
    t2.join();

    close(sockfd);
    return 0;
}
