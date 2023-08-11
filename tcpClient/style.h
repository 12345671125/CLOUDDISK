#ifndef STYLE_H
#define STYLE_H
#include <QString>

const QString QMenu_Item_selected ="QMenu::item{background:rgba (82,130,164,1);border:1px solid rgba (82,130,164,1);}";
const QString m_stylesheet_QPushButton = ".QPushButton {color:white;width:70px;height:30px;font-family:Microsoft YaHei;font-size:14px;background-color:#0c7af2;border:none; border-radius: 5px;padding: 2px;}\
                                             .QPushButton:pressed {background-color: rgba(64,65,66,0.5);}\
    .QPushButton:hover {background-color: #3f91fc;}\
    .QPushButton:focus{outline: none;}";
const QString m_listWidget_style = "QListWidget{border:none;background:#1e1c1b;width:80px;padding:5px 5px 5px 5px;border-bottom-left-radius:5px;} \
                                    QListWidget::item{background-color:#1e1c1b;height:25px;color:white;font-family:Microsoft YaHei;font-size:12px;border:none; padding: 5px 5px 5px 5px;} \
                                    QListWidget::Item:hover{background:#0c7af2;padding-top:0px; padding-bottom:0px;} \
                                    QListWidget::Item:selected{background:#0c7af2;padding-top:0px; padding-bottom:0px;border-left:3px solid #ffe26d; } \
                                    QListWidget:focus{outline:none;}";
const QString m_StackWidget_style = "QStackWidget{backgroud:#1d1d1f;}";
const QString m_LogoWidget_style = ".LogoWidget{border:none;margin:0px;padding:0px;} .PushButton{width:32px;height:32px;border:none;color:white;}";

const QString m_filePage_style = ".QListWidget{background:#272a30;border-radius:10px;} \
                                  .QListWidget::item{height:35px;color:#a8abaf;font-family:Microsoft YaHei;font-size:14px;border:none; padding: 5px 5px 5px 5px;} \
                                .QListWidget::item:selected{border:1px dashed #fff7d6;} \
                                  .QListWidget:focus{outline:none;} \
                                 .QPushButton {color:white;width:70px;height:30px;font-family:Microsoft YaHei;font-size:14px;background-color:#0c7af2;border:none; border-radius: 5px;padding: 2px;}\
                                    .QPushButton:pressed {background-color: rgba(64,65,66,0.5);}\
                                    .QPushButton:hover {background-color: #3f91fc;}\
                                    .QPushButton:focus{outline: none;}";
#endif // STYLE_H
