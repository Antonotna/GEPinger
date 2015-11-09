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
    delimiter = "-----------------------------------------------------------------------------";
    ECN = 4; //Explicit Congestion Notification
    dscpList[0]=46*ECN; dscpList[1] = 0;
    for(int i=0;i<=17;i++){
        dscpList[i+2] = (i*2+8)*ECN;
    }
    dscpList[19] = 48*ECN; dscpList[20] = 56*ECN;

    /*rand use for id generate in ip header and for sid generate in icmp header */
    srand(65535);
    ui->sid->setText(QString::number(rand()));

    QObject::connect(snd,SIGNAL(recPacket(long, long, bool, int, int, int, char, short, int)),this, SLOT(rPacket(long, long, bool, int, int, int, char, short, int)));
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
        ui->groupBox->setEnabled(false);
        ui->groupBox_2->setEnabled(false);
        ui->groupBox_3->setEnabled(false);
        ui->pause->setEnabled(true);
        ui->ping->setText("Stop");

        jitter_sum = rtt_sum = pkt_num = pkt_loss = 0;

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
    DWORD dwRetVal, dwError;
    QString hoststr;
    u_long host;
    char *destination_ip;    
    QByteArray asc;
    u_long best_route, sadr;
    ULONG mac[2], size = 6;
    BYTE *pbyte;    


    hoststr = ui->ipAddr->text();

    asc = hoststr.toAscii();
    destination_ip  = asc.data();

    if(hoststr.isEmpty())
        return 1;

    /*Getting ip address*/
    WSAStartup(MAKEWORD(2,2),&wsaData);
    hostip = gethostbyname(destination_ip);
    if(hostip == NULL)
    {
        dwError = WSAGetLastError();
        if (dwError == WSAHOST_NOT_FOUND)
        {
            wsaError.setText("Host not found");
            wsaError.exec();
        }else{
            wsaError.setText("Unknown error");
            wsaError.exec();
        }
        ui->ip->clear();
        return 1;
    }else{
        host = *(u_long *) hostip->h_addr_list[0];
        in_addr_struct.S_un.S_addr = host;
        destination_ip  = inet_ntoa(in_addr_struct);
    }

    ui->ip->setText(QString(destination_ip));


    pktsize = ui->datasize->value();
    if((pkt = (u_char *) malloc(pktsize)) == NULL)
        return 1;

    ethhdr = (eth_header *) pkt;
    iphdr = (ip_header *) (pkt + sizeof(eth_header));
    icmphdr = (icmp_header *) (pkt + sizeof(eth_header) + sizeof(ip_header));

    /*filling data(zero)*/
    for(int i=sizeof(eth_header) + sizeof(ip_header) + sizeof(icmp_header);i < pktsize;i++)
        pkt[i] = 0;

    /*Destination ip building*/
    ctoi(destination_ip,&(iphdr->daddr));


    /*Getting ip of best route*/
    if(GetBestRoute(host, 0, &pMib))
        return 1;

    /*For local host dwForwardType = 3; for remote host dwForwardType = 4*/
    if(pMib.dwForwardType == 3)
        best_route = host;
    else
        best_route = (u_long) pMib.dwForwardNextHop;


    /*Source ip and source mac building*/
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
                if(ctoi(pAdapterInfo->IpAddressList.IpAddress.String,&(iphdr->saddr)) != 0)
                    return 1;                
                adrtopack(pAdapterInfo->Address,pAdapterInfo->AddressLength, ethhdr, true);
                break;

            }
            pAdapterInfo = pAdapterInfo->Next;
        }
    }

    if(pAdapterInfo == NULL)
        return 1;


    /*Destination mac building*/
    SendARP(best_route,NULL,mac,&size);
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

    iphdr->identification = (rand());

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

void MainWindow::rPacket(long time, long jt, bool timeout, int type, int code, int len, char ttl, short sn, int tos)
{
    QString tout = "timeout";
    QString tm = QString::number((u_int) len) + \
            " bytes. icmp_seq=" + QString::number((u_short) sn) + \
            " ttl=" + QString::number((u_char) ttl) + \
            " time=" + QString::number((u_long) time) +
            "ms jitter=" + QString::number((u_long) jt) + \
            " tos=" + QString::number((u_int) tos);

    if(timeout)
    {
        pkt_loss++;
        ui->Output->addItem(tout);
        ui->Output->scrollToBottom();
        return;
    }

    if(type == 0 && code == 0)
    {        
        getTos(tos);
        tm.append(tosCode);
        pkt_num++;
        rtt_sum += (u_long) time;
        jitter_sum += (u_long) jt;
        ui->Output->addItem(tm);
        ui->Output->scrollToBottom();
        return;
    }


    getIcmpError(type,code);
    pkt_loss++;
    ui->Output->addItem(err);
    ui->Output->scrollToBottom();

    return;


}

void MainWindow::ePing()
{        
    QString mid_res;
    u_long pkt_res;
    float percent = ((float)pkt_num/((float)pkt_num+(float)pkt_loss))*100;

    if(pkt_num == 0)
        pkt_res = 1;
    else
        pkt_res = pkt_num;

    mid_res = "success="+QString::number(percent)+"% ("+QString::number(pkt_num)+ "/" + QString::number(pkt_num+pkt_loss)+\
            ") avg rtt=" + QString::number(rtt_sum/pkt_res) + \
            "; avg jitter=" + QString::number(jitter_sum/pkt_res);

    free(pkt);
    ui->groupBox->setEnabled(true);
    ui->groupBox_2->setEnabled(true);
    ui->groupBox_3->setEnabled(true);
    ui->pause->setEnabled(false);
    ui->pause->setText("Pause ||");
    ui->ping->setText("Ping");    
    ui->Output->addItem(delimiter);
    ui->Output->addItem(mid_res);
    ui->Output->addItem(delimiter);
    ui->Output->scrollToBottom();

}

void MainWindow::pause_click()
{
    QString mid_res;
    u_long pkt_res;
    float percent = ((float)pkt_num/((float)pkt_num+(float)pkt_loss))*100;

    if(pkt_num == 0)
        pkt_res = 1;
    else
        pkt_res = pkt_num;

    mid_res = "success="+QString::number(percent)+"% ("+QString::number(pkt_num)+ "/" + QString::number(pkt_num+pkt_loss)+\
            ") avg rtt=" + QString::number(rtt_sum/pkt_res) + \
            "; avg jitter=" + QString::number(jitter_sum/pkt_res);

    if(ui->pause->text() == "Pause ||")
    {
        snd->pause = true;
        ui->pause->setText("Pause >");
        ui->Output->addItem(delimiter);
        ui->Output->addItem(mid_res);
        ui->Output->addItem(delimiter);
        ui->Output->scrollToBottom();

    } else {
        snd->pause = false;
        wait.wakeAll();
        ui->pause->setText("Pause ||");
    }
}

void MainWindow::radio_chg(bool chg)
{    
    ui->dscpBox->setEnabled(chg);
    ui->ipTos->setEnabled(!chg);

    return;
}

bool MainWindow::isIP(QString * host)
{
    QStringList ipList;
    bool ok;
    u_long bt;


    ipList = host->split(".");
    for(int i = 0; i<ipList.size(); ++i)
    {
        bt = ipList.at(i).toInt(&ok);        
        if(ok && bt < 255)
            continue;

        return false;
    }

    return true;

}

void MainWindow::getIcmpError(int type, int code)
{
    err.clear();
    switch(type)
    {
        case 3:
            switch(code)
            {
                case 1:
                    err.append("Host Unreachable");
                    break;
                case 2:
                    err.append("Protocol Unreachable");
                    break;
                case 3:
                    err.append("Port Unreachable");
                    break;
                case 4:
                    err.append("Fragmentation Needed and Don't Fragment was Set");
                    break;
                default:
                    err.append("Destination unreacheble");
                    break;
            }
            break;
        case 11:
            switch(code)
            {
                case 0:
                    err.append("Time to live exceeded in transit");
                    break;
                case 1:
                    err.append("Fragment reassembly time exceeded");
                    break;
                default:
                    err.append("Time Exceeded Message");
                    break;
            }
            break;
        case 12:
            err.append("Parameter Problem Message");
            break;

        case 4:
            err.append("Source Quench Message");
            break;

        case 5:
            switch(code)
            {
                case 0:
                    err.append("Redirect datagrams for the Network");
                    break;
                case 1:
                    err.append("Redirect datagrams for the Host");
                    break;
                case 2:
                    err.append("Redirect datagrams for the Type of Service and Network");
                    break;
                case 3:
                    err.append("Redirect datagrams for the Type of Service and Host");
                    break;
                default:
                    err.append("Redirect Message");
                    break;
            }
            break;

        default:
            err.append("Unknown error. type="+QString::number(type)+" code="+QString::number(code));
    }

    return;
}

void MainWindow::getTos(int tos)
{
    tosCode.clear();
    switch(tos)
    {
        case 32:
            tosCode.append(" cs1");
            break;
        case 40:
            tosCode.append(" af11");
            break;
        case 48:
            tosCode.append(" af12");
            break;
        case 56:
            tosCode.append(" af13");
            break;
        case 64:
            tosCode.append(" cs2");
            break;
        case 72:
            tosCode.append(" af21");
            break;
        case 80:
            tosCode.append(" af22");
            break;
        case 88:
            tosCode.append(" af23");
            break;
        case 96:
            tosCode.append(" cs3");
            break;
        case 104:
            tosCode.append(" af31");
            break;
        case 112:
            tosCode.append(" af32");
            break;
        case 120:
            tosCode.append(" af33");
            break;
        case 128:
            tosCode.append(" cs4");
            break;
        case 136:
            tosCode.append(" af41");
            break;
        case 144:
            tosCode.append(" af42");
            break;
        case 152:
            tosCode.append(" af43");
            break;
        case 160:
            tosCode.append(" cs5");
            break;
        case 184:
            tosCode.append(" ef");
            break;
        case 192:
            tosCode.append(" cs6");
            break;
        case 224:
            tosCode.append(" cs7");
            break;
        default:
            tosCode.append(" ");

    }
}
