#include "newsdialog.h"
#include "ui_newsdialog.h"

#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QCursor>

#include "preferences.h"

NewsDialog::NewsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewsDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    // Perform a news check now and ... each 30 mins
    onCheck();
    newsCheckIntervall = new QTimer(this);
    connect(newsCheckIntervall, &QTimer::timeout, this, &NewsDialog::onCheck);
    newsCheckIntervall->start(1800000);
}

NewsDialog::~NewsDialog()
{
    delete ui;
}

int NewsDialog::exec()
{
    Preferences& prefs = Preferences::instance();
    prefs.set(VPRO_NEWS_LASTREAD, QDateTime::currentDateTime().toSecsSinceEpoch());
    prefs.save();

    return QDialog::exec();
}

void NewsDialog::onCheck()
{
    setCursor(Qt::WaitCursor);

    QNetworkAccessManager* nm = new QNetworkAccessManager();
    connect(nm, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply)
            {
                setCursor(Qt::ArrowCursor);

                if (!reply->error())
                {
                    Preferences& prefs = Preferences::instance();
                    const QDateTime lastNewsCheck = QDateTime::fromSecsSinceEpoch(prefs.get<int>(VPRO_NEWS_LASTREAD));

                    QString strReply = (QString)reply->readAll();
                    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
                    QJsonObject jsonObject = jsonResponse.object();
                    newsList = jsonObject[channel].toArray();

                    // Find the latest single news
                    QDateTime latest = QDateTime::fromSecsSinceEpoch(0);
                    foreach (const QJsonValue& entry, newsList)
                    {
                        QJsonObject news = entry.toObject();
                        QDateTime date = QDateTime::fromSecsSinceEpoch(news["timestamp"].toInt());

                        if (date > latest)
                            latest = date;
                    }

                    ui->textBrowser->setHtml(renderNewsText());

                    Q_EMIT unreadNewsAvailable(latest > lastNewsCheck);
                }
                else
                {
                    ui->textBrowser->setHtml("Unable to load VoukoderPro news.<br><br>" + reply->errorString());
                }
            });

    QNetworkRequest request(QUrl(NEWS_JSON_REQUEST));
    nm->get(request);
}

const QString NewsDialog::renderNewsText()
{
    QString content;
    foreach (const QJsonValue& entry, newsList)
    {
        QJsonObject news = entry.toObject();

        QDateTime date = QDateTime::fromSecsSinceEpoch(news["timestamp"].toInt());

        content += "<h2>" + news["headline"].toString() + "</h2>" + date.toString("MM/dd/yyyy hh:mm");
        content += "<p>" + news["content"].toString();

        if (newsList.last() != entry)
            content += "<hr>";
    }

    return content;
}
