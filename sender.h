#ifndef SENDER_H
#define SENDER_H

#include <QThread>
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
    //void mystart(int cnt,eth_header *ethh, ip_header *iph, icmp_header *icmph);
    void mystart(int cnt, u_char *packet, int len, int tmout, int ival);
    unsigned short cksum(ip_header *ip, int len);
    u_short icmp_cksum(u_short *icmp, int len);
    u_short reverse_short(u_short i);
    bool abort;
    
signals:
    void recPacket(float time, float jitter, bool timeout);
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
    ip_header *snd_iph, *rec_iph;
    icmp_header *snd_icmph, *rec_icmp;

};

#endif // SENDER_H
