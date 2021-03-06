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
    connect(epl, SIGNAL(sigLedConnected()), this, SLOT(eplConnected()));
    connect(epl, SIGNAL(sigLedDisconnected()), this, SLOT(eplDisconnected()));

    connect(ui->monthSchedule, SIGNAL(toggled(bool)), ui->fromMonth, SLOT(setDisabled(bool)));
    connect(ui->monthSchedule, SIGNAL(toggled(bool)), ui->toMonth, SLOT(setDisabled(bool)));
    connect(ui->daySchedule, SIGNAL(toggled(bool)), ui->fromDay, SLOT(setDisabled(bool)));
    connect(ui->daySchedule, SIGNAL(toggled(bool)), ui->toDay, SLOT(setDisabled(bool)));
    connect(ui->timeSchedule, SIGNAL(toggled(bool)), ui->fromHTime, SLOT(setDisabled(bool)));
    connect(ui->timeSchedule, SIGNAL(toggled(bool)), ui->fromMTime, SLOT(setDisabled(bool)));
    connect(ui->timeSchedule, SIGNAL(toggled(bool)), ui->toHTime, SLOT(setDisabled(bool)));
    connect(ui->timeSchedule, SIGNAL(toggled(bool)), ui->toMTime, SLOT(setDisabled(bool)));
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
    case 0: inAnim = "\\edpc"; break;
    case 1: inAnim = "\\egpc"; break;
    case 2: inAnim = "\\ehpc"; break;
    case 3: inAnim = "\\ebpc"; break;
    case 4: inAnim = "\\ecpc"; break;
    case 5: inAnim = "\\eepc"; break;
    case 6: inAnim = "\\erpc"; break;
    case 7: inAnim = "\\eipc"; break;
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
    else if (ui->stopTime->value() < 10)
    {
        stopTime = "\\a" + QString::number(ui->stopTime->value());
    }
    else
    {
        stopTime = "\\a0" + QString::number(ui->stopTime->value());
    }

    return stopTime;
}

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
//    QString msgTableText;
//    QByteArray msg;
//    msg.append(ui->msgTable->currentItem()->text());
//    msgTableText.append(QString::fromRawData(ui->msgTable->currentItem()->text(), sizeof(QChar)));

    message = getMsgBank() + getFont() + getEffect() + getSpeed() + getInAnim() + "   " + ui->msgTable->currentItem()->text() + "   " + getTimeStop() + getOutAnim();
    return message;
}


// ******************* SLOTS ********************* //

void MainWindow::on_display_clicked()
{
    if (epl->isConnected())
    {
        if (!ui->msgTable->selectedItems().isEmpty())
        {
            epl->authorize(ui->msgTable->currentRow() + 1);
            this->updateCurrentMsgNum();
            ui->statusBar->showMessage("Transmission OK !", 5000);
        }
        else
        {
            ui->statusBar->showMessage("Invalid command : select a bank number first !", 5000);
        }
    }
    else
    {
        ui->statusBar->showMessage("Invalid command : select a bank number first !", 5000);
    }
}

void MainWindow::on_save_clicked()
{
    if (epl->isConnected())
    {
        if (!ui->msgTable->selectedItems().isEmpty())
        {
            epl->sendMessage(composeMessage());
            ui->statusBar->showMessage("Transmission OK !", 5000);
        }
        else
        {
            ui->statusBar->showMessage("Action impossible : sélectionnez une mémoire !", 5000);
        }
    }
    else
    {
        ui->statusBar->showMessage("Action impossible : veuillez d'abord vous connecter !", 5000);
    }
}

void MainWindow::on_connect_clicked()
{
    if (ui->connect->isChecked())
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
            case 5: epl->connection("192.168.5.74", 23); break;
            case 6: epl->connection("192.168.5.55", 23); break;
            case 7: epl->connection("192.168.5.57", 23); break;
            case 8: epl->connection("192.168.5.56", 23); break;
            case 9: epl->connection("192.168.5.59", 23); break;
            case 10: epl->connection("192.168.5.66", 23); break;
            case 11: epl->connection("192.168.5.65", 23); break;
            case 12: epl->connection("192.168.5.60", 23); break;
            case 13: epl->connection("192.168.5.62", 23); break;
            case 14: epl->connection("192.168.5.64", 23); break;
            case 15: epl->connection("192.168.5.67", 23); break;
            case 16: epl->connection("192.168.5.72", 23); break;
            }
        }
        else
        {
            epl->connection(ui->custom_ip->text(), 23);
        }

        if (!epl->isConnected())
        {
           ui->connect->setChecked(false);
            ui->statusBar->showMessage("Impossible de se connecter... vérifiez vos paramètres et votre connexion réseau.", 5000);
        }

    }
    else
    {
        epl->closeConnection();
    }
}

void MainWindow::on_freezeMode_clicked()
{
    if (epl->isConnected())
    {
        epl->freeze();
    }
    else
    {
        ui->statusBar->showMessage("Action impossible : veuillez d'abord vous connecter !", 5000);
    }
}

void MainWindow::on_runMode_clicked()
{
    if (epl->isConnected())
    {
        epl->run();
    }
    else
    {
        ui->statusBar->showMessage("Action impossible : veuillez d'abord vous connecter !", 5000);
    }
}

void MainWindow::on_clearDevice_clicked()
{
    if (epl->isConnected())
    {
        epl->clear();
        ui->msgTable->clearContents();
    }
    else
    {
        ui->statusBar->showMessage("Action impossible : veuillez d'abord vous connecter !", 5000);
    }
}

void MainWindow::on_rebootDevice_clicked()
{
    if (epl->isConnected())
    {
        epl->reboot();
    }
    else
    {
        ui->statusBar->showMessage("Action impossible : veuillez d'abord vous connecter !", 5000);
    }
}

void MainWindow::on_ledIntensity_sliderMoved(int position)
{
    if (epl->isConnected())
    {
        epl->brightness(position);
    }
    else
    {
        ui->statusBar->showMessage("Action impossible : veuillez d'abord vous connecter !", 5000);
    }
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
    }
}

void MainWindow::updateCurrentMsgNum()
{
  QString msgNum;
  msgNum = epl->getCurrentMsgNumber();

  int msgIndex;
  msgIndex = msgNum.toInt();

  for (int i = 0; i < 10 ; i++)
  {
      ui->msgTable->setVerticalHeaderItem(i,new QTableWidgetItem(QString::number(i + 1)));
  }

  ui->msgTable->setVerticalHeaderItem(msgIndex - 1,new QTableWidgetItem(QString::number(msgIndex) + " *"));
}

// ****************** SLOTS ********************* //

void MainWindow::udpDatagramReceived(QString datagram)
{
    int value = datagram.remove(0,ui->triggerDisplay->text().size()).toInt();
    epl->authorize(value);
}

void MainWindow::eplConnected()
{
    for (int i=0 ; i<10 ; i++)
    {
        QString resp;
        resp = epl->getStoredMessage(i+1);
        resp.remove(0, resp.indexOf("   ") + 3);
        resp.truncate(resp.indexOf("   "));
        ui->msgTable->setItem(i, 0, new QTableWidgetItem(resp));
    }
    ui->ip->setEnabled(false);
    ui->custom_ip->setEnabled(false);
    ui->connect->setChecked(true);
    ui->connect->setText("Déconnecter");
    ui->statusBar->showMessage("Connecté au journal !", 5000);
    this->updateCurrentMsgNum();
}

void MainWindow::eplDisconnected()
{
    for (int i = 0; i < 10 ; i++)
    {
        ui->msgTable->setVerticalHeaderItem(i,new QTableWidgetItem(QString::number(i + 1)));
    }
    ui->ip->setEnabled(true);
    ui->custom_ip->setEnabled(true);
    ui->connect->setChecked(false);
    ui->connect->setText("Connecter");
    ui->msgTable->clearContents();
    ui->statusBar->showMessage("Journal déconnecté !", 5000);
}

void MainWindow::on_clrBank_clicked()
{
    if (epl->isConnected())
    {
        if (!ui->msgTable->selectedItems().isEmpty())
        {
            epl->clearBank(ui->msgTable->currentRow() + 1);
            ui->msgTable->currentItem()->setText("");
            epl->run();
        }
        else
        {
            ui->statusBar->showMessage("Action impossible : sélectionnez une mémoire !", 5000);
        }
    }
    else
    {
        ui->statusBar->showMessage("Action impossible : veuillez d'abord vous connecter !", 5000);
    }
}


void MainWindow::on_authorizeAll_clicked()
{
    if (epl->isConnected())
    {
        epl->authorizeAll();
    }
    else
    {
        ui->statusBar->showMessage("Action impossible : veuillez d'abord vous connecter !", 5000);
    }
}

void MainWindow::on_authorizeAll_2_clicked()
{
    if (epl->isConnected())
    {
        epl->authorizeAll();
    }
    else
    {
        ui->statusBar->showMessage("Action impossible : veuillez d'abord vous connecter !", 5000);
    }

}

void MainWindow::on_blank_clicked()
{
    if (epl->isConnected())
    {
        epl->blank();
    }
    else
    {
        ui->statusBar->showMessage("Action impossible : veuillez d'abord vous connecter !", 5000);
    }

}

void MainWindow::on_clearSchedule_clicked()
{
    if (epl->isConnected())
    {

        epl->clearSchedule(ui->msgTable->currentRow() + 1);
    }
    else
    {
        ui->statusBar->showMessage("Action impossible : veuillez d'abord vous connecter !", 5000);
    }
}

void MainWindow::on_saveIP_clicked()
{
    if (epl->isConnected())
    {
        epl->setIpAddress(ui->deviceIP->text(), ui->deviceMask->text(), ui->deviceGateway->text());
    }
    else
    {
        ui->statusBar->showMessage("Action impossible : veuillez d'abord vous connecter !", 5000);
    }
}

