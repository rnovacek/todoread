
#include "readitlaterprovider.h"

#include <SignOn/Error>

#include <QNetworkAccessManager>
#include <QNetworkReply>

ReadItLaterPlugin::ReadItLaterPlugin(QObject *parent) :
    AuthPluginInterface(parent), m_networkManager(new QNetworkAccessManager(this))
{
}

ReadItLaterPlugin::~ReadItLaterPlugin()
{
}


QString ReadItLaterPlugin::type() const
{
    return "readitlater";
}

QStringList ReadItLaterPlugin::mechanisms() const
{
    return QStringList() << "ReadItLater";
}

void ReadItLaterPlugin::cancel()
{
}

void ReadItLaterPlugin::abort()
{
}

void ReadItLaterPlugin::process(const SignOn::SessionData &inData, const QString &mechanism)
{
    if (mechanism == "ReadItLater") {
        QUrl query;
        query.addQueryItem("username", inData.getProperty("UserName").toString());
        query.addQueryItem("password", inData.getProperty("Secret").toString());
        query.addQueryItem("apikey", "926TnD0Vg5663lb9e7dc565vF9p4cW5a");
        QNetworkRequest request(QUrl("http://readitlaterlist.com/v2/auth"));

        QNetworkReply *reply = m_networkManager->post(request, query.encodedQuery());
        connect(reply, SIGNAL(finished()), SLOT(authenticationFinished()));

        m_data = inData;
    }
}

void ReadItLaterPlugin::authenticationFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    switch (reply->error()) {
    case QNetworkReply::NoError:
        emit result(m_data);
        break;
    case QNetworkReply::AuthenticationRequiredError:
        emit error(SignOn::Error(SignOn::Error::InvalidCredentials, reply->errorString()));
        break;
    case QNetworkReply::ConnectionRefusedError:
        emit error(SignOn::Error(SignOn::Error::InternalServer, reply->errorString()));
        break;
    case QNetworkReply::RemoteHostClosedError:
        emit error(SignOn::Error(SignOn::Error::InternalServer, reply->errorString()));
        break;
    case QNetworkReply::HostNotFoundError:
        emit error(SignOn::Error(SignOn::Error::InternalCommunication, reply->errorString()));
        break;
    case QNetworkReply::TimeoutError:
        emit error(SignOn::Error(SignOn::Error::TimedOut, reply->errorString()));
        break;
    case QNetworkReply::OperationCanceledError:
        emit error(SignOn::Error(SignOn::Error::IdentityOperationCanceled, reply->errorString()));
        break;
    case QNetworkReply::SslHandshakeFailedError:
        emit error(SignOn::Error(SignOn::Error::Ssl, reply->errorString()));
        break;
    case QNetworkReply::TemporaryNetworkFailureError:
        emit error(SignOn::Error(SignOn::Error::Network, reply->errorString()));
        break;
    case QNetworkReply::ContentAccessDenied:
        emit error(SignOn::Error(SignOn::Error::NotAuthorized, reply->errorString()));
        break;
    default:
        emit error(SignOn::Error(SignOn::Error::Unknown, reply->errorString()));
        break;
    }
}

SIGNON_DECL_AUTH_PLUGIN(ReadItLaterPlugin)
