#include <QWidget>
#include <QVBoxLayout>

class ui_aboutComponent1;
class ui_aboutComponent2;

class About : public QWidget
{
        Q_OBJECT
public:
    static About* getInstance(QWidget* parent = nullptr);
    ui_aboutComponent1* quitCom;
    ui_aboutComponent2* storeCom;
    ui_aboutComponent2* versionCom;
    ui_aboutComponent2* webCom;
    void setStyle(QString style);
protected:
    bool event(QEvent *event) override;
private slots:
    void openWeb();
    void modifyDefaultStorePath();
    void openDefaultStorePath();
    void checkUpdate();
    void swithQuitModeToQuit();
    void swithQuitModeToMin();
private:
    About(QWidget *parent = nullptr);
    About(const About&) = delete;
    About& operator=(const About&) = delete;
    static About *m_pAboutPage;
    QVBoxLayout* m_mainBoxLayout;
    void connectedToSlot();



};
