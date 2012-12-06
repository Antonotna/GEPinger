#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <winsock2.h>
#include <IPHlpApi.h>
#include <stdlib.h>
#include "sender.h"
#include "StructPacket.h"

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_click();
    void rPacket(float time, float jt, bool timeout);
    void ePing(void);

signals:
    void ab();
    
private:
    Ui::MainWindow *ui;
    Sender *snd;
    u_char *pkt;
    eth_header *ethhdr;
    ip_header *iphdr;
    icmp_header *icmphdr;
    int pktsize;

    int makepacket();
    int ctoi(char *str_ip, ip_address *ipadr);
    void adrtopack(BYTE *bt, UINT cnt, eth_header *eth, bool sord);
    void testipandmac();

};

#endif // MAINWINDOW_H
