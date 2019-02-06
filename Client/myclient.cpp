#include "myclient.h"

MyClient::MyClient(const QString& strHost,
                   int            nPort,
                   QWidget*       pwgt /*=0*/
                  ) : QWidget(pwgt)
                    , m_nNextBlockSize(0),host(strHost),port(nPort)
{

    m_ptxtInfo  = new QTextEdit;
    m_ptxtInput = new QLineEdit;
    m_ptxtInfo->setReadOnly(true);

    QPushButton* cli_con = new QPushButton("&Connect");
    QPushButton* pcmd = new QPushButton("&Send");
    connect(pcmd, SIGNAL(clicked()), SLOT(slotSendToServer()));
    connect(cli_con,SIGNAL(clicked()),SLOT (slotConnect()));

    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Client</H1>"));
    pvbxLayout->addWidget(m_ptxtInfo);
    pvbxLayout->addWidget(m_ptxtInput);
    pvbxLayout->addWidget(pcmd);
    pvbxLayout->addWidget(cli_con);
    setLayout(pvbxLayout);
}

void MyClient::slotReadyRead()
{
    /*QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }*/

        QTime   time = time.currentTime();
        QString str = m_pTcpSocket->readAll();
        int num = this->parse(str);
      //  in >> time >> str;

        m_ptxtInfo->append(time.toString() + " " + str);
        m_nNextBlockSize = 0;
   }

void MyClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(m_pTcpSocket->errorString())
                    );
    m_ptxtInfo->append(strError);
}

void MyClient::slotSendToServer()
{
    m_pTcpSocket->write(m_ptxtInput->text().toStdString().c_str());
    m_ptxtInput->setText("");
}

void MyClient::slotConnected()
{
    m_wind->show();
    m_ptxtInfo->append("Received the connected() signal");
}
void MyClient::slotConnect()
{
    m_pTcpSocket = new QTcpSocket(this);
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,         SLOT(slotError(QAbstractSocket::SocketError)));
    connect(m_ptxtInput, SIGNAL(returnPressed()),
            this,        SLOT(slotSendToServer()));

   // connect(this,SIGNAL(new_user()),m_wind,SLOT(addPlayer()));
    m_pTcpSocket->connectToHost(host, port);
    m_wind = new MainWindow(m_pTcpSocket);
    connect(m_wind,SIGNAL(closed()),m_pTcpSocket,SLOT(disconnectFromHost()));
}

void MyClient::closeEvent(QCloseEvent *event)
{
    this->m_wind->close();
}

int MyClient::parse (QString str)
{
    if (str.contains("COUNT 0", Qt::CaseInsensitive))
        m_wind->set_num(1);
    if (str.contains("COUNT 1", Qt::CaseInsensitive))
    {
        //emit new_user();
        if(str.contains("START", Qt::CaseInsensitive))
        {
            str.remove(0,8);

        }
       m_wind->addPLayer();
       m_ptxtInfo->append("New player!");
       m_wind->set_num(2);
       m_wind->set_game(str); //test mode
    }
    if (str.contains("START", Qt::CaseInsensitive))
        m_wind->set_game(str); //test mode
    if(str.contains("RAISE", Qt::CaseInsensitive))
    {
        QStringList list = str.split(" ");
        int rate = list[1].toInt();
        m_wind->rival_raise(rate);
    }
    if (str.contains("CHECK", Qt::CaseInsensitive))
    {
        m_wind->rival_chek();
    }
    if (str.contains("FOLD", Qt::CaseInsensitive))
    {
        m_wind->rival_fold();
    }
    if (str.contains("WIN", Qt::CaseInsensitive) || str.contains("LOSE", Qt::CaseInsensitive))
    {
        m_wind->win(str);
    }
    return 0;
}
