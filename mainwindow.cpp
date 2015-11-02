#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    snd = new Sender;
    ui->ipAddr->setFocus();
    ui->datasize->setRange(60,1500);
    ui->datasize->setValue(100);
    ui->pause->setEnabled(false);
    ui->tos_dscp->setChecked(true);
    ui->Output->setUniformItemSizes(true);
    ECN = 4; //Explicit Congestion Notification
    dscpList[0]=46*ECN; dscpList[1] = 0;
    for(int i=0;i<=17;i++){
        dscpList[i+2] = (i*2+8)*ECN;
    }
    dscpList[19] = 48*ECN; dscpList[20] = 56*ECN;
    QObject::connect(snd,SIGNAL(recPacket(long, long, bool, int, int)),this, SLOT(rPacket(long, long, bool, int, int)));
    QObject::connect(snd, SIGNAL(endPing()),this, SLOT(ePing()));
    QObject::connect(this, SIGNAL(ab()), snd, SLOT(abrt()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_click()
{

    int ival, tmout;
    int ct = ui->count->text().toInt();

    if(ui->ping->text() == "Ping")
    {
        ui->Output->clear();
        if(makepacket())
            return;
        ival = ui->inter->text().toInt();
        tmout = ui->timeout->text().toInt();
        num = 1;
        ui->groupBox->setEnabled(false);
        ui->groupBox_2->setEnabled(false);
        ui->groupBox_3->setEnabled(false);
        ui->pause->setEnabled(true);
        ui->ping->setText("Stop");
        snd->mystart(ct,pkt,pktsize,tmout,ival,&wait,&mutex);
    } else {
        if(snd->pause)
        {
            snd->pause = false;
            ui->pause->setText("Pause ||");
            snd->abort = true;
            wait.wakeAll();
        }else{
            snd->abort = true;
        }
        //emit ab();
    }
}

int MainWindow::makepacket()
{
    MIB_IPFORWARDROW pMib;
    PIP_ADAPTER_INFO pAdapterInfo;
    QStringList iplist;
    DWORD netip = 0, dwRetVal = 0, dst;
    QString strIp;
    char *char_ip, *destination_ip;
    u_int i;
    int j;
    bool eq;
    QByteArray asc;
    IPAddr dadr=0, sadr;
    ULONG mac[2], size = 6;
    BYTE *pbyte;
    u_char *tmpip1, *tmpip2;

    if(ui->ipAddr->text().isEmpty())
        return 1;
    iplist = ui->ipAddr->text().split(".");


    pktsize = ui->datasize->value();    
    if((pkt = (u_char *) malloc(pktsize)) == NULL)
        return 1;

    ethhdr = (eth_header *) pkt;
    iphdr = (ip_header *) (pkt + sizeof(eth_header));
    icmphdr = (icmp_header *) (pkt + sizeof(eth_header) + sizeof(ip_header));

    /*filling data*/
    for((j=sizeof(eth_header) + sizeof(ip_header) + sizeof(icmp_header));j < pktsize;j++)
        pkt[j] = 0;


    /*Destination ip building*/
    asc = ui->ipAddr->text().toAscii();
    destination_ip = char_ip = asc.data();
    ctoi(char_ip,&(iphdr->daddr));


    netip = (iplist.at(3).toInt() << 24) | netip;
    netip = (iplist.at(2).toInt() << 16) | netip;
    netip = (iplist.at(1).toInt() << 8 ) | netip;
    netip = iplist.at(0).toInt() | netip;

    /*Getting ip of best route*/
    GetBestRoute(netip, 0, &pMib);
    dst=pMib.dwForwardNextHop;

    strIp = QString::number(dst & 0xff);
    strIp += ".";
    strIp += QString::number((dst >> 8) & 0xff);
    strIp += ".";
    strIp += QString::number((dst >> 16) & 0xff);
    strIp += ".";
    strIp += QString::number(dst >> 24);
    asc = strIp.toAscii();
    char_ip = asc.data();
    dadr = inet_addr(char_ip);


    ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof (IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL) {
        qWarning("Error allocating memory needed to call GetAdaptersinfo\n");
        return 1;
    }

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        FREE(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {        
        while (pAdapterInfo) {
            if(pAdapterInfo->Index == pMib.dwForwardIfIndex)
            {
                /*Source ip and source mac building*/
                if(ctoi(pAdapterInfo->IpAddressList.IpAddress.String,&(iphdr->saddr)) != 0)
                    return 1;                
                adrtopack(pAdapterInfo->Address,pAdapterInfo->AddressLength,ethhdr, true);
                break;

            }
            pAdapterInfo = pAdapterInfo->Next;
        }
    }

    if(pAdapterInfo == NULL)
        return 1;

    sadr = inet_addr(pAdapterInfo->IpAddressList.IpAddress.String);

    tmpip1 = (u_char *) &dadr;
    tmpip2 = (u_char *) &sadr;
    eq = true;

    for(i=0;i<sizeof(IPAddr);i++)
    {
        if(tmpip1[i] != tmpip2[i])
        {
            eq = false;
            break;
        }
    }

    if(eq)
    {
        dadr = inet_addr(destination_ip);
    }

    /*Destination mac building*/
    SendARP(dadr,NULL,mac,&size);
    pbyte = (BYTE *) mac;
    adrtopack(pbyte, 6, ethhdr, false);



    /*test ip and mac adr*/
    //testipandmac();

    ethhdr->type = 0x8;

    if(ui->tos_dscp->isChecked())
    {
        iphdr->tos = dscpList[ui->dscpBox->currentIndex()];

    }else
    {
        if(!(ui->ipTos->text().isEmpty()))
            iphdr->tos = ui->ipTos->text().toInt();
        else
            iphdr->tos = 0;
    }

    iphdr->ver_ihl = 0x45;
    iphdr->proto = 0x1;

    if(!(ui->ipTtl->text().isEmpty()))
        iphdr->ttl = ui->ipTtl->text().toInt();
    else
        iphdr->ttl = 0x80;

    iphdr->identification = (rand() % 65535);

    /*temp*/
    if(ui->checkBox->isChecked())
        iphdr->flags_fo = 0x40;
    else
        iphdr->flags_fo = 0;


    iphdr->crc = 0;
    icmphdr->crc = 0;
    icmphdr->type = 0x8;
    icmphdr->code = 0;

    icmphdr->sid = ui->sid->text().toInt();
    icmphdr->sn = ui->sn->text().toInt();

    //icmphdr->sid = icmphdr->sn = 0x400;

    return 0;

}

int MainWindow::ctoi(char *str_ip, ip_address *ipadr)
{
    u_char *pbt, bt=0;

    pbt = (u_char *) ipadr;

    while(*str_ip)
    {
        switch(*str_ip)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            bt = (bt * 10) + ((*str_ip) - 48);
            break;
        case '.':
            *pbt = bt;
            bt = 0;
            pbt++;
            break;
        default:
            return 1;

        }
        str_ip++;
    }
            *pbt = bt;

    return 0;

}

void MainWindow::adrtopack(BYTE *bt, UINT cnt, eth_header *eth, bool sord)
{
    u_int i, offset=0;
    u_char *chbt;
    chbt = (u_char *) eth;

    if(sord)
        offset = 6;
    for(i=0;i<cnt;i++)
       chbt[i+offset] = (u_char) bt[i];

}

void MainWindow::testipandmac()
{
    u_char i, *ip;
    ip =(u_char *) &(iphdr->saddr);
    for(i=0;i<4;i++)
    {
        qWarning("%i.",ip[i]);
    }
    qWarning("\n");

    ip = (u_char *) ethhdr;

    for(i=0;i<6;i++)
    {
        qWarning("%x ", ip[i]);
    }
}

void MainWindow::rPacket(long time, long jt, bool timeout, int type, int code)
{
    QString tout = "timeout";
    QString tm = "Packet " + QString::number(num) +  " recived for " + QString::number(time) + " msec. Jitter: " + QString::number(jt) + " msec";

    num++;
    if(type == 3)
    {
        switch(code)
        {       
            case 1:
                ui->Output->addItem("Host Unreachable");
                break;
            case 2:
                ui->Output->addItem("Protocol Unreachable");
                break;
            case 3:
                ui->Output->addItem("Port Unreachable");
                break;
            case 4:
                ui->Output->addItem("Fragmentation Needed and Don't Fragment was Set");
                break;
            default:
                ui->Output->addItem("Destination unreacheble");
                break;
        }
        return;
    }
    if(timeout)
        ui->Output->addItem(tout);
    else
        ui->Output->addItem(tm);
    ui->Output->scrollToBottom();    
}

void MainWindow::ePing()
{
    free(pkt);
    ui->groupBox->setEnabled(true);
    ui->groupBox_2->setEnabled(true);
    ui->groupBox_3->setEnabled(true);
    ui->pause->setEnabled(false);
    ui->pause->setText("Pause ||");
    ui->ping->setText("Ping");
}

void MainWindow::pause_click()
{
    if(ui->pause->text() == "Pause ||")
    {
        snd->pause = true;
        ui->pause->setText("Pause >");
    } else {
        snd->pause = false;
        wait.wakeAll();
        ui->pause->setText("Pause ||");
    }
}

void MainWindow::radio_chg(bool chg)
{
    //qWarning("%d",chg);
    ui->dscpBox->setEnabled(chg);
    ui->ipTos->setEnabled(!chg);

    return;
}

