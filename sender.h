#ifndef SENDER_H
#define SENDER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTime>
#include <qmath.h>
#define HAVE_REMOTE
#include <pcap.h>
//#include "reciver.h"
#include "StructPacket.h"
#include <stdlib.h>

class Sender : public QThread
{
    Q_OBJECT
public:
    explicit Sender(QObject *parent = 0);
    void run();   
    void mystart(int cnt, u_char *packet, int len, int tmout, int ival, QWaitCondition *wait, QMutex *mutex);
    unsigned short cksum(ip_header *ip, int len);
    u_short icmp_cksum(u_short *icmp, int len);
    u_short reverse_short(u_short i);
    bool abort, pause;
    
signals:
    void recPacket(long time, long jitter, bool timeout, int type, int code, int len, char ttl, short sn, int tos);
    void endPing();
    
public slots:
    void abrt(void);

private:
    int scnt, tlen, stmout, sival;
    pcap_if_t *alldevs;
    pcap_if_t *d;
    pcap_t *fsend, *frecv;
    int i;
    char errbuf[PCAP_ERRBUF_SIZE];
    eth_header *snd_ethh, *rec_ethh;
    ip_header *snd_iph, *rec_iph, *error_iph;
    icmp_header *snd_icmph, *rec_icmp;
    QMutex *snd_mutex;
    QWaitCondition *snd_wait;
    QTime rtt;

};

#endif // SENDER_H
