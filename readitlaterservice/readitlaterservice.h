#include <QObject>
#include <WebUpload/PluginInterface>
#include <WebUpload/PluginBase>
 
class ReadItLaterPlugin : public WebUpload::PluginBase
{
    Q_OBJECT
   
public:
    ReadItLaterPlugin(QObject *parent = 0);
    virtual ~ReadItLaterPlugin();
 
    virtual bool init();
    virtual WebUpload::PostInterface* getPost();
    virtual WebUpload::UpdateInterface* getUpdate();
 
};
