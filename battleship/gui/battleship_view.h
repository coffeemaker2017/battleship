#ifndef BATTLESHIPVIEW_H
#define BATTLESHIPVIEW_H

#include <string>
#include "common/battleship_observer.h"
#include <vector>

class ModelInterface;
class ControllerInterface;
class QDialog;
class QWidget;

namespace GUI {
/// A simple class that defines and argument exception
class BattleshipView : public BattleshipObserver
{
public:
    explicit BattleshipView(ModelInterface& model, ControllerInterface& ctrl);
    
    /**
     * @param msg actual error message message
     * @param title title of the window like QMessageBox
     */
    void showErrorMessage(const std::string& msg, const std::string& title =  "Oops, something went wrong");
    
    /**
     * inform user that he has to be patient. Being patient is always good... well at least for someone.
     */
    void showWaitingForGuestConnectingDialog();
    
    /**
     * closes top dialog programmatically, also without user having to click. Such dialog can be a popup informing about errors.
     * Top dialogs are informative popups, like QMessageBox, they don't affect BattleshipView#closeAllCurrentWindows()
     */
    void closeTopDialog();
    
    /**
     * close windows, what are stored by BattleshipView#addWidget()
     */
    void closeAllCurrentWindows();
    
    /**
     * adds pointer/holder of widget, so it can be closed later by BattleshipView#closeAllCurrentWindows()
     */
    void addWidget(QWidget* widget);
    

    void onGameStart(const std::deque<MODEL::Ship> & me, const std::deque<MODEL::Ship> & enemy, bool myTurn) override;
    
    /**
     * @see BattleshipObserver#onRcvUserInfo(const UserInfo&)
     */
    void onRcvUserInfo(const UserInfo& me, const UserInfo& enemy) override;
    
//     void shipPlacementStarted();
    
private:
    ModelInterface& model;
    ControllerInterface& ctrl;
    
    QDialog* topDialog{ nullptr };
    std::vector<QWidget*> currentWindowList;
};

} // end NS
#endif // BATTLESHIPVIEW_H
