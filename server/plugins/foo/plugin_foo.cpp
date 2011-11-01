#include "plugin_foo.h"
#include <stdlib.h>
#include <time.h>
#include "bunny.h"
#include "httprequest.h"
#include "log.h"
#include "messagepacket.h"
#include <QDate>
#include <QMap>
#include "bunny.h"
#include "cron.h"
#include "log.h"
#include "messagepacket.h"
#include "packet.h"
#include "settings.h"
#include "sleeppacket.h"
#include <QTimer>

Q_EXPORT_PLUGIN2(plugin_foo, PluginFoo)

PluginFoo::PluginFoo():PluginInterface("foo", "plugin foo")
{
}



bool PluginFoo::OnClick(Bunny * b, PluginInterface::ClickType type)
{
if (type == PluginInterface::SingleClick){
	QString urlf = b->GetPluginSetting(GetName(), "FooPath", "").toString();
	bool uwake = b->GetPluginSetting(GetName(), "FooWake", "").toBool();
	if (uwake == true){ 
		//on active le wake on lan 
		QString umac = b->GetPluginSetting(GetName(), "FooMac", "").toString(); //get mac
		b->SendPacket(MessagePacket("WW /n")); //WW etant le commande wakeonlan du custombootcode v1.1 j'ai vire le slash something.. mais bon comme je connais pas l api par coeur peut etre qui faut completer la requete
		QTimer::singleShot(7000, this, SLOT(b->SendPacket(MessagePacket("ST "+urlf.toAscii()+"/?cmd=PlayOrPause"+"\nMW\n"))));
	} // manque la tempo mais a voir;
	else{
	b->SendPacket(MessagePacket("ST "+urlf.toAscii()+"/?cmd=PlayOrPause"+"\nMW\n"));
	}	
	LogDebug(QString(" -- clickclickboom : %1 %2").arg(urlf,uwake));
	// Send packet to bunny with mp3 to be played
	
	
	}
	return true;
}
bool PluginFoo::OnRFID(Bunny * b, QByteArray const& tag)
{
QByteArray tagi = tag ;
	QString urlf = b->GetPluginSetting(GetName(), "FooPath", "").toString();
	bool uwake = b->GetPluginSetting(GetName(), "FooWake", "").toBool();
	if (uwake == true){ 
		//on active le wake on lan 
		QString umac = b->GetPluginSetting(GetName(), "FooMac", "").toString(); //get mac
		b->SendPacket(MessagePacket("WW "+umac.toAscii())); //WW etant le commande wakeonlan du custombootcode v1.1 j'ai vire le slash something.. mais bon comme je connais pas l api par coeur peut etre qui faut completer la requete
		QTime tempo;
		while(tempo.elapsed() < 1000){
			;	}
	} // tempo a tester voir a passer en parametre;
	LogDebug(QString(" -- clickclickboom : %1 %2").arg(urlf,uwake));
	// Send packet to bunny with mp3 to be played
	b->SendPacket(MessagePacket("ST "+urlf.toAscii()+"/?cmd=PlayOrPause"+"\nMW\n"));
	
	return true;
}

PluginFoo::~PluginFoo() 
{
Cron::UnregisterAll(this); 
}
///////////////////////////////////////////////////////////////////


void PluginFoo::OnBunnyConnect(Bunny * b)
{
	RegisterCrons(b);
	
}

void PluginFoo::OnBunnyDisconnect(Bunny * b)
{
	CleanCrons(b);
}

bool PluginFoo::IsConfigValid(QList<QVariant> const& wakeupListFoo, QList<QVariant> const& sleepListFoo)
{
	if(wakeupListFoo.count() == 0 && sleepListFoo.count() == 0) // Nothing configured, nothing to do
		return false;

	if(wakeupListFoo.count() != 7 || sleepListFoo.count() != 7) // Error :/
	{
		LogError("Bad list size");
		return false;
	}
	return true;
}

void PluginFoo::OnInitPacket(Bunny * b, AmbientPacket &, SleepPacket & )
{
	// Check if bunny need to sleep or not
	QList<QVariant> wakeupListFoo = b->GetPluginSetting(GetName(), QString("wakeupListFoo"), QList<QVariant>()).toList();
	QList<QVariant> sleepListFoo = b->GetPluginSetting(GetName(), QString("sleepListFoo"), QList<QVariant>()).toList();

	if(!IsConfigValid(wakeupListFoo, sleepListFoo))
		return;

	QTime currentTime = QTime::currentTime();
	
	QString umac = b->GetPluginSetting(GetName(), "FooMac", "").toString();
	QString uip = b->GetPluginSetting(GetName(), "FooIP", "").toString();
	
	b->SendPacket(MessagePacket("IM "+umac.toAscii()+"\n"));
	b->SendPacket(MessagePacket("IW "+uip.toAscii()+"\n"));
	
	// int day = QDate::currentDate().dayOfWeek()-1;

	/* if (wakeupListFoo.at(day).toTime() <= currentTime && currentTime < sleepListFoo.at(day).toTime())
		s.SetState(SleepPacket::Wake_Up);
	else
		s.SetState(SleepPacket::Sleep); */
}

void PluginFoo::UpdateState(Bunny * b)
{
	// Check if bunny need to sleep or not
	QList<QVariant> wakeupListFoo = b->GetPluginSetting(GetName(), QString("wakeupListFoo"), QList<QVariant>()).toList();
	QList<QVariant> sleepListFoo = b->GetPluginSetting(GetName(), QString("sleepListFoo"), QList<QVariant>()).toList();
/* 
	if(!IsConfigValid(wakeupListFoo, sleepListFoo))
		return;
	QTime currentTime = QTime::currentTime();
	int day = QDate::currentDate().dayOfWeek()-1; */

	// bool wakeup = (wakeupListFoo.at(day).toTime() <= currentTime && currentTime < sleepListFoo.at(day).toTime());

	/* if (wakeup && b->IsSleeping())
		b->SendPacket(SleepPacket(SleepPacket::Wake_Up));
	else if(!wakeup && b->IsIdle())
		b->SendPacket(SleepPacket(SleepPacket::Sleep)); */
}

void PluginFoo::OnCronWakeUp(Bunny * b, QVariant)
{
	QString urlf = b->GetPluginSetting(GetName(), "FooPath", "").toString();
	bool uwake = b->GetPluginSetting(GetName(), "FooWake", "").toBool();
		if (uwake == true){ 
				//on active le wake on lan 
				QString umac = b->GetPluginSetting(GetName(), "FooMac", "").toString(); //get mac
				b->SendPacket(MessagePacket("WW "+umac.toAscii())); //WW etant le commande wakeonlan du custombootcode v1.1 j'ai vire le slash something.. mais bon comme je connais pas l api par coeur peut etre qui faut completer la requete
				} // manque la tempo mais a voir;
	b->SendPacket(MessagePacket("ST "+urlf.toAscii()+"/?cmd=Play"+"\nMW\n"));;
}


void PluginFoo::OnCronSleep(Bunny * b, QVariant)
{
	QString urlf = b->GetPluginSetting(GetName(), "FooPath", "").toString();
		// Get a random value and create ID
		// Send packet to bunny with mp3 to be played
		b->SendPacket(MessagePacket("ST "+urlf.toAscii()+"/?cmd=Pause"+"\nMW\n"));;;
}

void PluginFoo::CleanCrons(Bunny * b)
{
	Cron::UnregisterAllForBunny(this, b);
}

void PluginFoo::RegisterCrons(Bunny * b)
{
	QList<QVariant> wakeupListFoo = b->GetPluginSetting(GetName(), QString("wakeupListFoo"), QList<QVariant>()).toList();
	QList<QVariant> sleepListFoo = b->GetPluginSetting(GetName(), QString("sleepListFoo"), QList<QVariant>()).toList();

	if(wakeupListFoo.count() == 0 && sleepListFoo.count() == 0) // Nothing configured
		return;

	if(wakeupListFoo.count() != 7 || sleepListFoo.count() != 7)
	{
		LogError("Bad list size");
		return;
	}

	for(int day = 0; day < 7; day++)
	{
		Cron::RegisterWeekly(this, (Qt::DayOfWeek)(day+1), wakeupListFoo.at(day).toTime(), b, QVariant(), "OnCronWakeUp");
		Cron::RegisterWeekly(this, (Qt::DayOfWeek)(day+1), sleepListFoo.at(day).toTime(), b, QVariant(), "OnCronSleep");
	}
}

/*******
 * API *
 *******/

void PluginFoo::InitApiCalls()
{
	DECLARE_PLUGIN_BUNNY_API_CALL("sleep()", PluginFoo, Api_Sleep);
	DECLARE_PLUGIN_BUNNY_API_CALL("wakeup()", PluginFoo, Api_Wakeup);
	DECLARE_PLUGIN_BUNNY_API_CALL("setfoo(fpath)", PluginFoo, Api_SetFoo);
	DECLARE_PLUGIN_BUNNY_API_CALL("setmac(mac,uip)", PluginFoo, Api_SetMac);
	DECLARE_PLUGIN_BUNNY_API_CALL("getmac()", PluginFoo, Api_GetMac);
	DECLARE_PLUGIN_BUNNY_API_CALL("setwake(wake)", PluginFoo, Api_SetWake);
	DECLARE_PLUGIN_BUNNY_API_CALL("setup(wakeupListFoo,sleepListFoo)", PluginFoo, Api_Setup);
	DECLARE_PLUGIN_BUNNY_API_CALL("getsetup()", PluginFoo, Api_GetSetup);
}

PLUGIN_BUNNY_API_CALL(PluginFoo::Api_Sleep)
{
	Q_UNUSED(account);
	Q_UNUSED(hRequest);

	if(!bunny->IsIdle())
		return new ApiManager::ApiError(QString("Bunny is not idle"));

	bunny->SendPacket(SleepPacket(SleepPacket::Sleep));
	return new ApiManager::ApiOk(QString("Bunny is going to sleep."));
}

PLUGIN_BUNNY_API_CALL(PluginFoo::Api_Wakeup)
{
	Q_UNUSED(account);
	Q_UNUSED(hRequest);

	if(!bunny->IsSleeping())
		return new ApiManager::ApiError(QString("Bunny is not sleeping"));

	bunny->SendPacket(SleepPacket(SleepPacket::Wake_Up));
	return new ApiManager::ApiOk(QString("Bunny is waking up."));
}
/* PLUGIN_BUNNY_API_CALL(PluginFoo::Api_SetFooPath) 
{
	Q_UNUSED(account);

	if(!hRequest.HasArg("fpath") or !hRequest.HasArg("mac") ){
		return new ApiManager::ApiError(QString("Missing Path for plugin Foo"));
	}
	
	QString tmp = hRequest.GetArg("fpath");
	QString tmp1 = hRequest.GetArg("mac");
	bunny->SetPluginSetting(GetName(), "FooPath", tmp);
	bunny->SetPluginSetting(GetName(), "FooMac", tmp1);

	return new ApiManager::ApiOk(QString("Added arrets '%1' for bunny '%2'").arg(tmp, QString(bunny->GetID())));
} */
PLUGIN_BUNNY_API_CALL(PluginFoo::Api_SetFoo) 
{
	Q_UNUSED(account);

	if(!hRequest.HasArg("fpath") ){
		return new ApiManager::ApiError(QString("Missing Path for plugin Foo"));
	}
	
	QString tmpbis = hRequest.GetArg("fpath");
	bunny->SetPluginSetting(GetName(), "FooPath", tmpbis);

	return new ApiManager::ApiOk(QString("Added arrets '%1' for bunny '%2'").arg(tmpbis, QString(bunny->GetID())));
}
PLUGIN_BUNNY_API_CALL(PluginFoo::Api_SetMac) 
{
	Q_UNUSED(account);

	if(!hRequest.HasArg("mac") ){
		return new ApiManager::ApiError(QString("Missing Mac for plugin Foo"));
	}
	
	QString tmpbis = hRequest.GetArg("mac");
	QString tmpbis2 = hRequest.GetArg("uip");
	bunny->SetPluginSetting(GetName(), "FooMac", tmpbis);
	bunny->SetPluginSetting(GetName(), "FooIP", tmpbis2);
	return new ApiManager::ApiOk(QString("Added arrets '%1' for bunny '%2'").arg(tmpbis, QString(bunny->GetID())));
}
PLUGIN_BUNNY_API_CALL(PluginFoo::Api_SetWake) 
{
	Q_UNUSED(account);

	if(!hRequest.HasArg("wake") ){
		return new ApiManager::ApiError(QString("Missing Wake for plugin Foo"));
	}
	
	bool tmpbis = hRequest.GetArg("wake").contains("true");
	bunny->SetPluginSetting(GetName(), "FooWake", tmpbis);

	return new ApiManager::ApiOk(QString("Added arrets for bunny merdik"));
}
PLUGIN_BUNNY_API_CALL(PluginFoo::Api_Setup)
{
	Q_UNUSED(account);

	QStringList wakeupListFoo =  hRequest.GetArg("wakeupListFoo").split(',');
	QStringList sleepListFoo = hRequest.GetArg("sleepListFoo").split(',');

	if(wakeupListFoo.count() != 7 || sleepListFoo.count() != 7)
		return new ApiManager::ApiError(QString("Bad list size"));

	// Transform QStringList to QList<QTime>
	QList<QVariant> wListFoo;
	QList<QVariant> sListFoo;
	for(int day = 0; day < 7; ++day)
	{
		QTime w = QTime::fromString(wakeupListFoo.at(day), "hh:mm");
		if(w.isValid())
			wListFoo.append(w);
		else
			return new ApiManager::ApiError(QString("Bad time '%1'").arg(wakeupListFoo.at(day)));

		QTime s = QTime::fromString(sleepListFoo.at(day), "hh:mm");
		if(s.isValid())
			sListFoo.append(s);
		else
			return new ApiManager::ApiError(QString("Bad time '%1'").arg(sleepListFoo.at(day)));
	}

	bunny->SetPluginSetting(GetName(), "wakeupListFoo", wListFoo);
	bunny->SetPluginSetting(GetName(), "sleepListFoo", sListFoo);

	CleanCrons(bunny);
	RegisterCrons(bunny);
	UpdateState(bunny);

	return new ApiManager::ApiOk(QString("Plugin configuration updated."));
}
PLUGIN_BUNNY_API_CALL(PluginFoo::Api_GetMac)
{
	Q_UNUSED(account);
	Q_UNUSED(hRequest);

	QString mactest = bunny->GetPluginSetting(GetName(), QString("FooMac"), "").toString();
	
		
	return new ApiManager::ApiString(mactest);
}
PLUGIN_BUNNY_API_CALL(PluginFoo::Api_GetSetup)
{
	Q_UNUSED(account);
	Q_UNUSED(hRequest);

	QStringList setup;
	QList<QVariant> wakeupListFoo = bunny->GetPluginSetting(GetName(), QString("wakeupListFoo"), QList<QVariant>()).toList();
	QList<QVariant> sleepListFoo = bunny->GetPluginSetting(GetName(), QString("sleepListFoo"), QList<QVariant>()).toList();
	if(wakeupListFoo.count() == 7 && sleepListFoo.count() == 7)
	{
		for(int day = 0; day < 7; day++)
		{
			setup << wakeupListFoo.at(day).toString();
		}
		for(int day = 0; day < 7; day++)
		{
			setup << sleepListFoo.at(day).toString();
		}
	}
	return new ApiManager::ApiList(setup);
}
