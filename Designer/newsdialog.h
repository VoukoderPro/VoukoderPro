#ifndef NEWSDIALOG_H
#define NEWSDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QJsonArray>

#define NEWS_CHANNEL_BETA "beta"
#define NEWS_CHANNEL_MAIN "main"
#define NEWS_JSON_REQUEST "https://www.voukoderpro.com/data/news.json"

namespace Ui {
class NewsDialog;
}

class NewsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewsDialog(QWidget *parent = nullptr);
    ~NewsDialog();

    int exec();

Q_SIGNALS:
    void unreadNewsAvailable(bool unreadNews);

private Q_SLOTS:
    void onCheck();

private:
    const QString renderNewsText();

private:
    Ui::NewsDialog* ui;
    QTimer* newsCheckIntervall;
    QString channel = NEWS_CHANNEL_MAIN;
    QJsonArray newsList;
};

#endif // NEWSDIALOG_H
