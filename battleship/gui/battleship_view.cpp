#include "battleship_view.h"
#include "common/model_interface.h"
#include "common/controller_interface.h"
#include "playerform.h"
#include <QMessageBox>

#include <QDebug>

/**
 * Application begins with PlayerForm Dialog asking user for his name
 */
GUI::BattleshipView::BattleshipView(ModelInterface& model, ControllerInterface& ctrl)
    : model{model}, ctrl {ctrl}
{
    auto* playerForm = new PlayerForm{ctrl};
    playerForm->show();
}


void GUI::BattleshipView::showWaitingForGuestConnectingDialog()
{
    auto msgBox = new QMessageBox{};
    topDialog = msgBox; //polymorphism
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle("Hosting game ...");
    msgBox->setText("Waiting for other player to connect ...");
    msgBox->setInformativeText("You can cancel and try to connect yourself to other available hosts");
    msgBox->setStandardButtons(QMessageBox::Cancel);
    msgBox->setDefaultButton(QMessageBox::Cancel);
    
    QObject::connect(msgBox, &QDialog::finished, [&] (int result) {
        qDebug() << "result: " << result; 
        if (result != -1) { //closed by user
            model.cancelHosting(); //direct access to model, QMessageBox is closed anyway
        }
        topDialog = nullptr;
    });

    msgBox->show();
//     msgBox->done(0);
//     msgBox->exec();
//     closeTopDialog();
}

void GUI::BattleshipView::closeTopDialog()
{
    if (topDialog) {
        topDialog->done(-1);
        //setting topDialog pointer to nullptr is done anyway in &QDialog::finished signal -> lambda [&] () { topDialog = nullptr; }
    }
}

void GUI::BattleshipView::showErrorMessage(const std::string& msg, const std::string& title)
{
    auto msgBox = new QMessageBox{};
    topDialog = msgBox;
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle(QString::fromStdString(title));
    msgBox->setText(QString::fromStdString(msg));
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->setDefaultButton(QMessageBox::Ok);
    
    QObject::connect(msgBox, &QDialog::finished, [&] (int result) {
        topDialog = nullptr;
    });

    msgBox->show();
}

void GUI::BattleshipView::shipPlacementStarted()
{
    qDebug() << "void GUI::BattleshipView::shipPlacementStarted()";
}






