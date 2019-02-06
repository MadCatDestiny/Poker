#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <mainwindow.h>
class QTextEdit;
class QLineEdit;

class MyClient : public QWidget {
Q_OBJECT
private:
    QTcpSocket* m_pTcpSocket;
    QTextEdit*  m_ptxtInfo;
    QLineEdit*  m_ptxtInput;
    quint16     m_nNextBlockSize;
    QString     host;
    int         port;
    MainWindow* m_wind;
public:
    MyClient(const QString& strHost, int nPort, QWidget* pwgt ) ;
     int parse (QString str);
signals:
     void new_user();
private slots:
    void slotReadyRead   ();
    void slotError       (QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected   ();
    void slotConnect     ();
    void closeEvent(QCloseEvent*);

};

#endif // MYCLIENT_H
