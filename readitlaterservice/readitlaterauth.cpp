#include "readitlaterauth.h"


ReadItLaterAuth::ReadItLaterAuth(QObject *parent) : WebUpload::AuthBase(parent)
{

}

bool ReadItLaterAuth::isAuthRequired()
{
    return true;
}

WebUpload::AuthData ReadItLaterAuth::getAuthData()
{
    WebUpload::AuthData data;
    data.methodName = "readitlater";
    data.mechanism = "GetPassword";
    return data;
}

void ReadItLaterAuth::handleResponse(const SignOn::SessionData &sessionData)
{
    m_username = sessionData.UserName();
    m_password = sessionData.getProperty("SecretPassword").toString();
    emit authResult(RESULT_SUCCESS);
}
