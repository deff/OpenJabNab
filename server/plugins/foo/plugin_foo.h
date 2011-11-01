#ifndef _PLUGINFOO_H_
#define _PLUGINFOO_H_
#include <time.h>
#include "bunny.h"
#include <stdlib.h>
#include "log.h"
#include "messagepacket.h"
#include "plugin_foo.h"
#include <QHttp>
#include <QList>
#include <QMap>
#include <QMultiMap>
#include <QPair>
#include <QStringList>
#include <QTextStream>
#include <QThread>
#include <QTime>
#include "plugininterface.h"
#include "httprequest.h"
	
class PluginFoo : public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)

public slots:
	void OnCronSleep(Bunny *, QVariant);
	void OnCronWakeUp(Bunny *, QVariant);
	
public:
	PluginFoo();
	virtual ~PluginFoo();
	bool OnClick(Bunny *, PluginInterface::ClickType);
	bool OnRFID(Bunny * b, QByteArray const& tag);
	void OnBunnyConnect(Bunny *);
	void OnBunnyDisconnect(Bunny *);
	void OnInitPacket(Bunny * b, AmbientPacket &, SleepPacket &);

	void InitApiCalls();
	PLUGIN_BUNNY_API_CALL(Api_Sleep);
	PLUGIN_BUNNY_API_CALL(Api_Wakeup);
	PLUGIN_BUNNY_API_CALL(Api_SetMac);
	PLUGIN_BUNNY_API_CALL(Api_SetFoo);
	PLUGIN_BUNNY_API_CALL(Api_SetWake);
	PLUGIN_BUNNY_API_CALL(Api_Setup);
	PLUGIN_BUNNY_API_CALL(Api_GetMac);
	PLUGIN_BUNNY_API_CALL(Api_GetSetup);

private:
	void RegisterCrons(Bunny *);
	void CleanCrons(Bunny *);
	void UpdateState(Bunny *);

	bool IsConfigValid(QList<QVariant> const& wakeupListFoo, QList<QVariant> const& sleepListFoo);
};

#endif
