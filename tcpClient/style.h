#ifndef STYLE_H
#define STYLE_H
#include <QString>

const QString QMenu_Item_selected ="QMenu::item{background:rgba (82,130,164,1);border:1px solid rgba (82,130,164,1);}";

const QString m_stylesheet_QPushButton = ".QPushButton {color:white;width:70px;height:30px;font-family:Microsoft YaHei;font-size:14px;background-color:#0c7af2;border:none; border-radius: 5px;padding: 2px;}\
                                             .QPushButton:pressed {background-color: rgba(64,65,66,0.5);}\
    .QPushButton:hover {background-color: #3f91fc;}\
    .QPushButton:focus{outline: none;}";


const QString m_LogoWidget_style = ".LogoWidget{border:none;margin:0px;padding:0px;} .PushButton{width:32px;height:32px;border:none;color:white;}";


const QString m_filePage_style = ".QListWidget{background:#272a30;border-radius:10px;} \
                                  .QListWidget::item{background:#272a30;height:35px;color:#a8abaf;font-family:Microsoft YaHei;font-size:14px;border:none; padding: 5px 5px 5px 5px;} \
                                .QListWidget::item:selected{border:1px dashed #fff7d6;border-radius:10px;} \
                                  .QListWidget:focus{outline:none;} \
                                 .QPushButton {color:white;width:70px;height:30px;font-family:Microsoft YaHei;font-size:14px;background-color:#0c7af2;border:none; border-radius: 5px;padding: 2px;}\
                                  .QPushButton:pressed {background-color: rgba(64,65,66,0.5);}\
                                 .QPushButton:hover {background-color: #3f91fc;}\
                                 .QPushButton:focus{outline: none;}";
const QString m_filePage_Light_style = ".QListWidget{background:#f5f5f5;border-radius:10px;border:1px solid black;} \
                                     .QListWidget::item{background:#f5f5f5;height:35px;color:#000000;font-family:Microsoft YaHei;font-size:14px;border:none; padding: 5px 5px 5px 5px;} \
                                     .QListWidget::item:selected{border-bottom:1px solid #000000;} \
                                     .QListWidget:focus{outline:none;} \
                                    .QPushButton {color:white;width:70px;height:30px;font-family:Microsoft YaHei;font-size:14px;background-color:#0c7af2;border:none; border-radius: 5px;padding: 2px;}\
                                    .QPushButton:pressed {background-color: rgba(64,65,66,0.5);}\
                                    .QPushButton:hover {background-color: #3f91fc;}\
                                     .QPushButton:focus{outline: none;}";
const QString m_opeWidget_style = "*{border:none;padding: 0px;margin:0px;} \
                                      .OpeWidget{background-color:#1e1c1b;color:white;border: none;border-bottom-left-radius: 10px;border-bottom-right-radius: 10px;} \
                                       .QStackWidget{backgroud:#1d1d1f;}\
                                  QListWidget{border:none;background:#1e1c1b;width:80px;padding:5px 20px 5px 5px;border-bottom-left-radius:5px;} \
                                QListWidget::item{background-color:#1e1c1b;height:25px;color:white;font-family:Microsoft YaHei;font-size:12px;border:none; padding: 5px 5px 5px 5px;} \
                                QListWidget::Item:hover{background:#0c7af2;padding-top:0px; padding-bottom:0px;} \
                                QListWidget::Item:selected{background:#0c7af2;padding-top:0px; padding-bottom:0px;border-left:3px solid #ffe26d; } \
                                QListWidget:focus{outline:none;}";
const QString m_opeWidget_Light_style = "*{border:none;padding: 0px;margin:0px;} \
                                      .OpeWidget{background-color:#f0f0f0;color:black;border: none;border-bottom-left-radius: 10px;border-bottom-right-radius: 10px;} \
                                        .QStackWidget{backgroud:#f0f0f0;}\
                                        QListWidget{background:#f0f0f0;width:80px;padding:5px 5px 5px 5px;border-bottom-left-radius:5px;} \
                                        QListWidget::item{background-color:#f0f0f0;height:25px;color:#000000;font-family:Microsoft YaHei;font-size:12px;padding: 5px 5px 5px 5px;} \
                                        QListWidget::Item:hover{border:1px solid #000000;padding-top:0px; padding-bottom:0px;border-radius:10px;} \
                                        QListWidget::Item:selected{border:1px solid #000000;padding-top:0px; padding-bottom:0px;border-radius:10px;} \
                                         QListWidget:focus{outline:none;}";


const QString m_windowflag_style = ".m_WindowFlag{border:none;background:#1e1c1b;border-top-left-radius:5px;border-top-right-radius:5px;} \
    QLabel{color:white;}";
const QString m_windowflag_Light_style = ".m_WindowFlag{border:none;background:#a8a8a8;border-top-left-radius:5px;border-top-right-radius:5px;} \
    QLabel{color:white;}";


const QString m_friendPage_style = ".QPushButton {color:white;width:70px;height:30px;font-family:Microsoft YaHei;font-size:14px;background-color:#0c7af2;border:none; border-radius: 5px;padding: 2px;}\
                                    .QPushButton:pressed {background-color: rgba(64,65,66,0.5);}\
                                    .QPushButton:hover {background-color: #3f91fc;}\
                                    .QPushButton:focus{outline: none;} \
                                    .QListWidget{alignment:center;background:#272a30;border-radius:10px;} \
                                    .QListWidget::item{background:#272a30;height:35px;color:#a8abaf;font-family:Microsoft YaHei;font-size:14px;border:none; padding: 5px 5px 5px 5px;} \
                                    .QListWidget::item:selected{border:1px dashed #fff7d6;border-radius:10px;} \
                                    .QListWidget:focus{outline:none;} \
                                   " ;
const QString m_friendPage_Light_style = ".QPushButton {color:white;width:70px;height:30px;font-family:Microsoft YaHei;font-size:14px;background-color:#0c7af2;border:none; border-radius: 5px;padding: 2px;}\
                                       .QPushButton:pressed {background-color: rgba(64,65,66,0.5);}\
                                       .QPushButton:hover {background-color: #3f91fc;}\
                                       .QPushButton:focus{outline: none;} \
                                       .QListWidget{alignment:center;border:1px solid black;background:#f5f5f5;border-radius:10px;} \
                                       .QListWidget::item{height:35px;color:#000000;font-family:Microsoft YaHei;font-size:14px;border:none; padding: 5px 5px 5px 5px;} \
                                       .QListWidget::item:selected{border-bottom:1px solid #000000;} \
                                       .QListWidget:focus{outline:none;} \
    " ;

const QString m_publicChatWidget_style = ".QTextEdit{background:#272a30;border-radius:10px;color:#a8abaf;font-family:Microsoft YaHei;font-size:14px;border:none; padding: 5px 5px 5px 5px;}";
const QString m_publicChatWidget_Light_style = ".QTextEdit{border:1px solid black;background:#f5f5f5;border-radius:10px;color:#a8abaf;font-family:Microsoft YaHei;font-size:14px;padding: 5px 5px 5px 5px;}";

const QString m_msgWidget_style = ".QPushButton {color:white;font-family:Microsoft YaHei;font-size:14px;background-color:#fed84c;border:none; border-radius: 5px;padding: 2px;}\
                                   .QPushButton:focus{outline: none;} \
                                    QLineEdit {border: 1px solid #A0A0A0;border-radius: 3px; padding-left: 5px; background-color: #F2F2F2;color: #A0A0A0;selection-background-color: #A0A0A0;selection-color: #F2F2F2;font-family:Microsoft YaHei;font-size: 10pt;} \
                                    QLineEdit:hover {border: 1px solid #298DFF;border-radius: 3px;background-color: #F2F2F2;color: #298DFF;selection-background-color: #298DFF;selection-color: #F2F2F2;}";
const QString m_About_Light_style = "*{background:#f5f5f5;border:1px solid black;}\
                                      .ui_aboutComponent1{border:none;} \
                                      .ui_aboutComponent2{border:none;} \
                                     .QWidget{border:none;background:#f5f5f5;border:1px solid black;font-size:14px;font-weight: bold;} \
                                     .QRadioButton{border:none;color:#000000;font-family:Microsoft YaHei;font-size:12px;}                   \
                                     .QTextEdit{border:none;border:0px;width:80px;font-family:Microsoft YaHei;font-size:12px;height:80px;background-color:rgba(0,0,0,0);text-align:center;;}\
                                     .QPushButton {color:white;width:70px;height:30px;font-family:Microsoft YaHei;font-size:14px;background-color:#0c7af2;border:none; border-radius: 5px;padding: 2px;}\
                                    .QPushButton:pressed {background-color: rgba(64,65,66,0.5);}\
                                    .QPushButton:hover {background-color: #3f91fc;}\
                                    .QPushButton:focus{outline: none;}";
const QString m_About_style = " .About{background:#272a30;border:1px solid black;}  \
                                 .ui_aboutComponent1{border:none;} \
                                 .ui_aboutComponent2{border:none;} \
                                 .QTextEdit{border:none;background:#272a30;width:80px;height:80px;color:#ffffff;font-family:Microsoft YaHei;font-size:12px;border:none;} \
                                .QRadioButton{color:#a8abaf;font-family:Microsoft YaHei;font-size:12px;}        \
                                .QPushButton {color:white;width:70px;height:30px;font-family:Microsoft YaHei;font-size:14px;background-color:#0c7af2;border:none; border-radius: 5px;padding: 2px;}\
                                .QPushButton:pressed {background-color: rgba(64,65,66,0.5);}\
                                .QPushButton:hover {background-color: #3f91fc;}\
                                .QPushButton:focus{outline: none;} \
                                .QRadioButton{border:none;font-family:Microsoft YaHei;font-size:12px;} ";

#endif // STYLE_H
