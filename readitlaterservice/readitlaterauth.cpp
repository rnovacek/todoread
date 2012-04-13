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
    data.methodName = "password";
    data.mechanism = "password";
    return data;
}

void ReadItLaterAuth::handleResponse(const SignOn::SessionData &sessionData)
{
    m_username = sessionData.UserName();
    m_password = sessionData.Secret();
    emit authResult(RESULT_SUCCESS);
}
