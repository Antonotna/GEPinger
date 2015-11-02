
#include "sender.h"

Sender::Sender(QObject *parent) :
    QThread(parent)
{
    abort = pause = false;
}

void Sender::run()
{
    int res;
    struct pcap_pkthdr *header;
    const u_char *pkt_data;
    long spkt, rpkt, diff, prvpkt=0, sspkt, uspkt;
    time_t send_time, rec_time;

    snd_iph->tlen = reverse_short(tlen-sizeof(eth_header));
    snd_iph->identification = reverse_short(snd_iph->identification);

    /*CRC for ip and icmp header*/
    snd_iph->crc = cksum(snd_iph, sizeof(ip_header));    
    snd_icmph->crc = icmp_cksum((u_short *) snd_icmph, tlen - (sizeof(eth_header) + sizeof(ip_header)));    


    /*to do infinity loop*/
    for(i=0;i<scnt;i++)
    {
        if(pause)
        {
            snd_mutex->lock();
               snd_wait->wait(snd_mutex);
            snd_mutex->unlock();
        }

        if(abort)
            break;

        time(&send_time);

        pcap_sendpacket(fsend,(u_char *) snd_ethh,tlen);



        /*Reciving ourself request packet to get time of sending */
        while((res = pcap_next_ex(frecv, &header, &pkt_data)) >= 0){
            if(res == 0)
                        /* Timeout elapsed */
                        continue;
            rec_ethh = (eth_header *) pkt_data;
            rec_iph = (ip_header *) (pkt_data + sizeof(eth_header));
            rec_icmp = (icmp_header *) (pkt_data + sizeof(eth_header) + sizeof(ip_header));
            if(rec_iph->identification == snd_iph->identification)
            {
                spkt = (header->ts.tv_sec * 1000) + header->ts.tv_usec;
                sspkt = header->ts.tv_sec;
                uspkt = header->ts.tv_usec;
                //qWarning("rec pkt sec = %u, usec = %u   ", header->ts.tv_sec, header->ts.tv_usec);
                break;
            }
        }


        /*Waiting for reply*/
        while((res = pcap_next_ex(frecv, &header, &pkt_data)) >= 0){

            time(&rec_time);
            if((rec_time - send_time) > stmout)
            {
                emit recPacket(0, 0, true, 0, 0);
                prvpkt = 0;
                break;
            }

            if(res == 0)
                        /* Timeout elapsed */
                        continue;
            rec_ethh = (eth_header *) pkt_data;
            rec_iph = (ip_header *) (pkt_data + sizeof(eth_header));
            rec_icmp = (icmp_header *) (pkt_data + sizeof(eth_header) + sizeof(ip_header));            
            if(rec_iph->proto == 0x1 && rec_icmp->type == 0 && rec_icmp->sid == snd_icmph->sid && rec_icmp->sn == snd_icmph->sn)
            {
                diff = ((header->ts.tv_sec - sspkt) * 1000) + ((header->ts.tv_usec - uspkt)/1000);
                emit recPacket(diff, qFabs(diff - prvpkt), false, 0, 0);
                prvpkt = diff;
                break;
            }            
            //qWarning("!!!!!");
            if(rec_iph->proto == 0x1 && rec_icmp->type == 0x3)
            {
                qWarning("id = %i id = %i", (((int) *(pkt_data + 46)) << 8) | (int) *(pkt_data + 47), snd_iph->identification);
                if((((int) *(pkt_data + 46)) << 8) | (int) *(pkt_data + 47) == reverse_short(snd_iph->identification))
                {
                    emit recPacket(diff, diff - prvpkt, false, 3, rec_icmp->code);
                    qWarning("yes");
                    break;
                }
            }
        }
        snd_iph->identification = reverse_short(snd_iph->identification);
        snd_iph->identification += 1;
        snd_iph->identification = reverse_short(snd_iph->identification);
        snd_icmph->sn += 1;

        /*CRC for ip and icmp header*/
        snd_iph->crc = snd_icmph->crc = 0;
        snd_iph->crc = cksum(snd_iph, sizeof(ip_header));
        //qWarning("crc = %x", snd_iph->crc);
        snd_icmph->crc = icmp_cksum((u_short *) snd_icmph, tlen - (sizeof(eth_header) + sizeof(ip_header)));


    }

    pcap_close(fsend);
    pcap_close(frecv);
    emit endPing();
    return;


}

void Sender::mystart(int cnt, u_char *packet, int len, int tmout, int ival, QWaitCondition *wait, QMutex *mutex)
{

    struct bpf_program fcode;

    stmout = tmout;
    sival = ival;
    scnt = cnt;
    tlen = len;
    snd_mutex = mutex;
    snd_wait = wait;
    snd_ethh = (eth_header *) packet;
    snd_iph = (ip_header *) (packet + sizeof(eth_header));
    snd_icmph = (icmp_header *) (packet + sizeof(eth_header) + sizeof(ip_header));

    unsigned long *snd_saddr;
    bool ext = false;
    pcap_addr_t *a;

    snd_saddr = (unsigned long *) &(snd_iph->saddr);

    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL /* auth is not needed */, &alldevs, errbuf) == -1)
    {
        qWarning("Error in pcap_findalldevs_ex: %s\n", errbuf);
        return;
    }
    for(d=alldevs;d;d=d->next)
    {
        for(a = d->addresses;a;a=a->next)
        {
            if(a->addr)
                //qWarning("ip = %x and %x", ((struct sockaddr_in *)a->addr)->sin_addr.s_addr, *snd_saddr);
                if(((struct sockaddr_in *)a->addr)->sin_addr.s_addr == *snd_saddr)
                {
                    //qWarning("Description: %s", d->description);
                    if((fsend = pcap_open(d->name, 65536, 0, 1000, NULL, errbuf)) == NULL)
                    {
                        qWarning("!!return!!");
                        return;
                    }


                    if((frecv = pcap_open(d->name, 65536, PCAP_OPENFLAG_NOCAPTURE_LOCAL, sival, NULL, errbuf)) == NULL)
                    {
                        qWarning("!!return!!");
                        return;
                    }

                    if (pcap_compile(frecv, &fcode, "icmp", 1, 0) <0 )
                    {
                        qWarning("\nUnable to compile the packet filter. Check the syntax.\n");
                        /* Free the device list */
                        pcap_freealldevs(alldevs);
                        return;
                    }

                    //set the filter
                    if (pcap_setfilter(frecv, &fcode)<0)
                    {
                        qWarning("\nError setting the filter.\n");
                        /* Free the device list */
                        pcap_freealldevs(alldevs);
                        return;
                    }

                    pcap_freealldevs(alldevs);
                    ext = true;
                    break;
                }
        }
        if(ext)
            break;
    }

    /*Start send&recv thread*/
    abort = false;
    start();
}

unsigned short Sender::cksum(ip_header *ip, int len){
  long sum = 0;  /* assume 32 bit long, 16 bit short */
  unsigned short *tmp;

  tmp = (unsigned short *) ip;

  while(len > 1){
    sum += *tmp;
    tmp++;


    if(sum & 0x80000000)   /*if high order bit set, fold*/
      sum = (sum & 0xFFFF) + (sum >> 16);

    len -= 2;
  }

  if(len)       /* take care of left over byte */
    sum += *(unsigned char *)tmp;

  //qWarning("sum = %x", sum);

  while(sum>>16)
    sum = (sum & 0xFFFF) + (sum >> 16);

  return ~sum;
}

u_short Sender::icmp_cksum(u_short *icmp, int len)
{
    register int nleft = len;
    register u_short *w;
    register int sum = 0;
    u_short answer = 0;

    w = (u_short *) icmp;

    while (nleft > 1)
    {
      sum += *w++;
      nleft -= 2;
    }
    if (nleft == 1)
    {
      *(u_char *)(&answer) = *(u_char *)w;
      sum += answer;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return (answer);
}

u_short Sender::reverse_short(u_short i)
{    
    u_short x = 0;
    x = (i & 0xff) << 8 | (i & 0xff00) >> 8;
    return x;

}

void Sender::abrt()
{
    abort = true;
}
