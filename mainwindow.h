#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWaitCondition>
#include <QMutex>
#include <QListWidgetItem>
#include <winsock2.h>
#include <IPHlpApi.h>
#include <stdlib.h>
#include <QMessageBox>
#include "sender.h"
#include "StructPacket.h"
#include "mylistwidget.h"

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
    QWaitCondition wait;
    QMutex mutex;

public slots:
    void on_click();
    void pause_click();
    void radio_chg(bool chg);
    void rPacket(long time, long jt, bool timeout, int type, int code, int len, char ttl, short sn, int tos);
    void ePing();

signals:
    void ab();


private:
    Ui::MainWindow *ui;
    Sender *snd;
    u_char *pkt;
    eth_header *ethhdr;
    ip_header *iphdr;
    icmp_header *icmphdr;    
    int pktsize, ECN, dscpList[21];
    struct hostent *hostip;
    in_addr in_addr_struct;
    WSADATA wsaData;
    QString err, tosCode;
    QMessageBox wsaError;

    int makepacket();
    int ctoi(char *str_ip, ip_address *ipadr);
    void adrtopack(BYTE *bt, UINT cnt, eth_header *eth, bool sord);
    void testipandmac();
    bool isIP(QString *host);
    void getIcmpError(int type, int code);
    void getTos(int tos);


};

#endif // MAINWINDOW_H
