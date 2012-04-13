#include "readitlaterservice.h"
#include <WebUpload/PluginApplication>

#include "readitlaterpost.h"

PLUGIN_MAIN_FUNCTION(ReadItLaterPlugin)

ReadItLaterPlugin::ReadItLaterPlugin(QObject *parent) : WebUpload::PluginBase(parent)
{
}

ReadItLaterPlugin::~ReadItLaterPlugin()
{
}

bool ReadItLaterPlugin::init()
{
    return true;
}

WebUpload::PostInterface *ReadItLaterPlugin::getPost()
{
    return new ReadItLaterPost(this);
}

WebUpload::UpdateInterface *ReadItLaterPlugin::getUpdate()
{
    return 0;
}
