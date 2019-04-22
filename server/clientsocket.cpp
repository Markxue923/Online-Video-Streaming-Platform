#include "clientsocket.h"
#include "socketmanager.h"
#include "sqldaoimp.h"
#include "room.h"
#include <QDebug>
#include <QVector>

//QVector<QString> room_num;
QVector<Room> rooms;

ClientSocket::ClientSocket(QTcpSocket* socket,QObject *parent)
    : QObject(parent),socket(socket)
{
    QObject::connect(this->socket,&QTcpSocket::readyRead,
                     this,&ClientSocket::onReadyRead,
                     Qt::BlockingQueuedConnection);
    QObject::connect(this->socket,&QTcpSocket::disconnected,
                     this,&ClientSocket::onDisConnected,
                     Qt::BlockingQueuedConnection);
}

void ClientSocket::onReadyRead()
{
    Protocol p;
    while(socket->read(p.toChar(),p.size())){
        switch(p.getType()){
        case Protocol::regist:
            qDebug()<<"regist";
            regist(p);
            break;
        case Protocol::login:
            login(p);
            break;
        case Protocol::chat:
            chat(p);
            break;
        case Protocol::quit:
            quit(p);
            break;
        case Protocol::sql:
            sql(p);
            break;
        case Protocol::create_room:
            create_room(p);
            break;
        case Protocol::refresh_room:
            refresh_room(p);
            break;
        case Protocol::enter_anchor:
            enter_anchor(p);
            break;
        case Protocol::anchor_quit:
            anchor_quit(p);
            break;
        case Protocol::refresh_client:
            refresh_client(p);
            break;
        case Protocol::client_quit:
            client_quit(p);
            break;

        default:
            break;
        }
    }
}

void ClientSocket::onDisConnected()
{
    SocketManager* manager = SocketManager::getInstance();
    manager->deleteSocket(socket);
}


void ClientSocket::regist(Protocol &p)
{
    QString name = p.getName();
    QString pwd = p.getPwd();

    User user(name,pwd);

    SqlDao* sd = new SqlDaoImp;
    User* ret = sd->selectUser(user);

    if(ret==NULL){
        sd->insertUser(user);
        //注册成功
        p.setType(Protocol::regist_success);
        p.setData("Regist success");
    }else{
        //重复注册
        p.setType(Protocol::regist_failed);
        p.setData("Regist fail");
    }

    emit sigSend(socket,p.toArray());

    delete sd;
}

void ClientSocket::login(Protocol &p)
{
    QString name = p.getName();
    QString pwd = p.getPwd();

    User user(name,pwd);

    SqlDao* sd = new SqlDaoImp;
    User* ret = sd->selectUser(user);

    if(ret==NULL){
        p.setType(Protocol::login_failed);
        p.setData("User does not exist");
    }else{
        if(ret->getIsOnline()){
            p.setType(Protocol::login_failed);
            p.setData("User already logged in");
        }else{
            if(ret->getPwd() == pwd){
                p.setType(Protocol::login_success);
                p.setData("Login success");
                ret->setIsOnline(true);
                sd->updateUser(*ret);
            }else{
                p.setType(Protocol::login_failed);

                p.setData("Wrong password");
            }
        }
        delete ret;
    }
    emit sigSend(socket,p.toArray());

    delete sd;
}

void ClientSocket::chat(Protocol &p)
{
    SqlDao* sd = new SqlDaoImp;

    User* ret = sd->selectUser(User(p.getName(),p.getPwd()));
    if(ret==NULL) return;


    QVector<Room>::iterator it=rooms.begin();
    for(;it!=rooms.end();++it)
    {
        if(it->get_room_name() == p.getName())
        {
            Room& room=*it;
            QVector<QTcpSocket*>& users=room.return_Users();
            QVector<QTcpSocket*>::iterator it_users=users.begin();
            QString text = p.getPwd();
            text += " : ";
            text += p.getData();
            p.setData(text);
            p.setType(Protocol::chat);
            for(auto it_users :users)
            {
               emit sigSend(it_users,p.toArray());
            }
        }
    }
    delete ret;
    delete sd;
}

void ClientSocket::quit(Protocol &p)
{
    SqlDao* sd = new SqlDaoImp;

    User* ret = sd->selectUser(User(p.getName(),p.getPwd()));
    if(ret == 0)    return;

    ret->setIsOnline(false);
    sd->updateUser(*ret);

    delete sd;
    delete ret;
}

void ClientSocket::sql(Protocol &p)
{
    SqlDao* sd = new SqlDaoImp;

    sd->exec(p.getData());

    delete sd;
}

void ClientSocket::create_room(Protocol &p){
    qDebug()<<"create";
    Room room;
    room.set_room_name(p.getName());
    QVector<QTcpSocket*>& users=room.return_Users();
    QVector<QString>& users_name=room.return_users_name();
    users_name.push_back(p.getName());
    users.push_back(socket);
    rooms.push_back(room);
    p.setName(p.getName());
    p.setType(Protocol::create_success);
    emit sigSend(socket,p.toArray());
}

void ClientSocket::refresh_room(Protocol &p){
    QVector<Room>::iterator it=rooms.begin();
    QString text;
    for(;it!=rooms.end();it++){
        text+=it->get_room_name();
        text+='\n';
    }
    text.chop(1);
    p.setType(Protocol::printf_room);
    p.setData(text);
    emit sigSend(socket,p.toArray());
}

void ClientSocket::enter_anchor(Protocol &p){
    QVector<Room>::iterator it;
    QString anchor_name=p.getData();
    QString client_name=p.getName();
    for(it=rooms.begin();it!=rooms.end();++it)
    {
        if(it->get_room_name() == anchor_name){
            Room& room=*it;
            QVector<QTcpSocket*>& users=room.return_Users();
            QVector<QString>& users_name=room.return_users_name();
            users_name.push_back(client_name);
            users.push_back(socket);
            p.setName(client_name);
            p.setData(anchor_name);
            p.setType(Protocol::enter_anchor_success);
            emit sigSend(socket,p.toArray());
        }
    }
}

void ClientSocket::anchor_quit(Protocol &p){
    QVector<Room>::iterator it=rooms.begin();
    while(it!=rooms.end()){
        if(p.getName()==(it->get_room_name())){
            rooms.erase(it);
        }else{
            ++it;
        }
    }
}

void ClientSocket::refresh_client(Protocol &p){
    QVector<Room>::iterator it;
    for(it=rooms.begin();it!=rooms.end();++it){
        if(it->get_room_name() == p.getName()){
            Room& room = *it;
            QVector<QString>& users_name=room.return_users_name();
            QVector<QString>::iterator it_users=users_name.begin();
            QString text;
            for(;it_users!=users_name.end();++it_users){
                text += *it_users;
                text += "\n";
            }
            text.chop(1);
            p.setType(Protocol::printf_client);
            p.setData(text);
            emit sigSend(socket,p.toArray());
        }
    }
}

void ClientSocket::client_quit(Protocol &p){
    QVector<Room>::iterator it;
    for(it=rooms.begin();it!=rooms.end();++it)
    {
        if(it->get_room_name() == p.getName())
        {
            Room& room=*it;
            QString client_name=p.getData();
            QVector<QTcpSocket*>& users=room.return_Users();
            QVector<QString>& users_name=room.return_users_name();
            QVector<QTcpSocket*>::iterator it_users=users.begin();
            QVector<QString>::iterator it_users_name=users_name.begin();
            while(it_users_name!=users_name.end())
            {
                if(client_name==*(it_users_name))
                {
                    users_name.erase(it_users_name);
                }else
                    ++it_users_name;
            }
            while(it_users!=users.end())
            {
                if(socket == *it_users)
                {
                    users.erase(it_users);
                }else
                    ++it_users;
            }
        }
    }
}



