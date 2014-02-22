#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    epl = new ledBar(this);
    udpReceiver = new UDPClient(this);
    connect(udpReceiver, SIGNAL(sigUdpDataReceived(QString)), this, SLOT(udpDatagramReceived(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getFont()
{
    QString font;
    switch (ui->font->currentIndex())
    {
    case 0: font = "\\g0"; break;
    case 1: font = "\\g1"; break;
    case 2: font = "\\g2"; break;
    case 3: font = "\\g3"; break;
    case 4: font = "\\g4"; break;
    case 5: font = "\\g5"; break;
    case 6: font = "\\g6"; break;
    case 7: font = "\\g7"; break;
    case 8: font = "\\g8"; break;
    case 9: font = "\\g9"; break;
    }
    return font;
}

QString MainWindow::getInAnim()
{
    QString inAnim;
    switch (ui->inAnim->currentIndex())
    {
    case 0: inAnim = ""; break;
    case 1: inAnim = "\\ed"; break;
    case 2: inAnim = "\\eg"; break;
    case 3: inAnim = "\\eh"; break;
    case 4: inAnim = "\\eb"; break;
    case 5: inAnim = "\\ec"; break;
    case 6: inAnim = "\\ee"; break;
    case 7: inAnim = "\\er"; break;
    case 8: inAnim = "\\ei"; break;
    }
    return inAnim;
}

QString MainWindow::getOutAnim()
{
    QString outAnim;
    switch (ui->outAnim->currentIndex())
    {
    case 0: outAnim = ""; break;
    case 1: outAnim = "\\sd"; break;
    case 2: outAnim = "\\sg"; break;
    case 3: outAnim = "\\sh"; break;
    case 4: outAnim = "\\sb"; break;
    case 5: outAnim = "\\sc"; break;
    case 6: outAnim = "\\se"; break;
    case 7: outAnim = "\\sr"; break;
    case 8: outAnim = "\\si"; break;
    }
    return outAnim;
}

QString MainWindow::getSpeed()
{
    QString speed;
    speed = "\\k" + QString::number(ui->animSpeed->value());
    return speed;
}

QString MainWindow::getTimeStop()
{
    QString stopTime;
    if (ui->stopTime->value() == 0)
    {
        stopTime = "";
    }
    else
    {
        stopTime = "\\a" + QString::number(ui->stopTime->value());
    }
    return stopTime;
}

//QString MainWindow::getBlink()
//{

//}

QString MainWindow::getEffect()
{
    QString effect;
    switch (ui->effect->currentIndex())
    {
    case 0: effect = ""; break;
    case 1: effect = "\\w"; break;
    case 2: effect = "'-"; break;
    }
    return effect;
}

QString MainWindow::getMsgBank()
{
    QString msgBank;
    if (ui->msgTable->currentRow() < 10)
    {
        msgBank = "MSG0" + QString::number(ui->msgTable->currentRow() + 1) + " |";
    }
    else
    {
        msgBank = "MSG" + QString::number(ui->msgTable->currentRow() + 1) + " |";
    }
    return msgBank;
}

QString MainWindow::composeMessage()
{
    QString message;
    message = getMsgBank() + getFont() + getEffect() + getSpeed() + getInAnim() + "/ " + ui->msgTable->currentItem()->text() + " /" + getTimeStop() + getOutAnim() + "\r\n";
    return message;
}



// ******************* SLOTS ********************* //

void MainWindow::on_display_clicked()
{
    epl->authorize(ui->msgTable->currentRow() + 1);
}

void MainWindow::on_save_clicked()
{

    if (!ui->msgTable->selectedItems().isEmpty())
    {
        epl->sendMessage(composeMessage());
    }
    else
    {
        qDebug() << "Sélectionnez une mémoire !";
    }
}

void MainWindow::on_connect_clicked()
{
    if (ui->custom_ip->text() == "")
    {
        switch (ui->ip->currentIndex())
        {
        case 0: epl->connection("192.168.5.54", 23); break;
        case 1: epl->connection("192.168.5.53", 23); break;
        case 2: epl->connection("192.168.5.52", 23); break;
        case 3: epl->connection("192.168.5.51", 23); break;
        case 4: epl->connection("192.168.5.50", 23); break;
        }
    }
    else
    {
        epl->connection(ui->custom_ip->text(), 23);
    }

    if (epl->isConnected())
    {
        for (int i=0 ; i<10 ; i++)
        {
            QString resp;
            resp = epl->getStoredMessage(i+1);
            resp.remove(0, resp.indexOf("/") + 1);
            resp.truncate(resp.lastIndexOf("/"));
            ui->msgTable->setItem(i, 0, new QTableWidgetItem(resp));
        }
    }
}

void MainWindow::on_freezeMode_clicked()
{
    epl->freeze();
}

void MainWindow::on_runMode_clicked()
{
    epl->run();
}

void MainWindow::on_clearDevice_clicked()
{
    epl->clear();
}

void MainWindow::on_rebootDevice_clicked()
{
    epl->reboot();
}

void MainWindow::on_ledIntensity_sliderMoved(int position)
{
    epl->brightness(position);
}

void MainWindow::udpDatagramReceived(QString datagram)
{
    int value = datagram.remove(0,ui->triggerDisplay->text().size()).toInt();
    epl->authorize(value);
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Unchecked)
    {
        ui->groupTriggersConfig->setEnabled(false);
        udpReceiver->disconnect();
    }
    else if (arg1 == Qt::Checked)
    {
        ui->groupTriggersConfig->setEnabled(true);
        QString port = ui->triggerPort->text();
        udpReceiver->bind(port.toInt());
        qDebug() << "UDP socket bind to Localhost on port " << port.toInt();
    }
}
